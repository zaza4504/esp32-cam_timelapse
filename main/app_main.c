#include <esp_http_server.h>
#include <nvs_flash.h>
#include <mqtt_client.h>
#include "app_wifi.h"
#include "app_http.h"
#include "app_mqtt.h"

void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init();
    httpd_handle_t server = NULL;
    server = webserver_start();
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://192.168.1.100",
	.client_id = "esp32cam01"
    };
    mqtt_app_start(&mqtt_cfg);
}
