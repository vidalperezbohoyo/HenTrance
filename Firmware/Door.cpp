
#include "Door.hpp"
#include "Pins.hpp"

Door::Door() : motors_controller_(PIN_IN1_MOTORS, PIN_IN2_MOTORS, PIN_IN3_MOTORS, PIN_IN4_MOTORS), open_(false) {}


bool Door::open() 
{
    if (open_) return true; // Already open
    // Logic to open the door
    motors_controller_.forward();
    // Wait until the endstops indicate the door is open
    while (digitalRead(PIN_ENDSTOP_GOING_UP_RIGHT) == HIGH || digitalRead(PIN_ENDSTOP_GOING_UP_LEFT) == HIGH) {
        // Busy wait, can be improved with timeout
    }
    motors_controller_.stop();
    open_ = true;
    return true;
}


bool Door::close()
{
    if (!open_) return true; // Already closed
    // Logic to close the door
    motors_controller_.backward();
    // Wait until the endstops indicate the door is closed
    while (digitalRead(PIN_ENDSTOP_GOING_DOWN_RIGHT) == HIGH || digitalRead(PIN_ENDSTOP_GOING_DOWN_LEFT) == HIGH) {
        // Busy wait, can be improved with timeout
    }
    motors_controller_.stop();
    open_ = false;
    return true;
}

bool Door::isOpen() const 
{
    return open_;
}

bool Door::isClose() const
{
    return !open_;
}