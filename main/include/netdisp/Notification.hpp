#ifndef NETDISP_NOTIFICATION_HPP
#define NETDISP_NOTIFICATION_HPP

#include <netdisp/View.hpp>

namespace netdisp {
class DisplayController;
}

namespace netdisp {

// TODO refactor, take view to show as argument
class Notification : public View {
public:
  Notification(unsigned TimeoutMs);

  bool isTimedout() const;

private:
  unsigned TimeoutEndMs;
};

class IdxInfoView : public Notification {
public:
  IdxInfoView(unsigned Idx, unsigned TimeoutMs);

  void show(DisplayController &DC) override;

private:
  unsigned Idx;
};

} // namespace netdisp

#endif // #ifndef NETDISP_NOTIFICATION_HPP