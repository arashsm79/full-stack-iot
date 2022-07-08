#include "cJSON.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_tls.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_netif.h>
#include <esp_wifi.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "stdlib.h"
#include "time.h"
#include "wifi_manager.h"

#define GATEWAY_IP "192.168.4.1"
#define GATEWAY_ENDPOINT "/device"

#define EVENT_QUEUE_LENGTH 5
#define MAX_HTTP_OUTPUT_BUFFER 2048

static const char TAG[] = "main";
char device_name[20] = {0};

QueueHandle_t sensor_data_queue;

typedef enum sensor_data_type_t { SENSOR_TEM_HUM_DATA } sensor_data_type_t;

#define mainQUEUE_SENSOR_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define mainQUEUE_GATEWAY_CLIENT_PRIORITY (tskIDLE_PRIORITY + 1)

typedef struct sensor_data_queue_message_t {
  sensor_data_type_t sensor_data_type;
  void *param;
} sensor_data_queue_message_t;

typedef struct luminance_sensor_data_t {
  uint32_t luminance;
} luminance_sensor_data_t;

uint16_t simulate_rand_sensor(uint32_t min_temp, uint32_t max_temp) {
  return ((float)((float)rand() / (float)RAND_MAX) *
          (max_temp + 1 - min_temp)) +
         min_temp;
}

esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
  static char *output_buffer; // Buffer to store response of http request from
                              // event handler
  static int output_len;      // Stores number of bytes read
  switch (evt->event_id) {
  case HTTP_EVENT_ERROR:
    ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
    break;
  case HTTP_EVENT_ON_CONNECTED:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
    break;
  case HTTP_EVENT_HEADER_SENT:
    ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
    break;
  case HTTP_EVENT_ON_HEADER:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key,
             evt->header_value);
    break;
  case HTTP_EVENT_ON_DATA:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
    /*
     *  Check for chunked encoding is added as the URL for chunked encoding used
     * in this example returns binary data. However, event handler can also be
     * used in case chunked encoding is used.
     */
    if (!esp_http_client_is_chunked_response(evt->client)) {
      // If user_data buffer is configured, copy the response into the buffer
      if (evt->user_data) {
        memcpy(evt->user_data + output_len, evt->data, evt->data_len);
      } else {
        if (output_buffer == NULL) {
          output_buffer =
              (char *)malloc(esp_http_client_get_content_length(evt->client));
          output_len = 0;
          if (output_buffer == NULL) {
            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
            return ESP_FAIL;
          }
        }
        memcpy(output_buffer + output_len, evt->data, evt->data_len);
      }
      output_len += evt->data_len;
    }

    break;
  case HTTP_EVENT_ON_FINISH:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
    if (output_buffer != NULL) {
      // Response is accumulated in output_buffer. Uncomment the below line to
      // print the accumulated response ESP_LOG_BUFFER_HEX(TAG, output_buffer,
      // output_len);
      free(output_buffer);
      output_buffer = NULL;
    }
    output_len = 0;
    break;
  case HTTP_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
    int mbedtls_err = 0;
    esp_err_t err = esp_tls_get_and_clear_last_error(
        (esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
    if (err != 0) {
      ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
      ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
    }
    if (output_buffer != NULL) {
      free(output_buffer);
      output_buffer = NULL;
    }
    output_len = 0;
    break;
  }
  return ESP_OK;
}

static void http_post_gateway(const char *post_data) {
  // char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};

  esp_http_client_config_t config = {
      .host = GATEWAY_IP,
      .path = GATEWAY_ENDPOINT,
      .transport_type = HTTP_TRANSPORT_OVER_TCP,
      .event_handler = _http_event_handler,
  };
  // .user_data = local_response_buffer, // Pass address of local buffer to get
  // response
  esp_http_client_handle_t client = esp_http_client_init(&config);

  // POST
  esp_http_client_set_method(client, HTTP_METHOD_POST);
  esp_http_client_set_post_field(client, post_data, strlen(post_data));
  esp_err_t err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    ESP_LOGI(TAG, "Successfully made POST request with data: %s", post_data);
    ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %d",
             esp_http_client_get_status_code(client),
             esp_http_client_get_content_length(client));
  } else {
    ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
  }

  esp_http_client_cleanup(client);
}

