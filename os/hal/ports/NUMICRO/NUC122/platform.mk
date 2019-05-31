ifeq ($(USE_SMART_BUILD),yes)
HALCONF := $(strip $(shell cat halconf.h halconf_community.h 2>/dev/null | egrep -e "define"))

# List of all the NUMICRO1x platform files.
PLATFORMSRC  = ${CHIBIOS}/os/hal/ports/common/ARMCMx/nvic.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC122/hal_st_lld.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC122/hal_lld.c

ifneq ($(findstring HAL_USE_PAL TRUE,$(HALCONF)),)
PLATFORMSRC += ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC122/hal_pal_lld.c
endif
ifneq ($(findstring HAL_USE_SERIAL TRUE,$(HALCONF)),)
PLATFORMSRC += ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC122/hal_serial_lld.c
endif
ifneq ($(findstring HAL_USE_USB TRUE,$(HALCONF)),)
PLATFORMSRC += ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC122/hal_usb_lld.c
endif
else
PLATFORMSRC  = ${CHIBIOS}/os/hal/ports/common/ARMCMx/nvic.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC122/hal_lld.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC122/hal_pal_lld.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC122/hal_serial_lld.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC122/hal_st_lld.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC122/hal_usb_lld.c
endif

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/ports/common/ARMCMx \
              ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC122 \
              ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC122


# Fetch NUC122.h from github and apply the patch.
NUC122_H = ${CHIBIOS_CONTRIB}/os/hal/ports/NUMICRO/NUC122/NUC122.h
NUC122_H_URL = https://raw.githubusercontent.com/OpenNuvoton/NUC122BSP/master/Library/Device/Nuvoton/NUC122/Include/NUC122.h
ifeq ($(filter clean,$(MAKECMDGOALS)),clean)
$(info $(shell if [ -f ${NUC122_H} ]; then rm ${NUC122_H}; fi))
else
ifeq ($(wildcard ${NUC122_H}),)
$(info fetching NUC122.h ...)
$(info $(shell wget -O ${NUC122_H} ${NUC122_H_URL}))
$(info $(shell cd $(dir ${NUC122_H}) && patch -p0 < NUC122.h.patch))
endif
endif
