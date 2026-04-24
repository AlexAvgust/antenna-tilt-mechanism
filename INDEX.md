# Project Index & Navigation Guide

## 📑 Complete File Structure with Descriptions

```
antenna_control/
├── README.md ........................ START HERE - Project overview
├── QUICK_REFERENCE.md .............. Cheat sheet for common tasks
├── REVIEW.md ........................ Review summary & improvements
├── ARCHITECTURE.md ................. Detailed system design
├── DEVELOPMENT.md .................. Setup & development guide
├── VISUAL_SUMMARY.md ............... Diagrams, charts, and flows
├── CONSTANTS.h ..................... Global configuration
├── CMakeLists.txt .................. Build configuration
├── platformio.ini .................. PlatformIO settings
│
├── src/
│   ├── main.cpp .................... Application entry point
│   ├── main.h ...................... Main header
│   │
│   ├── uart/
│   │   ├── uart.h .................. UART driver API
│   │   └── uart.cpp ................ UART implementation
│   │
│   ├── crsf/
│   │   ├── crsf.h .................. CRSF protocol structures
│   │   └── crsf.cpp ................ CRSF parser implementation
│   │
│   ├── pwm/
│   │   ├── pwm.h ................... PWM driver API
│   │   └── pwm.cpp ................. PWM servo control
│   │
│   ├── crc8/
│   │   ├── crc8.h .................. CRC8 checksum API
│   │   └── crc8.cpp ................ CRC8 implementation
│   │
│   ├── helpers/
│   │   └── pwm_helpers/
│   │       ├── pwm_helpers.h ....... Channel conversion API
│   │       └── pwm_helpers.cpp ..... Conversion implementation
│   │
│   └── CMakeLists.txt .............. Source build configuration
│
├── include/ ......................... (User includes, currently empty)
├── lib/ ............................. (External libraries, currently empty)
├── test/ ............................ (Test directory, currently empty)
│
└── Other Files
    ├── sdkconfig.esp32-c3-devkitm-1  ESP-IDF config
    ├── sdkconfig.esp32c3_supermini ... ESP-IDF config variant
    └── (generated build artifacts)
```

---

## 🗺️ Navigation Guide by Purpose

### 🚀 First Time? Start Here
1. **README.md** - Understand what the project does
2. **QUICK_REFERENCE.md** - Get it built and running
3. **VISUAL_SUMMARY.md** - See how data flows through the system
4. **ARCHITECTURE.md** - Understand the design

### 🔧 Want to Build/Flash?
1. **QUICK_REFERENCE.md** - Copy-paste build commands
2. **DEVELOPMENT.md** - Detailed build instructions
3. **platformio.ini** - Board and framework settings

### 🐛 Debugging an Issue?
1. **QUICK_REFERENCE.md** - Check "Common Issues" section
2. **DEVELOPMENT.md** - "Troubleshooting" and "Testing" sections
3. **ARCHITECTURE.md** - "Error Handling Strategy"
4. **Source code** - Check the relevant module

### 📚 Understanding the Code?
1. **ARCHITECTURE.md** - "Module Architecture" section
2. **VISUAL_SUMMARY.md** - Data flow diagrams
3. **Source files** - Read .h files first, then .cpp
4. **CONSTANTS.h** - Configuration values explained

### 🎯 Want to Extend/Modify?
1. **ARCHITECTURE.md** - "Future Enhancements" section
2. **DEVELOPMENT.md** - "Adding New Features"
3. **Source code** - Understand current implementation
4. **REVIEW.md** - Known limitations and TODOs

### 📖 Want to Master the System?
1. **README.md** - Project overview
2. **VISUAL_SUMMARY.md** - Signal flows and timing
3. **ARCHITECTURE.md** - Complete system design
4. **DEVELOPMENT.md** - Development practices
5. **Source code** - Implementation details
6. **QUICK_REFERENCE.md** - Keep handy for reference

---

## 🔍 Documentation Map by Topic

### CRSF Protocol
- **README.md** → "CRSF Protocol Details"
- **VISUAL_SUMMARY.md** → "CRSF RC Channels Frame"
- **ARCHITECTURE.md** → "CRSF Parser Module"
- **src/crsf/crsf.h** → Struct definitions
- **src/crsf/crsf.cpp** → Parser implementation

