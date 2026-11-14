/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Jerzy Kasenberg
 * Copyright (c) 2025 BambooMaster
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <string.h>

#include "bsp/board_api.h"
#include "tusb.h"
#include "usb_descriptors.h"

#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/dma.h"
#include "pico/multicore.h"

#include "i2s.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTOTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 25 ms   : streaming data
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum {
  BLINK_STREAMING = 25,
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

enum {
  VOLUME_CTRL_0_DB = 0,
  VOLUME_CTRL_10_DB = 2560,
  VOLUME_CTRL_20_DB = 5120,
  VOLUME_CTRL_30_DB = 7680,
  VOLUME_CTRL_40_DB = 10240,
  VOLUME_CTRL_50_DB = 12800,
  VOLUME_CTRL_60_DB = 15360,
  VOLUME_CTRL_70_DB = 17920,
  VOLUME_CTRL_80_DB = 20480,
  VOLUME_CTRL_90_DB = 23040,
  VOLUME_CTRL_100_DB = 25600,
  VOLUME_CTRL_SILENCE = 0x8000,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

// Audio controls
// Current states
uint8_t mute[CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_RX + 1];   // +1 for master channel 0
int16_t volume[CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_RX + 1];// +1 for master channel 0
uint32_t current_sample_rate = 44100;
const uint8_t resolutions_per_format[CFG_TUD_AUDIO_FUNC_1_N_FORMATS] = {CFG_TUD_AUDIO_FUNC_1_FORMAT_1_RESOLUTION_RX,
                                                                        CFG_TUD_AUDIO_FUNC_1_FORMAT_2_RESOLUTION_RX};
uint8_t current_resolution;

#define LED_GREEN_PIN 6
#define LED_RED_PIN   7

static inline void update_rate_leds(uint32_t rate) {
  if (rate == 44100) {
    gpio_put(LED_GREEN_PIN, 1);
    gpio_put(LED_RED_PIN, 1);
  } else if (rate == 48000) {
    gpio_put(LED_GREEN_PIN, 1);
    gpio_put(LED_RED_PIN, 0);
  } else if (rate == 88200 || rate == 96000) {
    gpio_put(LED_GREEN_PIN, 0);
    gpio_put(LED_RED_PIN, 1);
  } else {
    gpio_put(LED_GREEN_PIN, 0);
    gpio_put(LED_RED_PIN, 0);
  }
}


// Buffer for speaker data
// uint16_t i2s_dummy_buffer[CFG_TUD_AUDIO_FUNC_1_EP_OUT_SW_BUF_SZ / 2];
// uint8_t spk_buf[CFG_TUD_AUDIO_FUNC_1_EP_OUT_SW_BUF_SZ];
// volatile int spk_data_size;

// void led_blinking_task(void);
// void audio_task(void);
void core1_main(void);

#define TUD_TASK_INTERVAL_US      250
#define DEQUEUE_MAX_LEN   (CFG_TUD_AUDIO_FUNC_1_MAX_SAMPLE_RATE_FS / 2000 + 1)

__isr bool __time_critical_func(tud_timer_callback)(__unused struct repeating_timer *t) {
  tud_task();
  return true;
}

/*------------- MAIN -------------*/
int main(void) {
  i2s_set_config(pio0, CLOCK_MODE_LOW_JITTER, MODE_I2S);
  board_init();

  // i2s初期化
  i2s_set_pin(18, 20, 22);
  i2s_init(current_sample_rate);
  i2s_volume_change(0, 0);

  // i2s送信開始
  multicore_launch_core1(core1_main);

  // init device stack on configured roothub port
  tusb_rhport_init_t dev_init = {
      .role = TUSB_ROLE_DEVICE,
      .speed = TUSB_SPEED_AUTO};
  tusb_init(BOARD_TUD_RHPORT, &dev_init);

  board_init_after_tusb();

  TU_LOG1("Speaker running\r\n");

  // tud_task()とaudio_task()を実行するタイマ
  static struct repeating_timer tud_timer;
  alarm_pool_t* low_prio_pool = alarm_pool_create_with_unused_hardware_alarm(1);
  uint irq_num = hardware_alarm_get_irq_num(alarm_pool_hardware_alarm_num(low_prio_pool));
  irq_set_priority(irq_num, PICO_LOWEST_IRQ_PRIORITY);
  alarm_pool_add_repeating_timer_us(low_prio_pool, -TUD_TASK_INTERVAL_US, tud_timer_callback, NULL, &tud_timer);

  while (1) __wfi;
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
  blink_interval_ms = tud_mounted() ? BLINK_MOUNTED : BLINK_NOT_MOUNTED;
}

//--------------------------------------------------------------------+
// Application Callback API Implementations
//--------------------------------------------------------------------+

//--------------------------------------------------------------------+
// UAC1 Helper Functions
//--------------------------------------------------------------------+

static bool audio10_set_req_ep(tusb_control_request_t const *p_request, uint8_t *pBuff) {
  uint8_t ctrlSel = TU_U16_HIGH(p_request->wValue);

  switch (ctrlSel) {
    case AUDIO10_EP_CTRL_SAMPLING_FREQ:
      if (p_request->bRequest == AUDIO10_CS_REQ_SET_CUR) {
        // Request uses 3 bytes
        TU_VERIFY(p_request->wLength == 3);

        current_sample_rate = tu_unaligned_read32(pBuff) & 0x00FFFFFF;
        i2s_change_clock(current_sample_rate);
        update_rate_leds(current_sample_rate);

        TU_LOG2("EP set current freq: %" PRIu32 "\r\n", current_sample_rate);

        return true;
      }
      break;

    // Unknown/Unsupported control
    default:
      TU_BREAKPOINT();
      return false;
  }

  return false;
}

static bool audio10_get_req_ep(uint8_t rhport, tusb_control_request_t const *p_request) {
  uint8_t ctrlSel = TU_U16_HIGH(p_request->wValue);

  switch (ctrlSel) {
    case AUDIO10_EP_CTRL_SAMPLING_FREQ:
      if (p_request->bRequest == AUDIO10_CS_REQ_GET_CUR) {
        TU_LOG2("EP get current freq\r\n");

        uint8_t freq[3];
        freq[0] = (uint8_t) (current_sample_rate & 0xFF);
        freq[1] = (uint8_t) ((current_sample_rate >> 8) & 0xFF);
        freq[2] = (uint8_t) ((current_sample_rate >> 16) & 0xFF);
        return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, freq, sizeof(freq));
      }
      break;

    // Unknown/Unsupported control
    default:
      TU_BREAKPOINT();
      return false;
  }

  return false;
}

static bool audio10_set_req_entity(tusb_control_request_t const *p_request, uint8_t *pBuff) {
  uint8_t channelNum = TU_U16_LOW(p_request->wValue);
  uint8_t ctrlSel = TU_U16_HIGH(p_request->wValue);
  uint8_t entityID = TU_U16_HIGH(p_request->wIndex);

  // If request is for our feature unit
  if (entityID == UAC1_ENTITY_FEATURE_UNIT) {
    switch (ctrlSel) {
      case AUDIO10_FU_CTRL_MUTE:
        switch (p_request->bRequest) {
          case AUDIO10_CS_REQ_SET_CUR:
            // Only 1st form is supported
            TU_VERIFY(p_request->wLength == 1);

            mute[channelNum] = pBuff[0];

            TU_LOG2("    Set Mute: %d of channel: %u\r\n", mute[channelNum], channelNum);
            return true;

          default:
            return false; // not supported
        }

      case AUDIO10_FU_CTRL_VOLUME:
        switch (p_request->bRequest) {
          case AUDIO10_CS_REQ_SET_CUR:
            // Only 1st form is supported
            TU_VERIFY(p_request->wLength == 2);

            volume[channelNum] = (int16_t)tu_unaligned_read16(pBuff) / 256;
            i2s_volume_change((int16_t)tu_unaligned_read16(pBuff), channelNum);

            TU_LOG2("    Set Volume: %d dB of channel: %u\r\n", volume[channelNum], channelNum);
            return true;

          default:
            return false; // not supported
        }

        // Unknown/Unsupported control
      default:
        TU_BREAKPOINT();
        return false;
    }
  }

  return false;
}

static bool audio10_get_req_entity(uint8_t rhport, tusb_control_request_t const *p_request) {
  uint8_t channelNum = TU_U16_LOW(p_request->wValue);
  uint8_t ctrlSel = TU_U16_HIGH(p_request->wValue);
  uint8_t entityID = TU_U16_HIGH(p_request->wIndex);

  // If request is for our feature unit
  if (entityID == UAC1_ENTITY_FEATURE_UNIT) {
    switch (ctrlSel) {
      case AUDIO10_FU_CTRL_MUTE:
        // Audio control mute cur parameter block consists of only one byte - we thus can send it right away
        // There does not exist a range parameter block for mute
        TU_LOG2("    Get Mute of channel: %u\r\n", channelNum);
        return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, &mute[channelNum], 1);

      case AUDIO10_FU_CTRL_VOLUME:
        switch (p_request->bRequest) {
          case AUDIO10_CS_REQ_GET_CUR:
            TU_LOG2("    Get Volume of channel: %u\r\n", channelNum);
            {
              int16_t vol = (int16_t) volume[channelNum];
              vol = vol * 256; // convert to 1/256 dB units
              return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, &vol, sizeof(vol));
            }

          case AUDIO10_CS_REQ_GET_MIN:
            TU_LOG2("    Get Volume min of channel: %u\r\n", channelNum);
            {
              int16_t min = -90; // -90 dB
              min = min * 256; // convert to 1/256 dB units
              return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, &min, sizeof(min));
            }

          case AUDIO10_CS_REQ_GET_MAX:
            TU_LOG2("    Get Volume max of channel: %u\r\n", channelNum);
            {
              int16_t max = 0; // 0 dB
              max = max * 256; // convert to 1/256 dB units
              return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, &max, sizeof(max));
            }

          case AUDIO10_CS_REQ_GET_RES:
            TU_LOG2("    Get Volume res of channel: %u\r\n", channelNum);
            {
              int16_t res = 256; // 1 dB
              return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, &res, sizeof(res));
            }
            // Unknown/Unsupported control
          default:
            TU_BREAKPOINT();
            return false;
        }
        break;

        // Unknown/Unsupported control
      default:
        TU_BREAKPOINT();
        return false;
    }
  }

  return false;
}

