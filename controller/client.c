#include <M5StickCPlus.h>
#include <esp_http_client.h>
#include <WiFi.h>
#include "time.h"

const char* ssid = "**";
const char* password = "**";

// void timer() {
//  struct tm timeInfo;
//   getLocalTime(&timeInfo);
// 
//   char now[20];
//   sprintf(now, "%04d/%02d/%02d %02d:%02d:%02d",
//     timeInfo.tm_year + 1900,
//     timeInfo.tm_mon + 1,
//     timeInfo.tm_mday,
//     timeInfo.tm_hour,
//     timeInfo.tm_min,
//     timeInfo.tm_sec
//   );
// 
//   M5.Lcd.fillScreen(BLACK);
//   M5.Lcd.setCursor(0, 0, 2);
//   M5.Lcd.println(now);
// 
//   delay(1000); 
// }

static void http_rest_with_url(void)
{
  const int MAX_SIZE = 2048;
  char local_response_buffer[MAX_SIZE] = {0};

  esp_http_client_config_t config = {
    .host = "192.168.12.138",
    .port = 8000,
    .path = "/",
    .user_data = local_response_buffer,
  };
  esp_http_client_handle_t client = esp_http_client_init(&config);
  if (client == NULL) {
    M5.Lcd.println("Failed to init.");
    return;
  }

  esp_err_t err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    M5.Lcd.println("OK");
    M5.Lcd.println(local_response_buffer);
  }
  else {
    M5.Lcd.print(esp_err_to_name(err));
  }

  delay(100000);
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
    // configTime(9 * 3600, 0, "ntp.nict.jp");

    // http_rest_with_url();
}


void loop() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  http_rest_with_url();
}
