#ifndef NETDISP_NOTIFICATION_HPP
#define NETDISP_NOTIFICATION_HPP

#include <netdisp/View.hpp>

namespace netdisp {
class DisplayController;
}

namespace netdisp {

class IdxInfoView : public View {
public:
  IdxInfoView(unsigned Idx);

  void show(DisplayController &DC) override;

private:
  unsigned Idx;
};

} // namespace netdisp

#endif // #ifndef NETDISP_NOTIFICATION_HPP