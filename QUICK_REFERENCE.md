# Quick Reference Guide

## 🚀 Quick Start

### Upload & Monitor (One Command)
```bash
platformio run --target upload && platformio monitor
```

### What Should Happen
```
[APP] Starting antenna control system
[APP] UART initialized
[APP] PWM initialized
[CRSF] RC channels frame received and parsed
[PWM] angle: 90°, duty: 307 (7.5%)
```

---

## 📋 Command Cheat Sheet

| Task | Command |
|------|---------|
| Build only | `platformio run` |
| Upload | `platformio run --target upload` |
| Clean | `platformio run --target clean` |
| Monitor | `platformio monitor --speed 115200` |
| Full rebuild | `platformio run --target clean && platformio run` |
| List boards | `platformio boards esp32c3` |

---

## 🔌 Hardware Pinout

```
ESP32-C3 (SuperMini)
┌─────────────────────┐
│    USB-C Port       │
└──────┬──────────────┘
       │ Power/GND/Serial
       
GPIO Assignments:
  GPIO 20 → UART1 TX (not used)
  GPIO 21 → UART1 RX (CRSF receiver input)
  GPIO  7 → PWM Output (Servo signal)
  GND     → Common ground
  3V3     → Power
```

---

## 📊 Expected Values

### CRSF Channel Range
| Condition | Channel Value | Servo Angle | PWM Duty |
|-----------|--------------|-------------|----------|
| Minimum | 172 | 0° | 102 |
| Center | 991 | 90° | 307 |
| Maximum | 1811 | 180° | 512 |

### Servo Pulse Widths (50Hz)
| Angle | Pulse Width | Duty % |
|-------|------------|---------|
| 0° | 1.02 ms | 10.2% |
| 90° | 3.07 ms | 15.3% |
| 180° | 5.12 ms | 25.6% |

---

## 🐛 Common Issues & Quick Fixes

### Issue: No Serial Output
**Causes:**
- USB cable not connected properly
- Wrong COM port selected
- Board drivers not installed

**Fix:**
```bash
# List available COM ports
platformio device monitor --list

# Upload to specific port
platformio run --target upload --upload-port /dev/ttyUSB0
```

### Issue: CRSF CRC Errors
```
[CRSF] CRC mismatch - computed: 0xAB, received: 0xCD
```

**Causes:**
- UART connection quality (noise)
- Wrong baud rate
- Receiver incompatibility

**Quick fixes:**
1. Use shorter, twisted-pair cables
2. Verify receiver in CRSF mode (not SBUS)
3. Check transmitter binding

### Issue: Servo Not Moving
1. Check PWM signal with multimeter or scope (should swing 1-2V)
2. Verify servo power supply (4.8-6V typical)
3. Check log for duty values - should change when stick moves
4. Try different servo (hardware test)

### Issue: Continuous Reboot
```
Guru Meditation Error: Core 0 panic'ed (Interrupt wdt timeout on CPU0)
```

**Cause:** Missing `vTaskDelay()` in main loop  
**Fix:** Ensure main loop calls `vTaskDelay(pdMS_TO_TICKS(10))` or similar

---

## 📈 Performance Monitoring

### Check CPU Usage (Rough Estimate)
- Read UART buffer: ~500 µs (when data present)
- Parse CRSF frame: <1 ms
- Convert & output PWM: <100 µs
- **Total per cycle**: ~1-2 ms
- **CPU usage**: ~2-4% @ 160 MHz

### Monitor Logs for Health
```bash
platformio monitor | grep -i "error\|warning\|crc"
```

Expected behavior:
- Should see occasional [CRSF] frames
- Should see [PWM] angle updates
- Should NOT see errors unless signal lost

---

## 🔧 Common Code Patterns

### Read RC Channel Value
```cpp
uint16_t* channels = get_rc_channels_array();
uint16_t ch15_value = channels[15];  // Read channel 15
```

### Add Debug Logging
```cpp
#include "esp_log.h"

static const char *TAG = "MYMODULE";

ESP_LOGI(TAG, "Info message: %d", value);
ESP_LOGW(TAG, "Warning message");
ESP_LOGE(TAG, "Error message");
ESP_LOGD(TAG, "Debug message (requires CONFIG_LOG_DEFAULT_LEVEL_DEBUG)");
```

### Configure New PWM Channel (Future)
```cpp
// In pwm.cpp init_PWM()
ledc_channel_config_t new_channel = {
    .gpio_num = GPIO_NUM_9,      // New GPIO
    .speed_mode = LEDC_MODE,
    .channel = LEDC_CHANNEL_1,   // Use channel 1
    .timer_sel = LEDC_TIMER,
    .duty = 0,
};
ESP_ERROR_CHECK(ledc_channel_config(&new_channel));
```

---

## 📚 File Guide

