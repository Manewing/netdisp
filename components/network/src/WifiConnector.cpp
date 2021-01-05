#include <cstring>
#include <network/WifiConnector.hpp>

#include <esp_event.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <freertos/event_groups.h>

#define LOGGER_TAG "WifiConnector"

/* The event group allows multiple bits for each event, but we only care about
 * two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

namespace network {

WifiConnector &WifiConnector::getInstance() {
  static WifiConnector *WC = nullptr;
  if (!WC) {
    WC = new WifiConnector();
  }
  return *WC;
}

bool WifiConnector::connect(std::string const &SSID,
                            std::string const &Password, unsigned MaxRetry) {
  // Reset connection information
  ConnectNumRetry = 0;
  ConnectMaxRetry = MaxRetry;

  initWifi();

  registerEventHandlers();

  startWifi(SSID, Password);

  bool Connected = waitForConnection(SSID);

  deregisterEventHandlers();

  return Connected;
}

uint32_t WifiConnector::getIpAddr() const { return Ipv4Addr; }

std::string WifiConnector::getIpAddrStr() const {
  char Buffer[16];
  // FIXME use proper conversion function
  const uint8_t *Addr = reinterpret_cast<const uint8_t *>(&Ipv4Addr);
  sprintf(Buffer, "%d.%d.%d.%d", Addr[0], Addr[1], Addr[2], Addr[3]);
  return std::string(Buffer);
}

void WifiConnector::initWifi() {
  ESP_ERROR_CHECK(esp_netif_init());

  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t WifiInitCfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&WifiInitCfg));
}

void WifiConnector::startWifi(std::string const &SSID,
                              std::string const &Password) {
  wifi_config_t WifiConfig;
  std::memset(&WifiConfig, 0, sizeof(WifiConfig));

  std::memcpy(WifiConfig.sta.ssid, SSID.c_str(),
              std::min(sizeof(WifiConfig.sta.ssid), SSID.size()));
  std::memcpy(WifiConfig.sta.password, Password.c_str(),
              std::min(sizeof(WifiConfig.sta.password), Password.size()));

  WifiConfig.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
  WifiConfig.sta.pmf_cfg.capable = true;
  WifiConfig.sta.pmf_cfg.required = false;

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &WifiConfig));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(LOGGER_TAG, "started wifi");
}

void WifiConnector::registerEventHandlers() {
  EventHandle = xEventGroupCreate();

  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &WifiConnector::handleWifiEventCallback,
      this, &InstanceGotAnyId));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &WifiConnector::handleIpEventCallback,
      this, &InstanceGotIp));
}

bool WifiConnector::waitForConnection(std::string const &SSID) const {
  // Waiting until either the connection is established (WIFI_CONNECTED_BIT) or
  // connection failed for the maximum number of re-tries (WIFI_FAIL_BIT). The
  // bits are set by the respective event handlers
  EventBits_t bits =
      xEventGroupWaitBits(EventHandle, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                          pdFALSE, pdFALSE, portMAX_DELAY);

  // xEventGroupWaitBits() returns the bits before the call returned, hence we
  // can test which event actually happened
  if (bits & WIFI_CONNECTED_BIT) {
    ESP_LOGI(LOGGER_TAG, "connected to ap SSID: %s", SSID.c_str());
    return true;
  }

  if (bits & WIFI_FAIL_BIT) {
    ESP_LOGI(LOGGER_TAG, "failed to connect to SSID: %s", SSID.c_str());
    return false;
  }

  ESP_LOGE(LOGGER_TAG, "connection failed: UNEXPECTED EVENT");
  return false;
}

void WifiConnector::deregisterEventHandlers() {
  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(
      IP_EVENT, IP_EVENT_STA_GOT_IP, InstanceGotIp));
  InstanceGotIp = nullptr;

  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(
      WIFI_EVENT, ESP_EVENT_ANY_ID, InstanceGotAnyId));
  InstanceGotAnyId = nullptr;

  vEventGroupDelete(EventHandle);
  EventHandle = nullptr;
}

void WifiConnector::handleWifiEventCallback(void *Arg,
                                            esp_event_base_t EventBase,
                                            int32_t EventId, void *EventData) {
  if (EventBase != WIFI_EVENT) {
    ESP_LOGW(LOGGER_TAG, "handleWifiEventCallback called for invalid event");
    return;
  }
  network::WifiConnector &WC = *reinterpret_cast<network::WifiConnector *>(Arg);
  WC.handleWifiEvent(EventId, EventData);
}

void WifiConnector::handleWifiEvent(int32_t EventId, void *EventData) {
  if (EventId == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
    return;
  }

  if (EventId != WIFI_EVENT_STA_DISCONNECTED) {
    return;
  }

  if (ConnectNumRetry < ConnectMaxRetry) {
    ConnectNumRetry++;
    esp_wifi_connect();
    ESP_LOGI(LOGGER_TAG, "%u/%u retry to connect to the AP", ConnectNumRetry,
             ConnectMaxRetry);
  } else {
    xEventGroupSetBits(EventHandle, WIFI_FAIL_BIT);
  }
}

void WifiConnector::handleIpEventCallback(void *Arg, esp_event_base_t EventBase,
                                          int32_t EventId, void *EventData) {
  if (EventBase != IP_EVENT) {
    ESP_LOGW(LOGGER_TAG, "handIpEventCallback called for invalid event");
    return;
  }
  network::WifiConnector &WC = *reinterpret_cast<network::WifiConnector *>(Arg);
  WC.handleIpEvent(EventId, EventData);
}

void WifiConnector::handleIpEvent(int32_t EventId, void *EventData) {
  // TODO check for lost IP?
  if (EventId != IP_EVENT_STA_GOT_IP) {
    return;
  }

  ip_event_got_ip_t *EventGotIp =
      reinterpret_cast<ip_event_got_ip_t *>(EventData);
  ESP_LOGI(LOGGER_TAG, "got IP: " IPSTR, IP2STR(&EventGotIp->ip_info.ip));
  Ipv4Addr = EventGotIp->ip_info.ip.addr;

  ConnectNumRetry = 0;
  xEventGroupSetBits(EventHandle, WIFI_CONNECTED_BIT);
}

} // namespace network