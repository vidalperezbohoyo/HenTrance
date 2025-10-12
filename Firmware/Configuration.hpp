#pragma once
// Configuration of firmware CONSTANTS.

/**
 * @brief Time spent in Deep Sleep between time and brightness checks.
 * @note Time in milliseconds.
 */
const uint32_t DEEP_SLEEP_TIME = 15000;

/**
 * @brief Threshold to open or close based on brightness (if operation is permitted).
 * @note ADC value. May vary in every situation.
 */
const uint32_t BRIGHTNESS_OPEN_THRESHOLD = 400;
const uint32_t BRIGHTNESS_CLOSE_THRESHOLD = 200;

/**
 * @brief Range when opening can be performed. Outside this range, opening can only be performed manually with the user buttons.
 * @note The value is the hour. Must be an AM hour in 24h format.
 */
const uint32_t MIN_OPEN_HOUR = 6;
const uint32_t MAX_OPEN_HOUR = 12;

/**
 * @brief Range when closing can be performed. Outside this range, closing can only be performed manually with the user buttons.
 * @note The value is the hour. Must be a PM hour in 24h format.
 */
const uint32_t MIN_CLOSE_HOUR = 18;
const uint32_t MAX_CLOSE_HOUR = 24;

/**
 * @brief For motor torque failure detection. Over this threshold the motor will stop safely, and open the door.
 * @note ADC value. May vary in every situation.
 */
const uint32_t MOTOR_STALL_THRESHOLD = 100;