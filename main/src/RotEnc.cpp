#include <netdisp/RotEnc.hpp>

namespace netdisp {

RotEncController::RotEncController(int PinA, int PinB, bool HalsSteps,
                                   bool Flip) {
  // TODO Move this to main?
  ESP_ERROR_CHECK(gpio_install_isr_service(0));

  ESP_ERROR_CHECK(rotary_encoder_init(&Info, static_cast<gpio_num_t>(PinA),
                                      static_cast<gpio_num_t>(PinB)));
  ESP_ERROR_CHECK(rotary_encoder_enable_half_steps(&Info, HalsSteps));

  if (Flip) {
    ESP_ERROR_CHECK(rotary_encoder_flip_direction(&Info));
  }

  EventQueue = rotary_encoder_create_queue();

  ESP_ERROR_CHECK(rotary_encoder_set_queue(&Info, EventQueue));
}

RotEncController::~RotEncController() {
  ESP_ERROR_CHECK(rotary_encoder_uninit(&Info));
}

bool RotEncController::waitForEvent(int &Position, unsigned TimeoutMs) {
  rotary_encoder_event_t Event;
  if (xQueueReceive(EventQueue, &Event, TimeoutMs / portTICK_PERIOD_MS)) {
    Position = Event.state.position;
    return true;
  }
  return false;
}

} // namespace netdisp