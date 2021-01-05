#include <netdisp/Config.hpp>
#include <netdisp/Icons.hpp>
#include <netdisp/Led.hpp>
#include <netdisp/Parser.hpp>
#include <netdisp/RotEnc.hpp>
#include <netdisp/View.hpp>
#include <network/UdpReceiver.hpp>
#include <network/WifiConnector.hpp>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// FIXME #include <lcdgfx.h> breaks STL headers
#include <netdisp/LcdgfxDisplay.hpp>

#include <esp_log.h>
#include <nvs_flash.h>

#include <cstring>

namespace netdisp {

std::shared_ptr<View> getLoadingView() {
  // FIXME hardcoded display size
  return std::make_shared<BitmapView>(32, 0, 64, 64, netdisp_bitmap_data,
                                      sizeof(netdisp_bitmap_data));
}

std::shared_ptr<View> getReadyView(std::string const &IpStr,
                                   std::string const &PortStr) {
  return std::make_shared<TextView>("~*Ready*\n\n~" + IpStr + "\n~" + PortStr);
}

void main() {
  LcdgfxDisplayController DispCtrl;
  netdisp::LedController LedCtrl({NETDISP_LED_0_PIN, NETDISP_LED_1_PIN});
  netdisp::RotEncController RotEnc(NETDISP_ROTENC_PIN_A, NETDISP_ROTENC_PIN_B);
  netdisp::ViewController ViewCtrl(getLoadingView(), NETDISP_VIEW_COUNT);
  netdisp::Context Ctx{LedCtrl, ViewCtrl};

  // Show loading view
  LedCtrl.setLeds(true);
  ViewCtrl.show(DispCtrl);

  // Setup Wifi connection
  auto &WifiConn = network::WifiConnector::getInstance();
  if (!WifiConn.connect(NETDISP_WIFI_SSID, NETDISP_WIFI_PASSWORD,
                        NETDISP_WIFI_MAX_RETRY)) {
    return;
  }

  // Initialize UDP receiver
  network::AsyncUdpReceiver AsyncRecv(NETDISP_PORT, NETDISP_MAX_MSG_LEN);
  if (!AsyncRecv.isReady()) {
    ESP_LOGE("NetDisp", "Could not setup async receiver");
    return;
  }
  AsyncRecv.onRecv([&Ctx](uint8_t *Buffer, int Count) {
    if (Count < 0) {
      ESP_LOGE("NetDisp", "Failed to receive data");
      return;
    }
    ESP_LOGI("NetDisp", "Got message of length %u", Count);

    netdisp::Parser Parser(Buffer, Count);
    if (auto Cmd = Parser.parse()) {
      Ctx.lock();
      Cmd->execute(Ctx);
      Ctx.unlock();
    } else {
      ESP_LOGW("NestDisp", "Failed to parse message command");
    }
  });

  // Switch to ready view
  LedCtrl.setLeds(false);
  ViewCtrl.setDefaultView(
      getReadyView(WifiConn.getIpAddrStr(), NETDISP_PORT_STR));
  ViewCtrl.show(DispCtrl);

  int LastPosition = 0;
  while (1) {
    ViewCtrl.show(DispCtrl);

    int Position = 0;
    if (RotEnc.waitForEvent(Position, /*TimeoutMs=*/2)) {
      Ctx.lock();
      if (LastPosition < Position) {
        Ctx.ViewCtrl.showView(Ctx.ViewCtrl.getShownViewIdx() + 1);
      } else if (LastPosition > Position) {
        Ctx.ViewCtrl.showView(Ctx.ViewCtrl.getShownViewIdx() - 1);
      }
      Ctx.unlock();
      LastPosition = Position;
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

} // namespace netdisp

extern "C" {

void app_main(void) {
  // Initialize NVS
  // TODO move this
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  netdisp::main();
}
}