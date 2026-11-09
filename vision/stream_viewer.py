"""영상 띄우기. 인자가 없으면 노트북 웹캠, 있으면 그 주소(XIAO 스트림)를 연다."""
import sys
import cv2

source = sys.argv[1] if len(sys.argv) > 1 else 0
cap = cv2.VideoCapture(source)
if not cap.isOpened():
    sys.exit(f"영상을 열 수 없습니다: {source}")

while True:
    ok, frame = cap.read()
    if not ok:
        break
    cv2.imshow("stream", frame)
    if cv2.waitKey(1) & 0xFF == ord("q"):  # q를 누르면 종료
        break

cap.release()
cv2.destroyAllWindows()
