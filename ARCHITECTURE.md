# Architecture Document

## System Overview

The Antenna Control System is a real-time embedded application running on ESP32-C3 that receives remote control commands via the CRSF protocol and controls a servo motor.

```
┌─────────────────────────────────────────────────────────────┐
│                   ANTENNA CONTROL SYSTEM                    │
│                   (ESP32-C3 + FreeRTOS)                     │
│                                                             │
│  Input Layer                 Processing Layer             │
│  ┌──────────────┐           ┌──────────────┐             │
│  │ CRSF Receiver│──UART1─→ │ CRSF Parser  │             │
│  │ (420k baud)  │           │ + CRC Check  │             │
│  └──────────────┘           └──────┬───────┘             │
│                                    │                      │
│                         Output Layer                       │
│                                    │                      │
│                            ┌───────▼───────┐             │
│                            │ PWM Converter  │             │
│                            │ (0-180°)       │             │
│                            └───────┬───────┘             │
│                                    │                      │
│                         ┌──────────▼──────────┐           │
│                         │ LEDC PWM Output     │           │
│                         │ (GPIO 7, 50Hz)      │           │
│                         └─────────────────────┘           │
└─────────────────────────────────────────────────────────────┘
         │                                           │
         └──────────────────┬──────────────────────┘
                            │
         ┌──────────────────┴──────────────────────┐
         │                                         │
    ┌────▼────┐                              ┌────▼─────┐
    │  UART1  │                              │ PWM GPIO7 │
    │ RX:GPIO21                              │ Servo    │
    │ TX:GPIO20                              │ Signal   │
    └──────────┘                             └──────────┘
```

## Module Architecture

### 1. UART Driver (`uart/`)
**Purpose**: Low-level UART peripheral abstraction

**Responsibilities**:
- Initialize UART1 (420k baud, 8N1)
- Read data from UART FIFO into buffer
- Handle hardware errors

**Data Flow**:
```
CRSF Receiver → UART Hardware → ESP32 UART Peripheral → RAM Buffer
```

**Key Functions**:
- `init_UART()` - One-time hardware setup
- `raw_read_UART()` - Non-blocking read from RX buffer

**Configuration** (CONSTANTS.h):
```cpp
RX_PIN = 21
TX_PIN = 20
UART_BAUD_RATE = 420000
```

---

### 2. CRSF Parser (`crsf/`)
**Purpose**: Decode CRSF protocol frames and extract RC channel values

**Responsibilities**:
- Implement frame state machine (WAIT_SYNC → WAIT_LENGTH → READ_PAYLOAD)
- Validate frame structure
- Verify CRC checksum
- Unpack 11-bit channel values from bitstream
- Maintain current channel array

**CRSF Frame Structure**:
```
Byte 0:    [0xC8]         - SYNC byte
Byte 1:    [len 4-62]     - Frame length (payload + type + crc)
Byte 2:    [0x16]         - Frame type (RC_CHANNELS_PACKED)
Bytes 3-22: [payload]     - 22 bytes = 16 × 11-bit channels packed
Byte 23:   [crc]          - CRC8 checksum
```

**State Machine**:
```
    WAIT_SYNC
        ↓ (found 0xC8)
    WAIT_LENGTH
        ↓ (4 ≤ len ≤ 62)
    READ_PAYLOAD
        ↓ (collected all bytes)
    CRC_CHECK
        ├→ Valid → UNPACK_CHANNELS → WAIT_SYNC
        └→ Invalid → WAIT_SYNC
```

**Key Functions**:
- `process_uart_data()` - Main entry point, feeds bytes to parser
- `parser_feed_byte()` - State machine implementation (static)
- `unpack_channels()` - Extracts 11-bit values from packed bitstream
- `get_rc_channels_array()` - Returns current channel values

**Data Structure**:
```cpp
struct rc_channels_packed {  // 22 bytes, 176 bits
    uint16_t channel0:11;    // Bits 0-10
    uint16_t channel1:11;    // Bits 11-21
    // ... channel2-15 ...
} __attribute__((packed));
```

---

### 3. CRC8 Module (`crc8/`)
**Purpose**: Calculate and verify CRSF frame checksums

**Responsibilities**:
- Compute CRC8 using CRSF polynomial (0xD5)
- Verify frame integrity

**Algorithm**:
```
Polynomial: x^8 + x^7 + x^6 + x^5 + x^2 + 1 = 0xD5
Initial: 0x00
For each byte:
  XOR with CRC
  For each bit:
    if MSB set: (crc << 1) ^ 0xD5
    else: crc << 1
```

