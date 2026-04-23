#pragma once
#include <cstdint>

/**
 * @brief Initialize LEDC PWM peripheral for servo control
 * Configures PWM on GPIO 7 with 50Hz frequency and 12-bit resolution
 * Should be called once during system initialization
 */
void init_PWM();

/**
 * @brief Convert servo angle to PWM duty cycle and apply
 * @param angle Servo position in degrees (0-180)
 * 
 * Servo calibration (ESP32 12-bit LEDC, 50 Hz → 20 ms period, 4096 counts):
 * - 0°   → 1.0 ms pulse → duty = 205  ( 5.0% of 4096)
 * - 90°  → 1.5 ms pulse → duty = 307  ( 7.5% of 4096)
 * - 180° → 2.0 ms pulse → duty = 410  (10.0% of 4096)
 * Formula: duty = 205 + (angle × 205) / 180
 * 
 * Updates PWM output and logs the change
 */
void angle_to_duty_cycle(uint32_t angle);