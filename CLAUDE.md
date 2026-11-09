# CLAUDE.md — 팀 공용 프로젝트 맥락

병렬컴퓨팅 3인 팀 프로젝트. 팀원 누구나 이 파일을 Claude와 함께 쓴다.

## 누구를 돕는가
- 사용자는 아래 역할 표의 세 명 중 한 명이다. **누구인지 모르면 먼저 묻거나, 작업하는 폴더나 브랜치로 판단한다.** 다른 팀원 역할로 착각하지 않는다.
- 한국어로 답한다. 처음 보는 개념은 쉽게 풀어서 설명한다 (전문 용어만 던지지 않기).
- 규칙과 인터페이스는 `docs/PRD.md`를 따른다. PRD와 다르게 하려면 사용자에게 먼저 알린다.

## 역할
| 이름 | 역할 | 브랜치 | 폴더 | 환경 |
|---|---|---|---|---|
| 동혁 | 보드 · 펌웨어 (보드 보유) | `firmware` | `firmware/` | Windows, VS Code + PlatformIO |
| 민혁 | ROS 2 · 통합, 시연 PC | `ros2` | `ros2_ws/` | |
| 동민 | 영상처리 · AI · 발표 | `vision` | `vision/`, `docs/` | Mac |

## 과제 (근거 자료는 이 두 개만 본다)
- `xiaoesp.pdf` = 과제 로드맵. 목표: "엣지 머신러닝 시스템에 ROS2, AI 기능 추가 — 구글 TFLite Micro 비전시스템 또는 아무거나 제작".
  - 1쪽 진행 순서: 1 보드 파악 → 2 설치 → 3 카메라 → 4 Wi-Fi 웹 카메라 → 5 Wi-Fi 영상 → 간단한 AI → 객체/사람/방향 판단 → ROS로
  - 2~7쪽은 5단계 참고 설명. 핵심: 영상 경로와 제어·상태 경로 분리, XIAO는 ROS 2를 직접 돌리지 않고 PC가 담당, micro-ROS부터 시작하지 말 것.
- `보드.pdf` = XIAO ESP32S3 Sense 판매 페이지. ESP32-S3 240MHz 듀얼코어, PSRAM 8MB, Flash 8MB, microSD(≤32GB FAT32), 2.4GHz Wi-Fi/BLE. 보드는 교수님이 제공.
- 카메라 센서: OV2640 / OV3660 / OV5640 중 하나 (위키 기준 현재 기본은 OV5640). 실물 확인 필요.
- micro-ROS, 로봇 HW, 3.1 강의자료는 이번 과제 범위에서 **제외**.

## 만들 시스템
```
[XIAO 카메라] --Wi-Fi(HTTP 스트림)--> [PC: Python + OpenCV + AI] --> [ROS 2 토픽]
```
- 1~4단계는 Seeed 위키 실습. 요약은 `docs/wiki-notes.md` (Blink, softAP LED, SD 사진, CameraWebServer).
- 5단계는 위키에 없음 → 직접 구현.
- 스트림 주소: `http://<보드IP>:81/stream` (11/11에 실제 확인 필요).
- 토픽: `/vision/detection`, `/camera/status`, `/camera/command`. 메시지 형식은 `docs/PRD.md` 5절.
- AI는 PC에서 실행. 보드 위 TFLite Micro는 여유 있을 때 보강.

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

## 개발 환경
- ROS 2는 팀 전체 **Jazzy**로 통일. Windows = WSL2(Ubuntu 24.04, `networkingMode=mirrored`), Mac = RoboStack.
- 펌웨어는 **VS Code + PlatformIO**. 실습마다 `firmware/<실습이름>/`에 PlatformIO 프로젝트 하나 (`platformio.ini`, `src/main.cpp`, `include/`).
  - 위키의 `.ino` 코드는 `src/main.cpp`로 옮기고 맨 위에 `#include <Arduino.h>`를 붙인다.
  - 카메라 실습은 `platformio.ini`에 `board_build.arduino.memory_type = qio_opi`, `build_flags = -DBOARD_HAS_PSRAM` 필수.
- 학교 Wi-Fi 대신 휴대폰 2.4GHz 핫스팟 사용.

## 저장소 규칙
- 역할별 브랜치(`firmware` / `vision` / `ros2`)에서 **자기 폴더만** 수정. `main`에 직접 push 금지, PR로만 합친다.
- 작업 전에는 자기 브랜치에서 `git pull origin main`.
- 공용 파일(`README.md`, `.gitignore`, `CLAUDE.md`, `docs/PRD.md`)은 단톡방에 먼저 말하고 고친다.
- Wi-Fi 비밀번호는 `secrets.h` / `.env` (깃 제외). `.pio/`, `.vscode/`, ROS 2 build/install/log 제외.
- 커밋, 푸시는 사용자가 요청할 때만.

## 폴더별 맥락
- 각 폴더의 `CLAUDE.md`에 그 역할의 세부 맥락을 둔다 (예: `vision/CLAUDE.md`). 필요하면 담당자가 `firmware/CLAUDE.md`, `ros2_ws/CLAUDE.md`를 만든다.
- 개인 메모는 `CLAUDE.local.md` (깃 제외).
