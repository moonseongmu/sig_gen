/*
 *Copyright (C) 2026  Moon Seongmu
 *
 *This program is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or any later
 *version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License along with
 *this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <stdint.h>

constexpr uint32_t BLOCK_SIZE = 512;

typedef struct
{
    uint32_t phase_accumulator;
    uint32_t tuning_word;
    uint16_t value;
} dds_t;

typedef enum active_buffer_t
{
    buffer_1,
    buffer_2
} active_buffer_t;

typedef struct oscillator_t
{
    uint16_t buffer_1[BLOCK_SIZE];
    uint16_t buffer_2[BLOCK_SIZE];

    dds_t dds;

    active_buffer_t active_buffer;

    float out_freq;
    float clock_freq;

    bool buffers_swapped;
} oscillator_t;

//------------ port to implement ------------//

void system_init(void);

// led blnk task
void led_blink(void *pvParameters);

// setup interrupt/trigger rate timer, setup dma(if available)
void block_transfer_init(oscillator_t *oscillator);

// output to dac at timer rate
void block_transfer_start(void);

// stop transfer of data
void block_transfer_end(void);

//------------ implemented in app code ------------//

// implemented in oscillator.c
// swaps buffers & gives notification to oscillation_task & yields
// call from isr that fires when block transfer completes
void block_transfer_complete_ISR(void);

oscillator_t *get_oscillator(void);
