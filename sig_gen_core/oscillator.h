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

#include "interface.h"

// FreeRTOS.h needs to be called first
//clang-format off
#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"
//clang-format on

extern TaskHandle_t oscillator_task_handle;

void fill_buffer(oscillator_t *oscillator);
void oscillator_task(void *params);

extern oscillator_t oscillator_struct;
