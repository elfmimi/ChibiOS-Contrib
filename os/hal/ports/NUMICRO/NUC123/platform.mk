ifeq ($(USE_SMART_BUILD),yes)
HALCONF := $(strip $(shell cat halconf.h halconf_community.h 2>/dev/null | egrep -e "define"))

# List of all the NUMICRO1x platform files.
PLATFORMSRC  = ${CHIBIOS}/os/hal/ports/common/ARMCMx/nvic.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123/hal_st_lld.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123/hal_lld.c

ifneq ($(findstring HAL_USE_PAL TRUE,$(HALCONF)),)
PLATFORMSRC += ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123/hal_pal_lld.c
endif
ifneq ($(findstring HAL_USE_SERIAL TRUE,$(HALCONF)),)
PLATFORMSRC += ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123/hal_serial_lld.c
endif
ifneq ($(findstring HAL_USE_I2C TRUE,$(HALCONF)),)
PLATFORMSRC += ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123/hal_i2c_lld.c
endif
ifneq ($(findstring HAL_USE_USB TRUE,$(HALCONF)),)
PLATFORMSRC += ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123/hal_usb_lld.c
endif
else
PLATFORMSRC  = ${CHIBIOS}/os/hal/ports/common/ARMCMx/nvic.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123/hal_lld.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123/hal_pal_lld.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123/hal_serial_lld.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123/hal_i2c_lld.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123/hal_st_lld.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123/hal_usb_lld.c
endif

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/ports/common/ARMCMx \
              ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123


# Fetch NUC123.h from github and apply the patch.
NUC123_H = ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC123/NUC123.h
NUC123_H_URL = https://raw.githubusercontent.com/OpenNuvoton/NUC123BSP/master/Library/Device/Nuvoton/NUC123/Include/NUC123.h
ifeq ($(filter clean,$(MAKECMDGOALS)),clean)
$(info $(shell if [ -f ${NUC123_H} ]; then rm ${NUC123_H}; fi))
else
ifeq ($(wildcard ${NUC123_H}),)
$(info fetching NUC123.h ...)
$(info $(shell wget -O ${NUC123_H} ${NUC123_H_URL}))
$(info $(shell cd $(dir ${NUC123_H}) && patch --binary -p0 < NUC123.h.patch))
endif
endif
