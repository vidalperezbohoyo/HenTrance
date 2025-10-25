

#include "Controller.hpp"

Controller::Key Controller::key_ = Controller::Key::None;

Controller::Controller() {}

void Controller::init()
{
  pinMode(PIN_UP_MANUAL_BTN, INPUT_PULLUP);
  pinMode(PIN_DOWN_MANUAL_BTN, INPUT_PULLUP);

  LowPower.attachInterruptWakeup(PIN_UP_MANUAL_BTN, ISR_BUTTON_UP, FALLING, SLEEP_MODE);
  LowPower.attachInterruptWakeup(PIN_DOWN_MANUAL_BTN, ISR_BUTTON_DOWN, FALLING, SLEEP_MODE);
}

Controller::Key Controller::read()
{
    if (digitalRead(PIN_UP_MANUAL_BTN) == LOW)
    {
        return Key::Up;
    }

    if (digitalRead(PIN_DOWN_MANUAL_BTN) == LOW)
    {
        return Key::Down;
    }

    return Key::None;

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
