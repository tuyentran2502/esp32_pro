#include "mqtt.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_log.h"
#include "mqtt_client.h"

static esp_mqtt_client_handle_t client;
static mqtt_handle_data_t mqtt_data_handle = NULL;
static const char *TAG = "MQTT_APP";

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    client = event->client;

    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        MQTT_subscribe("/topic/Tuyen");
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        MQTT_publish("/topic/Tuyen", "hello", 5);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        mqtt_data_handle(event->data, event->data_len);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    
    return ESP_OK;
}

void MQTT_init(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://broker.hivemq.com:1883",
        .event_handle = mqtt_event_handler,
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
}

void MQTT_appStart(void)
{
    esp_mqtt_client_start(client);
}

void MQTT_setCallback(void *callback)
{
    if (callback != NULL)
    {
        mqtt_data_handle = callback;
    }
}

void MQTT_subscribe(char *topic)
{
    esp_mqtt_client_subscribe(client, topic, 1);
}

void MQTT_publish(char *topic, char *data, int len)
{
    esp_mqtt_client_publish(client, topic, data, len, 1, 0);
}
