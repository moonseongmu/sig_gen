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

#include "interface.h"
#include "oscillator.h"

//clang-format off
#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"
//clang-format on

int main(void)
{
    system_init();

    xTaskCreate(led_blink,
                "led_blink",
                configMINIMAL_STACK_SIZE,
                NULL,
                1,
                NULL);

    xTaskCreate(oscillator_task,
                "oscillator_task",
                configMINIMAL_STACK_SIZE,
                (void *)&oscillator,
                5,
                NULL);

    vTaskStartScheduler();
    while (1)
    {
    }

    return 0; // should never reach here!!
}