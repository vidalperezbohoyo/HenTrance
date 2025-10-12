#pragma once
// Configuration of firmware PINS.

/**
 * @brief Pin of the shunt resistor for motor current measurement.
 */
#define MOTOR_SHUNT_PIN A2

/**
 * @brief Pin of the LDR voltage divider that reads the brightness.
 */
#define LDR_PIN A1

/**
 * @brief Pins for the user buttons.
 */
#define PIN_UP_MANUAL_BTN PC14
#define PIN_DOWN_MANUAL_BTN PC15

/**
 * @brief Pins for the end stops of the door movement.
 */
#define PIN_ENDSTOP_GOING_UP_RIGHT PB1
#define PIN_ENDSTOP_GOING_UP_LEFT PB11
#define PIN_ENDSTOP_GOING_DOWN_RIGHT PB0
#define PIN_ENDSTOP_GOING_DOWN_LEFT PB10

/**
 * @brief Pins for the motor controller.
 */
#define PIN_IN1_MOTORS PB15
#define PIN_IN2_MOTORS PB14
#define PIN_IN3_MOTORS PB13
#define PIN_IN4_MOTORS PB12