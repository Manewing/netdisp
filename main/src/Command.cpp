#include <netdisp/CanvasView.hpp>
#include <netdisp/Command.hpp>
#include <netdisp/Led.hpp>
#include <netdisp/TextView.hpp>
#include <netdisp/View.hpp>
#include <netdisp/ViewController.hpp>

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace netdisp {

Command *Command::setNext(std::unique_ptr<Command> Next) {
  NextCmd = std::move(Next);
  return NextCmd.get();
}

void Command::execute(Context &Ctx) const {
  executeInternal(Ctx);
  if (NextCmd) {
    NextCmd->execute(Ctx);
  }
}

SelectViewCmd::SelectViewCmd(unsigned Idx) : Idx(Idx) {}

void SelectViewCmd::executeInternal(Context &Ctx) const {
  ESP_LOGI("Command", "execute select view: %d", Idx);
  if (!Ctx.ViewCtrl.selectCurrentViewIdx(Idx)) {
    ESP_LOGI("Command", "failed to select view: %d", Idx);
  }
}

ShowViewCmd::ShowViewCmd(unsigned Idx) : Idx(Idx) {}

void ShowViewCmd::executeInternal(Context &Ctx) const {
  ESP_LOGI("Command", "execute show view: %d", Idx);
  if (!Ctx.ViewCtrl.showView(Idx)) {
    ESP_LOGI("Command", "failed to show view: %d", Idx);
  }
}

SetLedCmd::SetLedCmd(unsigned Led, bool State) : Led(Led), State(State) {}

void SetLedCmd::executeInternal(Context &Ctx) const {
  ESP_LOGI("Command", "execute set LED[%d] to %s", Led,
           (State ? "HIGH" : "LOW"));

  if (!Ctx.LedCtrl.setLed(Led, State)) {
    ESP_LOGW("Command", "failed to set LED[%d]", Led);
  }
}

BlinkLedCmd::BlinkLedCmd(unsigned Led, unsigned Times)
    : Led(Led), Times(Times) {}

void BlinkLedCmd::executeInternal(Context &Ctx) const {
  ESP_LOGI("Command", "execute blink LED[%d] %d times", Led, Times);
  if (!Ctx.LedCtrl.setLed(Led, false)) {
    ESP_LOGW("Command", "failed to set LED[%d]", Led);
    return;
  }
  for (unsigned Count = 0; Count < Times; Count++) {
    Ctx.LedCtrl.setLed(Led, true);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    Ctx.LedCtrl.setLed(Led, false);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

ShowTextCmd::ShowTextCmd(std::string Text, bool Raw, bool UCV)
    : UseCurrentView(UCV), TxtView() {
  ESP_LOGI("Command", "prepare show text: %s, raw=%s", Text.c_str(),
           (Raw ? "yes" : "no"));
  if (Raw) {
    TxtView = std::make_shared<RawTextView>(std::move(Text));
  } else {
    TxtView = std::make_shared<TextView>(std::move(Text));
  }
}

void ShowTextCmd::executeInternal(Context &Ctx) const {
  unsigned ViewIdx = UseCurrentView ? Ctx.ViewCtrl.getCurrentViewIdx() : 0;
  Ctx.ViewCtrl.setView(ViewIdx, TxtView);
}

ShowBitmapCmd::ShowBitmapCmd(unsigned X, unsigned Y, unsigned Width,
                             unsigned Height, const uint8_t *Data,
                             unsigned Length) {
  ESP_LOGI("Command",
           "prepare show bitmap X: %u, Y: %u W: %u, H: %u, Len %u bytes", X, Y,
           Width, Height, Length);
  BmpView = std::make_shared<BitmapView>(X, Y, Width, Height, Data, Length);
}

void ShowBitmapCmd::executeInternal(Context &Ctx) const {
  Ctx.ViewCtrl.setView(Ctx.ViewCtrl.getCurrentViewIdx(), BmpView);
}

} // namespace netdisp