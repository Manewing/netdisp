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

LineView::LineView(int X0, int Y0, int X1, int Y1)
    : X0(X0), Y0(Y0), X1(X1), Y1(Y1) {}

void LineView::show(DisplayController &DC) { DC.drawLine(X0, Y0, X1, Y1); }

RectView::RectView(int X, int Y, unsigned W, unsigned H)
    : X(X), Y(Y), W(W), H(H) {}

void RectView::show(DisplayController &DC) { DC.drawRect(X, Y, W, H); }

CircleView::CircleView(int X, int Y, unsigned Radius)
    : X(X), Y(Y), Radius(Radius) {}

void CircleView::show(DisplayController &DC) { DC.drawCircle(X, Y, Radius); }

} // namespace netdisp