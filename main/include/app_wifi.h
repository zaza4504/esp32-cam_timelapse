#ifndef _APP_WIFI_H_
#define _APP_WIFI_H_

#define SSID_LEN 32
#define PASSWD_LEN 64
struct WIFI_auth {
    char ssid[SSID_LEN];
    char passwd[PASSWD_LEN];
};
void wifi_init(struct WIFI_auth *wifi_auth);
#endif
