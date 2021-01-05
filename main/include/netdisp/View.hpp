#ifndef NETDISP_VIEW_HPP
#define NETDISP_VIEW_HPP

namespace netdisp {
class DisplayController;
}
namespace netdisp {

class View {
public:
  virtual void show(DisplayController &DC) = 0;
};

} // namespace netdisp

#endif // #ifndef NETDISP_VIEW_HPP