# vision (PC 쪽 Python)

XIAO 스트림(또는 노트북 웹캠)을 받아 사람을 탐지하고 방향을 판단한다.

```bash
python3 -m venv .venv && source .venv/bin/activate   # 처음 한 번 (Windows: .venv\Scripts\activate)
pip install -r requirements.txt

python stream_viewer.py                                        # 웹캠으로 테스트
python stream_viewer.py http://<보드IP>:81/stream              # XIAO 스트림
python stream_viewer.py http://<보드IP>:81/stream --rotate 90  # 영상이 누워 있을 때 (90/180/270)
```

- 화면 왼쪽 위에 FPS와 해상도가 표시된다. `q`를 누르면 종료.
- 스트림이 끊기면 2초 뒤 자동으로 다시 연결한다.
- PC와 보드가 **같은 핫스팟**에 연결되어 있어야 한다.
- Mac에서 웹캠 화면이 까맣게 나오면 `python stream_viewer.py 1` (0번이 아이폰 연속성 카메라일 수 있음).

## ROS 2로 상태 보내기 (`--ros`)

1초마다 `/camera/status`에 상태를 보낸다. 형식은 `docs/PRD.md` 5.3절.

```bash
python stream_viewer.py http://<보드IP>:81/stream --ros
```

- ROS 2가 없는 PC(Mac)에서는 같은 내용을 **터미널에 출력만** 한다. 코드 수정 없이 양쪽에서 실행된다.
- **ROS 2 PC(WSL2 Ubuntu 24.04)에서 실행할 때** — 가상환경 대신 시스템 Python을 쓴다 (rclpy가 시스템에 설치되어 있기 때문).
  ```bash
  sudo apt install python3-opencv          # 처음 한 번
  source /opt/ros/jazzy/setup.bash
  cd vision
  python3 stream_viewer.py http://<보드IP>:81/stream --ros
  ```
  다른 터미널에서 확인: `ros2 topic echo /camera/status`
