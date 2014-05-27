#
# File: Makefile
#
# Copyright (c) 2013, 0xc0170
#
# This program is free software. It comes without any warranty, to
# the extent permitted by applicable law. You can redistribute it
# and/or modify it under the terms of the Do What The Fuck You Want
# To Public License, Version 2, as published by Sam Hocevar. See
# http://www.wtfpl.net/ for more details.


# toolchain specific
TOOLCHAIN = arm-none-eabi-
CC = $(TOOLCHAIN)gcc
CXX = $(TOOLCHAIN)g++
AS = $(TOOLCHAIN)gcc -x assembler-with-cpp
LD = $(TOOLCHAIN)gcc
OBJCP = $(TOOLCHAIN)objcopy
AR = $(TOOLCHAIN)ar
GDB = $(TOOLCHAIN)gdb
OBJDUMP = $(TOOLCHAIN)objdump

# application specific
CPU := cortex-m4
INSTRUCTION_MODE := thumb
TARGET := main
TARGET_EXT := elf
LD_SCRIPT := ../../lib/link/k60n512_flash.ld

LIB := 

LIBS := ../../lib

# directories
INC_DIRS = . $(LIBS)/common $(LIBS)/cpu $(LIBS)/cpu/headers $(LIBS)/. $(LIBS)/hw
SRC_DIRS := . $(LIBS)/cpu $(LIBS)/. $(LIBS)/hw $(LIBS)/common
OUT_DIR := out
INC_DIRS_F = -I. $(patsubst %, -I%, $(INC_DIRS))

# add traling / if not empty
ifeq ($(strip $(OUT_DIR)), )
	OBJ_FOLDER =
else
	OBJ_FOLDER = $(strip $(OUT_DIR))/
endif

COMPILER_OPTIONS =	-O0 -g -ggdb -Wall -fno-strict-aliasing \
										-fmessage-length=0 -fno-builtin -m$(INSTRUCTION_MODE) \
										-mcpu=$(CPU) -MD -MP

DEPEND_OPTS = -MF $(OBJ_FOLDER)$(@F:.o=.d)

# Flags
CFLAGS = $(COMPILER_OPTIONS) $(DEPEND_OPTS) $(INC_DIRS_F) -c
CXXFLAGS = $(COMPILER_OPTIONS) $(INC_DIRS_F) -c
ASFLAGS = $(COMPILER_OPTIONS) $(INC_DIRS_F) -c
OBJCPFLAGS = -O ihex
ARFLAGS = cr

# Linker options
LD_OPTIONS = -Wl,-Map=$(OBJ_FOLDER)$(TARGET).map $(COMPILER_OPTIONS) -L ../ -T $(LD_SCRIPT) $(INC_DIRS_F)
LD_OPTIONS += -nostartfiles

RM := rm -rf

USER_OBJS :=
C_SRCS :=
S_SRCS :=
C_OBJS :=
S_OBJS :=

# All source/object files inside SRC_DIRS
C_SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
C_OBJS := $(patsubst %.c,$(OBJ_FOLDER)%.o,$(notdir $(C_SRCS)))
S_SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))
S_OBJS := $(patsubst %.c,$(OBJ_FOLDER)%.o,$(notdir $(S_SRCS)))

VPATH := $(SRC_DIRS)

$(OBJ_FOLDER)%.o : %.c
	@echo ‘Building file: $(@F)’
	@echo ‘Invoking: MCU C Compiler’
	$(CC) $(CFLAGS) $< -o $@
	@echo 'Finished building: $(@F)'
	@echo ' '

$(OBJ_FOLDER)%.o : %.s
	@echo 'Building file: $(@F)'
	@echo 'Invoking: MCU Assembler'
	$(AS) $(ASFLAGS) $< -o $@
	@echo 'Finished building: $(@F)'
	@echo ' '

all: create_outputdir $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT) print_size

create_outputdir:
	$(shell mkdir $(OBJ_FOLDER) 2>/dev/null)

# Tool invocations
$(OBJ_FOLDER)$(TARGET).$(TARGET_EXT): $(C_OBJS) $(S_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	$(LD) $(LD_OPTIONS) $(C_OBJS) $(S_OBJS) $(USER_OBJS) $(LIB) -o $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT)
	@echo 'Finished building target: $@'
	${OBJDUMP} -DS $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT) >| $(OBJ_FOLDER)$(TARGET).out.s
	@echo ' '

# Other Targets
clean:
	@echo 'Removing entire out directory'
	$(RM) $(TARGET).$(TARGET_EXT) $(TARGET).bin $(TARGET).map $(OBJ_FOLDER)*.* $(OBJ_FOLDER)
	@echo ' '

print_size:
	@echo 'Printing size'
	arm-none-eabi-size --totals $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT); arm-none-eabi-objcopy -O binary $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT) $(OBJ_FOLDER)$(TARGET).bin ;
	@echo ' '

gdb:
	@echo
	@echo Launching GDB...
	@echo
	${GDB} --eval-command="target remote localhost:2331" $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT)
.PHONY: all clean dependents print_size
