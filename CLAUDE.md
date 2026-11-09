# CLAUDE.md — 프로젝트 맥락 (claude.ai 대화에서 넘겨받음, 2026-11-09)

## 누구를 돕는가
- 소프트웨어 4학년, 병렬컴퓨팅 과목 3인 팀 프로젝트. Mac 사용.
- 한국어로 답한다. 처음 보는 개념은 쉽게 풀어서 설명한다 (전문 용어만 던지지 않기).
- 본인 역할: **영상처리 · AI · 발표** (`vision/`, `docs/`).

## 과제 (근거 자료는 이 두 개만 본다)
- `xiaoesp.pdf` = 과제 로드맵. 목표: "엣지 머신러닝 시스템에 ROS2, AI 기능 추가 — 구글 TFLite Micro 비전시스템 또는 아무거나 제작".
  - 1쪽 진행 순서: 1 보드 파악 → 2 설치 → 3 카메라 → 4 Wi-Fi 웹 카메라 → 5 Wi-Fi 영상 → 간단한 AI → 객체/사람/방향 판단 → ROS로
  - 2~7쪽은 5단계를 풀어 쓴 참고 설명(AI 챗봇 답변 캡처). 핵심: 영상 경로와 제어·상태 경로 분리, XIAO는 ROS 2를 직접 돌리지 않고 PC가 담당, micro-ROS부터 시작하지 말 것.
- `보드.pdf` = XIAO ESP32S3 Sense 판매 페이지. ESP32-S3 240MHz 듀얼코어, PSRAM 8MB, Flash 8MB, microSD(≤32GB FAT), 2.4GHz Wi-Fi/BLE, 카메라(판매 페이지는 OV2640, 과제 문서는 OV3660 — 실물 확인 필요). 보드는 교수님이 제공.
- micro-ROS, 로봇 HW, 3.1 강의자료 등은 이번 과제 범위에서 **제외**하기로 함.

## Seeed 위키 (1~4단계) — 할 것 / 뺄 것
- getting_started: 보드 설정, Blink ✅ / 배터리, UF2 ⏭️
- wifi_usage: Wi-Fi 스캔, 연결, softAP(192.168.4.1) LED 웹 제어 ✅ / MQTT, Mesh ⏭️
- camera_usage: SD 사진 저장, 짧은 영상 녹화, **CameraWebServer 스트리밍**(얼굴 탐지 포함) ✅ / Round Display, OV5640 전용 설정 ⏭️
- 주의: 카메라는 PSRAM(OPI) 필수. CameraWebServer는 ESP32 코어 2.0.x / 3.0.x 코드가 따로 있음.

## 만들 시스템 (5단계는 위키에 없음 → 직접 구현)
```
[XIAO 카메라] --Wi-Fi(HTTP 스트림)--> [PC: Python + OpenCV + AI] --> [ROS 2 토픽]
```
- 스트림 주소: CameraWebServer 기준 `http://<보드IP>:81/stream` → `cv2.VideoCapture(주소)`로 받음. 지금은 웹캠(`0`)으로 개발하고 주소만 바꾼다.
- 토픽 계획: `/vision/detection`(object, confidence, x, y), `/camera/status`, `/camera/command`(START/STOP)
- 방향 판단: 탐지 박스 x 좌표로 왼쪽/가운데/오른쪽.
- AI는 PC에서 실행(본 계획). 보드 위 TFLite Micro는 여유 있을 때 보강.

## 일정 (매주 수요일 수업에서 완료 기준 시연)
| 수업일 | 단계 | 완료 기준 |
|---|---|---|
| 11/4 | 1~2 설치 · Wi-Fi | Blink, softAP 웹페이지 LED 제어 |
| 11/11 | 3~4 카메라 · 스트리밍 | SD 사진 저장, 브라우저 실시간 영상 |
| 11/18 | 5-① PC로 영상 + ROS 2 | PC 창에 영상, ROS 2 토픽 확인 |
| 11/25 | 5-② AI 판단 | 사람 탐지 박스 + 방향 출력 |
| 12/2 | 5-③ ROS로 (**핵심 마감**) | 사람이 보이면 `/vision/detection` 발행 |
| 12/9 | 통합 점검 + 보강 | 전체 흐름 점검 |
| 12/16 | 최종 시연 | 카메라 → ROS 2 토픽 전체 |
종강이 12/9일 수도 있으므로 12/2 마감은 꼭 지킨다.

## 역할
- 동혁(보드 보유): 보드 · 펌웨어 (`firmware/`)
- 민혁: ROS 2 · 통합, 시연 PC (`ros2_ws/`)
- 동민: 영상처리 · AI · 발표 (`vision/`, `docs/`)

## 개발 환경
- ROS 2는 팀 전체 **Jazzy**로 통일. 윈도우 = WSL2(Ubuntu 24.04, `networkingMode=mirrored`), Mac = RoboStack.
- 펌웨어 도구 미확정: Arduino IDE(위키와 동일) vs VS Code + PlatformIO(동혁이 한 번 빌드해 봄).
  - PlatformIO면 `.ino`→`src/main.cpp` + `#include <Arduino.h>`, `platformio.ini`에 `board_build.arduino.memory_type = qio_opi`, `build_flags = -DBOARD_HAS_PSRAM`.
- 학교 Wi-Fi 대신 휴대폰 2.4GHz 핫스팟 사용.

## 저장소 규칙
- 역할별 폴더만 각자 수정. 작업 전 `git pull`, 시연 성공 시 커밋.
- Wi-Fi 비밀번호는 `secrets.h` / `.env` (깃 제외). `.pio/`, ROS 2 build/install/log 제외.
- 커밋, 푸시는 사용자가 요청할 때만.

## claude.ai에 있는 관련 자료
- 일정표 문서(Docs): https://claude.ai/artifact/P254G6cdmhEN31fHQRK5fT — 주차별 작업표, 윈도우 WSL2 + ROS 2 설치 순서 포함
- 발표 슬라이드(10/28 계획 발표): https://claude.ai/artifact/9c7GsC1QT5DgQ9C9xxnxBc

## 다음 할 일 (2026-11-09 기준)
- 펌웨어 도구 통일 (동혁에게 확인 중)
- 11/11 카메라 실습: 2.0.x용 CameraWebServer, PSRAM 설정
- 본인: `vision/stream_viewer.py`로 웹캠 확인 → 사람 탐지 + 방향 판단 코드 시작