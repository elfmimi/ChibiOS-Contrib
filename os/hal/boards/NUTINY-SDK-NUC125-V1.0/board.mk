# List of all the board related files.
BOARDSRC = $(CHIBIOS_CONTRIB)/os/hal/boards/NUTINY-SDK-NUC125-V1.0/board.c

# Required include directories
BOARDINC = $(CHIBIOS_CONTRIB)/os/hal/boards/NUTINY-SDK-NUC125-V1.0

LDSCRIPT= $(STARTUPLD_CONTRIB)/NUC125xC2xx.ld

ALLCSRC += $(BOARDSRC)
ALLINC += $(BOARDINC)
