#include "http_server_app.h"
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include <esp_http_server.h>
#include "cJSON.h"
#include <sys/cdefs.h>

#define JSON_USERNAME_KEY "ssid"
#define JSON_PASSWORD_KEY "pass"
static http_handle_data_t http_data_handle = NULL;

static const char *TAG = "HTTP_SERVER";
static httpd_handle_t server = NULL;

extern const uint8_t config_wifi_html_start[] asm("_binary_config_wifi_html_start");
extern const uint8_t config_wifi_html_end[] asm("_binary_config_wifi_html_end");

int json_login_deserialize(char *input_data, char *user, char *password)
{
    cJSON *item;
    cJSON *root = cJSON_Parse(input_data);
    if (root == NULL)
    {
        ESP_LOGI(TAG, "Can't parse login json");
        return 1;
    }
    else
    {
        /* Get version info */
        item = cJSON_GetObjectItem(root, JSON_USERNAME_KEY);
        sprintf(user, "%s", item->valuestring);

        item = cJSON_GetObjectItem(root, JSON_PASSWORD_KEY);
        sprintf(password, "%s", item->valuestring);

        cJSON_Delete(root);

        return 0;
    }
}
/* An HTTP GET handler */
static esp_err_t web_get_handler(httpd_req_t *req)
{
    /* Send response with custom headers and body set as the
     * string passed in user context*/
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)config_wifi_html_start, config_wifi_html_end - config_wifi_html_start);
    httpd_resp_send(req, NULL, 0);

    return ESP_OK;
}

static const httpd_uri_t web_config = {
    .uri = "/UIweb",
    .method = HTTP_GET,
    .handler = web_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx = "Hello World!"};

/* An HTTP POST handler */
static esp_err_t wifi_post_handler(httpd_req_t *req)
{
    char buf[100];
    esp_err_t ret;
    int32_t remaining = req->content_len;


    while (remaining > 0)
    {
        /* Read the data for the request */
        if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0)
        {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT)
            {
                /* Retry receiving if timeout occurred */
                continue;
            }
            return ESP_FAIL;
        }

        http_data_handle(buf);
        remaining -= ret;
    }

    // End response
    httpd_resp_send_chunk(req, NULL, 0);

    return ESP_OK;
}

static const httpd_uri_t receive_wifi_info = {
    .uri = "/wifi_info",
    .method = HTTP_POST,
    .handler = wifi_post_handler,
    .user_ctx = NULL};

void http_setCallback(void *callback)
{
    if (callback != NULL)
    {
        http_data_handle = callback;
    }
}

esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (strcmp("/UIweb", req->uri) == 0)
    {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/UIweb URI is not available");
        /* Return ESP_OK to keep underlying socket open */
        return ESP_OK;
    }
    else if (strcmp("/wifi_info", req->uri) == 0)
    {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/wifi_info URI is not available");
        /* Return ESP_FAIL to close underlying socket */
        return ESP_FAIL;
    }
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);

    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &web_config);
        httpd_register_uri_handler(server, &receive_wifi_info);
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler);
    }
    else
    {
        ESP_LOGI(TAG, " Error starting server!");
    }
}

void stop_webserver(void)
{
    // Stop the httpd server
    httpd_stop(server);
}
