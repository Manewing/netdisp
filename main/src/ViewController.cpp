#include <netdisp/Display.hpp>
#include <netdisp/ViewController.hpp>

namespace netdisp {

ViewController::ViewController(std::shared_ptr<View> DefView, unsigned MaxViews)
    : DefaultView(std::move(DefView)), Views() {
  Views.resize(MaxViews);
}

unsigned ViewController::getMaxViews() const { return Views.size(); }

void ViewController::show(DisplayController &DC) {
  DC.clear();

  if (Notify) {
    if (Notify->isTimedout()) {
      clearNotification();
    } else {
      Notify->show(DC);
      DC.flush();
      return;
    }
  }

  getViewToShow().show(DC);

  DC.flush();
}

void ViewController::setDefaultView(std::shared_ptr<View> DefView) {
  DefaultView = DefView;
}

bool ViewController::setView(unsigned Idx, std::shared_ptr<View> V) {
  if (Idx >= getMaxViews()) {
    return false;
  }
  Views.at(Idx) = V;
  return true;
}

void ViewController::setNotification(std::shared_ptr<Notification> N) {
  Notify = N;
}

void ViewController::clearNotification() { Notify = nullptr; }

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

  setNotification(std::make_shared<IdxInfoView>(Idx, /*TimeoutMs=*/500));
  ShownViewIdx = Idx;
  return true;
}

unsigned ViewController::getShownViewIdx() const { return ShownViewIdx; }

View &ViewController::getViewToShow() {
  if (!Views.at(ShownViewIdx)) {
    return *DefaultView;
  }
  return *Views.at(ShownViewIdx);
}

} // namespace netdisp