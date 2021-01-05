#ifndef NETDISP_COMPOSITE_VIEW_HPP
#define NETDISP_COMPOSITE_VIEW_HPP

#include <memory>
#include <netdisp/View.hpp>
#include <vector>

namespace netdisp {
class DisplayController;
}

namespace netdisp {

class CompositeView : public View {
public:
  void addView(std::shared_ptr<View> SubView);

  void show(DisplayController &DC) override;

protected:
  std::vector<std::shared_ptr<View>> SubViews;
};

} // namespace netdisp

#endif // #ifndef NETDISP_COMPOSITE_VIEW_HPP