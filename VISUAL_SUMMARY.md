# Antenna Control System - Visual Summary

## System Block Diagram

```
┌──────────────────────────────────────────────────────────────────┐
│                     ESP32-C3 (SuperMini)                          │
│                                                                    │
│  ┌────────────────────────────────────────────────────────────┐  │
│  │                    FREERTOS KERNEL                         │  │
│  │  ┌──────────────────────────────────────────────────────┐  │  │
│  │  │              Main Application Loop                   │  │  │
│  │  │  1. Read UART buffer                                │  │  │
│  │  │  2. Parse CRSF frames                               │  │  │
│  │  │  3. Extract channel value                           │  │  │
│  │  │  4. Convert to servo angle                          │  │  │
│  │  │  5. Update PWM duty cycle                           │  │  │
│  │  │  6. Yield for 10ms                                  │  │  │
│  │  └──────────────────────────────────────────────────────┘  │  │
│  └────────────────────────────────────────────────────────────┘  │
│                                                                    │
│  ┌─────────────┐  ┌─────────────┐  ┌──────────────┐             │
│  │   UART 1    │  │    CRC8     │  │   LEDC PWM   │             │
│  │             │  │  Checker    │  │              │             │
│  │ 420kbps     │  │             │  │ 50Hz, 12-bit │             │
│  └──────┬──────┘  └─────────────┘  └──────┬───────┘             │
│         │                                   │                    │
└─────────┼───────────────────────────────────┼────────────────────┘
          │                                   │
    ┌─────┴─────┐                      ┌──────┴──────┐
    │   GPIO21   │                      │   GPIO 7    │
    │   (RX1)    │                      │   (PWM OUT) │
    └─────▲─────┘                      └──────▲──────┘
          │                                    │
    ┌─────┴──────────────────────┐            │
    │  CRSF RC Receiver          │            │
    │  (420kbps serial stream)   │        ┌───┴───────┐
    └────────────────────────────┘        │Servo Motor│
                                          └────────────┘
```

## Signal Flow with Timing

```
Time (ms)
0    ├─── UART byte received ──┐
10   │   (420kbps = ~24µs/bit)  │
20   │                          ↓
     │                    ┌──────────────┐
30   │                    │CRSF Parser   │
     │                    │(state mach)  │
40   │                    └──────┬───────┘
     │                           ↓
50   │                    ┌──────────────┐
     │                    │CRC Check     │
60   │                    │ & Unpack     │
     │                    └──────┬───────┘
     │                           ↓
70   │                    ┌──────────────┐
     │                    │Channel→Angle │
80   │                    │Conversion    │
     │                    └──────┬───────┘
     │                           ↓
90   │                    ┌──────────────┐
     │                    │Duty Calc     │
100  │                    └──────┬───────┘
     │                           ↓
110  │                    ┌──────────────┐
     │                    │PWM Update    │
120  │                    └──────────────┘
     └── FreeRTOS vTaskDelay(10ms) ──→ Watchdog & Other Tasks
```

## Data Structures

### CRSF RC Channels Frame
```
Byte 0:        0xC8        (SYNC)
Byte 1:        0x18        (LENGTH = 24 bytes)
Byte 2:        0x16        (TYPE = RC_CHANNELS_PACKED)
Bytes 3-22:    [PAYLOAD]   (22 bytes of packed 11-bit channels)
Byte 23:       [CRC8]      (Checksum)

Payload packing (11 bits per channel × 16 channels):
┌─ Channel 0 (11 bits)
├─ Channel 1 (11 bits)
├─ Channel 2 (11 bits)
├─ ...
└─ Channel 15 (11 bits)
```

### Channel Value Range
```
CRSF Value          Angle       PWM Duty    Servo Position
172 .............. 0°  ........ 102 (1.02ms) ... Min
991 .............. 90° ........ 307 (3.07ms) ... Center  
1811 ............. 180° ...... 512 (5.12ms) ... Max
```

## State Machine: CRSF Parser

```
    ┌─────────────────────┐
    │   WAIT_SYNC         │◄──────────┐
    │ (looking for 0xC8)  │           │
    └──────────┬──────────┘           │
               │ Found SYNC           │
               ↓                      │
    ┌─────────────────────┐           │
    │  WAIT_LENGTH        │           │
    │ (4 ≤ len ≤ 62)      │           │
    └──────────┬──────────┘           │
               │ Valid Length         │
               ↓                      │
    ┌─────────────────────┐      Reset│
    │ READ_PAYLOAD        │      (bad │
    │ (collect bytes)     │      len) │
    └──────────┬──────────┘           │
               │ Frame Complete       │
               ↓                      │
    ┌─────────────────────┐           │
    │ CRC VERIFICATION    │           │
    └──────────┬──────────┘           │
               │ CRC Match            │
               ├──→ Unpack Channels ──┤
               │                      │
               └──────→ Reset ───────→└
```

## Performance Metrics

| Metric | Value |
|--------|-------|
| UART Baud Rate | 420 kbps |
| UART Latency (24 bytes) | ~460 µs |
| CRSF Frame Rate | ~200 Hz |
| PWM Frequency | 50 Hz |
| PWM Resolution | 12-bit (4096 levels) |
| Max Servo Speed | ~60°/frame @ 50Hz |
| Task Yield Interval | 10 ms |
| WDT Timeout | ~5 seconds |

## Power Consumption

```
Component           Typical Current
─────────────────────────────────────
ESP32-C3            ~80-100 mA
UART (RX)           ~5 mA
PWM Output          <1 mA (logic level)
Servo Motor         300-500 mA (load dependent)
─────────────────────────────────────
Total (idling)      ~85-105 mA
Total (servo moving) ~400-600 mA
```

## Pin Assignment Summary

```
ESP32-C3 Pin        Function        Connected To
─────────────────────────────────────────────────
GPIO 20             UART1 TX        (not used)
GPIO 21             UART1 RX        CRSF Receiver
GPIO 7              LEDC PWM        Servo Signal
─────────────────────────────────────────────────
3V3                 Power           Receiver VCC
GND                 Ground          Receiver GND, Servo GND
```

## Compilation Details

```
Framework:          ESP-IDF (native)
Platform:           Espressif32
Board:              ESP32-C3 DevKit M-1
Compiler:           GCC (xtensa-esp32-elf)
C++ Standard:       C++17 (via -std=gnu++17)
Optimization:       O2 (Speed optimized)
```
