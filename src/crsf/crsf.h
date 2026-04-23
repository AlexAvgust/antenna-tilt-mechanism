#pragma once
#include <stdint.h>

/**
 * @brief CRSF Parser States
 * WAIT_SYNC: Searching for frame synchronization byte (0xC8)
 * WAIT_LENGTH: Expecting frame length byte (4-62 bytes)
 * READ_PAYLOAD: Collecting frame payload and CRC byte
 */
typedef enum
{
    WAIT_SYNC,
    WAIT_LENGTH,
    READ_PAYLOAD
} parser_state_t;

/**
 * @brief CRSF Protocol Constants
 * SYNC: Frame synchronization byte (0xC8)
 * RC_CHANNELS_PACKED: RC channel frame type identifier
 */
typedef enum
{
    SYNC = 0xC8,
    RC_CHANNELS_PACKED = 0x16
} CRSF_BYTES_STATES;

/**
 * @brief Packed RC channels structure (88 bits / 11 bits per channel)
 * Represents 16 11-bit RC channel values packed into a continuous bitstream
 * Range per channel: 0-2047 (11-bit), typical use: 172-1811
 */
struct rc_channels_packed
{
    uint16_t channel0 : 11;
    uint16_t channel1 : 11;
    uint16_t channel2 : 11;
    uint16_t channel3 : 11;
    uint16_t channel4 : 11;
    uint16_t channel5 : 11;
    uint16_t channel6 : 11;
    uint16_t channel7 : 11;
    uint16_t channel8 : 11;
    uint16_t channel9 : 11;
    uint16_t channel10 : 11;
    uint16_t channel11 : 11;
    uint16_t channel12 : 11;
    uint16_t channel13 : 11;
    uint16_t channel14 : 11;
    uint16_t channel15 : 11;
} __attribute__((packed));

/**
 * @brief Process incoming UART data and feed to CRSF parser
 * Thread-safe wrapper that reads UART buffer and parses frames
 */
void process_uart_data();

/**
 * @brief Get decoded RC channel array
 * @return Pointer to array of 16 uint16_t channel values (0-2047)
 * @note Values are updated on successful frame reception
 */
uint16_t* get_rc_channels_array();