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
#include "tusb.h"

// FreeRTOS.h needs to be called first
//clang-format off
#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"
//clang-format on

// tinyusb device driver task
void usb_device_task(void *params [[maybe_unused]])
{
    usb_init();
    tusb_rhport_init_t port_init = { .role = TUSB_ROLE_DEVICE,
                                     .speed = TUSB_SPEED_FULL };

    tusb_init(0, &port_init);

    while (1)
    {
        tud_task();
    }
}

TaskHandle_t cdc_task_handle = NULL;

void tud_cdc_rx_cb(uint8_t itf [[maybe_unused]])
{
    if (cdc_task_handle != NULL)
    {
        BaseType_t higher_priority_task_woken = pdFALSE;
        // Notify the cdc processing task that data has arrived
        vTaskNotifyGiveFromISR(cdc_task_handle, &higher_priority_task_woken);
        portYIELD_FROM_ISR(higher_priority_task_woken);
    }
}

void usb_cdc_task(void *params [[maybe_unused]])
{
    while (1)
    {
        // Wait indefinitely until tud_cdc_rx_cb gives a notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // There are data available
        while (tud_cdc_available())
        {
            uint8_t buf[64];

            // read and echo back
            uint32_t count = tud_cdc_read(buf, sizeof(buf));

            // Echo back
            tud_cdc_write(buf, count);
        }

        tud_cdc_write_flush();
    }
}