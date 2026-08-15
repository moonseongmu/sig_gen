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
#include "stm32h7xx.h" // IWYU pragma: keep
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_pwr.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_utils.h"

//clang-format off
#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"
//clang-format on

#ifndef NVIC_PRIORITYGROUP_0
/*!< 0 bit  for pre-emption priority, 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_0 ((uint32_t)0x00000007)
/*!< 1 bit  for pre-emption priority, 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_1 ((uint32_t)0x00000006)
/*!< 2 bits for pre-emption priority, 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_2 ((uint32_t)0x00000005)
/*!< 3 bits for pre-emption priority, 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_3 ((uint32_t)0x00000004)
/*!< 4 bits for pre-emption priority, 0 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4 ((uint32_t)0x00000003)
#endif

void SystemClock_Config(void);

void system_init(void)
{
    LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SYSCFG);

    /* System interrupt init*/
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority(SysTick_IRQn,
                     NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));

    /* Configure the system clock */
    SystemClock_Config();

    /*Init GPIO*/
    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /* GPIO Ports Clock Enable */
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOH);
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);

    /**/
    LL_GPIO_ResetOutputPin(GPIOE, LL_GPIO_PIN_3);

    /**/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

// setup interrupt/trigger rate timer, setup dma(if available)
void block_transfer_init(void) {}

// output to dac at timer rate
void block_transfer_start(void) {}

// stop transfer of data
void block_transfer_end(void) {}

void led_blink(void *pvParameters [[maybe_unused]])
{
    TickType_t prev_wake_time = xTaskGetTickCount();
    while (1)
    {
        LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_3);
        xTaskDelayUntil(&prev_wake_time, pdMS_TO_TICKS(500));
    }
}

void SystemClock_Config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_3)
    {
    }
    LL_PWR_ConfigSupply(LL_PWR_LDO_SUPPLY);
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE0);
    while (LL_PWR_IsActiveFlag_VOS() == 0)
    {
    }
    LL_RCC_HSE_Enable();

    /* Wait till HSE is ready */
    while (LL_RCC_HSE_IsReady() != 1)
    {
    }
    LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
    LL_RCC_PLL1P_Enable();
    LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
    LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
    LL_RCC_PLL1_SetM(2);
    LL_RCC_PLL1_SetN(44);
    LL_RCC_PLL1_SetP(1);
    LL_RCC_PLL1_SetQ(2);
    LL_RCC_PLL1_SetR(2);
    LL_RCC_PLL1_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLL1_IsReady() != 1)
    {
    }

    /* Intermediate AHB prescaler 2 when target frequency clock is higher than
     * 80 MHz */
    LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);

    /* Wait till System clock is ready */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
    {
    }
    LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
    LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_2);
    LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_2);

    LL_Init1msTick(550000000);

    LL_SetSystemCoreClock(550000000);
}