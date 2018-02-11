/**********************************************************************
* - Description:		esp32-ccs811
* - File:				ccs811_application.c
* - Compiler:			xtensa-esp32
* - Debugger:			USB2USART
* - Author:				Mohamed El-Sabagh
* - Target:				ESP32
* - Created:			2017-7-29
* - Last changed:		2017-7-29
*
**********************************************************************/
#include "ccs811_application.h"
#include <string.h>
#include "i2c_driver.h"
#include <stdio.h>

#define CCS811_SLAVE_ADDR							0x5A

#define MEAS_MODE									0x01
#define ALG_RESULT_DATA								0x02

#define CCS811_SUCESS					 	       	0
#define CCS811_FAILURE						       	1

#define SENSOR_READING_FREQUENCY_MS	     			10000	// 10 seconds

static I2C_Status i2cStatus;
static uint8_t ccs811Buffer[8];

static uint8_t uCCS811Init();
static uint8_t uCCS811ReadValue();


/**
  * @brief  MPU6050 task.
  * @param  None
  * @retval None
  */
void vCCS811Task( void *pvParameters )
{
	if (uCCS811Init() != CCS811_SUCESS) {
        vTaskDelete(NULL);
        return;
    }

	for(;;)
	{
		vTaskDelay( SENSOR_READING_FREQUENCY_MS / portTICK_RATE_MS );
		if (uCCS811ReadValue() == CCS811_SUCESS) {
			printf("eCO2 Hight Byte = %02X", ccs811Buffer[0]);
			printf("eCO2 Low Byte = %02X", ccs811Buffer[1]);
			printf("TVOC Hight Byte = %02X", ccs811Buffer[2]);
			printf("TVOC Low Byte = %02X", ccs811Buffer[3]);
			printf("Status Byte = %02X", ccs811Buffer[4]);
			printf("ErrorId Byte = %02X", ccs811Buffer[5]);
			printf("Raw Byte1 = %02X", ccs811Buffer[6]);
			printf("Raw Byte2 = %02X", ccs811Buffer[7]);
        }
	}

	vTaskDelete(NULL);
}

/**
  * @brief  Configure the CCS811.
  * @param  None
  * @retval Success or Fail
  */
uint8_t uCCS811Init()
{
    portBASE_TYPE xStatus;

    // Change status from sleep to running
    ccs811Buffer[0] = MEAS_MODE;
    ccs811Buffer[1] = 0x10;
    // Write data to i2c
    i2cStatus.pBuffer = ccs811Buffer;
    i2cStatus.sDataLength = 2;
    i2cStatus.slaveAddress = CCS811_SLAVE_ADDR;

    xStatus = xQueueSend( xQueueI2CWriteBuffer, (void *)&i2cStatus, portMAX_DELAY );

    // Wait for end of writing process
    xStatus = xSemaphoreTake( xBinarySemaphoreI2CAppEndOfWrite, portMAX_DELAY );

    if (xStatus == pdFAIL)
	{
		return CCS811_FAILURE;
	}

    return CCS811_SUCESS;
}

/**
  * @brief  Read CCS811.
  * @param  None
  * @retval Success or Fail
  */
uint8_t uCCS811ReadValue()
{
    portBASE_TYPE xStatus;

    // Read sensor
    ccs811Buffer[0] = ALG_RESULT_DATA;
    // Write data to i2c
    i2cStatus.pBuffer = ccs811Buffer;
    i2cStatus.sDataLength = 1;
    i2cStatus.slaveAddress = CCS811_SLAVE_ADDR;

    xStatus = xQueueSend( xQueueI2CWriteBuffer, (void *)&i2cStatus, portMAX_DELAY );

    // Wait for end of writing process
    xStatus = xSemaphoreTake( xBinarySemaphoreI2CAppEndOfWrite, portMAX_DELAY );

    if (xStatus == pdFAIL)
	{
		return CCS811_FAILURE;
	}

    i2cStatus.pBuffer = ccs811Buffer;
    i2cStatus.sDataLength = 8;
    i2cStatus.slaveAddress = CCS811_SLAVE_ADDR;

    xStatus = xQueueSend( xQueueI2CReadBuffer, (void *)&i2cStatus, portMAX_DELAY );

    // Wait for end of reading process
    xStatus = xSemaphoreTake( xBinarySemaphoreI2CAppEndOfRead, portMAX_DELAY );

    if (xStatus == pdFAIL)
	{
		return CCS811_FAILURE;
	}

    return CCS811_SUCESS;
}
