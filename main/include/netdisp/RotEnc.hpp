#ifndef NETDISP_ROT_ENC_HPP
#define NETDISP_ROT_ENC_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <rotary_encoder.h>

namespace netdisp {

class RotEncController {
public:
  RotEncController(int PinA, int PinB, bool HalsSteps = false,
                   bool Flip = false);
  ~RotEncController();

  bool waitForEvent(int &Position, unsigned TimeoutMs = 2);

private:
  QueueHandle_t EventQueue;
  rotary_encoder_info_t Info;
};

} // namespace netdisp

#endif // #ifndef NETDISP_ROT_ENC_HPP