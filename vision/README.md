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
