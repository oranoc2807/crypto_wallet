#include <stdio.h>
#include "mqtt_client.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "MQTT_CLIENT";

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event) {
    esp_mqtt_client_handle_t client = event->client;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            esp_mqtt_client_subscribe(client, "wallet/falcon/incoming", 0);
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");

            char *msg = strndup(event->data, event->data_len);
            if (msg) {
                cJSON *root = cJSON_Parse(msg);
                if (root) {
                    cJSON *amount = cJSON_GetObjectItem(root, "amount");
                    if (cJSON_IsNumber(amount)) {
                        ESP_LOGI(TAG, "Received amount: %.2f", amount->valuedouble);
                    }
                    cJSON_Delete(root);
                }
                free(msg);
            }
            break;

        default:
            ESP_LOGI(TAG, "Other event id: %d", event->event_id);
            break;
    }
    return ESP_OK;
}

void mqtt_app_start(void) {
    const esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URI,  // set in sdkconfig or menuconfig
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler_cb, NULL);
    esp_mqtt_client_start(client);
}
