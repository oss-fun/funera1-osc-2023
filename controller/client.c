#include <M5StickCPlus.h>
#include <esp_http_client.h>
#include <WiFi.h>
#include "time.h"
#include "esp_log.h"

#if 1
const char* ssid = "";
const char* password = "";
const char* host_popos = "";
// const char* host_raspi = "";
#endif

#if 0
const char* ssid = "";
const char* password = "";
const char* host_popos = "";
const char* host_raspi = "";
#endif

const int BUFFER_MAX_SIZE = 2048;
static const char *TAG = "HTTP_CLIENT";


static void test_api(const char*);

static void start_app(const char* hostname) {
  const int MAX_SIZE = 2048;
  char local_response_buffer[BUFFER_MAX_SIZE] = {0};

  esp_http_client_config_t config = {
    .host = hostname,
    .port = 8000,
    .path = "/app/start",
    .method = HTTP_METHOD_POST,
    .user_data = local_response_buffer,
  };
  esp_http_client_handle_t client = esp_http_client_init(&config);
  if (client == NULL) {
    M5.Lcd.println("Failed to init.");
    return;
  }

  esp_err_t err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    M5.Lcd.println("Start application");
  }
  else {
    M5.Lcd.print(esp_err_to_name(err));
    return;
  }
}

static void stop_app(const char* hostname) {
  char local_response_buffer[BUFFER_MAX_SIZE] = {0};

  esp_http_client_config_t config = {
    .host = hostname,
    .port = 8000,
    .path = "/app/stop",
    .method = HTTP_METHOD_POST,
    .user_data = local_response_buffer,
  };
  esp_http_client_handle_t client = esp_http_client_init(&config);
  if (client == NULL) {
    M5.Lcd.println("Failed to init.");
    return;
  }

  esp_err_t err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    M5.Lcd.println("Stop application.");
  }
  else {
    M5.Lcd.print(esp_err_to_name(err));
  }
}

static void restore_app(const char* hostname) {
  char local_response_buffer[BUFFER_MAX_SIZE] = {0};

  esp_http_client_config_t config = {
    .host = hostname,
    .port = 8000,
    .path = "/app/restore",
    .method = HTTP_METHOD_POST,
    .user_data = local_response_buffer,
  };
  esp_http_client_handle_t client = esp_http_client_init(&config);
  if (client == NULL) {
    M5.Lcd.println("Failed to init.");
    return;
  }

  esp_err_t err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    // M5.Lcd.println("Restore application.");
  }
  else {
    M5.Lcd.print(esp_err_to_name(err));
  }
}


// imgsについての操作
static char* get_imgs(const char* hostname) {
  esp_http_client_config_t config = {
    .host = hostname,
    .port = 8000,
    .path = "/img",
    .method = HTTP_METHOD_GET,
  };
  esp_http_client_handle_t client = esp_http_client_init(&config);
  if (client == NULL) {
    M5.Lcd.println("Failed to init.");
    return "";
  }

  esp_err_t err = esp_http_client_open(client, 0);
  if (err == ESP_OK) {
    // M5.Lcd.println("Success open client.");
  }
  else {
    M5.Lcd.print(esp_err_to_name(err));
    return "";
  }

  // 受け取った返り値

  // M5.Lcd.println("Success init buffer");
  char* buffer;

  int content_length =  esp_http_client_fetch_headers(client);
  if (content_length < 0) {
    M5.Lcd.println("Failed to fetch headers");
    esp_http_client_close(client);
    return "";
  }

  buffer = (char*)malloc(content_length/20);
  if (buffer == NULL) {
    M5.Lcd.println("Failed to malloc");
    return "";
  }

  int read_len = esp_http_client_read(client, buffer, content_length);
  if (read_len <= 0) {
      M5.Lcd.println(read_len);
  }
  else {
    buffer[read_len] = 0;
  }

  esp_http_client_close(client);
  M5.Lcd.println("Success");
  return buffer;
}

static void post_imgs(const char* hostname, char* enc) {
  char local_response_buffer[BUFFER_MAX_SIZE] = {0};

  esp_http_client_config_t config = {
    .host = hostname,
    .port = 8000,
    .path = "/img",
    .method = HTTP_METHOD_POST,
    .user_data = enc,
  };
  esp_http_client_handle_t client = esp_http_client_init(&config);
  if (client == NULL) {
    M5.Lcd.println("Failed to init.");
    return;
  }

  esp_err_t err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    M5.Lcd.println("Get imgs.");
    M5.Lcd.println(local_response_buffer);
  }
  else {
    M5.Lcd.print(esp_err_to_name(err));
  }
}

void setup() {
    M5.begin(); 
    M5.Lcd.setTextSize(3);  
    M5.Lcd.setRotation(3); 

    M5.Lcd.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      M5.Lcd.print(".");
    }
    M5.Lcd.println(" CONNECTED");
    // start_app(host_popos);
}



void loop() {
  // M5.Lcd.fillScreen(BLACK);
// M5.Lcd.setCursor(0, 0, 2);

  delay(1000);
  M5.update();
  if (M5.BtnA.wasPressed()) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0, 2);

    // マイグレーション
    // stop_app(host_popos);
    // delay(3000);

    // char img[1];
    get_imgs(host_popos);
  //   M5.Lcd.fillScreen(BLACK);
  //   M5.Lcd.setCursor(0, 0, 2);  
    delay(3000);

  //   post_imgs(host_popos, img);
  //   M5.Lcd.fillScreen(BLACK);
  //   M5.Lcd.setCursor(0, 0, 2);
  //   delay(3000);

  //   restore_app(host_popos);
  }


}

static void test_api(const char* hostname) {
  const int MAX_SIZE = 2048;
  char local_response_buffer[MAX_SIZE] = {0};

  esp_http_client_config_t config = {
    .host = hostname,
    .port = 8000,
    .path = "/",
    .method = HTTP_METHOD_GET,
    // .user_data = local_response_buffer,
  };
  esp_http_client_handle_t client = esp_http_client_init(&config);
  if (client == NULL) {
    M5.Lcd.println("Failed to init.");
    return;
  }

  // 画面をclear
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);

  // esp_err_t err = esp_http_client_perform(client);
  esp_err_t err = esp_http_client_open(client, 0);
  if (err == ESP_OK) {
    M5.Lcd.println("Success");
  }
  else {
    M5.Lcd.print(esp_err_to_name(err));
    return;
  }

  // 受け取った返り値をprint
  char buffer[512];
  int content_length =  esp_http_client_fetch_headers(client);
  M5.Lcd.println(content_length);
  int total_read_len = 0, read_len;
  if (total_read_len < content_length && content_length <= 512) {
      read_len = esp_http_client_read(client, buffer, content_length);
      if (read_len <= 0) {
          ESP_LOGE(TAG, "Error read data");
      }
      buffer[read_len] = 0;
      M5.Lcd.println(buffer);
  }
  else {
    M5.Lcd.println("Can't println content.");
  }

  esp_http_client_close(client);
}
