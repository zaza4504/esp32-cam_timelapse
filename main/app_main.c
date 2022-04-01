#include <esp_http_server.h>
#include <nvs_flash.h>
#include "app_wifi.h"
#include "app_http.h"

void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init();
    httpd_handle_t server = NULL;
    server = webserver_start();
}
