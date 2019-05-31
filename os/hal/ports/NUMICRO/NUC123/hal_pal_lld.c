/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio
               Copyright (C) 2019 Ein Terakawa

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

/**
 * @file    hal_pal_lld.c
 * @brief   NUC123 PAL subsystem low level driver source.
 *
 * @addtogroup PAL
 * @{
 */

#include "hal.h"

#if (HAL_USE_PAL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   NUC123 I/O ports configuration.
 * @details Ports A-D(E, F, G, H) clocks enabled.
 *
 * @param[in] config    the NUC123 ports configuration
 *
 * @notapi
 */
void _pal_lld_init(const PALConfig *config) {
  IOPORT1->PMD = config->ioport1_pmd;
  IOPORT2->PMD = config->ioport2_pmd;
  IOPORT3->PMD = config->ioport3_pmd;
  IOPORT4->PMD = config->ioport4_pmd;
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] mode      the mode
 *
 * @notapi
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           iomode_t mode) {
  uint8_t pmd_flags;
  uint32_t i;
  uint32_t pmd = port->PMD;

  switch (mode) {
    default:
    case PAL_MODE_RESET:
      pmd_flags = 3;
      break;
    case PAL_MODE_INPUT:
      pmd_flags = 0;
      break;
    case PAL_MODE_OUTPUT_PUSHPULL:
      pmd_flags = 1;
      break;
    case PAL_MODE_OUTPUT_OPENDRAIN:
      pmd_flags = 2;
      break;
  }
  for (i=0; i<PAL_IOPORTS_WIDTH; i++) {
    if (mask & (1 << i)) {
      pmd = pmd & ~(0x03 << (i*2));
      pmd = pmd | (pmd_flags << (i*2));
    }
  }
  port->PMD = pmd;
}

#endif /* HAL_USE_PAL == TRUE */

/** @} */
