#ifndef __FMC_MIN_H__
#define __FMC_MIN_H__

#include "NUC123.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FMC_CONFIG_BASE 0x00300000

static __INLINE void FMC_Write(uint32_t adr, uint32_t dat) {
    FMC->ISPCMD = 0x21;
    FMC->ISPADR = adr;
    FMC->ISPDAT = dat;
    FMC->ISPTRG = 0x01;
    __ISB();
    while (FMC->ISPTRG);
}

static __INLINE uint32_t FMC_Read(uint32_t adr) {
    FMC->ISPCMD = 0x00;
    FMC->ISPADR = adr;
    FMC->ISPTRG = 0x01;
    __ISB();
    while (FMC->ISPTRG);

    return FMC->ISPDAT;
}

static __INLINE int32_t FMC_Erase(uint32_t adr) {
    FMC->ISPCMD = 0x22;
    FMC->ISPADR = adr;
    FMC->ISPTRG = 0x01;
    __ISB();
    while (FMC->ISPTRG);

    if (FMC->ISPCON & FMC_ISPCON_ISPFF_Msk) {
        FMC->ISPCON |= FMC_ISPCON_ISPFF_Msk;
        return -1;
    }
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif
