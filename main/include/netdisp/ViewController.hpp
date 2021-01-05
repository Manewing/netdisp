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

class ViewController {
public:
  // use uint8_t instead of unsigned
  ViewController(std::shared_ptr<View> DefaultView, unsigned MaxViews);

  unsigned getMaxViews() const;

  void show(DisplayController &DC);

  void setDefaultView(std::shared_ptr<View> DefaultView);

  bool setView(unsigned Idx, std::shared_ptr<View> V);
  // delete view

  void setNotification(std::shared_ptr<Notification> N);
  void clearNotification();

  bool selectCurrentViewIdx(unsigned Idx);
  unsigned getCurrentViewIdx() const;

  bool showView(unsigned Idx);
  unsigned getShownViewIdx() const;

private:
  View &getViewToShow();

private:
  std::shared_ptr<View> DefaultView;
  std::shared_ptr<Notification> Notify;
  std::vector<std::shared_ptr<View>> Views;

  unsigned CurrentViewIdx = 0;
  unsigned ShownViewIdx = 0;
};

} // namespace netdisp

#endif // #ifndef NETDISP_VIEW_CONTROLLER_HPP