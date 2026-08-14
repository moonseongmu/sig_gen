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

#include "oscillator.h"
#include "dds.h"
#include "interface.h"
#include <stdint.h>

void start_oscillations(float out_freq,
                        float clock_freq,
                        oscillator_t *oscillator,
                        uint32_t block_size)
{
    // start timer and dma dac, interrupt calls when buffer filled/finished
    // outputing
    block_transfer_init();
    // init cosdds with sweep starting frequency + starting phase 0
    oscillator->dds.tuning_word = freq_to_tuning_word(out_freq, clock_freq);
    oscillator->dds.phase_accumulator = 0;

    block_transfer_start();

    while (oscillator->buffers_swapped == true)
    {
        fill_buffer(oscillator, block_size);
        oscillator->buffers_swapped = false;
    }
}

void fill_buffer(oscillator_t *oscillator, uint32_t block_size)
{
    for (uint32_t i = 0; i < block_size; i++)
    {
        dds_calculate(&(oscillator->dds));
        switch (oscillator->active_buffer)
        {
            case buffer_1:
                oscillator->buffer_1[i] = oscillator->dds.value;
            case buffer_2:
                oscillator->buffer_2[i] = oscillator->dds.value;
        }
    }
}
