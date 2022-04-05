idf_component_register(
    SRCS "app_main.c"
         "app_wifi.c"
         "app_http.c"
         "app_mqtt.c"
    REQUIRES esp_http_server
             nvs_flash
             mqtt
    INCLUDE_DIRS "include"
    EMBED_FILES "www/index.html.gz"
                "www/style.css.gz"
                "www/script.js.gz"
)
