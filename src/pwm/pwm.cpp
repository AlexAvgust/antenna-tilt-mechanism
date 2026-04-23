#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "../CONSTANTS.h"
#include "esp_log.h"
#include "../helpers/pwm_helpers/pwm_helpers.h"

void init_PWM()
{
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {
        .gpio_num = LED_PIN,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .duty = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

static void set_PWM_duty(uint32_t duty)
{
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}

void angle_to_duty_cycle(uint32_t angle)
{
    // Standard RC servo pulse widths at 50 Hz (20 ms period), 12-bit LEDC (4096 counts):
    // - 0°   → 1.0 ms → duty = 205  ( 5.0% of 4096)
    // - 90°  → 1.5 ms → duty = 307  ( 7.5% of 4096)
    // - 180° → 2.0 ms → duty = 410  (10.0% of 4096)
    // Formula: duty = 205 + (angle * 205) / 180
    uint32_t duty = 205 + (angle * 205) / 180;
    set_PWM_duty(duty);
    ESP_LOGI("PWM", "angle: %lu°, duty: %lu (%.1f%%)", angle, duty, (duty * 100.0) / 4096.0);
}
