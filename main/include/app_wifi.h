#ifndef _APP_WIFI_H_
#define _APP_WIFI_H_

/**
 * https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/storage/nvs_flash.html?highlight=nvs_flash#keys-and-values
 * https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_wifi.html?highlight=wifi_config_t#_CPPv417wifi_sta_config_t
 * https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_wifi.html?highlight=wifi_config_t#_CPPv416wifi_ap_config_t
 *
 * Although the ssid has max length 32, but the maximum key lengh in nvs flash is 15
*/
#define SSID_LEN 15
#define PASSWD_LEN 64
#define DEFAULT_SSID "esp32cam"
#define DEFAULT_PASSWD "12345678"

struct WIFI_auth {
    char ssid[SSID_LEN];
    char passwd[PASSWD_LEN];
};
void wifi_init(struct WIFI_auth *wifi_auth);
#endif
