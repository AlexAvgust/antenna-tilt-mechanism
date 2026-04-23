#include "../CONSTANTS.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "./crsf.h"
#include "../uart/uart.h"
#include "../crc8/crc8.h"
#include <cstring>

static const char *TAG = "CRSF";

static rc_channels_packed rc_channels_frame;
static uint16_t rc_channels_frame_raw[16];

void unpack_channels(const uint8_t *payload)
{
    memcpy(&rc_channels_frame, payload, sizeof(rc_channels_packed));
    uint32_t bit_offset = 0;
    for (int i = 0; i < 16; i++)
    {
        uint32_t byte_offset = bit_offset / 8;
        uint32_t bit_shift = bit_offset % 8;
        rc_channels_frame_raw[i] = (payload[byte_offset] | payload[byte_offset + 1] << 8 | payload[byte_offset + 2] << 16) >> bit_shift & 0x07FF;
        bit_offset += 11;
    }
}

uint16_t* get_rc_channels_array()
{
    return rc_channels_frame_raw;
}

void parser_feed_byte(uint8_t byte)
{
    static parser_state_t state = WAIT_SYNC;
    static uint8_t crsf_buff_frame[MAX_CRSF_FRAME_LENGTH];
    static uint8_t expected_length;
    static uint8_t frame_pos = 0;

    switch (state)
    {
    case WAIT_SYNC:
        if (byte == SYNC)
        {
            crsf_buff_frame[frame_pos] = byte;
            frame_pos++;
            state = WAIT_LENGTH;
        }
        break;
        
    case WAIT_LENGTH:
        if (byte >= 4 && byte <= 62)
        {
            expected_length = byte;
            crsf_buff_frame[frame_pos] = byte;
            frame_pos++;
            state = READ_PAYLOAD;
        }
        else
        {
            // Invalid length, reset parser
            frame_pos = 0;
            state = WAIT_SYNC;
            ESP_LOGD(TAG, "Invalid CRSF frame length: %u", byte);
        }
        break;
        
    case READ_PAYLOAD:
    {
        uint8_t last_byte_position = expected_length + 1;

        if (frame_pos <= last_byte_position)
        {
            crsf_buff_frame[frame_pos] = byte;

            if (frame_pos == last_byte_position)
            {
                // Frame complete - verify CRC
                uint8_t computed = crc8(&crsf_buff_frame[2], expected_length - 1);
                uint8_t received = crsf_buff_frame[last_byte_position];

                if (computed == received)
                {
                    if (crsf_buff_frame[2] == RC_CHANNELS_PACKED)
                    {
                        unpack_channels(&crsf_buff_frame[3]);
                        ESP_LOGD(TAG, "RC channels frame received and parsed");
                    }
                    else
                    {
                        ESP_LOGD(TAG, "Unknown CRSF frame type: 0x%02X", crsf_buff_frame[2]);
                    }
                }
                else
                {
                    ESP_LOGW(TAG, "CRC mismatch - computed: 0x%02X, received: 0x%02X", computed, received);
                }

                // Reset parser for next frame
                frame_pos = 0;
                state = WAIT_SYNC;
            }
            else
            {
                frame_pos++;
            }
        }
        else
        {
            // Frame exceeded maximum size, reset
            ESP_LOGW(TAG, "CRSF frame exceeded maximum length");
            frame_pos = 0;
            state = WAIT_SYNC;
        }
        break;
    }
    }
}

void process_uart_data()
{
    uart_buffer_t uart_buffer = raw_read_UART();
    for (int i = 0; i < uart_buffer.length_of_uart_buff; i++)
    {
        parser_feed_byte(uart_buffer.data[i]);
    }
}
