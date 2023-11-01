#ifndef WIFI_APP_H_
#define WIFI_APP_H_


/******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/



/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

#define WIFI_CONNECTED_BIT   BIT0
#define WIFI_FAIL_BIT        BIT1

#define WIFI_SSID            "Tang 5"
#define WIFI_PASS            "03071971"

#define ESP_WIFI_SSID        "Esp32pro"
#define ESP_WIFI_PASS        "25022000"

#define WIFI_MAXIMUM_RETRY   5U
#define WIFI_CHANNEL         1U
#define MAX_CONN_CNT         1U
#define MAX_STA_CONN         7U


typedef enum wifi_status
{
    WIFI_CONNECTED = 0U,
    WIFI_DISCONNECTED = 1U
}wifi_status_t;

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/



/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/



/******************************************************************************/
/*                                FUNCTIONS                                   */
/******************************************************************************/

/*!
 * @brief  run app in the station mode.
 */
void WIFI_initStationMode(wifi_config_t wifi_config);

/*!
 * @brief  run app in the access point mode.
 */
void WIFI_initAccessPointMode(wifi_config_t wifi_config);

/*!
 * @brief  check wifi status.
 */
wifi_status_t WIFI_status(void);

#endif /* WIFI_APP_H_*/