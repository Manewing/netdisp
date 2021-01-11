#ifndef NETDISP_LED_HPP
#define NETDISP_LED_HPP

#include <vector>

namespace netdisp {

class LedController {
public:
  LedController(std::vector<int> LedPins);

  bool setLed(unsigned Led, bool State) const;
  bool setLeds(bool State) const;
  bool blinkLed(unsigned Led, unsigned Times, unsigned DelayMs = 100) const;

private:
  std::vector<int> LedPins;
};

} // namespace netdisp

#endif // #ifndef NETDISP_LED_HPP