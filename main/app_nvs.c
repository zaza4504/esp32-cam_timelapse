#include <nvs_flash.h>
#include <nvs.h>
#include <esp_log.h>
#include "app_nvs.h"

#define TAG "NVS"

void nvs_setting_init()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
       // Retry nvs_flash_init
       ESP_ERROR_CHECK(nvs_flash_erase());
       err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

esp_err_t nvs_setting_get(const char *name, const char *key, void *out_value, size_t *len)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(name, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	return err;
    }
    return nvs_get_blob(nvs_handle, key, out_value, len);
}

esp_err_t nvs_setting_set(const char *name, const char *key, const void *in_value, size_t len)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(name, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	return err;
    }
    return nvs_set_blob(nvs_handle, key, in_value, len);
}
