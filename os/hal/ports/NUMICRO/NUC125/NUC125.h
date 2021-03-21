#ifndef __NUC121_H__
#define __NUC121_H__

#define GCR_T NUC123_GCR_T
#define SPI_T NUC123_SPI_T
#include "NUC123.h"
#undef GCR_T
#undef SPI_T

#undef __HIRC
#define __HIRC 48000000

#define PE ((GPIO_T *) PE_BASE)

// #define SYS ((GCR_T *) GCR_BASE)
typedef struct
{
    __I  uint32_t PDID;          /* Offset: 0x00  Part Device Identification Number Register                         */
    __IO uint32_t RSTSRC;        /* Offset: 0x04  System Reset Source Register                                       */
    __IO uint32_t IPRSTC1;       /* Offset: 0x08  Peripheral Reset Control Register 1                                */
    __IO uint32_t IPRSTC2;       /* Offset: 0x0C  Peripheral Reset Control Register 2                                        */
    __I  uint32_t RESERVE0[2];
    __IO uint32_t BODCR;         /* Offset: 0x18  Brown-out Detector Control Register                                */
    __IO uint32_t IVSCTL;        /* Offset: 0x1C */
    __I  uint32_t RESERVE1;
    __IO uint32_t PORCR;         /* Offset: 0x24  Power-on-Reset Controller Register                                 */
    __I  uint32_t RESERVE2[2];
    __IO uint32_t RESERVE3;      /* Offset: 0x30  GPIOA Multiple Function and Input Type Control Register            */
    __IO uint32_t GPA_MFPH;      /* Offset: 0x34  GPIOB Multiple Function and Input Type Control Register            */
    __IO uint32_t GPB_MFPL;      /* Offset: 0x38  GPIOC Multiple Function and Input Type Control Register            */
    __IO uint32_t GPB_MFPH;      /* Offset: 0x3C  GPIOD Multiple Function and Input Type Control Register            */
    __IO uint32_t GPC_MFPL;
    __IO uint32_t GPC_MFPH;      /* Offset: 0x44  GPIOF Multiple Function and Input Type Control Register            */
    __IO uint32_t GPD_MFPL;
    __IO uint32_t GPD_MFPH;
    __IO uint32_t GPE_MFPL;      /* Offset: 0x50  Alternative Multiple Function Pin Control Register                 */
    __IO uint32_t RESERVE4;      /* Offset: 0x54  Alternative Multiple Function Pin Control Register 1               */
    __IO uint32_t GPF_MFPL;
    __I  uint32_t RESERVE5[9];
    __IO uint32_t IRCTCTL;       /* Offset: 0x80 */
    __IO uint32_t IRCTIEN;       /* Offset: 0x84 */
    __IO uint32_t IRCTISTS;       /* Offset: 0x88 */
    __I  uint32_t RESERVE6[13];
    __IO uint32_t MODCTL;        /* Offset: 0xC0  GPIOA IO Control Register                                          */
    __I  uint32_t RESERVE7[15];
    __IO uint32_t REGWRPROT;     /* Offset: 0x100  Register Write Protect register                                   */

} GCR_T;

typedef struct
{
    __IO uint32_t CTL;
    __IO uint32_t CLKDIV;
    __IO uint32_t SSCTL;
    __IO uint32_t PDMACTL;     

    __IO uint32_t FIFOCTL;
    __IO uint32_t STATUS;     
    __I  uint32_t RESERVE1[2];

    __O  uint32_t TX;
    __I  uint32_t RESERVE2[3];

    __I  uint32_t RX;
    __I  uint32_t RESERVE3[11];

    __IO uint32_t I2SCTL;
    __IO uint32_t I2SCLK;
    __IO uint32_t I2SSTS;

} SPI_T;

#endif
