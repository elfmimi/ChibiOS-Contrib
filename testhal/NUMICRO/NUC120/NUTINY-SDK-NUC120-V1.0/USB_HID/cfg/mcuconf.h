/*
  ChibiOS - Copyright (C) 2020 Alex Lewontin

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef _MCUCONF_H_
#define _MCUCONF_H_

/*
 * Board setting
 */

/*
 * HAL driver system settings.
 */
#define NUC123_HSE_ENABLED TRUE
#define NUC123_PLL_ENABLED TRUE
#define NUC123_PLLSRC NUC123_PLLSRC_HSE

// #define NUC123_SERIAL_CLKSRC NUC123_SERIAL_CLKSRC_PLL
// #define NUC123_SERIAL_CLKSRC NUC123_SERIAL_CLKSRC_HSE
#define NUC123_SERIAL_CLKSRC NUC123_SERIAL_CLKSRC_HSI
#define NUC123_SERIAL_USE_UART0 FALSE
#define NUC123_SERIAL_USE_UART1 TRUE

#define NUC123_I2C_USE_I2C0 FALSE
#define NUC123_I2C_USE_I2C1 FALSE

#define NUC123_USB_USE_USB1 TRUE

// #define NUC123_CONFIG_ENABLED FALSE
// #define NUC123_CONFIG_DATAFLASH_SIZE 20480
#define NUC123_CONFIG_DATAFLASH_SIZE (NUC123_FLASH_SIZE - FMC->DFBADR)
// #define NUC123_DFBADDR 0xC000
#define NUC123_DFBADDR ((void *)(FMC->DFBADR))
// #define NUC123_DFBADDR ((0x11000UL - NUC123_CONFIG_DATAFLASH_SIZE) & ~(0x1FFUL))
// #define NUC123_DATAFLASH_ENABLED TRUE
// #define NUC123_DATAFLASH_SIZE 4096
// #define NUC123_EFL_ACCESS_APROM TRUE
#define NUC123_EFL_ACCESS_APROM FALSE
#define NUC123_EFL_ACCESS_DATAFLASH TRUE
// #define NUC123_EFL_ACCESS_LDROM TRUE
// #define NUC123_EFL_ACCESS_CONFIG TRUE
#define NUC123_EFL_DYNAMICALLY_CHECK_CONFIG FALSE

#define NUC123_MCUCONF

#endif /* _MCUCONF_H_ */
