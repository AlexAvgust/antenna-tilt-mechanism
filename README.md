# Antenna Control System

ESP32-C3 based antenna control system that receives CRSF (Crossfire) RC protocol commands via UART and controls a servo motor via PWM.

## Project Overview

This project reads radio control commands from a CRSF receiver connected via UART and converts them into PWM servo control signals. It's designed for autonomous antenna pointing systems in FPV (First Person View) applications.

### Hardware
- **MCU**: ESP32-C3 (SuperMini)
- **Communication**: CRSF Protocol @ 420kbps via UART1
- **Servo Control**: PWM (50Hz, 12-bit resolution)
- **GPIO Pins**:
  - RX: GPIO 21
  - TX: GPIO 20
  - PWM Output: GPIO 7

## Architecture

### Module Structure

```
src/
├── main.cpp           - Application entry point and main loop
├── uart/              - UART communication driver
├── crsf/              - CRSF protocol parser and frame handler
├── pwm/               - PWM servo control
├── helpers/           - Utility functions
│   └── pwm_helpers/   - PWM value conversion helpers
└── crc8/              - CRC8 checksum calculation
```

### Data Flow

```
CRSF Receiver (UART)
    ↓
UART Driver (uart.cpp)
    ↓
CRSF Parser (crsf.cpp) - validates frame, checks CRC
    ↓
RC Channel Array (16 channels)
    ↓
Convert Channel → Angle (pwm_helpers.cpp)
    ↓
Angle → PWM Duty Cycle (pwm.cpp)
    ↓
Servo Motor
```

## CRSF Protocol Details

- **Frame Structure**: [SYNC] [LENGTH] [TYPE] [PAYLOAD...] [CRC8]
- **Sync Byte**: 0xC8
- **Supported Frame**: RC_CHANNELS_PACKED (0x16) - 16 11-bit RC channels
- **Channel Range**: 172 (0°) to 1811 (180°) per CRSF spec
- **Baud Rate**: 420000 bps (8N1)

## PWM Configuration

- **Frequency**: 50 Hz (20ms period)
- **Resolution**: 12-bit (0-4095)
- **Servo Range**: 
  - 1.02ms (duty: 102) = 0°
  - 5.12ms (duty: 512) = 180°

## Current Status

### ✅ Implemented Features
- CRSF protocol frame parsing with CRC validation
- RC channel extraction and decoding
- Channel value to servo angle conversion
- Servo control via PWM output
- FreeRTOS task integration

### ⚠️ Known Limitations
- Only single channel (CH15) antenna control
- No multi-channel support
- No failsafe mechanism
- No PID control for smooth movement
- Limited error logging

## Compilation & Upload

```bash
# Build and upload
platformio run --target upload --environment esp32c3_supermini

# Monitor serial output
platformio monitor
```

## Troubleshooting

### No signal received
- Verify CRSF receiver wired to GPIO 20 (RX1) and GPIO 21 (TX1)
- Check baud rate: 420000 bps
- Verify transmitter is binding with receiver

### Servo not responding
- Check PWM output on GPIO 7 with oscilloscope
- Verify servo control power supply (4.8V-6V)
- Confirm servo signal range (1000-2000 µs)

### CRC errors in logs
- Check UART connection quality (use shorter, twisted cables)
- Verify receiver firmware version compatibility
- Try lowering baud rate if noise issues persist

## Future Improvements

1. **Multi-channel support** - Control multiple antennas simultaneously
2. **Failsafe mechanism** - Center servo on signal loss
3. **Smoother movement** - Add PID controller for acceleration profiles
4. **Telemetry** - Send antenna position back via CRSF
5. **Configuration** - Save calibration to NVS flash
6. **Unit tests** - Add test suite for CRSF parser

## License

This project is provided as-is for educational and hobby use.
