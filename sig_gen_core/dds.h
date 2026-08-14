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

typedef struct
{
    uint32_t phase_accumulator;
    uint32_t tuning_word;
    uint16_t value;
} dds_t;

void dds_init(dds_t *dds, uint32_t start_phase);
void dds_calculate(dds_t *dds);
void dds_update_tuning_word(dds_t *dds, uint32_t tuning_word);
uint32_t freq_to_tuning_word(float frequency, float clock_frequency);
