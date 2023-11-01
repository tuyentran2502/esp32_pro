/*
 *  hw_api.c
 *
 *  Created on: Jan 5, 2023
 */

/******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include "hw_api.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

#define WIFI_SSID_KEY                               "ssid"
#define WIFI_PASSWORD_KEY                           "password"

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

static const char* TAG = "HW";
static nvs_handle_t storage_nvs;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/



/******************************************************************************/
/*                                FUNCTIONS                                   */
/******************************************************************************/



/******************************************************************************/

/*!
 * @brief  Read data from NVS
 */
static esp_err_t hw_api_nvs_read(char *key, void *data, uint32_t *length){
    *length = 0;

    /* Probe data length */
    esp_err_t err = nvs_get_blob(storage_nvs, key, NULL, length);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        return err;
    }

    /* Read data */
    if(*length > 0){
        err = nvs_get_blob(storage_nvs, key, data, length);
    }
    return err;
}

/*!
 * @brief  Write data to NVS
 */
static esp_err_t hw_api_nvs_write(char *key, void *data, uint32_t length){
    esp_err_t err = nvs_set_blob(storage_nvs, key, data, length);

    /* Write data */
    if (err == ESP_OK) {
        /* Commit written value.
         * After setting any values, nvs_commit() must be called to ensure changes are written
         * to flash storage. Implementations may write to storage at other times,
         * but this is not guaranteed.
         */
        err = nvs_commit(storage_nvs);
    }

    return err;
}

/******************************************************************************/

/*!
 * @brief  Store wifi info to nvs
 */
esp_err_t hw_api_store_wifi(char *ssid, char *pass){
    esp_err_t ret;

    /* Store state into flash */
    ESP_LOGI(TAG, "Store wifi ssid %s password %s", ssid, pass);
    ret = hw_api_nvs_write(WIFI_SSID_KEY, ssid, strlen(ssid));
    ret += hw_api_nvs_write(WIFI_PASSWORD_KEY, pass, strlen(pass));
    return ret;
}

/*!
 * @brief  Restore wifi info from nvs
 */
esp_err_t hw_api_restore_wifi(char *ssid, char *pass){
    esp_err_t ret;
    uint32_t length = 0;

    ret = hw_api_nvs_read(WIFI_SSID_KEY, ssid, &length);
    ret += hw_api_nvs_read(WIFI_PASSWORD_KEY, pass, &length);
    return ret;
}

/*!
 * @brief  Hardware components initialization
 */
void hw_api_init(void){
    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    /* Open NVS */
    ret = nvs_open("nvs_store", NVS_READWRITE, &storage_nvs);
    ESP_ERROR_CHECK(ret);
}