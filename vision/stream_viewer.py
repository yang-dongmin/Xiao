"""영상 띄우기 + FPS 표시.

사용법
    python stream_viewer.py                                   # 노트북 웹캠
    python stream_viewer.py http://<보드IP>:81/stream         # XIAO 스트림
    python stream_viewer.py http://<보드IP>:81/stream --rotate 90   # 영상이 누워 있을 때
    python stream_viewer.py http://<보드IP>:81/stream --ros         # 상태를 /camera/status로 발행

q를 누르면 종료.
"""
import argparse
import sys
import time

import cv2

from ros_bridge import TopicPublisher

# --rotate 값 → OpenCV 회전 방법 (시계 방향)
ROTATIONS = {
    0: None,
    90: cv2.ROTATE_90_CLOCKWISE,
    180: cv2.ROTATE_180,
    270: cv2.ROTATE_90_COUNTERCLOCKWISE,
}
RECONNECT_WAIT = 2.0  # 스트림이 끊겼을 때 다시 연결하기 전 기다리는 시간(초)
STATUS_PERIOD = 1.0   # /camera/status 발행 간격(초)


def parse_args():
    p = argparse.ArgumentParser(description="XIAO 스트림 / 웹캠 뷰어")
    p.add_argument("source", nargs="?", default="0",
                   help="웹캠 번호(기본 0) 또는 스트림 주소")
    p.add_argument("--rotate", type=int, choices=ROTATIONS, default=0,
                   help="시계 방향 회전 각도 (0, 90, 180, 270)")
    p.add_argument("--ros", action="store_true",
                   help="1초마다 /camera/status 토픽으로 상태(연결 여부, FPS, 해상도) 발행")
    return p.parse_args()


def open_source(source):
    """숫자면 웹캠, 아니면 주소로 연다."""
    return cv2.VideoCapture(int(source) if source.isdigit() else source)


def draw_info(frame, fps):
    """왼쪽 위에 FPS와 해상도를 적는다. 검은 테두리를 깔아서 밝은 화면에서도 보이게."""
    h, w = frame.shape[:2]
    text = f"FPS {fps:4.1f} | {w}x{h}"
    for color, thickness in (((0, 0, 0), 4), ((0, 255, 0), 2)):
        cv2.putText(frame, text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX,
                    0.8, color, thickness, cv2.LINE_AA)


def main():
    args = parse_args()
    is_stream = not args.source.isdigit()
    rotation = ROTATIONS[args.rotate]

    cap = open_source(args.source)
    if not cap.isOpened():
        sys.exit(f"영상을 열 수 없습니다: {args.source}")
    print(f"연결됨: {args.source}  (q = 종료)")

    status_pub = TopicPublisher("/camera/status") if args.ros else None
    last_status = 0.0

    def send_status(state, frame=None):
        """1초에 한 번만 상태를 보낸다 (매 프레임 보내면 토픽이 넘친다)."""
        nonlocal last_status
        if status_pub is None or time.time() - last_status < STATUS_PERIOD:
            return
        last_status = time.time()
        data = {"state": state, "source": args.source, "fps": round(fps, 1)}
        if frame is not None:
            data["width"], data["height"] = frame.shape[1], frame.shape[0]
        status_pub.publish(data)

    fps = 0.0
    prev = None  # 직전 사진이 도착한 시각

    while True:
        ok, frame = cap.read()

        if not ok:
            # 웹캠은 다시 붙일 이유가 없으니 종료, 스트림은 Wi-Fi가 잠깐 끊긴 것일 수 있으니 재연결
            if not is_stream:
                print("웹캠에서 영상을 읽지 못했습니다.")
                break
            last_status = 0.0  # 끊김은 바로 알린다
            send_status("disconnected")
            print(f"스트림이 끊겼습니다. {RECONNECT_WAIT:.0f}초 후 다시 연결합니다...")
            cap.release()
            time.sleep(RECONNECT_WAIT)
            cap = open_source(args.source)
            prev = None
            if cv2.waitKey(1) & 0xFF == ord("q"):
                break
            continue

        # FPS: 직전 프레임과의 시간 간격으로 계산하고, 숫자가 튀지 않게 부드럽게(지수 평균) 만든다
        # 첫 사진은 비교할 직전 사진이 없으니 시각만 기록하고 넘어간다
        now = time.perf_counter()
        if prev is not None:
            instant = 1.0 / max(now - prev, 1e-6)
            fps = instant if fps == 0 else fps * 0.9 + instant * 0.1
        prev = now

        if rotation is not None:
            frame = cv2.rotate(frame, rotation)

        send_status("streaming", frame)
        draw_info(frame, fps)
        cv2.imshow("stream", frame)
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

    cap.release()
    cv2.destroyAllWindows()
    if status_pub is not None:
        last_status = 0.0
        send_status("stopped")
        status_pub.close()


if __name__ == "__main__":
    main()