// gateway client handler tasks
void gateway_client_task(void *pvParameter) {
  ESP_LOGI(TAG, "Gateway client task started.");
  sensor_data_queue_message_t sensor_data_queue_message;
  for (;;) {
    xQueueReceive(sensor_data_queue, &sensor_data_queue_message, portMAX_DELAY);
    ESP_LOGI(TAG, "Received sensor data with type: %d from the queue.",
             sensor_data_queue_message.sensor_data_type);
    switch (sensor_data_queue_message.sensor_data_type) {
    case SENSOR_TEM_HUM_DATA:;
      luminance_sensor_data_t *luminance_sensor_data =
          (luminance_sensor_data_t *)sensor_data_queue_message.param;
      ESP_LOGI(TAG, "Sent sensor data: luminance=%d to the gateway.",
               luminance_sensor_data->luminance);
      cJSON *req_body_json;
      req_body_json = cJSON_CreateObject();
      cJSON_AddStringToObject(req_body_json, "name", device_name);
      cJSON_AddNumberToObject(req_body_json, "timestamp", time(NULL));
      cJSON_AddNumberToObject(req_body_json, "luminance",
                              luminance_sensor_data->luminance);
      char *req_body_str = cJSON_Print(req_body_json);
      cJSON_Delete(req_body_json);
      http_post_gateway(req_body_str);
      cJSON_free(req_body_str);

      free(luminance_sensor_data);
      break;
    default:
      break;
    }
  }
  vTaskDelete(NULL);
}

// sensor reading tasks
void sensor_task(void *pvParameter) {
  ESP_LOGI(TAG, "Sensor task started.");
  for (;;) {
    sensor_data_queue_message_t sensor_data_queue_message;
    luminance_sensor_data_t *luminance_sensor_data =
        malloc(sizeof(luminance_sensor_data_t));
    luminance_sensor_data->luminance = simulate_rand_sensor(30, 50);
    sensor_data_queue_message.sensor_data_type = SENSOR_TEM_HUM_DATA;
    sensor_data_queue_message.param = luminance_sensor_data;
    xQueueSend(sensor_data_queue, &sensor_data_queue_message, 0);
    ESP_LOGI(TAG, "Sent sensor data: luminance=%d to queue.",
             luminance_sensor_data->luminance);
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
  vTaskDelete(NULL);
}

void cb_connection_ok(void *pvParameter) {
  // log the acquired ip
  ip_event_got_ip_t *param = (ip_event_got_ip_t *)pvParameter;
  char str_ip[16];
  esp_ip4addr_ntoa(&param->ip_info.ip, str_ip, IP4ADDR_STRLEN_MAX);
  ESP_LOGI(TAG, "Connected to gateway with IP: %s!", str_ip);

  // start tasks to send http request to gateway
  sensor_data_queue =
      xQueueCreate(EVENT_QUEUE_LENGTH, sizeof(sensor_data_queue_message_t));

  xTaskCreate(&gateway_client_task, "gateway_client_task", 8192, NULL,
              mainQUEUE_GATEWAY_CLIENT_PRIORITY, NULL);
  xTaskCreate(&sensor_task, "sensor_task", 8192, NULL,
              mainQUEUE_SENSOR_TASK_PRIORITY, NULL);
}

void app_main() {

  // Get the derived MAC address for each network interface
  uint8_t derived_mac_addr[6] = {0};
  // Get MAC address for WiFi Station interface
  ESP_ERROR_CHECK(esp_read_mac(derived_mac_addr, ESP_MAC_WIFI_STA));
  sprintf(device_name, "%02x:%02x:%02x:%02x:%02x:%02x",
           derived_mac_addr[0], derived_mac_addr[1], derived_mac_addr[2],
           derived_mac_addr[3], derived_mac_addr[4], derived_mac_addr[5]);
  ESP_LOGI("WIFI_STA MAC", "%02x:%02x:%02x:%02x:%02x:%02x",
           derived_mac_addr[0], derived_mac_addr[1], derived_mac_addr[2],
           derived_mac_addr[3], derived_mac_addr[4], derived_mac_addr[5]);
  wifi_manager_start();
  wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_connection_ok);
}
