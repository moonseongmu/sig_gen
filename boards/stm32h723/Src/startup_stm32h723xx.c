#include "stm32h723xx.h" // IWYU pragma: keep

/*---------------------------------------------------------------------------
  External References
 *---------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;

extern __NO_RETURN void __PROGRAM_START(void);

extern void __libc_init_array(void);

extern int main(void);

/* start address for the initialization values of the .data section.
defined in linker script */
extern uint32_t _sidata;
/* start address for the .data section. defined in linker script */
extern uint32_t _sdata;
/* end address for the .data section. defined in linker script */
extern uint32_t _edata;
/* start address for the .bss section. defined in linker script */
extern uint32_t _sbss;
/* end address for the .bss section. defined in linker script */
extern uint32_t _ebss;

/*---------------------------------------------------------------------------
  Internal References
 *---------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void);
void Default_Handler(void);
__NO_RETURN void _start(void);

/* ToDo: Add Cortex exception handler according the used Cortex-Core */
/*---------------------------------------------------------------------------
  Exception / Interrupt Handler
 *---------------------------------------------------------------------------*/
/* Exceptions */
[[gnu::weak]] void HardFault_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void NMI_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void MemManage_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void BusFault_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void UsageFault_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SecureFault_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SVC_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DebugMon_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void PendSV_Handler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SysTick_Handler(void);

