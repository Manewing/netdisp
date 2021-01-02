#include <netdisp/Command.hpp>

#include <esp_log.h>

#include <netdisp/Led.hpp>
#include <netdisp/View.hpp>

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
  ESP_LOGI("Command", "execute set LED %d to %s", Led,
           (State ? "HIGH" : "LOW"));

  if (!Ctx.LedCtrl.setLed(Led, State)) {
    ESP_LOGW("Command", "failed to set LED %d", Led);
  }
}

ShowTextCmd::ShowTextCmd(std::string Text, bool Raw) : TxtView() {
  if (Raw) {
    TxtView = std::make_shared<RawTextView>(std::move(Text));
  } else {
    TxtView = std::make_shared<TextView>(std::move(Text));
  }
}


void ShowTextCmd::executeInternal(Context &Ctx) const {
  Ctx.ViewCtrl.setView(Ctx.ViewCtrl.getCurrentViewIdx(), TxtView);
}

} // namespace netdisp