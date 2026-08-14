#include "FreeRTOS.h"
#include "interface.h"
#include "task.h"

int main(void)
{
    system_init();

    TaskHandle_t led_blink_handle = NULL;
    xTaskCreate(led_blink,
                "led_blink",
                configMINIMAL_STACK_SIZE,
                NULL,
                1,
                &led_blink_handle);

    vTaskStartScheduler();
    while (1)
    {
    }

    return 0; // should never reach here!!
}