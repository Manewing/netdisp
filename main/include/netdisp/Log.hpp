#ifndef NETDISP_LOG_HPP
#define NETDISP_LOG_HPP

#ifndef NETDISP_NO_LOG

#include <esp_log.h>

#define ND_LOGI(TAG, MSG, ...) ESP_LOGI(TAG, MSG, __VA_ARGS__)
#define ND_LOGW(TAG, MSG, ...) ESP_LOGW(TAG, MSG, __VA_ARGS__)
#define ND_LOGE(TAG, MSG, ...) ESP_LOGE(TAG, MSG, __VA_ARGS__)

#else

#define ND_LOGI(TAG, MSG, ...)
#define ND_LOGW(TAG, MSG, ...)
#define ND_LOGE(TAG, MSG, ...)

#endif


#endif // #ifndef NETDISP_LOG_HPP