### Servo Control & PWM
- **README.md** → "PWM Configuration"
- **VISUAL_SUMMARY.md** → "Channel Value Range"
- **ARCHITECTURE.md** → "PWM Driver Module"
- **src/pwm/pwm.h** → API & calibration
- **src/pwm/pwm.cpp** → Implementation

### Hardware Pinout
- **QUICK_REFERENCE.md** → "Hardware Pinout"
- **CONSTANTS.h** → All pin definitions
- **ARCHITECTURE.md** → "Pin Assignment Summary"

### Data Flow
- **VISUAL_SUMMARY.md** → "System Block Diagram" & "Complete Signal Path"
- **ARCHITECTURE.md** → "Data Flow Diagram"
- **README.md** → "Architecture" section

### Error Handling
- **DEVELOPMENT.md** → "Troubleshooting"
- **ARCHITECTURE.md** → "Error Handling Strategy"
- **QUICK_REFERENCE.md** → "Common Issues & Fixes"

### Performance & Timing
- **VISUAL_SUMMARY.md** → "Signal Flow with Timing" & "Performance Metrics"
- **ARCHITECTURE.md** → "Performance Characteristics"
- **QUICK_REFERENCE.md** → "Performance Monitoring"

### Building & Flashing
- **QUICK_REFERENCE.md** → "Command Cheat Sheet"
- **DEVELOPMENT.md** → "Building" & "Uploading"
- **platformio.ini** → Board configuration

---

## 📊 Information Density by Document

### Quick Start (5 minutes)
1. QUICK_REFERENCE.md - "Quick Start"
2. platformio.ini + README first paragraph

### User Manual (20 minutes)
1. README.md (complete)
2. QUICK_REFERENCE.md (complete)
3. VISUAL_SUMMARY.md (skim diagrams)

### Developer Guide (1 hour)
1. README.md
2. ARCHITECTURE.md
3. DEVELOPMENT.md
4. Source file headers

### Deep Mastery (2-3 hours)
1. All documentation files
2. Complete source code review
3. Hardware schematic review
4. Testing with actual hardware

---

## 🎯 Quick Links by Activity

### Activity: Upload Firmware
```
QUICK_REFERENCE.md → Command Cheat Sheet
  ↓
platformio run --target upload
```

### Activity: Monitor Serial Output
```
QUICK_REFERENCE.md → Quick Start
  ↓
platformio monitor
```

### Activity: Understand Main Loop
```
README.md → Architecture
  ↓ VISUAL_SUMMARY.md → System Block Diagram
  ↓ ARCHITECTURE.md → "Main Loop Timing"
  ↓ src/main.cpp
```

### Activity: Debug CRSF Parser
```
QUICK_REFERENCE.md → Common Issues ("CRSF CRC errors")
  ↓
ARCHITECTURE.md → "CRSF Parser Module"
  ↓
DEVELOPMENT.md → "Troubleshooting Build Issues"
  ↓
src/crsf/crsf.cpp
```

### Activity: Add New Feature
```
ARCHITECTURE.md → "Future Enhancements"
  ↓
DEVELOPMENT.md → "Adding New Features"
  ↓
ARCHITECTURE.md → "Module Architecture"
  ↓
Source code
```

---

## 🔧 Configuration Guide

### To Change UART Baud Rate
```
CONSTANTS.h → UART_BAUD_RATE
Also update: DEVELOPMENT.md → "Troubleshooting" section
```

### To Change PWM Frequency
```
CONSTANTS.h → LEDC_FREQUENCY
Update servo documentation in: pwm.h, README.md
```

### To Change Servo Calibration
```
src/pwm/pwm.cpp → angle_to_duty_cycle() formula
Update documentation in: pwm.h, README.md, VISUAL_SUMMARY.md
```

### To Change RC Channel
```
CONSTANTS.h → CHANNEL_TO_CONTROL_ANTENA_ANGLE
Update in: main.cpp, README.md
```

---

## 📋 Checklist for Common Tasks

### Compiling the Project
- [ ] Read QUICK_REFERENCE.md → "Quick Start"
- [ ] Run: `platformio run`
- [ ] Check for errors in build output
- [ ] If issues, see DEVELOPMENT.md → "Troubleshooting Build Issues"

