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

void system_init(void);

void led_blink(void *pvParameters);

// setup interrupt/trigger rate timer, setup dma(if available)
void block_transfer_init(void);

// output to dac at timer rate, call block_transfer_complete_ISR
// when transfer of block complete
void block_transfer_start(void);
void block_transfer_complete_ISR(void); // swap buffers
void block_transfer_end(void);          // stop transfer of data

void dac_init(void);             // setup dac
void dac_update(uint16_t value); // send new value to dac