[[gnu::weak, gnu::alias("Default_Handler")]] void WWDG_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void PVD_AVD_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TAMP_STAMP_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void RTC_WKUP_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void FLASH_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void RCC_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void EXTI0_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void EXTI1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void EXTI2_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void EXTI3_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void EXTI4_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA1_Stream0_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA1_Stream1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA1_Stream2_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA1_Stream3_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA1_Stream4_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA1_Stream5_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA1_Stream6_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void ADC_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void FDCAN1_IT0_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void FDCAN2_IT0_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void FDCAN1_IT1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void FDCAN2_IT1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void EXTI9_5_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM1_BRK_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM1_UP_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM1_TRG_COM_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM1_CC_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM2_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM3_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM4_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void I2C1_EV_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void I2C1_ER_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void I2C2_EV_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void I2C2_ER_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SPI1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SPI2_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void USART1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void USART2_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void USART3_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void EXTI15_10_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void RTC_Alarm_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM8_BRK_TIM12_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM8_UP_TIM13_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM8_TRG_COM_TIM14_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM8_CC_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA1_Stream7_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void FMC_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SDMMC1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM5_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SPI3_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void UART4_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void UART5_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM6_DAC_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM7_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA2_Stream0_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA2_Stream1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA2_Stream2_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA2_Stream3_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA2_Stream4_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void ETH_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void ETH_WKUP_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void FDCAN_CAL_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA2_Stream5_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA2_Stream6_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA2_Stream7_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void USART6_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void I2C3_EV_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void I2C3_ER_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void OTG_HS_EP1_OUT_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void OTG_HS_EP1_IN_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void OTG_HS_WKUP_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void OTG_HS_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DCMI_PSSI_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void RNG_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void FPU_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void UART7_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void UART8_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SPI4_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SPI5_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SPI6_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SAI1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void LTDC_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void LTDC_ER_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMA2D_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void OCTOSPI1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void LPTIM1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void CEC_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void I2C4_EV_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void I2C4_ER_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SPDIF_RX_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMAMUX1_OVR_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DFSDM1_FLT0_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DFSDM1_FLT1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DFSDM1_FLT2_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DFSDM1_FLT3_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SWPMI1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM15_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM16_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM17_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void MDIOS_WKUP_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void MDIOS_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void MDMA_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SDMMC2_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void HSEM1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void ADC3_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DMAMUX2_OVR_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void BDMA_Channel0_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void BDMA_Channel1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void BDMA_Channel2_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void BDMA_Channel3_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void BDMA_Channel4_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void BDMA_Channel5_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void BDMA_Channel6_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void BDMA_Channel7_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void COMP1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void LPTIM2_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void LPTIM3_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void LPTIM4_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void LPTIM5_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void LPUART1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void CRS_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void ECC_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void SAI4_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void DTS_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void WAKEUP_PIN_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void OCTOSPI2_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void FMAC_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void CORDIC_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void UART9_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void USART10_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void I2C5_EV_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void I2C5_ER_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void FDCAN3_IT0_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void FDCAN3_IT1_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM23_IRQHandler(void);
[[gnu::weak, gnu::alias("Default_Handler")]] void TIM24_IRQHandler(void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

/* ToDo: Add Cortex exception vectors according the used Cortex-Core */
extern const VECTOR_TABLE_Type __VECTOR_TABLE[];
const VECTOR_TABLE_Type __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
    (VECTOR_TABLE_Type)(&__INITIAL_SP), /*     Initial Stack Pointer */
    Reset_Handler,                      /*     Reset Handler */
    NMI_Handler,                        /* -14 NMI Handler */
    HardFault_Handler,                  /* -13 Hard Fault Handler */
    MemManage_Handler,                  /* -12 MPU Fault Handler */
    BusFault_Handler,                   /* -11 Bus Fault Handler */
    UsageFault_Handler,                 /* -10 Usage Fault Handler */
    0,                                  /*  -9 Secure Fault Handler */
    0,                                  /*     Reserved */
    0,                                  /*     Reserved */
    0,                                  /*     Reserved */
    SVC_Handler,                        /*  -5 SVCall Handler */
    DebugMon_Handler,                   /*  -4 Debug Monitor Handler */
    0,                                  /*     Reserved */
    PendSV_Handler,                     /*  -2 PendSV Handler */
    SysTick_Handler,                    /*  -1 SysTick Handler */

    /* Interrupts */
    WWDG_IRQHandler,         /* Window WatchDog              */
    PVD_AVD_IRQHandler,      /* PVD/AVD through EXTI Line detection */
    TAMP_STAMP_IRQHandler,   /* Tamper and TimeStamps through the EXTI line */
    RTC_WKUP_IRQHandler,     /* RTC Wakeup through the EXTI line */
    FLASH_IRQHandler,        /* FLASH                        */
    RCC_IRQHandler,          /* RCC                          */
    EXTI0_IRQHandler,        /* EXTI Line0                   */
    EXTI1_IRQHandler,        /* EXTI Line1                   */
    EXTI2_IRQHandler,        /* EXTI Line2                   */
    EXTI3_IRQHandler,        /* EXTI Line3                   */
    EXTI4_IRQHandler,        /* EXTI Line4                   */
    DMA1_Stream0_IRQHandler, /* DMA1 Stream 0                */
    DMA1_Stream1_IRQHandler, /* DMA1 Stream 1                */
    DMA1_Stream2_IRQHandler, /* DMA1 Stream 2                */
    DMA1_Stream3_IRQHandler, /* DMA1 Stream 3                */
    DMA1_Stream4_IRQHandler, /* DMA1 Stream 4                */
    DMA1_Stream5_IRQHandler, /* DMA1 Stream 5                */
    DMA1_Stream6_IRQHandler, /* DMA1 Stream 6                */
    ADC_IRQHandler,          /* ADC1, ADC2 and ADC3s         */
    FDCAN1_IT0_IRQHandler,   /* FDCAN1 interrupt line 0      */
    FDCAN2_IT0_IRQHandler,   /* FDCAN2 interrupt line 0      */
    FDCAN1_IT1_IRQHandler,   /* FDCAN1 interrupt line 1      */
    FDCAN2_IT1_IRQHandler,   /* FDCAN2 interrupt line 1      */
    EXTI9_5_IRQHandler,      /* External Line[9:5]s          */
    TIM1_BRK_IRQHandler,     /* TIM1 Break interrupt         */
    TIM1_UP_IRQHandler,      /* TIM1 Update interrupt        */
    TIM1_TRG_COM_IRQHandler, /* TIM1 Trigger and Commutation interrupt */
    TIM1_CC_IRQHandler,      /* TIM1 Capture Compare         */
    TIM2_IRQHandler,         /* TIM2                         */
    TIM3_IRQHandler,         /* TIM3                         */
    TIM4_IRQHandler,         /* TIM4                         */
    I2C1_EV_IRQHandler,      /* I2C1 Event                   */
    I2C1_ER_IRQHandler,      /* I2C1 Error                   */
    I2C2_EV_IRQHandler,      /* I2C2 Event                   */
    I2C2_ER_IRQHandler,      /* I2C2 Error                   */
    SPI1_IRQHandler,         /* SPI1                         */
    SPI2_IRQHandler,         /* SPI2                         */
    USART1_IRQHandler,       /* USART1                       */
    USART2_IRQHandler,       /* USART2                       */
    USART3_IRQHandler,       /* USART3                       */
    EXTI15_10_IRQHandler,    /* External Line[15:10]s        */
    RTC_Alarm_IRQHandler,    /* RTC Alarm (A and B) through EXTI Line */
    0,                       /* Reserved                     */
    TIM8_BRK_TIM12_IRQHandler,     /* TIM8 Break and TIM12         */
    TIM8_UP_TIM13_IRQHandler,      /* TIM8 Update and TIM13        */
    TIM8_TRG_COM_TIM14_IRQHandler, /* TIM8 Trigger and Commutation and TIM14 */
    TIM8_CC_IRQHandler,            /* TIM8 Capture Compare         */
    DMA1_Stream7_IRQHandler,       /* DMA1 Stream7                 */
    FMC_IRQHandler,                /* FMC                          */
    SDMMC1_IRQHandler,             /* SDMMC1                       */
    TIM5_IRQHandler,               /* TIM5                         */
    SPI3_IRQHandler,               /* SPI3                         */
    UART4_IRQHandler,              /* UART4                        */
    UART5_IRQHandler,              /* UART5                        */
    TIM6_DAC_IRQHandler,           /* TIM6 and DAC1&2 underrun errors */
    TIM7_IRQHandler,               /* TIM7                         */
    DMA2_Stream0_IRQHandler,       /* DMA2 Stream 0                */
    DMA2_Stream1_IRQHandler,       /* DMA2 Stream 1                */
    DMA2_Stream2_IRQHandler,       /* DMA2 Stream 2                */
    DMA2_Stream3_IRQHandler,       /* DMA2 Stream 3                */
    DMA2_Stream4_IRQHandler,       /* DMA2 Stream 4                */
    ETH_IRQHandler,                /* Ethernet                     */
    ETH_WKUP_IRQHandler,           /* Ethernet Wakeup through EXTI line */
    FDCAN_CAL_IRQHandler,          /* FDCAN calibration unit interrupt*/
    0,                             /* Reserved                     */
    0,                             /* Reserved                     */
    0,                             /* Reserved                     */
    0,                             /* Reserved                     */
    DMA2_Stream5_IRQHandler,       /* DMA2 Stream 5                */
    DMA2_Stream6_IRQHandler,       /* DMA2 Stream 6                */
    DMA2_Stream7_IRQHandler,       /* DMA2 Stream 7                */
    USART6_IRQHandler,             /* USART6                       */
    I2C3_EV_IRQHandler,            /* I2C3 event                   */
    I2C3_ER_IRQHandler,            /* I2C3 error                   */
    OTG_HS_EP1_OUT_IRQHandler,     /* USB OTG HS End Point 1 Out   */
    OTG_HS_EP1_IN_IRQHandler,      /* USB OTG HS End Point 1 In    */
    OTG_HS_WKUP_IRQHandler,        /* USB OTG HS Wakeup through EXTI */
    OTG_HS_IRQHandler,             /* USB OTG HS                   */
    DCMI_PSSI_IRQHandler,          /* DCMI, PSSI                   */
    0,                             /* Reserved                     */
    RNG_IRQHandler,                /* Rng                          */
    FPU_IRQHandler,                /* FPU                          */
    UART7_IRQHandler,              /* UART7                        */
    UART8_IRQHandler,              /* UART8                        */
    SPI4_IRQHandler,               /* SPI4                         */
    SPI5_IRQHandler,               /* SPI5                         */
    SPI6_IRQHandler,               /* SPI6                         */
    SAI1_IRQHandler,               /* SAI1                         */
    LTDC_IRQHandler,               /* LTDC                         */
    LTDC_ER_IRQHandler,            /* LTDC error                   */
    DMA2D_IRQHandler,              /* DMA2D                        */
    0,                             /* Reserved                     */
    OCTOSPI1_IRQHandler,           /* OCTOSPI1                     */
    LPTIM1_IRQHandler,             /* LPTIM1                       */
    CEC_IRQHandler,                /* HDMI_CEC                     */
    I2C4_EV_IRQHandler,            /* I2C4 Event                   */
    I2C4_ER_IRQHandler,            /* I2C4 Error                   */
    SPDIF_RX_IRQHandler,           /* SPDIF_RX                     */
    0,                             /* Reserved                     */
    0,                             /* Reserved                     */
    0,                             /* Reserved                     */
    0,                             /* Reserved                     */
    DMAMUX1_OVR_IRQHandler,        /* DMAMUX1 Overrun interrupt    */
    0,                             /* Reserved                     */
    0,                             /* Reserved                     */
    0,                             /* Reserved                     */
    0,                             /* Reserved                     */
    0,                             /* Reserved                     */
    0,                             /* Reserved                     */
    0,                             /* Reserved                     */
    DFSDM1_FLT0_IRQHandler,        /* DFSDM Filter0 Interrupt      */
    DFSDM1_FLT1_IRQHandler,        /* DFSDM Filter1 Interrupt      */
    DFSDM1_FLT2_IRQHandler,        /* DFSDM Filter2 Interrupt      */
    DFSDM1_FLT3_IRQHandler,        /* DFSDM Filter3 Interrupt      */
    0,                             /* Reserved                     */
    SWPMI1_IRQHandler,        /* Serial Wire Interface 1 global interrupt */
    TIM15_IRQHandler,         /* TIM15 global Interrupt          */
    TIM16_IRQHandler,         /* TIM16 global Interrupt          */
    TIM17_IRQHandler,         /* TIM17 global Interrupt          */
    MDIOS_WKUP_IRQHandler,    /* MDIOS Wakeup  Interrupt         */
    MDIOS_IRQHandler,         /* MDIOS global Interrupt          */
    0,                        /* Reserved                        */
    MDMA_IRQHandler,          /* MDMA global Interrupt           */
    0,                        /* Reserved                        */
    SDMMC2_IRQHandler,        /* SDMMC2 global Interrupt         */
    HSEM1_IRQHandler,         /* HSEM1 global Interrupt          */
    0,                        /* Reserved                        */
    ADC3_IRQHandler,          /* ADC3 global Interrupt           */
    DMAMUX2_OVR_IRQHandler,   /* DMAMUX Overrun interrupt        */
    BDMA_Channel0_IRQHandler, /* BDMA Channel 0 global Interrupt */
    BDMA_Channel1_IRQHandler, /* BDMA Channel 1 global Interrupt */
    BDMA_Channel2_IRQHandler, /* BDMA Channel 2 global Interrupt */
    BDMA_Channel3_IRQHandler, /* BDMA Channel 3 global Interrupt */
    BDMA_Channel4_IRQHandler, /* BDMA Channel 4 global Interrupt */
    BDMA_Channel5_IRQHandler, /* BDMA Channel 5 global Interrupt */
    BDMA_Channel6_IRQHandler, /* BDMA Channel 6 global Interrupt */
    BDMA_Channel7_IRQHandler, /* BDMA Channel 7 global Interrupt */
    COMP1_IRQHandler,         /* COMP1 global Interrupt          */
    LPTIM2_IRQHandler,        /* LP TIM2 global interrupt        */
    LPTIM3_IRQHandler,        /* LP TIM3 global interrupt        */
    LPTIM4_IRQHandler,        /* LP TIM4 global interrupt        */
    LPTIM5_IRQHandler,        /* LP TIM5 global interrupt        */
    LPUART1_IRQHandler,       /* LP UART1 interrupt              */
    0,                        /* Reserved                        */
    CRS_IRQHandler,           /* Clock Recovery Global Interrupt */
    ECC_IRQHandler,           /* ECC diagnostic Global Interrupt */
    SAI4_IRQHandler,          /* SAI4 global interrupt           */
    DTS_IRQHandler,           /* Digital Temperature Sensor  interrupt */
    0,                        /* Reserved                              */
    WAKEUP_PIN_IRQHandler,    /* Interrupt for all 6 wake-up pins      */
    OCTOSPI2_IRQHandler,      /* OCTOSPI2 Interrupt       */
    0,                        /* Reserved                 */
    0,                        /* Reserved                 */
    FMAC_IRQHandler,          /* FMAC Interrupt           */
    CORDIC_IRQHandler,        /* CORDIC Interrupt         */
    UART9_IRQHandler,         /* UART9 Interrupt          */
    USART10_IRQHandler,       /* UART10 Interrupt         */
    I2C5_EV_IRQHandler,       /* I2C5 Event Interrupt     */
    I2C5_ER_IRQHandler,       /* I2C5 Error Interrupt     */
    FDCAN3_IT0_IRQHandler,    /* FDCAN3 interrupt line 0  */
    FDCAN3_IT1_IRQHandler,    /* FDCAN3 interrupt line 1  */
    TIM23_IRQHandler,         /* TIM23 global interrupt   */
    TIM24_IRQHandler,         /* TIM24 global interrupt   */

};

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

/*---------------------------------------------------------------------------
  Reset Handler called on controller reset
 *---------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void)
{
    __set_PSP((uint32_t)(&__INITIAL_SP));
    /* Call the ExitRun0Mode function to configure the power supply */
    ExitRun0Mode();
    /* CMSIS System Initialization */
    SystemInit();
    __PROGRAM_START(); /* Enter PreMain (C library entry point) */
}

/*---------------------------------------------------------------------------
  Hard Fault Handler
 *---------------------------------------------------------------------------*/
void HardFault_Handler(void)
{
    while (1)
        ;
}

/*---------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *---------------------------------------------------------------------------*/
void Default_Handler(void)
{
    while (1)
        ;
}

void _start(void)
{
    // copy data from flash to ram
    uint32_t *data_src = &_sidata;
    uint32_t *data_dst = &_sdata;

    while (data_dst < &_edata)
    {
        *data_dst++ = *data_src++;
    }

    // zero initialise bss
    uint32_t *bss_src = &_sbss;
    while (bss_src < &_ebss)
    {
        *bss_src++ = 0;
    }

    // Call static constructors
    __libc_init_array();

    // call main
    main();

    // infinite loop if exit
    while (1)
    {
    }
}
