# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# Directories
STL_DIR = /home/kristina/Embedded/stm32f3_discovery/stlink-1.8.0
STM_LIB_DIR = /home/kristina/Embedded/stm32f3_discovery/STM32F3-Discovery_FW_V1.1.0
LINKER_SCRIPT_DIR = $(STM_LIB_DIR)/Project/Peripheral_Examples/FLASH_Program/TrueSTUDIO/FLASH_Program
SYSTEM_SCRIPT_DIR = $(STM_LIB_DIR)/Project/Peripheral_Examples/FLASH_Program

# Files
SOURCES =  src/main.c \
           src/drivers/gpio.c \
           src/drivers/i2c.c
SOURCES += $(STM_LIB_DIR)/Libraries/CMSIS/Device/ST/STM32F30x/Source/Templates/TrueSTUDIO/startup_stm32f30x.s \
           $(SYSTEM_SCRIPT_DIR)/system_stm32f30x.c
SOURCES += stm32f30x_rcc.c \
           stm32f30x_gpio.c \
           stm32f30x_i2c.c
OBJECTS = $(SRCS:.c=.o)
TARGET  = headlights

vpath %.c $(STM_LIB_DIR)/Libraries/STM32F30x_StdPeriph_Driver/src \

# Flags
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

# Env Variables
LD_LIBRARY_PATH=$(STL_DIR)/build/Release/lib

.PHONY: proj

# Commands
all: proj

proj: $(TARGET).elf

$(TARGET).elf: $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

clean:
	rm -f *.o $(TARGET).elf $(TARGET).hex $(TARGET).bin

flash: proj
	$(STL_DIR)/st-flash write $(TARGET).bin 0x08000000
