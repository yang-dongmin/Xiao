# Seeed 위키 실습 요약 (1~4단계)

우리 팀이 하는 실습만 골라 정리했다. 전체 코드는 위키 원문을 본다.
2026-11-09 기준으로 위키를 읽고 정리함 (동민).

- [시작하기](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/)
- [Wi-Fi](https://wiki.seeedstudio.com/xiao_esp32s3_wifi_usage/)
- [카메라](https://wiki.seeedstudio.com/xiao_esp32s3_camera_usage/)

---

## 0. 공통 준비 (Arduino IDE 기준)

1. Arduino IDE > Preferences > "Additional Boards Manager URLs"에 아래 주소 추가
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
2. Boards Manager에서 `esp32` (Espressif) 설치. **2.0.8 이상** 필요
3. 보드: `XIAO_ESP32S3`
4. 포트: Mac은 `usbmodem` / `usbserial`이 들어간 것. 헷갈리면 보드를 뺐다 꽂아서 사라지는 항목을 고른다
5. 시리얼 모니터 속도: **115200**

**안테나를 꼭 단다.** 위키에 안테나가 없으면 Wi-Fi 연결이 안 될 수 있다고 나와 있다. 끼울 때는 한쪽을 먼저 맞추고 반대쪽을 눌러서 끼운다. 뺄 때 위로 잡아당기지 않는다.

**업로드가 안 될 때 (포트가 안 보이거나 업로드 실패)**
1. 먼저 Reset 버튼을 누른다
2. 그래도 안 되면 BOOT 버튼을 누른 채로 USB를 꽂고, 꽂은 뒤에 BOOT를 뗀 다음 다시 업로드한다

---

## 1. Blink (getting_started)

- File > Examples > 01.Basics > Blink를 열어서 업로드
- LED는 주황색 User LED (GPIO21)
- **반대로 동작함 (active-low):** `LOW`가 켜짐, `HIGH`가 꺼짐
- 성공: 보드 오른쪽 주황 LED가 깜빡인다

## 2. Wi-Fi (wifi_usage)

### 2.1 Wi-Fi 스캔
- File > Examples > WiFi > WiFiScan
- 5초마다 주변 Wi-Fi 목록을 출력한다: `1: 이름 (-신호세기)`. 끝에 `*`가 붙으면 비밀번호가 있는 망
- 휴대폰 핫스팟이 목록에 보이는지 확인한다. **2.4GHz만 보인다**

### 2.2 Wi-Fi 연결 (Station 모드)
- 핵심 코드: `WiFi.mode(WIFI_STA); WiFi.begin(ssid, password);` 실행 후 연결될 때까지 대기
- 연결되면 `WiFi.localIP()`로 IP를 출력한다. 이 IP를 4단계에서 쓴다
- 우리 저장소에서는 ssid와 password를 `secrets.h`에 둔다 ([firmware/secrets.example.h](../firmware/secrets.example.h) 참고)

### 2.3 softAP로 LED 제어 ✅ 11/4 시연
- File > Examples > WiFi > WiFiAccessPoint. 위키 버전은 `#define LED_BUILTIN 2` 줄을 주석 처리했다 (보드 내장 LED 사용)
- 보드가 핫스팟이 된다. 기본값은 SSID `XIAO_ESP32S3`, 비밀번호 `password`
- 테스트 방법
  1. 업로드 후 시리얼 모니터에서 `AP IP address: 192.168.4.1`과 `Server started`가 나오는지 확인
  2. 휴대폰을 `XIAO_ESP32S3`에 연결
  3. 브라우저에서 `192.168.4.1` 접속 → "here" 링크로 LED 켜기/끄기 (`/H` = 켜기, `/L` = 끄기)

**Wi-Fi가 약할 때 (위키 FAQ)**
- softAP 예제에서 칩이 약 50°C까지 올라간다. 연결이 안 되면 식힌 뒤 다시 시도
- 2025년 8월 이전 제품은 신호가 약할 수 있다. 이때는 `WiFi.begin()` 전에 송신 출력을 조정한다:
  ```cpp
  WiFi.mode(WIFI_STA);
  esp_wifi_set_max_tx_power((int8_t)(15 / 0.25));  // 15 dBm
  WiFi.begin(ssid, password);
  ```

## 3. 카메라 (camera_usage)

### 3.0 카메라 공통 설정
- **PSRAM 켜기:** Tools > PSRAM > **OPI PSRAM**. 안 켜면 고해상도를 못 쓰고 얼굴 탐지도 안 된다
- **Partition Scheme:** APP 공간이 **3MB 이상**인 것을 고른다 (위키 경고)
- 코드 맨 위: `#define CAMERA_MODEL_XIAO_ESP32S3` + `#include "camera_pins.h"`
- **카메라 센서:** 위키 기준 현재 기본은 **OV5640**이고, OV3660이나 OV2640이 올 수도 있다. 위키 예제는 셋 다 지원한다. ⚠️ 실물 확인 필요
- **추가 라이브러리:** 예제가 `ESP32_OV5640_AF.h`를 include한다 → [github.com/0015/ESP32-OV5640-AF](https://github.com/0015/ESP32-OV5640-AF) 설치 필요
- **microSD:** 32GB 이하, **FAT32로 포맷**. 코드는 `SD.begin(21)` 사용

### 3.1 사진 찍어 SD에 저장 ✅ 11/11 시연
- **1분마다 자동으로** 한 장씩 찍는다 (버튼 아님). 시작하면 "Photos will begin in one minute" 출력
- 파일 이름: `/image1.jpg`, `/image2.jpg`, …
- 확인: SD 카드를 빼서 카드 리더기로 PC에서 연다

### 3.2 짧은 영상 녹화 (선택)
- 1분마다 10초씩 `/video0.avi`, `/video1.avi`, … 로 저장한다
- JPEG 사진을 이어 붙인 형식(MJPEG AVI)이다. 화면에 변화가 없으면 1초만 재생될 수 있다

### 3.3 CameraWebServer 스트리밍 ✅ 11/11 시연
- 위키가 링크한 GitHub 예제 폴더 중 **ESP32 코어 버전에 맞는 것**을 받는다
  - 코어 2.0.x → `CameraWebServer_for_esp-arduino_2.0.x`
  - 코어 3.0.x → `CameraWebServer_for_esp-arduino_3.0.x`
  - 설치한 코어 버전은 Boards Manager에서 확인한다
- 수정할 곳: `ssid`, `password` (모델 define은 이미 XIAO로 되어 있음)
- 업로드 → 시리얼 모니터(115200)에 `Camera Ready! Use 'http://<IP>' to connect` 출력
- **PC와 보드가 같은 핫스팟에 연결되어 있어야 한다.** Chrome에서 `http://<IP>` 접속 → 왼쪽 아래 **Start Stream**
- 얼굴 탐지: 웹 화면에서 Face Detection 스위치를 켠다. **해상도는 CIF 이하**여야 한다 (더 높으면 에러)
- 시작 해상도는 프레임 속도를 위해 QVGA(320×240)로 설정되어 있다

**PC에서 받을 주소 (5단계, 동민 · 민혁)**
- 웹 화면은 `http://<IP>`(80번 포트)이고, 영상 스트림 자체는 `http://<IP>:81/stream`이다
- 위키에는 이 주소가 안 적혀 있다. ESP32 CameraWebServer 예제 코드의 기본 구조(스트림 서버 = 80+1번 포트)를 근거로 했다. 11/11에 브라우저로 직접 열어서 확인한다 ⚠️

### 3.4 발열 주의
위키에 따르면 스트리밍을 오래 하면 **매우 뜨거워진다 (화상 주의)**. 방열판도 권장한다. 줄이는 방법:
- CPU 240MHz → 160MHz (`setCpuFrequencyMhz(160);` 또는 Tools > CPU Frequency)
- 해상도 한 단계 낮추기
- `fb_count = 1`, `grab_mode = CAMERA_GRAB_WHEN_EMPTY`

### 3.5 자주 나오는 메시지
| 메시지 | 뜻 / 확인할 것 |
|---|---|
| `Camera init failed with error 0x...` | 카메라 초기화 실패. PSRAM 설정, 확장보드 결합, 카메라 커넥터 확인 |
| `Card Mount Failed` / `No SD card attached` | SD 카드 인식 실패. FAT32 포맷, 32GB 이하인지 확인 |
| `MFN: Partition Not found` | 무시해도 됨 (위키) |
| 시리얼에 `.`만 계속 찍힘 | Wi-Fi 연결 중. 이름 · 비밀번호 · 2.4GHz 여부 · 안테나 확인 |

---

## 하드웨어 메모

- **확장보드(Sense) 분리:** 옆으로 밀어서 뺀다. 위로 잡아당기면 커넥터가 망가질 수 있다
- **공장 출하 펌웨어:** 처음 켜면 `XIAO_ESP32S3_Sense` / `seeedstudio` 핫스팟이 뜬다 (2025년 6월 이후 제품)
- **핀 충돌:** 위키 핀표에서 SD 카드 CS와 User LED가 둘 다 GPIO21로 나온다. SD를 쓰는 예제에서는 LED 동작이 이상할 수 있다
