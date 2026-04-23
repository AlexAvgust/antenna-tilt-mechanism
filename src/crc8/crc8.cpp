#include <stdint.h>

/**
 * CRC8 calculation using CRSF polynomial (0xD5)
 * 
 * CRSF protocol uses CRC8 with:
 * - Polynomial: x^8 + x^7 + x^6 + x^5 + x^2 + 1 = 0xD5
 * - Initial CRC: 0x00
 * - Input/Output reflected: No
 * - Final XOR: 0x00
 * 
 * Calculation method:
 * For each byte:
 *   1. XOR byte with current CRC
 *   2. For each bit (8 iterations):
 *      - If MSB (bit 7) is set: shift left and XOR with polynomial
 *      - Otherwise: just shift left
 */
uint8_t crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0;
    for (int i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x80)  // Check if MSB is set
            {
                crc = (crc << 1) ^ 0xD5;  // Shift and XOR with polynomial
            }
            else
            {
                crc = (crc << 1);  // Just shift left
            }
        }
    }
    return crc;
}
