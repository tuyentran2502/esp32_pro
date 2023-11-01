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

void http_receive_wifi_info_callback(char *buf)
{
    nvs_handle_t nvs_handle;
    char username_rev[32];
    char password_rev[64];

    memset(username_rev, 0, sizeof(username_rev));
    memset(password_rev, 0, sizeof(password_rev));

    ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
    ESP_LOGI(TAG, "%s\n", buf);
    ESP_LOGI(TAG, "====================================");

    if (json_login_deserialize(buf, username_rev, password_rev) == ESP_OK)
    {
        ESP_LOGI(TAG, "user name: %s", username_rev);
        ESP_LOGI(TAG, "password: %s", password_rev);
    }

    /* Initialize NVS */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    ESP_ERROR_CHECK(err);
    /*save to the nvs flash. */
    err = nvs_open("wifi_config", NVS_READWRITE, &nvs_handle);
    ESP_ERROR_CHECK(err);

    err = nvs_set_str(nvs_handle, "ssid", username_rev);
    ESP_ERROR_CHECK(err);

    err = nvs_set_str(nvs_handle, "password", password_rev);
    ESP_ERROR_CHECK(err);

    err = nvs_commit(nvs_handle);
    ESP_ERROR_CHECK(err);

    esp_restart();
}

void app_main(void)
{
    nvs_handle_t nvs_handle;
    size_t ssidLen, passLen;
    char *ssid = NULL, *pass = NULL;

    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (nvs_get_str(nvs_handle, "pass", ssid, &ssidLen) == ESP_OK && nvs_get_str(nvs_handle, "pass", pass, &passLen) == ESP_OK)
    {
        WIFI_initStationMode(wifi_STA_default_config);
    }
    else
    {
        WIFI_initAccessPointMode(wifi_AP_default_config);
    }
    start_webserver();
    http_setCallback(http_receive_wifi_info_callback);

}