//--------------------------------------------------------------------+
// UAC2 Helper Functions
//--------------------------------------------------------------------+

#if TUD_OPT_HIGH_SPEED
// List of supported sample rates for UAC2
const uint32_t sample_rates[] = {44100, 48000, 88200, 96000};

#define N_SAMPLE_RATES TU_ARRAY_SIZE(sample_rates)

static bool audio20_clock_get_request(uint8_t rhport, tusb_control_request_t const *p_request) {
  uint8_t const ctrl_sel = TU_U16_HIGH(p_request->wValue);

  if (ctrl_sel == AUDIO20_CS_CTRL_SAM_FREQ) {
    if (p_request->bRequest == AUDIO20_CS_REQ_CUR) {
      TU_LOG1("Clock get current freq %" PRIu32 "\r\n", current_sample_rate);

      audio20_control_cur_4_t curf = {(int32_t) tu_htole32(current_sample_rate)};
      return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, &curf, sizeof(curf));
    } else if (p_request->bRequest == AUDIO20_CS_REQ_RANGE) {
      audio20_control_range_4_n_t(N_SAMPLE_RATES) rangef =
          {
              .wNumSubRanges = tu_htole16(N_SAMPLE_RATES)};
      TU_LOG1("Clock get %d freq ranges\r\n", N_SAMPLE_RATES);
      for (uint8_t i = 0; i < N_SAMPLE_RATES; i++) {
        rangef.subrange[i].bMin = (int32_t) sample_rates[i];
        rangef.subrange[i].bMax = (int32_t) sample_rates[i];
        rangef.subrange[i].bRes = 0;
        TU_LOG1("Range %d (%d, %d, %d)\r\n", i, (int) rangef.subrange[i].bMin, (int) rangef.subrange[i].bMax, (int) rangef.subrange[i].bRes);
      }

      return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, &rangef, sizeof(rangef));
    }
  } else if (ctrl_sel == AUDIO20_CS_CTRL_CLK_VALID &&
             p_request->bRequest == AUDIO20_CS_REQ_CUR) {
    audio20_control_cur_1_t cur_valid = {.bCur = 1};
    TU_LOG1("Clock get is valid %u\r\n", cur_valid.bCur);
    return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, &cur_valid, sizeof(cur_valid));
  }
  TU_LOG1("Clock get request not supported, selector = %u, request = %u\r\n",
          ctrl_sel, p_request->bRequest);
  return false;
}

