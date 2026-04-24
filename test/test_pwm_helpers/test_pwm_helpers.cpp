#include <unity.h>
// esp_log.h stub is found via build_flags -I test/stubs
#include "../../src/helpers/pwm_helpers/pwm_helpers.cpp"

void setUp(void) {}
void tearDown(void) {}

// ── map_range() ────────────────────────────────────────────────────────────────────

void test_map_min(void) {
    TEST_ASSERT_EQUAL_INT(0, map_range(0, 0, 100, 0, 200));
}

void test_map_max(void) {
    TEST_ASSERT_EQUAL_INT(200, map_range(100, 0, 100, 0, 200));
}

void test_map_midpoint(void) {
    TEST_ASSERT_EQUAL_INT(100, map_range(50, 0, 100, 0, 200));
}

void test_map_clamp_below_min(void) {
    TEST_ASSERT_EQUAL_INT(0, map_range(-10, 0, 100, 0, 200));
}

void test_map_clamp_above_max(void) {
    TEST_ASSERT_EQUAL_INT(200, map_range(110, 0, 100, 0, 200));
}

void test_map_identity(void) {
    TEST_ASSERT_EQUAL_INT(42, map_range(42, 0, 100, 0, 100));
}

// ── convert_channel_value_to_angle() ─────────────────────────────────────────

// 172 (CRSF min) → 0°
void test_convert_min_to_zero(void) {
    TEST_ASSERT_EQUAL_UINT8(0, convert_channel_value_to_angle(172));
}

// 1811 (CRSF max) → 180°
void test_convert_max_to_180(void) {
    TEST_ASSERT_EQUAL_UINT8(180, convert_channel_value_to_angle(1811));
}

// 991 (CRSF center) → 89°  [(991-172)*180/1639 = 89 with integer truncation]
void test_convert_center(void) {
    TEST_ASSERT_EQUAL_UINT8(89, convert_channel_value_to_angle(991));
}

// Values below minimum clamp to 0°
void test_convert_clamp_below_min(void) {
    TEST_ASSERT_EQUAL_UINT8(0, convert_channel_value_to_angle(100));
}

// Values above maximum clamp to 180°
void test_convert_clamp_above_max(void) {
    TEST_ASSERT_EQUAL_UINT8(180, convert_channel_value_to_angle(2000));
}

// Quarter-way point: (172 + (1811-172)/4) ≈ 582  → ~45°
void test_convert_quarter_point(void) {
    uint16_t quarter = 172 + (1811 - 172) / 4;  // 582
    uint8_t angle = convert_channel_value_to_angle(quarter);
    TEST_ASSERT_UINT8_WITHIN(2, 45, angle);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_map_min);
    RUN_TEST(test_map_max);
    RUN_TEST(test_map_midpoint);
    RUN_TEST(test_map_clamp_below_min);
    RUN_TEST(test_map_clamp_above_max);
    RUN_TEST(test_map_identity);
    RUN_TEST(test_convert_min_to_zero);
    RUN_TEST(test_convert_max_to_180);
    RUN_TEST(test_convert_center);
    RUN_TEST(test_convert_clamp_below_min);
    RUN_TEST(test_convert_clamp_above_max);
    RUN_TEST(test_convert_quarter_point);
    return UNITY_END();
}
