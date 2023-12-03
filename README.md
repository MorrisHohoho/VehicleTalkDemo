# Vehicle Talk GD32 Version 1.0

## Hardware
1. MCU: GD32F450VKT6.
2. Smart Model Car: WheelTec L130.
3. LED and its transmitter with 200KHz or above.
4. PD Receiver.

## Software
STM32 Cube MX + OpenOcd
### Remember to modify the CMakeLists
Every time you change the ```ioc```, remember to overwrite the ```CMakeLists``` with ```RealCMakeLists```
### OpenOcd configuration
1. Copy the `GD32F450.cfg` to the openocd `target` directory.  
2. Use the following `cfg` in the project, which I have already put in the project.
```
# Choose the adaptor configuration. I use stlink-v2, which is cheap and easy to get.
source [find interface/stlink-v2.cfg]

# How to upload. In most case, it is swd.
transport select hla_swd

# increase working area to 128KB. Default is good.
set WORKAREASIZE 0x20000

# Choose the target board configuration. The configuration file is not included in the official
# openocd project. It is developed by others.
source [find target/gd32f450.cfg]

# Default is good.
reset_config srst_only
```

## IO Usages
1. UART4 TX/RX: Used for VLC.
2. UART1 TX/RX: For CH340 TTL. Used for output debuging logs.
3. PB15 PWM: Used for controlling the servo.
4. 2 PWMs + 4 IOs: Used for controlling the motor module TB6612.

## VLC System
* **Warning: GD32 doesn't support UART and GPIO interrupt at the same pin at the same time.**

So, I chose to use `UART IDLE Interrupt` to handle this situation.

### Tx Side

* **Data Frame Structure:**  `UART IDLE` + 0xFF ( Header ) + PSN ( 1 byte ) + data ( 59 bytes ) + `UART IDLE` + 0x00 ( Tailer )

  * `UART IDLE`: Flush the buffer
  * **Header:**  Detect the VLC data packet.
  * **Tailer:**  Stop DMA receiving and gives the MCU enough time to handle the data.
* **Idle Signals:**  10 bytes 0xAA and 2 bytes "1111"

### Rx Side

Use `Uart_Idle_Interrupt` to implement receiving a physical frame. Only when the UART receives more than **Frame Length** data, the MCU starts to measure the data.

### Coding Scheme

Currently, only `manchester coding` is used.