static bool audio20_clock_set_request(tusb_control_request_t const *p_request, uint8_t const *buf) {
  uint8_t const ctrl_sel = TU_U16_HIGH(p_request->wValue);

  TU_VERIFY(p_request->bRequest == AUDIO20_CS_REQ_CUR);

  if (ctrl_sel == AUDIO20_CS_CTRL_SAM_FREQ) {
    TU_VERIFY(p_request->wLength == sizeof(audio20_control_cur_4_t));

    current_sample_rate = (uint32_t) ((audio20_control_cur_4_t const *) buf)->bCur;
    update_rate_leds(current_sample_rate);

    TU_LOG1("Clock set current freq: %" PRIu32 "\r\n", current_sample_rate);

    return true;
  } else {
    TU_LOG1("Clock set request not supported, selector = %u, request = %u\r\n",
            ctrl_sel, p_request->bRequest);
    return false;
  }
}

static bool audio20_feature_unit_get_request(uint8_t rhport, tusb_control_request_t const *p_request) {
  uint8_t const ctrl_sel    = TU_U16_HIGH(p_request->wValue);
  uint8_t const channel_num = TU_U16_LOW(p_request->wValue);

  if (ctrl_sel == AUDIO20_FU_CTRL_MUTE && p_request->bRequest == AUDIO20_CS_REQ_CUR) {
    audio20_control_cur_1_t mute1 = {.bCur = mute[channel_num]};
    TU_LOG1("Get channel %u mute %d\r\n", channel_num, mute1.bCur);
    return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, &mute1, sizeof(mute1));
  } else if (ctrl_sel == AUDIO20_FU_CTRL_VOLUME) {
    if (p_request->bRequest == AUDIO20_CS_REQ_RANGE) {
      audio20_control_range_2_n_t(1) range_vol = {
          .wNumSubRanges = tu_htole16(1),
          .subrange[0] = {.bMin = tu_htole16(-VOLUME_CTRL_50_DB), tu_htole16(VOLUME_CTRL_0_DB), tu_htole16(256)}};
      TU_LOG1("Get channel %u volume range (%d, %d, %u) dB\r\n", channel_num,
              range_vol.subrange[0].bMin / 256, range_vol.subrange[0].bMax / 256, range_vol.subrange[0].bRes / 256);
      return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, &range_vol, sizeof(range_vol));
    } else if (p_request->bRequest == AUDIO20_CS_REQ_CUR) {
      audio20_control_cur_2_t cur_vol = {.bCur = tu_htole16(volume[channel_num])};
      TU_LOG1("Get channel %u volume %d dB\r\n", channel_num, cur_vol.bCur / 256);
      return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, &cur_vol, sizeof(cur_vol));
    }
  }
  TU_LOG1("Feature unit get request not supported, selector = %u, request = %u\r\n",
          ctrl_sel, p_request->bRequest);

  return false;
}

