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

#include "dds.h"
#include <stdint.h>

constexpr uint32_t BLOCK_SIZE = 512;

typedef enum active_buffer_t
{
    buffer_1,
    buffer_2
} active_buffer_t; // buffer used for calculations, not currently being output
                   // by dac

typedef struct oscillator_t
{
    uint16_t buffer_1[BLOCK_SIZE];
    uint16_t buffer_2[BLOCK_SIZE];

    dds_t dds;

    active_buffer_t active_buffer;

    bool buffers_swapped;
} oscillator_t;

void fill_buffer(oscillator_t *oscillator, uint32_t block_size);
void start_oscillations(float out_freq,
                        float clock_freq,
                        oscillator_t *oscillator,
                        uint32_t block_size);
