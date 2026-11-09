# firmware (보드 코드)

실습 하나당 폴더 하나. 도구에 따라 폴더 안 모양이 다르다.

**Arduino IDE를 쓰면** — 폴더 이름과 .ino 파일 이름이 같아야 한다.
```
blink/blink.ino
```

**VS Code + PlatformIO를 쓰면** — 폴더 하나가 PlatformIO 프로젝트 하나.
```
blink/
  platformio.ini
  src/main.cpp
```

카메라 예제(camera_capture, camera_webserver)는 PSRAM을 켜야 한다.
- Arduino IDE: Tools > PSRAM > OPI PSRAM
- PlatformIO: platformio.ini에 `board_build.arduino.memory_type = qio_opi`, `build_flags = -DBOARD_HAS_PSRAM`

Wi-Fi 이름·비밀번호는 각 폴더의 `secrets.h`에 두고 깃에 올리지 않는다
(`secrets.example.h`를 복사해서 만든다).
