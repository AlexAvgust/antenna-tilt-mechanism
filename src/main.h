/**
 * @file main.h
 * @brief Antenna Control System Main Header
 * 
 * This is the entry point for the ESP32-C3 antenna control application.
 * The system reads CRSF RC protocol commands and controls a servo motor.
 */

#pragma once

// Application main function - called by ESP-IDF
// Initializes all peripherals and runs the main control loop
extern "C" void app_main();
