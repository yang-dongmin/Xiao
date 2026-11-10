
#include <Arduino.h>
#include <WiFi.h>
#include "esp_camera.h"
#include "esp_http_server.h"
#include "secrets.h"

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

httpd_handle_t webServer = NULL;
httpd_handle_t streamServer = NULL;

// 웹페이지
static esp_err_t index_handler(httpd_req_t *req) {
    const char *html =
        "<html><body>"
        "<h2>XIAO ESP32S3 Camera</h2>"
        "<p>Camera streaming is ready.</p>"
        "<img src='http://' id='camera' width='320'>"
        "<script>"
        "document.getElementById('camera').src="
        "'http://' + location.hostname + ':81/stream';"
        "</script>"
        "</body></html>";

    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);
}

// MJPEG 실시간 스트리밍
static esp_err_t stream_handler(httpd_req_t *req) {
    esp_err_t res = httpd_resp_set_type(
        req,
        "multipart/x-mixed-replace;boundary=frame"
    );

    if (res != ESP_OK) {
        return res;
    }

    while (true) {
        camera_fb_t *fb = esp_camera_fb_get();

        if (!fb) {
            Serial.println("Camera capture failed");
            return ESP_FAIL;
        }

        char header[128];

        int headerLength = snprintf(
            header,
            sizeof(header),
            "--frame\r\n"
            "Content-Type: image/jpeg\r\n"
            "Content-Length: %u\r\n\r\n",
            (unsigned int)fb->len
        );

        res = httpd_resp_send_chunk(
            req, header, headerLength
        );

        if (res == ESP_OK) {
            res = httpd_resp_send_chunk(
                req,
                (const char *)fb->buf,
                fb->len
            );
        }

        if (res == ESP_OK) {
            res = httpd_resp_send_chunk(
                req, "\r\n", 2
            );
        }

        esp_camera_fb_return(fb);

        if (res != ESP_OK) {
            break;
        }

        delay(30);
    }

    return res;
}

// 웹서버 시작
void startCameraServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_uri_t index_uri = {};
    index_uri.uri = "/";
    index_uri.method = HTTP_GET;
    index_uri.handler = index_handler;

    httpd_uri_t stream_uri = {};
    stream_uri.uri = "/stream";
    stream_uri.method = HTTP_GET;
    stream_uri.handler = stream_handler;

    // 포트 80: 웹페이지
    config.server_port = 80;

    if (httpd_start(&webServer, &config) == ESP_OK) {
        httpd_register_uri_handler(webServer, &index_uri);
        Serial.println("Web server started");
    } else {
        Serial.println("Web server failed");
    }

    // 포트 81: MJPEG 스트림
    config.server_port = 81;
    config.ctrl_port += 1;

    if (httpd_start(&streamServer, &config) == ESP_OK) {
        httpd_register_uri_handler(
            streamServer, &stream_uri
        );
        Serial.println("Stream server started");
    } else {
        Serial.println("Stream server failed");
    }
}

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("Camera WebServer Start");

    // 카메라 설정
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

    // QVGA 320x240
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 2;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.grab_mode = CAMERA_GRAB_LATEST;

    // 카메라 초기화
    esp_err_t err = esp_camera_init(&config);

    if (err != ESP_OK) {
        Serial.printf("Camera init failed: 0x%x\n", err);
        return;
    }

    Serial.println("Camera initialized!");

    // 휴대폰 핫스팟 연결
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi");

    unsigned long started = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - started < 20000) {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nWiFi connection failed!");
        return;
    }

    Serial.println("\nWiFi connected!");

    // 웹서버 시작
    startCameraServer();

    Serial.print("Camera page: http://");
    Serial.println(WiFi.localIP());

    Serial.print("Stream URL: http://");
    Serial.print(WiFi.localIP());
    Serial.println(":81/stream");
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("WiFi connected! IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("WiFi disconnected!");
    }

    delay(5000);
}
