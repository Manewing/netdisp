#ifndef NETWORK_WIFI_CONNECTOR_HPP
#define NETWORK_WIFI_CONNECTOR_HPP

#include <stdint.h>
#include <string>

namespace network {

class WifiConnector {
public:
  static WifiConnector &getInstance();

  /// Connect to Wifi AP with given SSID and password, tries to connect to AP
  /// MaxRetry times before failure.
  /// \return True if connection was successful
  bool connect(std::string const &SSID, std::string const &Password,
               unsigned MaxRetry = 10);

  uint32_t getIpAddr() const;

  std::string getIpAddrStr() const;

protected:
  static void initWifi();
  static void startWifi(std::string const &SSID, std::string const &Password);

  void registerEventHandlers();
  bool waitForConnection(std::string const &SSID) const;
  void deregisterEventHandlers();

  // Callbacks to handle ESP Wifi events
  static void handleWifiEventCallback(void *Arg, const char *EventBase,
                                      int32_t EventId, void *EventData);
  void handleWifiEvent(int32_t EventId, void *EventData);

  // Callbacks to handle ESP IP events
  static void handleIpEventCallback(void *Arg, const char *EventBase,
                                    int32_t EventId, void *EventData);
  void handleIpEvent(int32_t EventId, void *EventData);

private:
  uint32_t Ipv4Addr = 0;
  void *EventHandle = nullptr;
  void *InstanceGotAnyId = nullptr;
  void *InstanceGotIp = nullptr;
  unsigned ConnectNumRetry = 0;
  unsigned ConnectMaxRetry = 10;
};

} // namespace network

#endif // #ifndef NETWORK_WIFI_CONNECTOR_HPP