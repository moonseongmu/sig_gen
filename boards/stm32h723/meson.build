c_args+=[
'-mcpu=cortex-m7',
'-mfpu=fpv5-d16',
'-mfloat-abi=hard',
'--target=arm-none-eabi']

linkerscript = '-T' + meson.current_source_dir() + '/STM32H723xG_flash.ld'

c_link_args+=[
'-mcpu=cortex-m7',
'-mfpu=fpv5-d16',
'-mfloat-abi=hard',
'--target=arm-none-eabi',
linkerscript,
'-Wl,--gc-sections',
'-Wl,--print-memory-usage']

src+=files(
'Src/interface_implementation.c'
)

src+=files(
'Src\startup_stm32h723xx.c',
'Src\stm32h7xx_it.c',
'Src\syscalls.c',
'Src\sysmem.c',
'Src\system_stm32h7xx.c')

src+=files(
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_adc.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_bdma.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_comp.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_cordic.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_crc.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_crs.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_dac.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_dma.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_dma2d.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_exti.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_fmac.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_gpio.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_hrtim.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_i2c.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_lptim.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_lpuart.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_mdma.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_opamp.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_pwr.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_rcc.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_rng.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_rtc.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_spi.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_swpmi.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_tim.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_usart.c',
'Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_ll_utils.c')

st_inc=include_directories('Src',
'Drivers/CMSIS/Device/ST/STM32H7xx/Include',
'Drivers/STM32H7xx_HAL_Driver/Inc')

inc+=[st_inc]

add_project_arguments('-DUSE_FULL_LL_DRIVER',
'-DHSE_VALUE=25000000',
'-DHSE_STARTUP_TIMEOUT=100',
'-DLSE_STARTUP_TIMEOUT=5000',
'-DLSE_VALUE=32768',
'-DEXTERNAL_CLOCK_VALUE=12288000',
'-DHSI_VALUE=64000000',
'-DLSI_VALUE=32000',
'-DVDD_VALUE=3300',
'-DUSE_PWR_LDO_SUPPLY',
'-DSTM32H723xx',
language: 'c')