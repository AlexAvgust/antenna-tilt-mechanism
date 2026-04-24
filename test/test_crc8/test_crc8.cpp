#include <unity.h>
#include "../../src/crc8/crc8.cpp"

void setUp(void) {}
void tearDown(void) {}

// crc8([], 0) — no bytes processed, initial CRC is 0
void test_zero_length(void) {
    uint8_t data[] = {0xFF};
    TEST_ASSERT_EQUAL_UINT8(0x00, crc8(data, 0));
}

// crc8([0x00]) — XOR with 0 leaves CRC unchanged through all shifts
void test_single_zero_byte(void) {
    uint8_t data[] = {0x00};
    TEST_ASSERT_EQUAL_UINT8(0x00, crc8(data, 1));
}

// Precomputed: crc8([0x16]) = 0xD3
void test_single_byte_type_rc_channels(void) {
    uint8_t data[] = {0x16};
    TEST_ASSERT_EQUAL_UINT8(0xD3, crc8(data, 1));
}

// Precomputed: crc8([0xC8]) = 0x5B  (CRSF SYNC byte)
void test_single_byte_sync(void) {
    uint8_t data[] = {0xC8};
    TEST_ASSERT_EQUAL_UINT8(0x5B, crc8(data, 1));
}

// Precomputed: crc8([0x16, 0x00]) = 0x8A
void test_two_bytes(void) {
    uint8_t data[] = {0x16, 0x00};
    TEST_ASSERT_EQUAL_UINT8(0x8A, crc8(data, 2));
}

// Precomputed: crc8([0x16, 0x00, 0xFF]) = 0x40
void test_three_bytes(void) {
    uint8_t data[] = {0x16, 0x00, 0xFF};
    TEST_ASSERT_EQUAL_UINT8(0x40, crc8(data, 3));
}

void test_deterministic(void) {
    uint8_t data[] = {0x16, 0x00, 0xFF};
    TEST_ASSERT_EQUAL_UINT8(crc8(data, 3), crc8(data, 3));
}

void test_different_inputs_differ(void) {
    uint8_t a[] = {0x16};
    uint8_t b[] = {0x17};
    TEST_ASSERT_NOT_EQUAL(crc8(a, 1), crc8(b, 1));
}

// Appending a zero byte to a non-zero CRC must change the result
void test_length_matters(void) {
    uint8_t data[] = {0x16, 0x00};
    TEST_ASSERT_NOT_EQUAL(crc8(data, 1), crc8(data, 2));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_zero_length);
    RUN_TEST(test_single_zero_byte);
    RUN_TEST(test_single_byte_type_rc_channels);
    RUN_TEST(test_single_byte_sync);
    RUN_TEST(test_two_bytes);
    RUN_TEST(test_three_bytes);
    RUN_TEST(test_deterministic);
    RUN_TEST(test_different_inputs_differ);
    RUN_TEST(test_length_matters);
    return UNITY_END();
}
