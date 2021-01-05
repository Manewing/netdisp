#include <cstring>
#include <netdisp/CanvasView.hpp>
#include <netdisp/Display.hpp>

namespace netdisp {

BitmapView::BitmapView(unsigned X, unsigned Y, unsigned Width, unsigned Height,
                       const uint8_t *BmpData, unsigned Length)
    : X(X), Y(Y), Width(Width), Height(Height),
      BitmapData(new uint8_t[Length]) {
  std::memcpy(BitmapData.get(), BmpData, Length);
}

void BitmapView::show(DisplayController &DC) {
  DC.drawBitmap(X, Y, Width, Height, BitmapData.get());
}

} // namespace netdisp