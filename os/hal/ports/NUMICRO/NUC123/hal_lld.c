/*
    Copyright (C) 2017 Frank Zschockelt
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
 * @file    NUMICRO/NUC123/hal_lld.c
 * @brief   NUC123 HAL Driver subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "hal.h"

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

#define CLK_PLLCON_144MHz_XTAL    0x402E // 144MHz

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void)
{
  /* Enable XT1_OUT (PF0) and XT1_IN (PF1) */
  // SYS->GPF_MFP |= SYS_GPF_MFP_PF0_XT1_OUT | SYS_GPF_MFP_PF1_XT1_IN;
  SYS->GPF_MFP |= 1 | 2;

  UNLOCKREG();

  /* enable internal osc and low frequency osc*/
  CLK->PWRCON = CLK_PWRCON_PD_WU_DLY_Msk | CLK_PWRCON_OSC22M_EN_Msk | CLK_PWRCON_XTL12M_EN_Msk;

  while ((CLK->CLKSTATUS & CLK_CLKSTATUS_OSC22M_STB_Msk) == 0)
    ;

  // CLK->PWRCON |= CLK_PWRCON_XTL12M_EN_Msk;
  while ((CLK->CLKSTATUS & CLK_CLKSTATUS_XTL12M_STB_Msk) == 0)
    ;

  //  CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_PLL, CLK_CLKDIV_HCLK(2));
  /* Switch to HIRC for Safe. Avoid HCLK too high when applying new divider. */
  // CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLK_S_Msk)) | CLK_CLKSEL0_HCLK_S_HIRC;
  CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLK_S_Msk)) | (7<<CLK_CLKSEL0_HCLK_S_Pos);

  /* configure the PLL */
  /* page 108/109 of NUC122 Technical Reference Manual */
  /* PLL =~ 239.98464Mhz */
  // CLK->PLLCON = (215 << CLK_PLLCON_FB_DV_Pos) | (8 <<CLK_PLLCON_IN_DV_Pos) | (1<<CLK_PLLCON_OUT_DV_Pos) | CLK_PLLCON_PLL_SRC_Msk;
  
  /* PLL = 144 Mhz */
  CLK->PLLCON = CLK_PLLCON_144MHz_XTAL;

  /* Apply new Divider */
  // CLK->CLKDIV = (CLK->CLKDIV & (~CLK_CLKDIV_HCLK_N_Msk)) | CLK_CLKDIV_HCLK(2);

  /* HCLK = 72 MHz, USB_CLK = 48 MHz */
  // CLK->CLKDIV = CLK_CLKDIV_HCLK(2) | CLK_CLKDIV_USB(3);
  // CLK->CLKDIV = (CLK->CLKDIV & (~CLK_CLKDIV_HCLK_N_Msk)) | CLK_CLKDIV_HCLK(2);
  CLK->CLKDIV = (CLK->CLKDIV & (~CLK_CLKDIV_HCLK_N_Msk)) | (2-1);
  // CLK->CLKDIV = (CLK->CLKDIV & (~CLK_CLKDIV_USB_N_Msk)) | CLK_CLKDIV_USB(3);  
  CLK->CLKDIV = (CLK->CLKDIV & (~CLK_CLKDIV_USB_N_Msk)) | ((3-1)<<CLK_CLKDIV_USB_N_Pos);
  /* enable pll */
  CLK->PLLCON &= ~(CLK_PLLCON_OE_Msk | CLK_PLLCON_PD_Msk);

  while ((CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB_Msk) == 0)
    ;

  /* Switch to PLL as clock source*/
  // CLK->CLKSEL0 = CLK_CLKSEL0_HCLK_S_PLL | CLK_CLKSEL0_STCLK_S_HIRC_DIV2;
  CLK->CLKSEL0 = (2<<CLK_CLKSEL0_HCLK_S_Pos) | (7<<CLK_CLKSEL0_STCLK_S_Pos);

  /* Set PC.13 as CLKO function pin */
  // SYS->GPC_MFP |= SYS_GPC_MFP_PC13_CLKO;
  // SYS->ALT_MFP |= SYS_ALT_MFP_PC13_CLKO;

  /* Enable CLKO (PC.13) for monitor HCLK. CLKO = HCLK/8 Hz*/
  // CLK_EnableCKO(CLK_CLKSEL2_FRQDIV_S_HCLK, 2, NULL);
  /* CKO = clock source / 2^(u32ClkDiv + 1) */
  // CLK->FRQDIV = (CLK_FRQDIV_DIVIDER_EN_Msk | 2 /* u32ClkDiv */) ;
  // CLK->APBCLK |= CLK_APBCLK_FDIV_EN_Msk;
  // CLK->CLKSEL2 = (CLK->CLKSEL2 & ~CLK_CLKSEL2_FRQDIV_S_Msk) | CLK_CLKSEL2_FRQDIV_S_HCLK;

  CLK->APBCLK |= CLK_APBCLK_USBD_EN_Msk;

  LOCKREG();
}

/**
 * @}
 */
