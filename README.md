# LED Headlights
The project consists of a **CAN transmitter** (hereinafter CAN TX), **CAN receivers** (hereinafter CAN RX), **buttons**, **PWM chips** and **outer LEDs**
- The **CAN transmitter** is connected to the **buttons**. There are **4** buttons to select a *mode* of the headlights. Each button corresponds to the particular mode
- The **CAN TX** sends messages to a queue, that is read by each **CAN RX**
- The **CAN RX** processes the mode from the message and provides this info to the **PWM chip**
- The **PWM chip** coordinates the **LEDs**

# Schema
<img src="./docs/e_schema.png">

# Set Up
- Download the special ```arm-none-eabi``` ```13.2.1``` toolchain for the **ARM** architecture
    ```bash
    sudo apt install gcc-arm-none-eabi gdb-arm-none-eabi
    ```
- Download ```.deb``` ```1.8.0``` release of the `stlink` [repository](https://github.com/stlink-org/stlink/releases) and install:
    ```bash
    sudo dpkg -i stlink_1.8.0-1_amd64.deb
    ```
- Check everything is installed correctly by running:
	```bash
	arm-none-eabi-gcc --version
	st-info --version
    st-flash --version
	```

# Build
- To build **elf**, **bin** and **hex** files run `make` command with the `TARGET_NAME` argument. Set `TX` value to compile code of the transmitter, `RX` - for receiver.
	```bash
	make TARGET_NAME=TX
	make TARGET_NAME=RX
	```

- To build the executable and flash it to the MCU:
	```bash
	make flash TARGET_NAME=TX
	make flash TARGET_NAME=RX
	```

# Checks
### Static Analysis
- Download `cppcheck` version `2.13.0`
- To perform code static analysis run `make check TARGET_NAME=<TX|RX>`

### Asserts
User can enable/disable asserts in the project by providing `USE_ASSERTS` building parameter. For example: `make TARGET_NAME=RX USE_ASSERTS=1`