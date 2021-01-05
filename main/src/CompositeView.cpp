#include <netdisp/CompositeView.hpp>

namespace netdisp {

void CompositeView::addView(std::shared_ptr<View> SubView) {
  SubViews.push_back(std::move(SubView));
}

void CompositeView::show(DisplayController &DC) {
  for (auto const &SubView : SubViews) {
    SubView->show(DC);
  }
}

}