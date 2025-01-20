# CHECK ARGS
ifndef TARGET_NAME
$(error "Must pass TARGET_NAME=TX or TARGET_NAME=RX")
endif

# TOOLCHAIN
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# DIRECTORIES
LINKER_SCRIPT_DIR = $(STM_LIB_DIR)/Project/Peripheral_Examples/FLASH_Program/TrueSTUDIO/FLASH_Program
SYSTEM_SCRIPT_DIR = $(STM_LIB_DIR)/Project/Peripheral_Examples/FLASH_Program
BUILD_DIR = build

ifndef STL_DIR
$(error "STL_DIR must be specified")
endif

ifndef STM_LIB_DIR
$(error "STM_LIB_DIR must be specified")
endif

# FILES
ifeq ($(TARGET_NAME),TX) 
TARGET = $(BUILD_DIR)/tx
SOURCES = src/tx/main.c \
          src/drivers/exti.c \
          src/drivers/buttons.c \
          stm32f30x_exti.c \
          stm32f30x_syscfg.c

else ifeq ($(TARGET_NAME),RX)
TARGET = $(BUILD_DIR)/rx
SOURCES = src/rx/main.c \
          src/drivers/pca9685.c \
          src/app/state_machine.c
endif

SOURCES += src/drivers/gpio.c \
           src/drivers/can.c \
           src/drivers/i2c.c \
           src/drivers/stm32f30x_it.c \
           src/common/assert.c

SOURCES += $(STM_LIB_DIR)/Libraries/CMSIS/Device/ST/STM32F30x/Source/Templates/TrueSTUDIO/startup_stm32f30x.s \
           $(SYSTEM_SCRIPT_DIR)/system_stm32f30x.c

SOURCES += stm32f30x_rcc.c \
           stm32f30x_gpio.c \
           stm32f30x_i2c.c \
           stm32f30x_can.c \
           stm32f30x_misc.c 

OBJECTS = $(SRCS:.c=.o)

vpath %.c $(STM_LIB_DIR)/Libraries/STM32F30x_StdPeriph_Driver/src \

# FLAGS
CFLAGS  = --specs=nosys.specs
CFLAGS += -g -O2 -Wall -T$(LINKER_SCRIPT_DIR)/STM32_FLASH.ld
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -I.
CFLAGS += -I$(STM_LIB_DIR)/Libraries/CMSIS/Include
CFLAGS += -I$(STM_LIB_DIR)/Libraries/CMSIS/Device/ST/STM32F30x/Include
CFLAGS += -I$(STM_LIB_DIR)/Libraries/STM32F30x_StdPeriph_Driver/inc
CFLAGS += -I$(STM_LIB_DIR)/Utilities/STM32F3_Discovery
CFLAGS += -I$(STM_LIB_DIR)/Project/Demonstration
CFLAGS += -Isrc
CFLAGS += -D$(TARGET_NAME) # global define to access it from the multiple compilation units

ifeq ($(USE_ASSERTS), 1)
    CFLAGS += -DUSE_ASSERTS
endif

# ENV VARIABLES
# Path to a dynamic ST-Link library
LD_LIBRARY_PATH=$(STL_DIR)/build/Release/lib

# PHONIES
.PHONY: all clean flash check

# COMMANDS
all: $(TARGET).elf

$(TARGET).elf: $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

clean:
	rm -f *.o $(TARGET).elf $(TARGET).hex $(TARGET).bin

flash: $(TARGET).elf
	$(STL_DIR)/st-flash write $(TARGET).bin 0x08000000

check:
	cppcheck \
        --enable=all \
        --force \
        --std=c11 \
        -I./src \
        -I./src/drivers \
        -I./src/app \
        -I./src/common \
        -DSTM32F30x \
        -D__weak= \
        -D__IO= \
        ./src
