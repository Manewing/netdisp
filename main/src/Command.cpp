#include <netdisp/CanvasView.hpp>
#include <netdisp/Command.hpp>
#include <netdisp/CompositeView.hpp>
#include <netdisp/Context.hpp>
#include <netdisp/Led.hpp>
#include <netdisp/Log.hpp>
#include <netdisp/TextView.hpp>
#include <netdisp/View.hpp>
#include <netdisp/ViewController.hpp>

namespace netdisp {

SelectViewCmd::SelectViewCmd(unsigned Idx) : Idx(Idx) {}

void SelectViewCmd::executeInternal(Context &Ctx) const {
  ND_LOGI("Command", "execute select view: %d", Idx);
  if (!Ctx.ViewCtrl.selectCurrentViewIdx(Idx)) {
    ND_LOGI("Command", "failed to select view: %d", Idx);
  }
}

ShowViewCmd::ShowViewCmd(unsigned Idx) : Idx(Idx) {}

void ShowViewCmd::executeInternal(Context &Ctx) const {
  ND_LOGI("Command", "execute show view: %d", Idx);
  if (!Ctx.ViewCtrl.showView(Idx)) {
    ND_LOGI("Command", "failed to show view: %d", Idx);
  }
}

SetLedCmd::SetLedCmd(unsigned Led, bool State) : Led(Led), State(State) {}

void SetLedCmd::executeInternal(Context &Ctx) const {
  ND_LOGI("Command", "execute set LED[%d] to %s", Led,
          (State ? "HIGH" : "LOW"));

  if (!Ctx.LedCtrl.setLed(Led, State)) {
    ND_LOGW("Command", "failed to set LED[%d]", Led);
  }
}

BlinkLedCmd::BlinkLedCmd(unsigned Led, unsigned Times)
    : Led(Led), Times(Times) {}

void BlinkLedCmd::executeInternal(Context &Ctx) const {
  ND_LOGI("Command", "execute blink LED[%d] %d times", Led, Times);
  if (!Ctx.LedCtrl.blinkLed(Led, Times)) {
    ND_LOGW("Command", "failed to blink LED[%d]", Led);
    return;
  }
}

void ViewCommand::setView(Context &Ctx, unsigned Idx,
                          const std::shared_ptr<View> &V) const {
  if (Ctx.ParentViews.empty()) {
    Ctx.ViewCtrl.setView(Idx, V);
  } else {
    Ctx.ParentViews.top()->addView(V);
  }
}

ShowTextCmd::ShowTextCmd(std::string Text, bool Raw, bool UCV)
    : UseCurrentView(UCV), TxtView() {
  ND_LOGI("Command", "prepare show text: %s, raw=%s", Text.c_str(),
          (Raw ? "yes" : "no"));
  if (Raw) {
    TxtView = std::make_shared<RawTextView>(std::move(Text));
  } else {
    TxtView = std::make_shared<TextView>(std::move(Text));
  }
}

void ShowTextCmd::executeInternal(Context &Ctx) const {
  unsigned ViewIdx = UseCurrentView ? Ctx.ViewCtrl.getCurrentViewIdx() : 0;
  setView(Ctx, ViewIdx, TxtView);
}

ShowBitmapCmd::ShowBitmapCmd(unsigned X, unsigned Y, unsigned Width,
                             unsigned Height, const uint8_t *Data,
                             unsigned Length) {
  ND_LOGI("Command",
          "prepare show bitmap X: %u, Y: %u W: %u, H: %u, Len %u bytes", X, Y,
          Width, Height, Length);
  BmpView = std::make_shared<BitmapView>(X, Y, Width, Height, Data, Length);
}

void ShowBitmapCmd::executeInternal(Context &Ctx) const {
  setView(Ctx, Ctx.ViewCtrl.getCurrentViewIdx(), BmpView);
}

DrawLineCmd::DrawLineCmd(int X0, int Y0, int X1, int Y1) {
  LnView = std::make_shared<LineView>(X0, Y0, X1, Y1);
}

void DrawLineCmd::executeInternal(Context &Ctx) const {
  setView(Ctx, Ctx.ViewCtrl.getCurrentViewIdx(), LnView);
}

DrawRectCmd::DrawRectCmd(int X, int Y, unsigned W, unsigned H) {
  RctView = std::make_shared<RectView>(X, Y, W, H);
}

void DrawRectCmd::executeInternal(Context &Ctx) const {
  setView(Ctx, Ctx.ViewCtrl.getCurrentViewIdx(), RctView);
}

DrawCircleCmd::DrawCircleCmd(int X, int Y, unsigned Radius) {
  CrclView = std::make_shared<CircleView>(X, Y, Radius);
}

void DrawCircleCmd::executeInternal(Context &Ctx) const {
  setView(Ctx, Ctx.ViewCtrl.getCurrentViewIdx(), CrclView);
}

CreateCompositeViewCmd::CreateCompositeViewCmd() {
  CompView = std::make_shared<CompositeView>();
}

void CreateCompositeViewCmd::executeInternal(Context &Ctx) const {
  ND_LOGI("Command", "execute create composite, at stack %u",
          Ctx.ParentViews.size());
  setView(Ctx, Ctx.ViewCtrl.getCurrentViewIdx(), CompView);
  Ctx.ParentViews.push(CompView);
}

CreateNotificationCmd::CreateNotificationCmd(unsigned TMS) : TimeoutMs(TMS) {
  Notification = std::make_shared<CompositeView>();
}

void CreateNotificationCmd::executeInternal(Context &Ctx) const {
  ND_LOGI("Command", "execute create notification, at stack %u",
          Ctx.ParentViews.size());
  Ctx.ViewCtrl.setNotification(Notification, TimeoutMs);
  Ctx.ParentViews.push(Notification);
}

void EndViewCmd::executeInternal(Context &Ctx) const {
  ND_LOGI("Command", "execute end view, stack %u", Ctx.ParentViews.size());
  if (Ctx.ParentViews.empty()) {
    return;
  }
  Ctx.ParentViews.pop();
}

} // namespace netdisp