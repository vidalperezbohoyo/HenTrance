#pragma once

class Controller
{
    enum class Key
    {
        NONE = 0,
        UP = PIN_UP_MANUAL_BTN,
        DOWN = PIN_DOWN_MANUAL_BTN
    }


    static Key get();


private:

    static void ISR_BUTTON_UP();

    static void ISR_BUTTON_DOWN();



    static Key key_;


};