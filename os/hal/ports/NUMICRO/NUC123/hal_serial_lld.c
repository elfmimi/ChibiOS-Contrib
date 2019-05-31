/*
    ChibiOS - Copyright (C) 2017 Frank Zschockelt
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
 * @file    hal_serial_lld.c
 * @brief   NUC123 serial subsystem low level driver source.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "osal.h"
#include "hal.h"
#include "numicro_registry.h"

#if (HAL_USE_SERIAL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief UART0 serial driver identifier.*/
#if (NUC123_SERIAL_USE_UART0 == TRUE) || defined(__DOXYGEN__)
SerialDriver SD1;
#endif
/** @brief UART1 serial driver identifier.*/
#if (NUC123_SERIAL_USE_UART1 == TRUE) || defined(__DOXYGEN__)
SerialDriver SD2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static const SerialConfig default_config = {
 115200
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void send_byte(SerialDriver *sdp) {
  UART_T *u = sdp->uart;
  if ((u->IER & UART_IER_THRE_IEN_Msk) == 0) { /* interrupt not enabled */
    msg_t b = oqGetI(&sdp->oqueue);

    if (b < Q_OK) {
      osalSysLockFromISR();
      chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
      osalSysUnlockFromISR();
      return;
    }

    u->DATA = b;
    u->IER |= UART_IER_THRE_IEN_Msk;
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if NUC123_SERIAL_USE_UART0 == TRUE
static void notify1(io_queue_t *qp) {
  (void)qp;
  send_byte(&SD1);
}
#endif

#if NUC123_SERIAL_USE_UART1 == TRUE
static void notify2(io_queue_t *qp) {
  (void)qp;
  send_byte(&SD2);
}
#endif

static void serve_uart_irq(SerialDriver *sdp) {
  UART_T *u = sdp->uart;

  if ((u->ISR & UART_ISR_RDA_INT_Msk) != 0) { /* Data received */
    osalSysLockFromISR();
    if (iqIsEmptyI(&sdp->iqueue))
      chnAddFlagsI(sdp, CHN_INPUT_AVAILABLE);
    if (iqPutI(&sdp->iqueue, u->DATA) < Q_OK)
      chnAddFlagsI(sdp, SD_OVERRUN_ERROR);
    osalSysUnlockFromISR();
  }

  if ((u->ISR & UART_ISR_THRE_INT_Msk) != 0) { /* Data transmitted */
    msg_t b;
    osalSysLockFromISR();
    b = oqGetI(&sdp->oqueue);
    osalSysUnlockFromISR();
    if (b < Q_OK) {
      osalSysLockFromISR();
      chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
      u->IER &= ~UART_IER_THRE_IEN_Msk;
      osalSysUnlockFromISR();
    }
    else {
      u->DATA = b;
    }
  }
}

#if NUC123_SERIAL_USE_UART0 == TRUE
OSAL_IRQ_HANDLER(NUMICRO_SERIAL0_IRQ_VECTOR) {
  OSAL_IRQ_PROLOGUE();
  serve_uart_irq(&SD1);
  OSAL_IRQ_EPILOGUE();
}
#endif

#if NUC123_SERIAL_USE_UART1 == TRUE
OSAL_IRQ_HANDLER(NUMICRO_SERIAL1_IRQ_VECTOR) {
  OSAL_IRQ_PROLOGUE();
  serve_uart_irq(&SD2);
  OSAL_IRQ_EPILOGUE();
}
#endif


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @notapi
 */
void sd_lld_init(void) {

#if NUC123_SERIAL_USE_UART0 == TRUE
  sdObjectInit(&SD1, NULL, notify1);
  SD1.uart = UART0;
#endif
#if NUC123_SERIAL_USE_UART1 == TRUE
  sdObjectInit(&SD2, NULL, notify2);
  SD2.uart = UART1;
#endif
}

static void configure_uart(UART_T *u, const SerialConfig *config) {
  /* Reset RX/TX internal state machine */
  u->FCR = (UART_FCR_TFR_Msk | UART_FCR_RFR_Msk);
  /* 8bit, no parity, one stop bit */
  u->LCR = (0x03 << UART_LCR_WLS_Pos);
  /* no Time-Out or TX Delay  */
  u->TOR = 0;
  /* values above 230400 are untested,
     values are only valid for internal 22.1184MHz clock */
  switch (config->speed) {
  case 921600:
      u->BAUD = (22 << UART_BAUD_BRD_Pos) | UART_BAUD_DIV_X_EN_Msk |
        UART_BAUD_DIV_X_ONE_Msk;
      break;
  case 460800:
      u->BAUD = 1 << UART_BAUD_BRD_Pos;
      break;
  case 230400:
      u->BAUD = 4 << UART_BAUD_BRD_Pos;
      break;
  case 115200:
      u->BAUD = 10 << UART_BAUD_BRD_Pos;
      break;
  case 57600:
      u->BAUD = 22 << UART_BAUD_BRD_Pos;
      break;
  case 38400:
      u->BAUD = (8 << UART_BAUD_DIVIDER_X_Pos)  | (62 << UART_BAUD_BRD_Pos) |
        UART_BAUD_DIV_X_EN_Msk;
      break;
  case 19200:
      u->BAUD = (8 << UART_BAUD_DIVIDER_X_Pos)  | (126 << UART_BAUD_BRD_Pos) |
        UART_BAUD_DIV_X_EN_Msk;
      break;
  case 9600:
      u->BAUD = (8 << UART_BAUD_DIVIDER_X_Pos)  | (254 << UART_BAUD_BRD_Pos) |
        UART_BAUD_DIV_X_EN_Msk;
      break;
  case 4800:
      u->BAUD = (8 << UART_BAUD_DIVIDER_X_Pos)  | (510 << UART_BAUD_BRD_Pos) |
        UART_BAUD_DIV_X_EN_Msk;
      break;
  default:
      osalDbgAssert(FALSE, "not supported");
  }
  /* only enable Receive interrupt */
  u->IER = UART_IER_RDA_IEN_Msk;
}


/**
 * @brief   Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 *
 * @notapi
 */
void sd_lld_start(SerialDriver *sdp, const SerialConfig *config) {

  if (config == NULL) {
    config = &default_config;
  }


  if (sdp->state == SD_STOP) {
#if NUC123_SERIAL_USE_UART0 == TRUE
    if (&SD1 == sdp) {
      UNLOCKREG();
      /* Use internal 22.1184 MHz as UART clocksource */
      CLK->CLKSEL1 &= ~(CLK_CLKSEL1_UART_S_Msk);
      CLK->CLKSEL1 |= (0x03 << CLK_CLKSEL1_UART_S_Pos);
      /* Enable UART clock */
      CLK->APBCLK |= CLK_APBCLK_UART0_EN_Msk;
      /* Reset IP */
      SYS->IPRSTC2 |= SYS_IPRSTC2_UART0_RST_Msk;
      SYS->IPRSTC2 &= ~(SYS_IPRSTC2_UART0_RST_Msk);
      LOCKREG();

      configure_uart(sdp->uart,  config);

      /* configure pins */
      SYS->GPB_MFP = SYS->GPB_MFP | (0x0F);
      // SYS->GPC_MFP = SYS->GPC_MFP | (0x30);
      // SYS->ALT_MFP = SYS->ALT_MFP | 0x60000000;

      nvicEnableVector(UART0_IRQn, NUC123_SERIAL_UART0_PRIORITY);
    }
#endif
#if NUC123_SERIAL_USE_UART1 == TRUE
    if (&SD2 == sdp) {
      UNLOCKREG();
      /* Use internal 22.1184 MHz as UART clocksource */
      CLK->CLKSEL1 &= ~(CLK_CLKSEL1_UART_S_Msk);
      CLK->CLKSEL1 |= (0x03 << CLK_CLKSEL1_UART_S_Pos);
      /* Enable UART clock */
      CLK->APBCLK |= CLK_APBCLK_UART1_EN_Msk;
      /* Reset IP */
      SYS->IPRSTC2 |= SYS_IPRSTC2_UART1_RST_Msk;
      SYS->IPRSTC2 &= ~(SYS_IPRSTC2_UART1_RST_Msk);
      LOCKREG();

      configure_uart(sdp->uart,  config);
      /* configure pins */
      SYS->GPB_MFP = SYS->GPB_MFP | (0xF0);

      nvicEnableVector(UART1_IRQn, NUC123_SERIAL_UART1_PRIORITY);
    }
#endif
  }
  /* Configures the peripheral.*/
  (void)config; /* Warning suppression, remove this.*/
}

/**
 * @brief   Low level serial driver stop.
 * @details De-initializes the UART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @notapi
 */
void sd_lld_stop(SerialDriver *sdp) {

  if (sdp->state == SD_READY) {
#if NUC123_SERIAL_USE_UART0 == TRUE
    if (&SD1 == sdp) {
      /* Disable UART clock */
      CLK->APBCLK &= ~(CLK_APBCLK_UART0_EN_Msk);
      nvicDisableVector(UART0_IRQn);
    }
#endif
#if NUC123_SERIAL_USE_UART1 == TRUE
    if (&SD2 == sdp) {
      /* Disable UART clock */
      CLK->APBCLK &= ~(CLK_APBCLK_UART1_EN_Msk);
      nvicDisableVector(UART1_IRQn);
    }
#endif
    sdp->uart->IER = 0; /* Disable UART interrupts */
  }
}

#endif /* HAL_USE_SERIAL == TRUE */

/** @} */
