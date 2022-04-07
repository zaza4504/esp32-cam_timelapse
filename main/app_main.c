#include <esp_http_server.h>
#include <nvs_flash.h>
#include <mqtt_client.h>
#include "app_wifi.h"
#include "app_http.h"
#include "app_mqtt.h"
#include "app_nvs.h"
#include "app_cam.h"

void app_main()
{
    nvs_setting_init();

    struct WIFI_auth wifi_auth;
    size_t len = sizeof(wifi_auth);
    esp_err_t err = nvs_setting_get("settings","wifiauth", &wifi_auth, &len);
    if(err != ESP_OK || len != sizeof(wifi_auth)) {
        memset(&wifi_auth, 0, sizeof(wifi_auth));
        memcpy(wifi_auth.ssid, DEFAULT_SSID, strlen(DEFAULT_SSID));
        memcpy(wifi_auth.passwd, DEFAULT_PASSWD, strlen(DEFAULT_PASSWD));
    }
    wifi_init(&wifi_auth);
    ESP_ERROR_CHECK(camera_init());
    httpd_handle_t server = NULL;
    server = webserver_start();
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://192.168.1.100",
	.client_id = "esp32cam01"
    };
    mqtt_app_start(&mqtt_cfg);
}