static bool audio20_feature_unit_set_request(tusb_control_request_t const *p_request, uint8_t const *buf) {
  uint8_t const ctrl_sel    = TU_U16_HIGH(p_request->wValue);
  uint8_t const channel_num = TU_U16_LOW(p_request->wValue);

  TU_VERIFY(p_request->bRequest == AUDIO20_CS_REQ_CUR);

  if (ctrl_sel == AUDIO20_FU_CTRL_MUTE) {
    TU_VERIFY(p_request->wLength == sizeof(audio20_control_cur_1_t));

    mute[channel_num] = ((audio20_control_cur_1_t const *) buf)->bCur;

    TU_LOG1("Set channel %d Mute: %d\r\n", channel_num, mute[channel_num]);

    return true;
  } else if (ctrl_sel == AUDIO20_FU_CTRL_VOLUME) {
    TU_VERIFY(p_request->wLength == sizeof(audio20_control_cur_2_t));

    volume[channel_num] = ((audio20_control_cur_2_t const *) buf)->bCur;

    TU_LOG1("Set channel %d volume: %d dB\r\n", channel_num, volume[channel_num] / 256);

    return true;
  } else {
    TU_LOG1("Feature unit set request not supported, selector = %u, request = %u\r\n",
            ctrl_sel, p_request->bRequest);
    return false;
  }
}

static bool audio20_get_req_entity(uint8_t rhport, tusb_control_request_t const *p_request) {
  uint8_t const entity_id = TU_U16_HIGH(p_request->wIndex);

  if (entity_id == UAC2_ENTITY_CLOCK)
    return audio20_clock_get_request(rhport, p_request);
  if (entity_id == UAC2_ENTITY_FEATURE_UNIT)
    return audio20_feature_unit_get_request(rhport, p_request);
  else {
    TU_LOG1("Get request not handled, entity = %d, selector = %d, request = %d\r\n",
            entity_id, TU_U16_HIGH(p_request->wValue), p_request->bRequest);
  }
  return false;
}

