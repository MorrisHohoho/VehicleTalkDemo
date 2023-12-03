# Vehicle Talk GD32 Version 1.0

## Hardware
1. MCU: GD32F450VKT6.
2. Smart Model Car: WheelTec L130.
3. LED and its transmitter with 200KHz or above.
4. PD Receiver.

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