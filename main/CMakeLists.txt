set(APP_SRC
    "app_main.c"
    "mqtt/mqtt.c"
    "wifi_app/wifi_app.c"
    "http_webserver/http_server_app.c"
)
set(COMPONENTS_REQ
        nvs_flash
        mqtt
        driver
        esp_wifi
        esp_http_server
        app_update
)
set(APP_INCLUDES
        "."
        "wifi_app"
        "mqtt"
        "http_webserver"
)
idf_component_register(
        SRCS ${APP_SRC}
        INCLUDE_DIRS ${APP_INCLUDES}
        REQUIRES ${COMPONENTS_REQ}
)