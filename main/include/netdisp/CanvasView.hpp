#ifndef NETDISP_CANVAS_VIEW_HPP
#define NETDISP_CANVAS_VIEW_HPP

#include <memory>
#include <netdisp/View.hpp>

namespace netdisp {
class DisplayController;
}

namespace netdisp {

class BitmapView : public View {
public:
  BitmapView(unsigned X, unsigned Y, unsigned Width, unsigned Height,
             const uint8_t *Data, unsigned Length);

  void show(DisplayController &DC) override;

private:
  unsigned X;
  unsigned Y;
  unsigned Width;
  unsigned Height;
  std::unique_ptr<uint8_t> BitmapData;
};

// TODO rect, line, circle

} // namespace netdisp

#endif // #ifndef NETDISP_CANVAS_VIEW_HPP