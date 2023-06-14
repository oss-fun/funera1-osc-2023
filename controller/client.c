#include <M5StickCPlus.h>
#include <esp_http_client.h>
#include <WiFi.h>
#include "time.h"

const char* ssid = "**";
const char* password = "**";
const int BUFFER_MAX_SIZE = 2048;

void timer() {
 struct tm timeInfo;
  getLocalTime(&timeInfo);

  char now[20];
  sprintf(now, "%04d/%02d/%02d %02d:%02d:%02d",
    timeInfo.tm_year + 1900,
    timeInfo.tm_mon + 1,
    timeInfo.tm_mday,
    timeInfo.tm_hour,
    timeInfo.tm_min,
    timeInfo.tm_sec
  );

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.println(now);

  delay(1000); 
}

// static void http_rest_with_url(void)
// {
//   char local_response_buffer[BUFFER_MAX_SIZE] = {0};

//   esp_http_client_config_t config = {
//     .host = "192.168.12.138",
//     .port = 8000,
//     .path = "/",
//     .user_data = local_response_buffer,
//   };
//   esp_http_client_handle_t client = esp_http_client_init(&config);
//   if (client == NULL) {
//     M5.Lcd.println("Failed to init.");
//     return;
//   }

//   esp_err_t err = esp_http_client_perform(client);
//   if (err == ESP_OK) {
//     M5.Lcd.println("OK");
//     M5.Lcd.println(local_response_buffer);
//   }
//   else {
//     M5.Lcd.print(esp_err_to_name(err));
//   }

//   delay(100000);
// }

static void start_app() {
  const int MAX_SIZE = 2048;
  char local_response_buffer[BUFFER_MAX_SIZE] = {0};

  esp_http_client_config_t config = {
    .host = "192.168.12.138",
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
    M5.Lcd.println(local_response_buffer);
  }
  else {
    M5.Lcd.print(esp_err_to_name(err));
  }
}

static void stop_app() {
  const int MAX_SIZE = 2048;
  char local_response_buffer[BUFFER_MAX_SIZE] = {0};

  esp_http_client_config_t config = {
    .host = "192.168.12.138",
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
    M5.Lcd.println(local_response_buffer);
  }
  else {
    M5.Lcd.print(esp_err_to_name(err));
  }
}

static void restore_app() {
  const int MAX_SIZE = 2048;
  char local_response_buffer[BUFFER_MAX_SIZE] = {0};

  esp_http_client_config_t config = {
    .host = "192.168.12.138",
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
    M5.Lcd.println("Stop application.");
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
    start_app();
}


void loop() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  
  // delay(10000);
  M5.update();
  if (M5.BtnA.wasPressed()) {
    stop_app();
    delay(10000);
    restore_app();
  }
}