static bool audio20_set_req_entity(tusb_control_request_t const *p_request, uint8_t *buf) {
  uint8_t const entity_id = TU_U16_HIGH(p_request->wIndex);

  if (entity_id == UAC2_ENTITY_FEATURE_UNIT)
    return audio20_feature_unit_set_request(p_request, buf);
  if (entity_id == UAC2_ENTITY_CLOCK)
    return audio20_clock_set_request(p_request, buf);
  TU_LOG1("Set request not handled, entity = %d, selector = %d, request = %d\r\n",
          entity_id, TU_U16_HIGH(p_request->wValue), p_request->bRequest);

  return false;
}

#endif // TUD_OPT_HIGH_SPEED

//--------------------------------------------------------------------+
// Main Callback Functions
//--------------------------------------------------------------------+

bool tud_audio_set_itf_cb(uint8_t rhport, tusb_control_request_t const *p_request) {
  (void) rhport;
  uint8_t const itf = tu_u16_low(p_request->wIndex);
  uint8_t const alt = tu_u16_low(p_request->wValue);

  TU_LOG2("Set interface %d alt %d\r\n", itf, alt);
  if (ITF_NUM_AUDIO_STREAMING == itf && alt != 0)
    blink_interval_ms = BLINK_STREAMING;

  if (alt != 0) {
    current_resolution = resolutions_per_format[alt - 1];
  }

  return true;
}

// Invoked when audio class specific set request received for an EP
bool tud_audio_set_req_ep_cb(uint8_t rhport, tusb_control_request_t const *p_request, uint8_t *pBuff) {
  (void) rhport;
  (void) pBuff;

  if (tud_audio_version() == 1) {
    return audio10_set_req_ep(p_request, pBuff);
  } else if (tud_audio_version() == 2) {
    // We do not support any requests here
  }

  return false;// Yet not implemented
}

// Invoked when audio class specific get request received for an EP
bool tud_audio_get_req_ep_cb(uint8_t rhport, tusb_control_request_t const *p_request) {
  (void) rhport;

  if (tud_audio_version() == 1) {
    return audio10_get_req_ep(rhport, p_request);
  } else if (tud_audio_version() == 2) {
    // We do not support any requests here
  }

  return false;// Yet not implemented
}

// Invoked when audio class specific set request received for an entity
bool tud_audio_set_req_entity_cb(uint8_t rhport, tusb_control_request_t const *p_request, uint8_t *buf) {
  (void) rhport;

  if (tud_audio_version() == 1) {
    return audio10_set_req_entity(p_request, buf);
#if TUD_OPT_HIGH_SPEED
  } else if (tud_audio_version() == 2) {
    return audio20_set_req_entity(p_request, buf);
#endif
  }

  return false;
}

// Invoked when audio class specific get request received for an entity
bool tud_audio_get_req_entity_cb(uint8_t rhport, tusb_control_request_t const *p_request) {
  (void) rhport;

  if (tud_audio_version() == 1) {
    return audio10_get_req_entity(rhport, p_request);
#if TUD_OPT_HIGH_SPEED
  } else if (tud_audio_version() == 2) {
    return audio20_get_req_entity(rhport, p_request);
#endif
  }

  return false;
}

bool tud_audio_set_itf_close_ep_cb(uint8_t rhport, tusb_control_request_t const *p_request) {
  (void) rhport;

  uint8_t const itf = tu_u16_low(p_request->wIndex);
  uint8_t const alt = tu_u16_low(p_request->wValue);

  if (ITF_NUM_AUDIO_STREAMING == itf && alt == 0) {
    blink_interval_ms = BLINK_MOUNTED;
  }

  return true;
}

bool tud_audio_rx_done_isr(uint8_t rhport, uint16_t n_bytes_received, uint8_t func_id, uint8_t ep_out, uint8_t cur_alt_setting) {
  (void) rhport;
  (void) n_bytes_received;
  (void) func_id;
  (void) ep_out;
  (void) cur_alt_setting;

  static uint8_t spk_buf[CFG_TUD_AUDIO_FUNC_1_EP_OUT_SW_BUF_SZ];
  static int32_t uac_buf_l[I2S_QUEUE_MAX];
  static int32_t uac_buf_r[I2S_QUEUE_MAX];

  // i2sキューに積む
  int spk_data_size = tud_audio_read(spk_buf, n_bytes_received);
  int rx_length = i2s_unpack_uacdata(spk_buf, spk_data_size, current_resolution, uac_buf_l, uac_buf_r);
  i2s_enqueue(uac_buf_l, uac_buf_r, rx_length);

  return true;
}