### Uploading to Board
- [ ] Connect USB-C cable
- [ ] Run: `platformio run --target upload`
- [ ] Wait for completion message
- [ ] Run: `platformio monitor` to verify

### Debugging a Problem
- [ ] Check QUICK_REFERENCE.md → "Common Issues"
- [ ] Check DEVELOPMENT.md → "Troubleshooting"
- [ ] Check ARCHITECTURE.md → "Error Handling"
- [ ] Review serial logs: `platformio monitor`
- [ ] Add ESP_LOGD() statements if needed

### Adding a New Module
- [ ] Read DEVELOPMENT.md → "Adding New Features" → "Adding a New Module"
- [ ] Create directory: `src/newmodule/`
- [ ] Create: `newmodule.h` and `newmodule.cpp`
- [ ] Add to CMakeLists.txt
- [ ] Update main.cpp if needed
- [ ] Add documentation

### Code Review
- [ ] Check ARCHITECTURE.md → "Module Architecture"
- [ ] Understand data flow: VISUAL_SUMMARY.md
- [ ] Review function documentation in .h files
- [ ] Check error handling: ARCHITECTURE.md → "Error Handling Strategy"

---

## 📚 Document Audience Guide

### For Project Managers
→ README.md (Status, Features, Timeline)

### For Hardware Engineers
→ QUICK_REFERENCE.md (Pinout, Timing)
→ VISUAL_SUMMARY.md (Block diagram, Signals)

### For Software Engineers
→ ARCHITECTURE.md (Complete system design)
→ DEVELOPMENT.md (Setup and practices)
→ Source code with inline documentation

### For QA/Testers
→ DEVELOPMENT.md (Testing section)
→ QUICK_REFERENCE.md (Pre-flight checklist)
→ README.md (Troubleshooting)

### For Beginners
→ README.md (Start here)
→ QUICK_REFERENCE.md (Quick commands)
→ DEVELOPMENT.md (Gentle guidance)

### For Experts
→ ARCHITECTURE.md (Design decisions)
→ REVIEW.md (Implementation assessment)
→ Source code (Implementation details)

---

## 🎓 Recommended Reading Order

### Path 1: "I want to use this" (User)
1. README.md (5 min)
2. QUICK_REFERENCE.md (10 min)
3. Try building it
4. Check DEVELOPMENT.md if stuck

### Path 2: "I want to understand this" (Developer)
1. README.md (10 min)
2. VISUAL_SUMMARY.md (15 min)
3. ARCHITECTURE.md (30 min)
4. Browse source code (30 min)
5. DEVELOPMENT.md for specific tasks

### Path 3: "I want to improve this" (Contributor)
1. REVIEW.md (10 min) - See what was improved
2. README.md (10 min)
3. ARCHITECTURE.md (30 min)
4. DEVELOPMENT.md (20 min)
5. Source code (60 min)
6. Run tests and try modifications

### Path 4: "I'm lost" (Help!)
1. QUICK_REFERENCE.md → "Troubleshooting Flowchart"
2. DEVELOPMENT.md → "Troubleshooting section"
3. README.md → "Troubleshooting"
4. Check error messages in serial monitor

---

## 🔗 Cross-References

The documentation is heavily cross-referenced:
- **README.md** links to ARCHITECTURE.md for details
- **QUICK_REFERENCE.md** links to DEVELOPMENT.md for comprehensive guides
- **ARCHITECTURE.md** references specific code files
- **DEVELOPMENT.md** includes ARCHITECTURE.md examples
- **VISUAL_SUMMARY.md** complements ARCHITECTURE.md diagrams

To follow a topic:
1. Start in README.md
2. Jump to VISUAL_SUMMARY.md for diagrams
3. Go to ARCHITECTURE.md for deep details
4. Review source code and DEVELOPMENT.md as needed

---

## ✨ Tips for Navigation

1. **Use Ctrl+F** in documents to search
2. **Check the table of contents** at the start of long documents
3. **Follow links** between documents
4. **Keep QUICK_REFERENCE.md** bookmarked
5. **Use the Troubleshooting Flowchart** when stuck
6. **Check code comments** for inline explanations
7. **Look in CONSTANTS.h** for all configuration values

---

**Last Updated**: April 23, 2026  
**Document Version**: 1.0  
**Status**: ✅ Complete Navigation Guide
