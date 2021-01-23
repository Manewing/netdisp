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
  std::unique_ptr<uint8_t[]> BitmapData;
};

class LineView : public View {
public:
  LineView(int X0, int Y0, int X1, int Y1);

  void show(DisplayController &DC) override;

private:
  int X0 = 0;
  int Y0 = 0;
  int X1 = 0;
  int Y1 = 0;
};

class RectView : public View {
public:
  RectView(int X, int Y, unsigned W, unsigned H);

  void show(DisplayController &DC) override;

private:
  int X = 0;
  int Y = 0;
  unsigned W = 0;
  unsigned H = 0;
};

class CircleView : public View {
public:
  CircleView(int X, int Y, unsigned Radius);

  void show(DisplayController &DC) override;

private:
  int X = 0;
  int Y = 0;
  unsigned Radius = 0;
};


// TODO rect, line, circle

} // namespace netdisp

#endif // #ifndef NETDISP_CANVAS_VIEW_HPP