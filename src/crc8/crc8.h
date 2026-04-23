#pragma once
#include <stdint.h>

/**
 * @brief Calculate CRC8 checksum using CRSF polynomial
 * @param data Pointer to data buffer
 * @param len Number of bytes to checksum
 * @return Calculated CRC8 value
 * 
 * CRSF uses CRC8 with polynomial x^8 + x^7 + x^6 + x^5 + x^2 + 1 (0xD5)
 * Used to verify integrity of CRSF protocol frames
 * 
 * Standard usage in CRSF frame:
 * - Calculate CRC over: [TYPE | PAYLOAD] (exclude SYNC and LENGTH)
 * - Append CRC as last byte
 * - On reception: recalculate CRC and compare with received byte
 */
uint8_t crc8(const uint8_t *data, uint8_t len);