| File | Purpose |
|------|---------|
| `src/main.cpp` | Application entry point & main loop |
| `src/uart/uart.cpp` | UART driver implementation |
| `src/crsf/crsf.cpp` | CRSF protocol parser |
| `src/pwm/pwm.cpp` | PWM servo driver |
| `src/crc8/crc8.cpp` | CRC8 checksum calculation |
| `CONSTANTS.h` | Global configuration constants |
| `README.md` | Project documentation |
| `ARCHITECTURE.md` | System design & data flow |
| `DEVELOPMENT.md` | Development guide & setup |

---

## 🎯 Key Concepts

### CRSF Protocol
- **Sync byte**: 0xC8
- **Frame type**: 0x16 (RC_CHANNELS_PACKED)
- **Channel resolution**: 11 bits per channel (0-2047)
- **Update rate**: ~200 Hz (5ms per frame)
- **Baud rate**: 420 kbps (standard for CRSF)

### Servo Control (PWM)
- **Frequency**: 50 Hz (20ms period)
- **Pulse range**: 1.02-5.12 ms
- **Resolution**: 12-bit (4096 steps)
- **Safe range**: 1.0-2.0 ms (standard servo)

### System Architecture
```
Input (UART) → Parser (CRSF) → Converter (Angle) → Output (PWM)
```

---

## 📞 Troubleshooting Flowchart

```
System Not Responding
    │
    ├─ Serial output visible?
    │   ├─ YES: System is running
    │   │   └─ Servo moving?
    │   │       ├─ YES: Everything OK ✓
    │   │       └─ NO: Check PWM signal with scope
    │   │
    │   └─ NO: Check USB connection & drivers
    │       └─ Try different USB port
    │
    └─ Rebooting every 5 seconds?
        ├─ YES: Watchdog timeout (missing vTaskDelay)
        └─ NO: Check power supply
```

---

## 💡 Pro Tips

1. **Use `platformio monitor` with timestamps**
   ```bash
   platformio monitor --raw --eol CRLF
   ```

2. **Redirect log output to file**
   ```bash
   platformio monitor > serial_log.txt
   ```

3. **Test CRSF parsing with captured data**
   - Record real CRSF frames from receiver
   - Create test fixture with known data
   - Unit test parser state machine

4. **Oscilloscope measurements**
   - PWM signal: GPIO 7 (should be square wave 50Hz, 1-2ms high)
   - UART RX: GPIO 21 (should show serial traffic at 420kbps)
   - Servo: Should track PWM signal

5. **Servo calibration**
   - Note your servo's actual min/max pulse widths
   - Adjust formula in `angle_to_duty_cycle()` if needed
   - Typical servos: 1.0-2.0 ms, some digitals: 0.9-2.1 ms

---

## 📖 Documentation Map

```
Project Root
│
├─ README.md ..................... Overview & quick start
├─ ARCHITECTURE.md ............... Detailed system design
├─ DEVELOPMENT.md ................ Setup & development guide
├─ CONSTANTS.h ................... Configuration values
│
└─ src/
    ├─ main.cpp ................. Application entry
    ├─ uart/
    │   ├─ uart.h ............... UART API & constants
    │   └─ uart.cpp ............. UART implementation
    ├─ crsf/
    │   ├─ crsf.h ............... CRSF structs & API
    │   └─ crsf.cpp ............. CRSF parser
    ├─ pwm/
    │   ├─ pwm.h ................ PWM API
    │   └─ pwm.cpp .............. PWM driver
    ├─ crc8/
    │   ├─ crc8.h ............... CRC8 API
    │   └─ crc8.cpp ............. CRC8 implementation
    └─ helpers/
        └─ pwm_helpers/
            ├─ pwm_helpers.h .... Converter API
            └─ pwm_helpers.cpp .. Converters
```

---

## ✅ Pre-Flight Checklist

Before claiming "working":
- [ ] Firmware compiles without errors
- [ ] USB connection stable (no drops)
- [ ] UART RX pin connected to receiver (GPIO 21)
- [ ] PWM output connected to servo (GPIO 7)
- [ ] Both 3V3 power and GND connected properly
- [ ] Servo has separate 5V power supply
- [ ] Serial monitor shows startup messages
- [ ] Servo responds to transmitter stick input
- [ ] Servo angle matches transmitter position
- [ ] No watchdog resets happening
- [ ] No CRC errors in logs (or very rare)

---

## 🎓 Learning Path

1. **Start**: Read README.md
2. **Understand**: Study VISUAL_SUMMARY.md diagrams
3. **Deep dive**: Review ARCHITECTURE.md
4. **Develop**: Follow DEVELOPMENT.md
5. **Debug**: Use this Quick Reference Guide

---

**Last Updated**: April 2026  
**Version**: 1.0  
**Status**: ✅ Production Ready (with noted limitations)
