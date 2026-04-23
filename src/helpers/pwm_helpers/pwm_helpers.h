#pragma once
#include <stdint.h>

/**
 * @brief Map a value from one range to another
 * @param x Input value
 * @param in_min Minimum input value
 * @param in_max Maximum input value  
 * @param out_min Minimum output value
 * @param out_max Maximum output value
 * @return Mapped output value, clamped to [out_min, out_max]
 * 
 * Performs linear interpolation from input range to output range with automatic clamping.
 * Formula: ((x - in_min) / (in_max - in_min)) × (out_max - out_min) + out_min
 */
long map_range(long x, long in_min, long in_max, long out_min, long out_max);

/**
 * @brief Convert CRSF channel value to servo angle
 * @param channel_value CRSF channel value (typically 172-1811)
 * @return Angle in degrees (0-180)
 * 
 * Converts raw CRSF protocol channel values to servo angle:
 * - 172 (minimum) → 0°
 * - 991 (center) → 90°
 * - 1811 (maximum) → 180°
 * 
 * Input is automatically clamped to valid CRSF range.
 */
uint8_t convert_channel_value_to_angle(uint16_t channel_value);