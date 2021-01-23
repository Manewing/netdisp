#ifndef NETDISP_CONFIG_HPP
#define NETDISP_CONFIG_HPP

#include <sdkconfig.h>

#define NETDISP_STR_HELPER(x) #x
#define NETDISP_STR(x) NETDISP_STR_HELPER(x)

#ifndef CONFIG_NETDISP_PORT
#define NETDISP_PORT 5432
#else
#define NETDISP_PORT CONFIG_NETDISP_PORT
#endif
#define NETDISP_PORT_STR NETDISP_STR(NETDISP_PORT)

#ifndef CONFIG_NETDISP_WIFI_SSID
#define NETDISP_WIFI_SSID "<SSID>"
#else
#define NETDISP_WIFI_SSID CONFIG_NETDISP_WIFI_SSID
#endif

#ifndef CONFIG_NETDISP_WIFI_PASSWORD
#define NETDISP_WIFI_PASSWORD "<Password>"
#else
#define NETDISP_WIFI_PASSWORD CONFIG_NETDISP_WIFI_PASSWORD
#endif

#ifndef CONFIG_NETDISP_WIFI_MAX_RETRY
#define NETDISP_WIFI_MAX_RETRY 10
#else
#define NETDISP_WIFI_MAX_RETRY CONFIG_NETDISP_WIFI_MAX_RETRY
#endif

#ifndef CONFIG_NETDISP_MAX_MSG_LEN
#define NETDISP_MAX_MSG_LEN 2048
#else
#define NETDISP_MAX_MSG_LEN CONFIG_NETDISP_MAX_MSG_LEN
#endif

#ifndef CONFIG_NETDISP_LED_0_PIN
#define NETDISP_LED_0_PIN 17
#else
#define NETDISP_LED_0_PIN CONFIG_NETDISP_LED_0_PIN
#endif

#ifndef CONFIG_NETDISP_LED_1_PIN
#define NETDISP_LED_1_PIN 16
#else
#define NETDISP_LED_1_PIN CONFIG_NETDISP_LED_1_PIN
#endif

#ifndef CONFIG_NETDISP_ROTENC_PIN_A
#define NETDISP_ROTENC_PIN_A 19
#else
#define NETDISP_ROTENC_PIN_A CONFIG_NETDISP_ROTENC_PIN_A
#endif

#ifndef CONFIG_NETDISP_ROTENC_PIN_B
#define NETDISP_ROTENC_PIN_B 18
#else
#define NETDISP_ROTENC_PIN_B CONFIG_NETDISP_ROTENC_PIN_B
#endif

#ifndef CONFIG_NETDISP_VIEW_COUNT
#define NETDISP_VIEW_COUNT 10
#else
#define NETDISP_VIEW_COUNT CONFIG_NETDISP_VIEW_COUNT
#endif

#ifndef CONFIG_NETDISP_VIEW_TIMEOUT_MS
#define NETDISP_VIEW_TIMEOUT_MS 60000U /* 60s */
#else
#define NETDISP_VIEW_TIMEOUT_MS                                                \
  static_cast<unsigned>(CONFIG_NETDISP_VIEW_TIMEOUT_MS)
#endif

#ifndef CONFIG_NETDISP_NOTIFICATION_MAX_TIMEOUT_MS
#define NETDISP_NOTIFICATION_MAX_TIMEOUT_MS 5000U /* 5s */
#else
#define NETDISP_NOTIFICATION_MAX_TIMEOUT_MS                                    \
  static_cast<unsigned>(CONFIG_NETDISP_NOTIFICATION_MAX_TIMEOUT_MS)
#endif

#endif // #ifndef NETDISP_CONFIG_HPP