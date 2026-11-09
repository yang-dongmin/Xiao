# ros2_ws (ROS 2 워크스페이스)

ROS 2 Jazzy 기준. 패키지는 `src/` 아래에 만든다.

```bash
cd ros2_ws
colcon build
source install/setup.bash
```

계획한 토픽
- `/vision/detection` — 탐지 결과 (object, confidence, x, y)
- `/camera/status` — 카메라 상태
- `/camera/command` — 시작/정지 명령
