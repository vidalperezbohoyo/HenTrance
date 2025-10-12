
#pragma once
#include <LowPower.h>
#include "Arduino.h"
#include "Pins.hpp"

class Controller {
public:
    enum class Key {
        None = 0,
        Up,
        Down
    };

    Controller();

    /**
     * @brief Returns the last pressed key and clears it.
     */
    Key get();

private:
    /**
     * @brief ISR for UP button.
     */
    static void ISR_BUTTON_UP();

    /**
     * @brief ISR for DOWN button.
     */
    static void ISR_BUTTON_DOWN();

private:
    static Key key_;
};