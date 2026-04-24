#pragma once
#include <stdint.h>
#include <hal/ledc_types.h>
#include <driver/ledc.h>

// ============================================================================
// UART Configuration (CRSF Receiver Interface)
// ============================================================================
/// @brief UART1 TX pin (not used in this application)
constexpr uint8_t TX_PIN = 20;

/// @brief UART1 RX pin - connected to CRSF receiver
constexpr uint8_t RX_PIN = 21;

/// @brief UART FIFO buffer size in bytes
constexpr int UART_FIFO_SIZE = 1024;

/// @brief UART baud rate for CRSF protocol (standard 420kbps)
constexpr int UART_BAUD_RATE = 420000;

// ============================================================================
// CRSF Protocol Configuration
// ============================================================================
/// @brief Maximum CRSF frame length in bytes
/// CRSF frames: [SYNC(1) | LEN(1) | TYPE(1) | PAYLOAD(1-62) | CRC(1)]
/// Maximum payload = 62 bytes, so max frame = 1 + 1 + 1 + 62 = 65, use 64 for safety
constexpr uint8_t MAX_CRSF_FRAME_LENGTH = 64;

// ============================================================================
// PWM/Servo Configuration (Antenna Control Output)
// ============================================================================
/// @brief GPIO pin for PWM servo control output
constexpr uint8_t LED_PIN = 7;

/// @brief LEDC PWM channel (0-7 available, use channel 0)
constexpr ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_0;

/// @brief LEDC PWM timer (0-3 available, use timer 0)
constexpr ledc_timer_t LEDC_TIMER = LEDC_TIMER_0;

/// @brief LEDC speed mode (HIGH_SPEED or LOW_SPEED)
constexpr ledc_mode_t LEDC_MODE = LEDC_LOW_SPEED_MODE;

/// @brief LEDC duty resolution in bits (8-20 bit available)
/// 12-bit = 4096 levels, sufficient for servo control
constexpr ledc_timer_bit_t LEDC_DUTY_RES = LEDC_TIMER_12_BIT;

/// @brief PWM frequency in Hz - 50Hz standard for servo control (20ms period)
constexpr uint32_t LEDC_FREQUENCY = 50;

// ============================================================================
// Application Configuration
// ============================================================================
/// @brief RC channel index to control antenna angle (0-15)
/// Channel 15 is typically the AUX2 channel on RC transmitters
constexpr uint8_t CHANNEL_TO_CONTROL_ANTENA_ANGLE = 15;