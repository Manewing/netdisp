#include <algorithm>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <netdisp/Config.hpp>
#include <netdisp/Display.hpp>
#include <netdisp/ViewController.hpp>

namespace netdisp {

ViewHandle::ViewHandle(std::shared_ptr<View> V, unsigned TimeoutMs)
    : TimeoutEndMs(xTaskGetTickCount() * portTICK_PERIOD_MS + TimeoutMs),
      Vw(std::move(V)) {}

bool ViewHandle::hasTimedout() const {
  return TimeoutEndMs <= xTaskGetTickCount() * portTICK_PERIOD_MS;
}

ViewController::ViewController(std::shared_ptr<View> DefView, unsigned MaxViews)
    : DefaultView(std::move(DefView)), Views() {
  Views.resize(MaxViews);
}

unsigned ViewController::getMaxViews() const { return Views.size(); }

void ViewController::show(DisplayController &DC) {
  DC.clear();

  if (Notification.get()) {
    if (Notification.hasTimedout()) {
      clearNotification();
    } else {
      Notification->show(DC);
      DC.flush();
      return;
    }
  }

  getViewToShow()->show(DC);

  DC.flush();
}

void ViewController::setDefaultView(std::shared_ptr<View> DefView) {
  DefaultView = ViewHandle(std::move(DefView));
}

bool ViewController::setView(unsigned Idx, std::shared_ptr<View> V) {
  if (Idx >= getMaxViews()) {
    return false;
  }
  Views.at(Idx) =
      ViewHandle(std::move(V), /*TimeoutMs=*/NETDISP_VIEW_TIMEOUT_MS);
  return true;
}

void ViewController::setNotification(std::shared_ptr<View> N,
                                     unsigned TimeoutMs) {
  TimeoutMs = std::min(TimeoutMs, NETDISP_NOTIFICATION_MAX_TIMEOUT_MS);
  Notification = ViewHandle(N, TimeoutMs);
}

void ViewController::clearNotification() { Notification = ViewHandle(nullptr); }

bool ViewController::selectCurrentViewIdx(unsigned Idx) {
  if (Idx >= getMaxViews()) {
    return false;
  }
  CurrentViewIdx = Idx;
  return true;
}

unsigned ViewController::getCurrentViewIdx() const { return CurrentViewIdx; }

bool ViewController::showView(unsigned Idx) {
  if (Idx >= getMaxViews()) {
    return false;
  }

  setNotification(std::make_shared<IdxInfoView>(Idx));
  ShownViewIdx = Idx;
  return true;
}

unsigned ViewController::getShownViewIdx() const { return ShownViewIdx; }

ViewHandle &ViewController::getViewToShow() {
  if (!Views.at(ShownViewIdx).get()) {
    return DefaultView;
  }
  auto &VH = Views.at(ShownViewIdx);
  if (VH.hasTimedout()) {
    Views.at(ShownViewIdx) = ViewHandle(nullptr);
    return DefaultView;
  }
  return VH;
}

} // namespace netdisp