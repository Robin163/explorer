#!/bin/bash
#================================================================
# Generic Makefile (based on gcc)
#
# ChangeLog :
#================================================================

#-------------------------------------
# target
#-------------------------------------
TARGET_ELF              := $(TARGET).elf
TARGET_MAP              := $(TARGET).map
TARGET_BIN              := $(TARGET).bin
TARGET_HEX              := $(TARGET).hex
OBJCPFLAGS_ELF_TO_BIN    = -Obinary
OBJCPFLAGS_ELF_TO_HEX    = -Oihex
OBJCPFLAGS_HEX_TO_BIN    = -Iihex -Obinary

#-------------------------------------
# building variables
#-------------------------------------
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#-------------------------------------
# paths
#-------------------------------------
# Build path
DIR_OBJS = $(TOP)/build/objs
DIR_EXES = $(TOP)/build/exes
DIR_DEPS = $(TOP)/build/deps
DIR_LIBS = $(TOP)/build/libs
DIRS = $(DIR_OBJS)
DIR_TOOL = $(TOP)/build

ifneq ($(TARGET), )
DIRS += $(DIR_EXES)
endif

ifneq ($(CREATE_LIB), )
DIRS += $(DIR_LIBS)
endif

#-------------------------------------
# binaries
#-------------------------------------
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
LD = $(PREFIX)ld
AR = $(PREFIX)ar
CP = $(PREFIX)objcopy
GDB = $(PREFIX)gdb
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#-------------------------------------
# c files
#-------------------------------------
ifneq ($(C_INCLUDES), )
C_INCLUDES := $(strip $(C_INCLUDES))
C_INCLUDES := $(addprefix -I, $(C_INCLUDES))
endif


#-------------------------------------
# CFLAGS
#-------------------------------------
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi
FLOAT_ABI = -mfloat-abi=soft

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI) -march=armv7-m

# macros for gcc
# AS defines
AS_DEFS = $(CPU) -mthumb

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#-------------------------------------
# LDFLAGS
#-------------------------------------
# link script
LDSCRIPT = $(DIR_TOOL)/stm32_flash.ld

# libraries
LIBS = -lc -lm -lnosys

LIBDIR =
ifneq ($(LINK_LIB), )
LIBDIR := $(strip $(LINK_LIB))
LIBDIR := $(addprefix $(DIR_LIBS)/, $(LIBDIR))
#LIBDIR = $(DIR_LIBS)/$(LINK_LIB)
endif

LDFLAGS = $(MCU) -specs=nano.specs \
		-T$(LDSCRIPT) $(LIBDIR) $(LIBS) \
		-Wl,-Map=$(DIR_EXES)/$(TARGET).map,--cref -Wl,--gc-sections

#-------------------------------------
# OpenOCD specific variables
#-------------------------------------
OOCD             = openocd
OOCD_INTERFACE   = /usr/share/openocd/scripts/interface/jlink-swd.cfg
OOCD_TARGET      = /usr/share/openocd/scripts/target/stm32f1x.cfg

# Debug flags
OOCDFLAGS := -f $(OOCD_INTERFACE) -f $(OOCD_TARGET)


.PHONY: all clean debug program tar release

# default action: build all

ifneq ($(TARGET), )
all: $(DIR_EXES)/$(TARGET).elf $(DIR_EXES)/$(TARGET).hex $(DIR_EXES)/$(TARGET).bin
#all: $(TARGET)
endif

ifneq ($(CREATE_LIB), )
all: $(DIR_LIBS)/$(CREATE_LIB)
endif
#-------------------------------------
# build the application
#-------------------------------------
# list of objects
OBJECTS = $(addprefix $(DIR_OBJS)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(DIR_OBJS)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(DIR_OBJS)/%.o: %.c Makefile | $(DIR_OBJS)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(DIR_OBJS)/$(notdir $(<:.c=.lst)) $< -o $@

$(DIR_OBJS)/%.o: %.s Makefile | $(DIR_OBJS)
	$(AS) -c $(CFLAGS) $< -o $@

$(DIR_LIBS)/$(CREATE_LIB): $(OBJECTS) Makefile | $(DIR_LIBS)
	$(AR) crs $@ $(OBJECTS)

$(DIR_EXES)/$(TARGET).elf: $(OBJECTS) Makefile | $(DIR_EXES)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(DIR_EXES)/%.hex: $(DIR_EXES)/%.elf | $(DIR_EXES)
	$(HEX) $< $@

$(DIR_EXES)/%.bin: $(DIR_EXES)/%.elf | $(DIR_EXES)
	$(BIN) $< $@

$(DIRS):
	mkdir -p $@

#-------------------------------------
# clean up
#-------------------------------------
clean:
	-rm -rf $(DIRS)

#-------------------------------------
# debug
#-------------------------------------
debug:
	@printf "  GDB DEBUG $(DIR_EXES)/$(TARGET_ELF)\n"
	@ $(GDB) -iex 'target extended | $(OOCD) $(OOCDFLAGS) -c "gdb_port pipe"' \
    -iex 'monitor reset halt' -ex 'load' -ex 'break main' -ex 'c' $(DIR_EXES)/$(TARGET_ELF)

#-------------------------------------
# program(download the hex to memory)
#-------------------------------------
program:
	$(OOCD) $(OOCDFLAGS) \
	-c init -c halt -c "flash write_image erase $(DIR_EXES)/$(TARGET_HEX)" -c reset -c shutdown

#-------------------------------------
# tar
#-------------------------------------
tar:
	tar -jcvf ../backup/$(shell basename `pwd`)-`date +%Y%m%d`.tar.bz2 ./

#-------------------------------------
# release the project
#-------------------------------------
release:
	tar -jcvf ../backup/$(shell basename `pwd`)-`date +%Y%m%d`.tar.bz2 ./

#-------------------------------------
# format the source code
#-------------------------------------
style:
	sh $(TOP)/system/style.sh

#-------------------------------------
# dependencies
#-------------------------------------
-include $(wildcard $(DIR_OBJS)/*.d)

