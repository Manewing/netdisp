#ifndef NETDISP_VIEW_CONTROLLER_HPP
#define NETDISP_VIEW_CONTROLLER_HPP

#include <memory>
#include <netdisp/Notification.hpp>
#include <netdisp/View.hpp>
#include <vector>

namespace netdisp {
class DisplayController;
}

namespace netdisp {

class ViewHandle {
public:
  ViewHandle() = default;
  explicit ViewHandle(std::shared_ptr<View> Vw, unsigned TimeoutMs = 0);

  bool hasTimedout() const;

  View &operator*() { return *Vw; }

  const View &operator*() const { return *Vw; }

  View *operator->() { return Vw.get(); }

  const View *operator->() const { return Vw.get(); }

  const std::shared_ptr<View> &get() const { return Vw; }

private:
  unsigned TimeoutEndMs = 0;
  std::shared_ptr<View> Vw = nullptr;
};

class ViewController {
public:
  // use uint8_t instead of unsigned
  ViewController(std::shared_ptr<View> DefaultView, unsigned MaxViews);

  unsigned getMaxViews() const;

  void show(DisplayController &DC);

  void setDefaultView(std::shared_ptr<View> DefaultView);

  bool setView(unsigned Idx, std::shared_ptr<View> V);
  // delete view

  void setNotification(std::shared_ptr<View> N, unsigned TimeoutMs = 500);
  void clearNotification();

  bool selectCurrentViewIdx(unsigned Idx);
  unsigned getCurrentViewIdx() const;

  bool showView(unsigned Idx);
  unsigned getShownViewIdx() const;

private:
  ViewHandle &getViewToShow();

private:
  ViewHandle DefaultView;
  ViewHandle Notification;
  std::vector<ViewHandle> Views;

  unsigned CurrentViewIdx = 0;
  unsigned ShownViewIdx = 0;
};

} // namespace netdisp

#endif // #ifndef NETDISP_VIEW_CONTROLLER_HPP