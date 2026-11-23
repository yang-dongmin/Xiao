
#include <Arduino.h>
#include "esp_camera.h"
#include <SPI.h>
#include <SD.h>

// XIAO ESP32S3 Sense 카메라 핀
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  10
#define SIOD_GPIO_NUM  40
#define SIOC_GPIO_NUM  39

#define Y9_GPIO_NUM    48
#define Y8_GPIO_NUM    11
#define Y7_GPIO_NUM    12
#define Y6_GPIO_NUM    14
#define Y5_GPIO_NUM    16
#define Y4_GPIO_NUM    18
#define Y3_GPIO_NUM    17
#define Y2_GPIO_NUM    15

#define VSYNC_GPIO_NUM 38
#define HREF_GPIO_NUM  47
#define PCLK_GPIO_NUM  13

// microSD 카드 SPI 핀
#define SD_CS_PIN   21
#define SD_SCK_PIN   7
#define SD_MISO_PIN  8
#define SD_MOSI_PIN  9

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("Camera Capture Start");

    // 1. 카메라 설정
    camera_config_t config = {};

    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;

    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;

    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;

    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;

    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

    // 2. 카메라 초기화
    esp_err_t err = esp_camera_init(&config);

    if (err != ESP_OK) {
        Serial.printf("Camera init failed: 0x%x\n", err);
        return;
    }

    Serial.println("Camera initialized!");

    // 3. SD 카드 초기화
    SPI.begin(
        SD_SCK_PIN,
        SD_MISO_PIN,
        SD_MOSI_PIN,
        SD_CS_PIN
    );

    if (!SD.begin(SD_CS_PIN, SPI)) {
        Serial.println("SD Card init failed!");
        return;
    }

    Serial.println("SD Card initialized!");

    // 4. 사진 촬영
    camera_fb_t *fb = esp_camera_fb_get();

    if (fb == NULL) {
        Serial.println("Camera capture failed!");
        return;
    }

    Serial.println("Photo captured!");

    // 5. SD 카드에 저장
    File file = SD.open("/photo.jpg", FILE_WRITE);

    if (!file) {
        Serial.println("Failed to open file!");
        esp_camera_fb_return(fb);
        return;
    }

    size_t written = file.write(fb->buf, fb->len);
    file.close();

    if (written == fb->len) {
        Serial.println("Photo saved: /photo.jpg");
    } else {
        Serial.println("Photo save failed!");
    }

    // 6. 카메라 버퍼 반환
    esp_camera_fb_return(fb);
}

void loop() {
    // 사진은 부팅할 때 한 번만 촬영
}
