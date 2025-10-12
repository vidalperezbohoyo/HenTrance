

#include "Controller.hpp"

Controller::Key Controller::key_ = Controller::Key::None;

Controller::Controller() {}

Controller::init()
{
  pinMode(PIN_UP_MANUAL_BTN, INPUT_PULLUP);
  pinMode(PIN_DOWN_MANUAL_BTN, INPUT_PULLUP);

  LowPower.attachInterruptWakeup(PIN_UP_MANUAL_BTN, ISR_onManualUpButtonPressed, FALLING, SLEEP_MODE);
  LowPower.attachInterruptWakeup(PIN_DOWN_MANUAL_BTN, ISR_onManualDownButtonPressed, FALLING, SLEEP_MODE);
}

Controller::Key Controller::get() {
    Key ret_key = key_;
    key_ = Key::None; // Limpiar valor tras leer
    return ret_key;
}

void Controller::ISR_BUTTON_UP() {
    key_ = Key::Up;
}

void Controller::ISR_BUTTON_DOWN() {
    key_ = Key::Down;
}
