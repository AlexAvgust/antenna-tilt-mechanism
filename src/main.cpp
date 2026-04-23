#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "crsf/crsf.h"
#include "uart/uart.h"
#include "pwm/pwm.h"
#include "helpers/pwm_helpers/pwm_helpers.h"
#include "../CONSTANTS.h"

extern "C" void app_main()
{
    ESP_LOGI("APP", "Starting antenna control system");
    
    init_UART();
    ESP_LOGI("APP", "UART initialized");
    
    init_PWM();
    ESP_LOGI("APP", "PWM initialized");
    
    while (1)
    {
        process_uart_data();
        uint16_t* channels = get_rc_channels_array();
        angle_to_duty_cycle(convert_channel_value_to_angle(channels[CHANNEL_TO_CONTROL_ANTENA_ANGLE]));
        
        // Yield to other FreeRTOS tasks and prevent watchdog timeout
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
