#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "i2c_application.h"
#include "i2c_driver.h"
#include "nvs_flash.h"
#include "AppConfig.h"

#define STACK_SIZE_2048 2048

/* Structure that will hold the TCB of the task being created. */
StaticTask_t xI2CWriteTaskBuffer;
StaticTask_t xI2CReadTaskBuffer;

/* Buffer that the task being created will use as its stack.  Note this is
    an array of StackType_t variables.  The size of StackType_t is dependent on
    the RTOS port. */

StackType_t xStack_I2C_Write[ STACK_SIZE_2048 ];
StackType_t xStack_I2C_Read[ STACK_SIZE_2048 ];

void app_main(void)
{
    esp_err_t ret;
    // Initialize NVS.
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initializations
	vI2CInit();

    xTaskCreateStaticPinnedToCore(vI2CWrite,            /* Function that implements the task. */
                                  "vI2CWrite",          /* Text name for the task. */
                                  STACK_SIZE_2048,      /* Number of indexes in the xStack array. */
                                  NULL,                 /* Parameter passed into the task. */
                                  osPriorityHigh,       /* Priority at which the task is created. */
                                  xStack_I2C_Write,     /* Array to use as the task's stack. */
                                  &xI2CWriteTaskBuffer, /* Variable to hold the task's data structure. */
                                  0                     /*  0 for PRO_CPU, 1 for APP_CPU, or tskNO_AFFINITY which allows the task to run on both */
                                  );

    xTaskCreateStaticPinnedToCore(vI2CRead,            /* Function that implements the task. */
                                  "vI2CRead",          /* Text name for the task. */
                                  STACK_SIZE_2048,     /* Number of indexes in the xStack array. */
                                  NULL,                /* Parameter passed into the task. */
                                  osPriorityHigh,      /* Priority at which the task is created. */
                                  xStack_I2C_Read,     /* Array to use as the task's stack. */
                                  &xI2CReadTaskBuffer, /* Variable to hold the task's data structure. */
                                  0                    /*  0 for PRO_CPU, 1 for APP_CPU, or tskNO_AFFINITY which allows the task to run on both */
                                  );
}
