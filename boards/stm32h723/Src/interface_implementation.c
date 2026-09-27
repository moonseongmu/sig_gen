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

#include "interface_implementation.h"
#include "interface.h"
#include "stm32h7xx.h" // IWYU pragma: keep
#include "stm32h7xx_ll_dac.h"
#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_dmamux.h"
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_pwr.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_tim.h"
#include "stm32h7xx_ll_utils.h"
#include "tusb.h"

// FreeRTOS.h needs to be called first
//clang-format off
#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"
//clang-format on

// cycle counting utils
void init_cyccnt(void)
{
    SET_BIT(DCB->DEMCR, DCB_DEMCR_TRCENA_Msk);
}

void clear_cyccnt(void)
{
    CLEAR_REG(DWT->CYCCNT);
}

void start_cyccnt(void)
{
    SET_BIT(DWT->CTRL, DWT_CTRL_CYCCNTENA_Msk);
}

void stop_cyccnt(void)
{
    CLEAR_BIT(DWT->CTRL, DWT_CTRL_CYCCNTENA_Msk);
}

uint32_t get_cyccnt(void)
{
    return READ_REG(DWT->CYCCNT);
}

void system_init(void)
{
    LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SYSCFG);

    /* System interrupt init*/
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority(SysTick_IRQn,
                     NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));

    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();

    init_cyccnt();

    __enable_irq();
}

// setup interrupt/trigger rate timer, setup dma(if available)
void block_transfer_init(oscillator_t *oscillator)
{
    // init timer
    // timer input clock is 275Mhz
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM8);
    LL_TIM_InitTypeDef TIM8_init_struct = {
        .Prescaler = 0,
        .ClockDivision = LL_TIM_CLOCKDIVISION_DIV1,
        .CounterMode = LL_TIM_COUNTERMODE_UP,
        .Autoreload = 275 - 1, // overflow at 1MHz
        .RepetitionCounter = 0
    };
    LL_TIM_Init(TIM8, &TIM8_init_struct);
    LL_TIM_EnableARRPreload(TIM8);
    LL_TIM_EnableUpdateEvent(TIM8);
    LL_TIM_SetUpdateSource(TIM8, LL_TIM_UPDATESOURCE_COUNTER);
    LL_TIM_SetTriggerOutput(TIM8, LL_TIM_TRGO_UPDATE);

    // enable gpio pin for dac
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);
    LL_GPIO_InitTypeDef dac_pin_init_struct = { .Pin = LL_GPIO_PIN_4,
                                                .Mode = LL_GPIO_MODE_ANALOG,
                                                .Pull = LL_GPIO_PULL_NO };
    LL_GPIO_Init(GPIOA, &dac_pin_init_struct);

    // init dac
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC12);
    LL_DAC_InitTypeDef dac_init_struct = {
        .TriggerSource = LL_DAC_TRIG_EXT_TIM8_TRGO,
        .OutputMode = LL_DAC_OUTPUT_MODE_NORMAL,
        .OutputBuffer = LL_DAC_OUTPUT_BUFFER_ENABLE,
        .OutputConnection = LL_DAC_OUTPUT_CONNECT_GPIO,
        .WaveAutoGeneration = LL_DAC_WAVE_AUTO_GENERATION_NONE
    };

    LL_DAC_Init(DAC1, LL_DAC_CHANNEL_1, &dac_init_struct);
    LL_DAC_EnableTrigger(DAC1, LL_DAC_CHANNEL_1);
    LL_DAC_EnableDMAReq(DAC1, LL_DAC_CHANNEL_1);

    // init dma
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    LL_DMA_InitTypeDef dma_init_struct = {
        .PeriphOrM2MSrcAddress
        = LL_DAC_DMA_GetRegAddr(DAC1,
                                LL_DAC_CHANNEL_1,
                                LL_DAC_DMA_REG_DATA_12BITS_RIGHT_ALIGNED),
        .Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH,
        .PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT,
        .MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT,
        .PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD,
        .MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD,
        .NbData = BLOCK_SIZE,
        .PeriphRequest = LL_DMAMUX1_REQ_DAC1_CH1,
        .Priority = LL_DMA_PRIORITY_VERYHIGH,
        .FIFOMode = LL_DMA_FIFOMODE_DISABLE,
    };
    LL_DMA_Init(DMA1, LL_DMA_STREAM_0, &dma_init_struct);
    LL_DMA_EnableDoubleBufferMode(DMA1, LL_DMA_STREAM_0);
    LL_DMA_SetMemoryAddress(DMA1,
                            LL_DMA_STREAM_0,
                            (uint32_t)(uintptr_t)oscillator->buffer_1);
    LL_DMA_SetMemory1Address(DMA1,
                             LL_DMA_STREAM_0,
                             (uint32_t)(uintptr_t)oscillator->buffer_2);

    // enable transfer complete interrupt
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_0);

    // set output frequency and system clock frequency !!!FOR TESTING!!!

    // timer output frequecy is 1MHz
    oscillator->clock_freq = 1000000;

    oscillator->out_freq = 100;
}