void tud_audio_feedback_params_cb(uint8_t func_id, uint8_t alt_itf, audio_feedback_params_t *feedback_param) {
  (void) func_id;
  (void) alt_itf;

  feedback_param->method = AUDIO_FEEDBACK_METHOD_FREQUENCY_FIXED;
}

void tud_audio_feedback_interval_isr(uint8_t func_id, uint32_t frame_number, uint8_t interval_shift) {
  (void) func_id;
  (void) frame_number;
  (void) interval_shift;

  int length =  i2s_get_queue_length();
  int trget_level = i2s_get_sample_rate_hz() * 3 / 2000;
  uint feedback = (uint32_t)(((uint64_t)current_sample_rate << 16u) / 1000u);

  // フィードバックの最大値、最小値
  uint feedback_max = (current_sample_rate / 1000 + 1) << 16;
  uint feedback_min = ((current_sample_rate - 1) / 1000) << 16;

  // フィードバック値計算
  if (trget_level > length){
    feedback = feedback + (uint32_t)((uint64_t)(trget_level - length) * (feedback_max - feedback) / trget_level);
  }
  else{
    feedback = feedback - (uint32_t)((uint64_t)(length - trget_level) * (feedback - feedback_min) / trget_level);
  }

  // フィードバック値を規定の値に収める
  if (feedback > feedback_max) feedback = feedback_max;
  if (feedback < feedback_min) feedback = feedback_min;

  tud_audio_fb_set(feedback);
}

//--------------------------------------------------------------------+
// AUDIO Task
//--------------------------------------------------------------------+

// This task simulates an audio transmit callback, one frame is sent every 1ms.
// In a real application, this would be replaced with actual I2S transmit callback.

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
#if 0
void led_blinking_task(void) {
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // Blink every interval ms
  if (tusb_time_millis_api() - start_ms < blink_interval_ms) return;
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state;
}
#endif

void core1_main(void){
  int dma_sample;
  bool mute = false;
  int buf_length;
  static int32_t dma_buf_a[2][DEQUEUE_MAX_LEN * 2], dma_buf_b[2][DEQUEUE_MAX_LEN * 2];
  uint8_t dma_use = 0;
  int dequeue_len;

  int sample;
  int32_t i2s_buf_l[DEQUEUE_MAX_LEN], i2s_buf_r[DEQUEUE_MAX_LEN];

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  gpio_init(LED_GREEN_PIN);
  gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
  gpio_init(LED_RED_PIN);
  gpio_set_dir(LED_RED_PIN, GPIO_OUT);
  update_rate_leds(current_sample_rate);
 

  while (1){
    buf_length = i2s_get_queue_length();
    // 0.5ms分ずつi2sに送る
    dequeue_len = i2s_get_sample_rate_hz() / 2000;
    if (dequeue_len > DEQUEUE_MAX_LEN) {
      dequeue_len = DEQUEUE_MAX_LEN;
    }

    // printf("%3d\n", buf_length);

    // i2sキューが一定以上溜まったらミュート解除
    if (buf_length == 0 && mute == false){
      mute = true;
      gpio_put(PICO_DEFAULT_LED_PIN, 0);
    }
    else if (buf_length >= (dequeue_len * 3) && mute == true){
      mute = false;
      gpio_put(PICO_DEFAULT_LED_PIN, 1);
    }

    if (mute == false){
      // i2sキューから取り出す
      sample = i2s_dequeue(i2s_buf_l, i2s_buf_r, dequeue_len);

      // キューから取り出したデータ量が要求より少ない場合は、0埋めしてミュート状態へ
      if (sample < dequeue_len){
        for (int i = sample; i < dequeue_len; i++){
          i2s_buf_l[i] = 0;
          i2s_buf_r[i] = 0;
        }
        sample = dequeue_len;
        mute = true;
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
      }
    }
    else{
      // ミュート状態の時は0を送信
      for (int i = 0; i < dequeue_len; i++){
        i2s_buf_l[i] = 0;
        i2s_buf_r[i] = 0;
      }
      sample = dequeue_len;
    }

    // 音量処理
    i2s_volume(i2s_buf_l, i2s_buf_r, sample);

    // pio送信形式に変換
    dma_sample = i2s_format_piodata(i2s_buf_l, i2s_buf_r, sample, dma_buf_a[dma_use], dma_buf_b[dma_use]);

    // dmaが終わるまで待機
    i2s_dma_transfer_blocking(dma_buf_a[dma_use], dma_buf_b[dma_use], dma_sample);
    dma_use ^= 1;
  }
}
