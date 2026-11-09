# vision/ — 영상처리 · AI (담당: 동민)

PC 쪽 Python 코드. XIAO 스트림(또는 웹캠)을 받아 사람을 탐지하고 방향을 판단한다.

## 개발 방식
- 보드가 1대(동혁 보유)라서 평소에는 **Mac 웹캠(`0`)으로 개발**하고, 스트림 주소만 바꿔서 XIAO에 붙인다.
  - `python stream_viewer.py` → 웹캠
  - `python stream_viewer.py http://<보드IP>:81/stream` → XIAO
- 의존성은 `requirements.txt`에 적는다. 가상환경(`.venv/`)은 깃 제외.
- ROS 2 발행 코드는 민혁과 맞물린다. 메시지 형식은 `docs/PRD.md` 5.2절을 따른다.

## 단계별 목표
| 수업일 | 할 것 | 완료 기준 |
|---|---|---|
| 11/18 | XIAO 스트림을 PC에서 받기 | `stream_viewer.py` 창에 XIAO 영상, FPS 표시 |
| 11/25 | 사람 탐지 + 방향 판단 | 사람 박스 · 신뢰도 · left/center/right 표시 |
| 12/2 | 탐지 결과 발행 | 사람이 보이면 `/vision/detection` 발행 (민혁과 통합) |

## 설계 메모
- 좌표 `x`, `y`는 박스 중심을 화면 크기로 나눈 0~1 값.
- 방향: `x < 0.33` → left, `x > 0.67` → right, 나머지 → center.
- 여러 명이면 신뢰도가 가장 높은 1명 (PRD에서 확정 전).
- 스트림이 끊기면 자동 재연결 (PRD 6절 안정성).
- 탐지 모델 미정: YOLO 계열 vs OpenCV HOG 등. 11/18까지 결정.
- 모델 파일(`*.pt`, `*.onnx`, `*.tflite`)은 깃 제외. 받는 방법을 README에 적는다.

## 발표 (docs/)
- 주차별 시연 기록: `docs/weekly/` (TEMPLATE.md 복사해서 사용). 막힌 점과 해결을 꼭 적는다 → 발표 자료.
- 병렬컴퓨팅 과목이므로 FPS, 지연 시간 같은 성능 수치를 측정해 두면 발표 근거가 된다.

## 동민 메모
- 동혁, 민혁에게 보낼 글은 단톡방용으로 짧게 쓴다.
- claude.ai 자료 (동민 계정으로만 열림)
  - 일정표 문서(Docs): https://claude.ai/artifact/P254G6cdmhEN31fHQRK5fT — 주차별 작업표, Windows WSL2 + ROS 2 설치 순서 포함
  - 발표 슬라이드(10/28 계획 발표): https://claude.ai/artifact/9c7GsC1QT5DgQ9C9xxnxBc

### 다음 할 일 (2026-11-09 기준)
- `stream_viewer.py`로 웹캠 확인 → FPS 표시 추가
- 사람 탐지 모델 고르기 (11/18까지) → 탐지 + 방향 판단 코드 시작
- 민혁에게 PRD 5.2 `/vision/detection` 메시지 형식 확인받기
- 11/11 실습 때 확인: 카메라 센서 모델, `:81/stream` 주소, 동혁 `platformio.ini` PSRAM 설정
