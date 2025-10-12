#pragma once
// Configuration of PINS of the firmware.

/**
 * @brief Pin of shunt resistor of the motor current to known the current applied.
 */
#define MOTOR_SHUNT_PIN A2

/**
 * @brief Pin of the LDR voltage divider that read the brightness.
 */
#define LDR_PIN A1

/**
 * @brief Pins of the user buttons.
 */
#define PIN_UP_MANUAL_BTN PC14
#define PIN_DOWN_MANUAL_BTN PC15

/**
 * @brief Pins of the end stops of the door movement. 
 */
#define PIN_ENDSTOP_GOING_UP_RIGHT PB1
#define PIN_ENDSTOP_GOING_UP_LEFT PB11
#define PIN_ENDSTOP_GOING_DOWN_RIGHT PB0
#define PIN_ENDSTOP_GOING_DOWN_LEFT PB10

/**
 * @brief Pins of the motor controller. 
 */
#define PIN_IN1_MOTORS PB15
#define PIN_IN2_MOTORS PB14
#define PIN_IN3_MOTORS PB13
#define PIN_IN4_MOTORS PB12