**Key Functions**:
- `crc8(data, length)` - Calculate checksum

---

### 4. PWM Driver (`pwm/`)
**Purpose**: Control servo motor via PWM output

**Responsibilities**:
- Initialize LEDC peripheral
- Convert servo angles to PWM duty cycles
- Apply PWM output

**Configuration**:
```
GPIO Pin: 7
Frequency: 50 Hz (20ms period)
Resolution: 12-bit (0-4095)
Max Duty: 512 (5.12ms @ 50Hz = 180°)
Min Duty: 102 (1.02ms @ 50Hz = 0°)
```

**Servo Calibration**:
```
Angle   Pulse Width   Duty (12-bit)   Percentage
0°      1.02 ms       102             10.2%
90°     3.07 ms       307             7.5% (offset)
180°    5.12 ms       512             12.5%

Formula: duty = 102 + (angle * 410) / 180
```

**Key Functions**:
- `init_PWM()` - One-time hardware setup
- `set_PWM_duty()` - Apply duty cycle to hardware
- `angle_to_duty_cycle()` - Convert angle → duty and apply

---

### 5. PWM Helpers (`helpers/pwm_helpers/`)
**Purpose**: Value conversion utilities

**Responsibilities**:
- Convert CRSF channel values (172-1811) to servo angles (0-180°)
- Generic linear mapping function with clamping

**Conversion Chain**:
```
CRSF Value (11-bit, 0-2047)
  ↓ (scale to CRSF operating range)
Channel Value (172-1811)
  ↓ (map to angle range)
Servo Angle (0-180°)
  ↓ (convert to PWM duty)
PWM Duty Cycle (102-512)
  ↓
Hardware PWM Output
```

**Key Functions**:
- `map()` - Generic linear interpolation with clamping
- `convert_channel_value_to_angle()` - CRSF → Angle conversion

---

## Data Flow Diagram

### Complete Signal Path
```
CRSF Receiver
     │ (UART 1 @ 420k baud)
     ↓
[UART1 Hardware Buffer]
     │ (periodic read)
     ↓
raw_UART_data[] (256 bytes)
     │ (process_uart_data)
     ↓
parser_feed_byte()
     │ (byte-by-byte parsing)
     ├→ WAIT_SYNC: search for 0xC8
     ├→ WAIT_LENGTH: validate frame size
     └→ READ_PAYLOAD: accumulate bytes
         │ (when complete)
         ├→ crc8() verification
         ├→ unpack_channels() if valid
         │
         ↓
rc_channels_frame_raw[16]
(updated only on valid frame)
     │ (main loop reads index 15)
     ↓
convert_channel_value_to_angle()
(172-1811 → 0-180°)
     │
     ↓
angle_to_duty_cycle()
     │ (102 + (angle * 410) / 180)
     ↓
ledc_set_duty() / ledc_update_duty()
     │
     ↓
[LEDC Hardware PWM]
     │ (50 Hz timer)
     ↓
GPIO 7 PWM Signal
     │ (1.02-5.12ms pulses)
     ↓
Servo Motor
```

### Main Loop Timing
```
Time    Task                        Duration    Notes
─────────────────────────────────────────────────────────
0 ms   │ process_uart_data()        ~500 µs     UART read + parsing
0.5 ms │ get_rc_channels_array()    ~1 µs       Array pointer
0.5 ms │ convert_channel_value...   ~5 µs       Math operation
0.5 ms │ angle_to_duty_cycle()      ~10 µs      PWM update + log
0.6 ms │ vTaskDelay(10ms)           10 ms       Yield control
10.6 ms│ [Next iteration]            ~10 Hz rate

Effective update rate: ~100 Hz
Max servo angle change per frame: ~6° @ 100Hz
```

## Memory Map

### Static Allocations
```
Section          Size      Usage
──────────────────────────────────────
Data/BSS         ~500 B    Global variables
  - raw_UART_data[256]     UART buffer
  - rc_channels_frame      Packed frame struct
  - rc_channels_frame_raw  Channel array
  - crsf_buff_frame[64]    Frame parser buffer (in function)

Stack            ~4 KB     FreeRTOS task stack
Heap             varies    FreeRTOS allocator
```

### Memory-Critical Areas
1. **UART Buffer**: 256 bytes - could be reduced to 128 for space
2. **CRSF Buffer**: 64 bytes - exactly sized to spec
3. **Channel Array**: 32 bytes - minimal, 16 channels × 2 bytes

---

## Concurrency & Thread Safety

