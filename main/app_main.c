#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt.h"
#include "wifi_app.h"
#include "http_server_app.h"

static const char *TAG = "EXAMPLE_APP";

static wifi_config_t wifi_STA_default_config = {
    .sta = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
        .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        .pmf_cfg = {
            .capable = true,
            .required = false},
    },
};

static wifi_config_t wifi_AP_default_config = {
    .ap = {
        .ssid = ESP_WIFI_SSID,
        .ssid_len = strlen(ESP_WIFI_SSID),
        .channel = WIFI_CHANNEL,
        .password = ESP_WIFI_PASS,
        .max_connection = MAX_CONN_CNT,
        .authmode = WIFI_AUTH_WPA_WPA2_PSK},
};

void mqtt_data_callback( char *data, int len)
{
    ESP_LOGI(TAG,"DATA : %s\n LEN : %d", data, len);
}

void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    WIFI_initAccessPointMode(wifi_AP_default_config);
    start_webserver();


    // if(WIFI_CONNECTED == WIFI_status())
    // {
    //     ESP_LOGI(TAG, "connected to ap SSID:%s password:%s", WIFI_SSID, WIFI_PASS);
    // }
    // else
    // {
    //     ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s", WIFI_SSID, WIFI_PASS);
    // }
    // MQTT_init();
    // MQTT_setCallback(mqtt_data_callback);
    // MQTT_appStart();
}
