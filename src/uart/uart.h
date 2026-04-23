#pragma once
#include <stdint.h>

/**
 * @brief UART buffer structure
 * Contains pointer to data and length of valid bytes
 */
typedef struct {
    int length_of_uart_buff;        ///< Number of valid bytes in buffer
    uint8_t *data;                  ///< Pointer to UART data (or nullptr if empty)
} uart_buffer_t;

/**
 * @brief Read available data from UART peripheral
 * @return UART buffer containing received data and length
 *         Returns empty buffer (length=0, data=nullptr) if no data or error
 * 
 * Non-blocking read with 100ms timeout
 * Uses internal static buffer (re-used on each call)
 */
uart_buffer_t raw_read_UART();

/**
 * @brief Initialize UART peripheral for CRSF receiver communication
 * Configures UART1 with:
 * - Baud: 420000 bps (CRSF standard)
 * - Data: 8-bit
 * - Parity: None
 * - Stop bits: 1
 * - Flow control: Disabled
 * - Pins: RX=GPIO21, TX=GPIO20
 * 
 * Should be called once during system initialization
 */
void init_UART();