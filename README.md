# XIAO ESP32S3 엣지 비전 + ROS 2

병렬컴퓨팅 팀 프로젝트. XIAO ESP32S3 Sense 카메라 영상을 Wi-Fi로 PC에 보내고,
PC에서 AI로 사람을 탐지해 결과를 ROS 2 토픽(`/vision/detection`)으로 발행한다.

```
[XIAO 카메라] --Wi-Fi--> [PC: OpenCV + AI] --> [ROS 2 토픽 /vision/detection]
```

## 역할

| 역할 | 담당 | 폴더 |
|---|---|---|
| 보드 · 펌웨어 | 동혁 | `firmware/` |
| ROS 2 · 통합 | 민혁 | `ros2_ws/` |
| 영상처리 · AI · 발표 | 동민 | `vision/`, `docs/` |

## 폴더 구조

```
firmware/            보드 코드 (실습별로 폴더 하나씩)
  blink/             1~2단계: LED 깜빡이기
  wifi_softap/       1~2단계: 핫스팟 웹페이지로 LED 제어
  camera_capture/    3단계: 사진 찍어 SD 저장
  camera_webserver/  4단계: 브라우저로 실시간 영상
vision/              PC 쪽 Python 코드 (영상 받기, 사람 탐지)
ros2_ws/src/         ROS 2 패키지
docs/weekly/         주차별 시연 기록
```

## 주차별 진행

| 수업일 | 단계 | 완료 기준 | 상태 |
|---|---|---|---|
| 11/4 | 1~2 설치 · Wi-Fi | Blink, 핫스팟 웹페이지로 LED 제어 | |
| 11/11 | 3~4 카메라 · 스트리밍 | SD에 사진 저장, 브라우저로 실시간 영상 | |
| 11/18 | 5-① PC로 영상 + ROS 2 | PC 창에 영상, ROS 2 토픽 확인 | |
| 11/25 | 5-② AI 판단 | 사람 탐지 박스와 방향 출력 | |
| 12/2 | 5-③ ROS로 | 사람이 보이면 `/vision/detection` 발행 | |
| 12/9 | 통합 점검 + 보강 | 전체 흐름 점검 | |
| 12/16 | 최종 시연 | 카메라부터 ROS 2 토픽까지 | |

## 작업 규칙

1. 작업 시작 전에 `git pull`
2. **자기 폴더만** 수정 (남의 폴더를 고쳐야 하면 단톡방에 먼저 말하기)
3. 시연에 성공하면 바로 커밋: `git commit -m "11/11 시연: 브라우저 스트리밍 성공"`
4. Wi-Fi 비밀번호는 코드에 직접 쓰지 말고 `secrets.h` / `.env`에 따로 (깃에 안 올라감)

## 참고 자료

- 과제 로드맵: `xiaoesp.pdf`
- 보드 사양: `보드.pdf`
- [Seeed 위키: 시작하기](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/)
- [Seeed 위키: 카메라](https://wiki.seeedstudio.com/xiao_esp32s3_camera_usage/)
- [Seeed 위키: Wi-Fi](https://wiki.seeedstudio.com/xiao_esp32s3_wifi_usage/)