// output to dac at timer rate
void block_transfer_start(void)
{
    // enable dac
    LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);
    // enable dma and irq
    NVIC_SetPriority(DMA1_Stream0_IRQn,
                     configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    NVIC_EnableIRQ(DMA1_Stream0_IRQn);
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
    // enable timer
    LL_TIM_EnableCounter(TIM8);
}

// stop transfer of data
void block_transfer_end(void)
{
    LL_TIM_DisableCounter(TIM8);
    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);
    NVIC_DisableIRQ(DMA1_Stream0_IRQn);
    LL_DAC_Disable(DAC1, LL_DAC_CHANNEL_1);
}

void DMA1_Stream0_IRQHandler(void)
{
    LL_DMA_ClearFlag_TC0(DMA1);
    block_transfer_complete_ISR();
}

// blinks led at 2Hz
void led_blink(void *params [[maybe_unused]])
{
    /*Init GPIO*/
    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    /* GPIO Ports Clock Enable */
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);

    LL_GPIO_ResetOutputPin(GPIOE, LL_GPIO_PIN_3);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    TickType_t prev_wake_time = xTaskGetTickCount();
    while (1)
    {
        LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_3);
        xTaskDelayUntil(&prev_wake_time, pdMS_TO_TICKS(1000));
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
    LL_RCC_PLL1R_Enable();
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

    LL_RCC_PLL3Q_Enable();
    LL_RCC_PLL3_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_1_2);
    LL_RCC_PLL3_SetVCOOutputRange(LL_RCC_PLLVCORANGE_MEDIUM);
    LL_RCC_PLL3_SetM(25);
    LL_RCC_PLL3_SetN(192);
    LL_RCC_PLL3_SetP(2);
    LL_RCC_PLL3_SetQ(4);
    LL_RCC_PLL3_SetR(2);
    LL_RCC_PLL3_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLL3_IsReady() != 1)
    {
    }

    /* Intermediate AHB prescaler 2 when target frequency clock is higher
     * than 80 MHz */
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

    /* Update the time base */
    if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

// HAL timebase functions

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM7)
    {
        HAL_IncTick();
    }
}

extern TIM_HandleTypeDef htim7;
void TIM7_IRQHandler(void)
{
    /* USER CODE BEGIN TIM7_IRQn 0 */

    /* USER CODE END TIM7_IRQn 0 */
    HAL_TIM_IRQHandler(&htim7);
    /* USER CODE BEGIN TIM7_IRQn 1 */

    /* USER CODE END TIM7_IRQn 1 */
}

void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
}

// USB Functions

PCD_HandleTypeDef hpcd_USB_OTG_HS;

/* USB_OTG_HS init function */

void OTG_HS_IRQHandler(void)
{
    tusb_int_handler(0, true);
}

// tinyusb device driver task
void usb_device_task(void *params [[maybe_unused]])
{
    USB_OTG_HS_PCD_Init();
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

void USB_OTG_HS_PCD_Init(void)
{
    hpcd_USB_OTG_HS.Instance = USB_OTG_HS;
    hpcd_USB_OTG_HS.Init.dev_endpoints = 9;
    hpcd_USB_OTG_HS.Init.speed = PCD_SPEED_FULL;
    hpcd_USB_OTG_HS.Init.dma_enable = DISABLE;
    hpcd_USB_OTG_HS.Init.phy_itface = USB_OTG_EMBEDDED_PHY;
    hpcd_USB_OTG_HS.Init.Sof_enable = DISABLE;
    hpcd_USB_OTG_HS.Init.low_power_enable = DISABLE;
    hpcd_USB_OTG_HS.Init.lpm_enable = DISABLE;
    hpcd_USB_OTG_HS.Init.vbus_sensing_enable = DISABLE;
    hpcd_USB_OTG_HS.Init.use_dedicated_ep1 = DISABLE;
    hpcd_USB_OTG_HS.Init.use_external_vbus = DISABLE;
    if (HAL_PCD_Init(&hpcd_USB_OTG_HS) != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_PCD_MspInit(PCD_HandleTypeDef *pcdHandle)
{
    if (pcdHandle->Instance == USB_OTG_HS)
    {
        LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_PLL3Q);
        LL_PWR_EnableUSBVoltageDetector();

        /* USB_OTG_HS clock enable */
        __HAL_RCC_USB_OTG_HS_CLK_ENABLE();

        /* USB_OTG_HS interrupt Init */
        HAL_NVIC_SetPriority(OTG_HS_IRQn,
                             configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY,
                             0);
        HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
    }
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef *pcdHandle)
{
    if (pcdHandle->Instance == USB_OTG_HS)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USB_OTG_HS_CLK_DISABLE();

        /* USB_OTG_HS interrupt Deinit */
        HAL_NVIC_DisableIRQ(OTG_HS_IRQn);
    }
}