### Single-Task Design
Current architecture runs entirely in the main FreeRTOS task:
- No separate threads
- All processing sequential
- No mutex/semaphore locks needed
- Safe from race conditions (single-threaded)

### Critical Sections
```cpp
// Only place data is updated: unpack_channels()
memcpy(&rc_channels_frame, payload, sizeof(...));
// Main loop reads rc_channels_frame_raw[] without lock
uint16_t value = channels[index];  // Safe - atomic 16-bit read
```

### Potential Issues
- If UART ISR is enabled in future, add spinlock protection
- Channel array could be partially read during update (11-bit → 16-bit)
- Negligible risk - worst case: one stale value

---

## Error Handling Strategy

### Error Handling by Module

#### UART
```
Issue                  Response              Consequence
──────────────────────────────────────────────────────
read() returns -1      Log error, return     No data processed
                       empty buffer          
read() returns 0       Return empty buffer   No data available
                       (normal condition)    (not an error)
Hardware error         ESP_ERROR_CHECK()     System reset
```

#### CRSF Parser
```
Issue                  Response              Consequence
──────────────────────────────────────────────────────
Invalid sync byte      Skip byte, try next   Byte lost, search continues
Invalid length         Reset parser          Frame lost, start new search
CRC mismatch           Log warning, discard  Frame lost, try next
Unknown frame type     Log debug, discard    Frame type ignored
Frame too long         Reset parser          Buffer overflow prevented
```

#### PWM
```
Issue                  Response              Consequence
──────────────────────────────────────────────────────
Channel out of range   Clamp to valid range  Limited servo range
(0-2047)              
Channel < 172          Clamp to 172          Servo pinned at 0°
Channel > 1811         Clamp to 1811         Servo pinned at 180°
```

### No Failsafe Currently
**Limitation**: System has no watchdog for lost signal
- If receiver power fails: channel values freeze
- Servo holds last position indefinitely
- **Future improvement**: Add signal loss detection

---

## Performance Characteristics

### Latency
```
Component              Latency
─────────────────────────────────────
UART acquisition       < 1ms (worst case)
CRSF parsing           < 1ms (worst case)
CRC verification       < 10µs
Channel conversion     < 5µs
PWM update            < 50µs
──────────────────────────────────────
Total (best case)      ~1ms
Total (worst case)     ~2ms
```

### Throughput
- UART: 420 kbps = 52.5 kB/s
- CRSF frames: ~200 Hz (typical RC rate)
- PWM updates: 50 Hz (servo hardware limited)

### CPU Usage
- Estimated: 5-10% of single-core CPU @ 160 MHz
- Margin for growth: Very good
- Can easily handle 10 channels, multiple servos

---

## Reliability & Robustness

### Implemented Safety Features
✅ CRC8 verification on each frame  
✅ Input value clamping (prevent servo damage)  
✅ Frame size validation  
✅ Log warnings for anomalies  
✅ FreeRTOS watchdog integration  
✅ Automatic parser state reset on errors  

### Potential Improvements
❌ Signal loss detection  
❌ Failsafe servo position  
❌ Over-temperature detection  
❌ Battery voltage monitoring  
❌ Servo feedback (position verification)  
❌ Redundant receiver support  

---

## Testability & Debugging

### Logging
```
Log Tag          Level    Usage
─────────────────────────────────────
APP              INFO     Startup messages
UART             INFO     RX errors
UART             ERROR    Hardware errors
CRSF             DEBUG    Frame details
CRSF             WARNING  CRC mismatches
PWM              INFO     Servo updates
PWM_HELPERS      WARNING  Out-of-range values
```

### Testing Approach
1. **Unit tests**: CRC8, channel conversion (future)
2. **Integration tests**: Frame parsing with real CRSF data
3. **Hardware tests**: Servo movement with scope
4. **System tests**: End-to-end with transmitter

---

## Future Architecture Enhancements

### Proposed Improvements
1. **Multi-servo support**
   - Add LEDC_CHANNEL_1, LEDC_CHANNEL_2
   - Configure multiple PWM outputs
   - Map RC channels to servos

2. **Telemetry return**
   - Add UART TX transmission
   - Send servo position back
   - Implement CRSF telemetry frames

3. **Advanced control**
   - PID loop for smooth movement
   - Acceleration profiles
   - Position ramping

4. **Robustness**
   - Signal loss detection
   - Failsafe handling
   - Redundant CRSF parsing

5. **Testing**
   - Unit test framework
   - Mock UART for testing
   - CRC verification tests
