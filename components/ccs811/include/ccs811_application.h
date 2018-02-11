/**********************************************************************
* - Description:		esp32-ccs811
* - File:				ccs811_application.h
* - Compiler:			xtensa-esp32
* - Debugger:			USB2USART
* - Author:				Mohamed El-Sabagh
* - Target:				ESP32
* - Created:			2017-7-29
* - Last changed:		2017-7-29
*
**********************************************************************/
#ifndef _CCS811_APPLICATION_H
#define _CCS811_APPLICATION_H

#include <stdint.h>
#include "i2c_application.h"
#include "AppConfig.h"

void vCCS811Task( void *pvParameters );

#endif
