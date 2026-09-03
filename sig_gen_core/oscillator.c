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

// FreeRTOS.h needs to be called first
//clang-format off
#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"
//clang-format on

[[gnu::section(".oscillator_struct")]]
oscillator_t oscillator_struct = {
    .buffer_1 = {},
    .buffer_2 = {},
    .dds = { .phase_accumulator = 0, .tuning_word = 0, .value = 0 },
    .out_freq = 0,
    .clock_freq = 0,
    .buffers_swapped = false,
    .active_buffer = buffer_1
};

TaskHandle_t oscillator_task_handle = NULL;

void oscillator_task(void *params)
{
    oscillator_t *oscillator = (oscillator_t *)params;

    // start timer and dma dac,
    // interrupt calls when buffer filled/finished  outputing
    block_transfer_init(oscillator);
    oscillator->dds.tuning_word = freq_to_tuning_word(oscillator->out_freq,
                                                      oscillator->clock_freq);
    oscillator->dds.phase_accumulator = 0;

    block_transfer_start();

    while (1)
    {
        // block task until block_transfer_complete_isr unblocks
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (oscillator->buffers_swapped == true)
        {
            fill_buffer(oscillator);
            oscillator->buffers_swapped = false;
        }
    }
}

void fill_buffer(oscillator_t *oscillator)
{
    for (uint32_t i = 0; i < BLOCK_SIZE; i++)
    {
        dds_calculate(&(oscillator->dds));
        switch (oscillator->active_buffer)
        {
            case buffer_1:
                oscillator->buffer_2[i] = oscillator->dds.value;
                break;
            case buffer_2:
                oscillator->buffer_1[i] = oscillator->dds.value;
                break;
        }
    }
}

void block_transfer_complete_ISR(void)
{
    // swap buffers & set buffers swapped var
    switch (oscillator_struct.active_buffer)
    {
        case buffer_1:
            oscillator_struct.active_buffer = buffer_2;
            break;
        case buffer_2:
            oscillator_struct.active_buffer = buffer_1;
            break;
    }
    oscillator_struct.buffers_swapped = true;

    // give notification to oscillator task
    BaseType_t higher_priority_task_woken = pdFALSE;
    vTaskNotifyGiveFromISR(oscillator_task_handle,
                           &higher_priority_task_woken);

    portYIELD_FROM_ISR(higher_priority_task_woken);
}