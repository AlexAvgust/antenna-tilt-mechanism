#include "../CONSTANTS.h"
#include "../uart/uart.h"
#include "driver/uart.h"
#include "esp_log.h"

static uint8_t raw_UART_data[UART_FIFO_SIZE];

void init_UART()
{
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, UART_FIFO_SIZE, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

uart_buffer_t raw_read_UART()
{
    int length_of_uart_buff = uart_read_bytes(UART_NUM_1, raw_UART_data, sizeof(raw_UART_data), pdMS_TO_TICKS(5));
    
    if (length_of_uart_buff < 0)
    {
        ESP_LOGE("UART", "Error reading UART data (error code: %d)", length_of_uart_buff);
        return uart_buffer_t{.length_of_uart_buff = 0, .data = nullptr};
    }
    else if (length_of_uart_buff == 0)
    {
        // No data available - this is normal, not an error
        return uart_buffer_t{.length_of_uart_buff = 0, .data = nullptr};
    }
    else
    {
        uart_buffer_t buffer = {
            .length_of_uart_buff = length_of_uart_buff,
            .data = raw_UART_data};
        return buffer;
    }
}
