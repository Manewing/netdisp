#include <netdisp/Config.hpp>
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

void main() {
  LcdgfxDisplayController DispCtrl;

  // TODO add pins to config
  netdisp::LedController LedCtrl({17, 16});
  LedCtrl.setLed(0, true);
  LedCtrl.setLed(1, true);

  auto &WifiConn = network::WifiConnector::getInstance();
  if (!WifiConn.connect(NETDISP_WIFI_SSID, NETDISP_WIFI_PASSWORD,
                        NETDISP_WIFI_MAX_RETRY)) {
    return;
  }

  // TODO add to config
  constexpr unsigned MaxMessageLength = 2048;
  network::AsyncUdpReceiver AsyncRecv(NETDISP_PORT, MaxMessageLength);
  if (!AsyncRecv.isReady()) {
    ESP_LOGE("NetDisp", "Could not setup async receiver");
    return;
  }

  auto DefaultView = std::make_shared<TextView>(
      "~*Ready*\n\n~" + WifiConn.getIpAddrStr() + "\n~" NETDISP_PORT_STR);
  netdisp::ViewController ViewCtrl(DefaultView, 10);

  LedCtrl.setLed(0, false);
  LedCtrl.setLed(1, false);

  netdisp::Context Ctx{LedCtrl, ViewCtrl};

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

  // TODO add pins to config
  netdisp::RotEncController RotEnc(19, 18);

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