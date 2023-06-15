#include <M5StickCPlus.h>
#include <esp_http_client.h>
#include <WiFi.h>
#include "time.h"
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>

typedef struct {
  char* host;
  int port;
} host_info_t;

// 家
#if 1
const char* ssid = "";
const char* password = "";
const host_info_t host_popos = {
  "",
  8000
};
const host_info_t host_popos2 = {
  "",
  8001
};
#endif

// 学校
#if 0
const char* ssid = "";
const char* password = "";
const host_info_t host_popos = {
  "",
  8000
};
const host_info_t host_raspi = {
  "",
  8000
};
#endif

const int BUFFER_MAX_SIZE = 2048;
static const char *TAG = "HTTP_CLIENT";

static void test_api(const char*);

static void start_app(const host_info_t* host_info) {
  const int MAX_SIZE = 2048;
  char local_response_buffer[BUFFER_MAX_SIZE] = {0};

  esp_http_client_config_t config = {
    .host = host_info->host,
    .port = host_info->port,
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

static void stop_app(const host_info_t* host_info) {
  char local_response_buffer[BUFFER_MAX_SIZE] = {0};

  esp_http_client_config_t config = {
    .host = host_info->host,
    .port = host_info->port,
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

static void restore_app(const host_info_t* host_info) {
  char local_response_buffer[BUFFER_MAX_SIZE] = {0};

  esp_http_client_config_t config = {
    .host = host_info->host,
    .port = host_info->port,
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

static char* add_str(char* a, char*b) {
  char* c = (char*)malloc(strlen(a) + strlen(b));
  strcat(c, a);
  strcat(c, b);
  return c;
}

static void migrate(const host_info_t* from_host, const host_info_t* to_host) {
  char local_response_buffer[BUFFER_MAX_SIZE] = {0};

  char query_str[128] = "host=";
  strcat(query_str, to_host->host);
  strcat(query_str, "&port=");
  char num[10];
  itoa(to_host->port, num, 10);
  strcat(query_str, num);

  esp_http_client_config_t config = {
    .host = from_host->host,
    .port = from_host->port,
    .path = "/app/migrate",
    .query = query_str,
    .method = HTTP_METHOD_POST,
  };
  esp_http_client_handle_t client = esp_http_client_init(&config);
  if (client == NULL) {
    M5.Lcd.println("Failed to init.");
    return;
  }

  esp_err_t err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    M5.Lcd.println("Success migration");
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
    // start_app(&host_popos);
}



void loop() {

  delay(1000);
  M5.update();
  if (M5.BtnA.wasPressed()) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0, 2);

    // マイグレーション
    stop_app(&host_popos);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0, 2);
    delay(2000);

    migrate(&host_popos, &host_popos2);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0, 2);
    delay(2000);

    restore_app(&host_popos);
  }
}

