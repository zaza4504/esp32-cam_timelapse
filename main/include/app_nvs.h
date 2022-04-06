#ifndef _APP_NVS_H_
#define _APP_NVS_H_

void nvs_setting_init();
esp_err_t nvs_setting_get(const char *name, const char *key, void *out_value, size_t *len);
esp_err_t nvs_setting_set(const char *name, const char *key, const void *in_value, size_t len);

#endif
