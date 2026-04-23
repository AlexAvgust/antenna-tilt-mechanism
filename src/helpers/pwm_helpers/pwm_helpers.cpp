#include <stdint.h>
#include "esp_log.h"

static const char *TAG = "PWM_HELPERS";

// CRSF channel value range constants
constexpr uint16_t CRSF_CHANNEL_MIN = 172;    // Minimum channel value
constexpr uint16_t CRSF_CHANNEL_MAX = 1811;   // Maximum channel value
constexpr uint16_t CRSF_CHANNEL_RANGE = CRSF_CHANNEL_MAX - CRSF_CHANNEL_MIN;

long map_range(long x, long in_min, long in_max, long out_min, long out_max)
{
    // Clamp input value to valid range
    if (x < in_min) x = in_min;
    if (x > in_max) x = in_max;
    
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t convert_channel_value_to_angle(uint16_t channel_value)
{
    // Clamp channel value to valid CRSF range
    if (channel_value < CRSF_CHANNEL_MIN)
    {
        ESP_LOGW(TAG, "Channel value %u below minimum %u", channel_value, CRSF_CHANNEL_MIN);
        channel_value = CRSF_CHANNEL_MIN;
    }
    if (channel_value > CRSF_CHANNEL_MAX)
    {
        ESP_LOGW(TAG, "Channel value %u exceeds maximum %u", channel_value, CRSF_CHANNEL_MAX);
        channel_value = CRSF_CHANNEL_MAX;
    }
    
    // Convert: (channel_value - 172) / (1811 - 172) * 180
    // Result: 0-180 degrees
    uint8_t angle = (channel_value - CRSF_CHANNEL_MIN) * 180 / CRSF_CHANNEL_RANGE;
    
    return angle;
}