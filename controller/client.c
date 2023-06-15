#include <M5StickCPlus.h>
#include <esp_http_client.h>
#include <WiFi.h>
#include "time.h"
#include "esp_log.h"
#include <string.h>

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
// const char* host_raspi = "192.168.10.108";
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


static void migrate(const host_info_t* host_info, char* to_url) {
  char local_response_buffer[BUFFER_MAX_SIZE] = {0};

  char* query_str = "host=";
  strcat(query_str, to_url);

  esp_http_client_config_t config = {
    .host = host_info->host,
    .port = host_info->port,
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
    delay(3000);

    char* to = (char*)(host_popos2.host);
    strcat(to, ":8001");
    migrate(&host_popos, to);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0, 2);
    delay(3000);

    restore_app(&host_popos);
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
