idf_component_register(
    SRCS "app_main.c"
         "app_wifi.c"
         "app_http.c"
    REQUIRES esp_http_server
             nvs_flash
    INCLUDE_DIRS "include")
