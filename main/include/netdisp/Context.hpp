#ifndef NETDISP_CONTEXT_HPP
#define NETDISP_CONTEXT_HPP

namespace netdisp {
class LedController;
class ViewController;
} // namespace netdisp

namespace netdisp {

struct Context {
  LedController &LedCtrl;
  ViewController &ViewCtrl;
};

} // namespace netdisp

#endif // #ifndef NETDISP_CONTEXT_HPP