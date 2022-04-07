/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <ctype.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include <esp_http_server.h>

#include "app_http.h"
#include "app_nvs.h"
#include "app_wifi.h"
#include "app_cam.h"
/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */

static const char *TAG = "HTTP";

static void urldecode2(char *dst, const char *src)      //https://stackoverflow.com/questions/2673207/c-c-url-decode-library/2766963
{
        char a, b;
        while (*src) {
                if ((*src == '%') &&
                    ((a = src[1]) && (b = src[2])) &&
                    (isxdigit(a) && isxdigit(b))) {
                        if (a >= 'a')
                                a -= 'a'-'A';
                        if (a >= 'A')
                                a -= ('A' - 10);
                        else
                                a -= '0';
                        if (b >= 'a')
                                b -= 'a'-'A';
                        if (b >= 'A')
                                b -= ('A' - 10);
                        else
                                b -= '0';
                        *dst++ = 16*a+b;
                        src+=3;
                } else if (*src == '+') {
                        *dst++ = ' ';
                        src++;
                } else {
                        *dst++ = *src++;
                }
        }
        *dst++ = '\0';
}

static esp_err_t index_put_handler(httpd_req_t *req){
    extern const unsigned char index_html_gz_start[] asm("_binary_index_html_gz_start");
    extern const unsigned char index_html_gz_end[]   asm("_binary_index_html_gz_end");
    size_t index_html_gz_len = index_html_gz_end - index_html_gz_start;

    httpd_resp_set_type(req, "text/html");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    return httpd_resp_send(req, (const char *)index_html_gz_start, index_html_gz_len);
}

static esp_err_t js_handler(httpd_req_t *req){
    extern const unsigned char script_js_gz_start[] asm("_binary_script_js_gz_start");
    extern const unsigned char script_js_gz_end[]   asm("_binary_script_js_gz_end");
    size_t script_js_gz_len = script_js_gz_end - script_js_gz_start;

    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    return httpd_resp_send(req, (const char *)script_js_gz_start, script_js_gz_len);
}

static esp_err_t css_handler(httpd_req_t *req){
    extern const unsigned char style_css_gz_start[] asm("_binary_style_css_gz_start");
    extern const unsigned char style_css_gz_end[]   asm("_binary_style_css_gz_end");
    size_t style_css_gz_len = style_css_gz_end - style_css_gz_start;

    httpd_resp_set_type(req, "text/css");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    return httpd_resp_send(req, (const char *)style_css_gz_start, style_css_gz_len);
}

static esp_err_t wifi_cfg_handler(httpd_req_t *req){
    struct WIFI_auth wifi_auth;
    memset(&wifi_auth, 0, sizeof(wifi_auth));
    size_t buf_len = httpd_req_get_url_query_len(req) + 1;
    if(buf_len < 1) {
        httpd_resp_send_404(req);
	return ESP_FAIL;
    }
    char*  buf = (char *)malloc(buf_len);
    if(!buf) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    if(httpd_req_get_url_query_str(req, buf, buf_len) != ESP_OK || httpd_query_key_value(buf, "wifi_accesspoint", wifi_auth.ssid, sizeof(wifi_auth.ssid)) != ESP_OK || httpd_query_key_value(buf, "wifi_passwd", wifi_auth.passwd, sizeof(wifi_auth.passwd)) != ESP_OK) {
        free(buf);
        httpd_resp_send_404(req);
	return ESP_FAIL;
    }
    free(buf);
    urldecode2(wifi_auth.passwd, wifi_auth.passwd);
    nvs_setting_set("settings", "wifiauth", &wifi_auth, sizeof(wifi_auth));

    return httpd_resp_send(req, NULL, HTTPD_RESP_USE_STRLEN);
}

static esp_err_t cam_stream_handler(httpd_req_t *req){
    return jpg_stream_httpd_handler(req);
}

static esp_err_t cam_snapshot_handler(httpd_req_t *req){
    return jpg_httpd_handler(req);
}

static const httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_put_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t css_uri = {
    .uri       = "/style.css",
    .method    = HTTP_GET,
    .handler   = css_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t js_uri = {
    .uri       = "/script.js",
    .method    = HTTP_GET,
    .handler   = js_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t wifi_cfg_uri = {
    .uri       = "/wifi_cfg",
    .method    = HTTP_GET,
    .handler   = wifi_cfg_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t cam_stream_uri = {
    .uri       = "/stream",
    .method    = HTTP_GET,
    .handler   = cam_stream_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t cam_snapshot_uri = {
    .uri       = "/snapshot",
    .method    = HTTP_GET,
    .handler   = cam_snapshot_handler,
    .user_ctx  = NULL
};

httpd_handle_t webserver_start(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &index_uri);
        httpd_register_uri_handler(server, &js_uri);
        httpd_register_uri_handler(server, &css_uri);
        httpd_register_uri_handler(server, &wifi_cfg_uri);
        httpd_register_uri_handler(server, &cam_stream_uri);
        httpd_register_uri_handler(server, &cam_snapshot_uri);

        #if CONFIG_EXAMPLE_BASIC_AUTH
        httpd_register_basic_auth(server);
        #endif
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

void webserver_stop(httpd_handle_t server)
{
    // Stop the httpd server
    httpd_stop(server);
}
