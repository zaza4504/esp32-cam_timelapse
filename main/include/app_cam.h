#ifndef _APP_CAM_H_
#define _APP_CAM_H_

//AI THINKER PIN Map
#define CAM_PIN_PWDN    32
#define CAM_PIN_RESET   -1 //software reset will be performed
#define CAM_PIN_XCLK    0
#define CAM_PIN_SIOD    26
#define CAM_PIN_SIOC    27

#define CAM_PIN_D7      35
#define CAM_PIN_D6      34
#define CAM_PIN_D5      39
#define CAM_PIN_D4      36
#define CAM_PIN_D3      21
#define CAM_PIN_D2      19
#define CAM_PIN_D1      18
#define CAM_PIN_D0       5
#define CAM_PIN_VSYNC   25
#define CAM_PIN_HREF    23
#define CAM_PIN_PCLK    22

#define PART_BOUNDARY "123456789000000000000987654321"

esp_err_t camera_init();

esp_err_t jpg_stream_httpd_handler(httpd_req_t *req);

esp_err_t jpg_httpd_handler(httpd_req_t *req);
#endif
