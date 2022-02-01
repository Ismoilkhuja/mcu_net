# mcu_net
Heterogeneous network of microcontrollers

module1 - Weact Black Pill  with MCU STM32F401CCU6

module2 - Black Board       with MCU STM32F103C8T6

module3 - Arduino UNO       with MCU ATmega328P


## Connection diagram

![alt text](https://github.com/Ismoilkhuja/mcu_net/blob/main/images/connection.png)



## Implementation of a microcontroller network on debug boards

![alt text](https://github.com/Ismoilkhuja/mcu_net/blob/main/images/modified.jpg)

At the picture above:

1 - Interface UART

2 - Interface SPI

3 - Interface UART

4 - Interface I2C (TWI)

Communication starts from PC.

PC --UART--> Weact Black Pill --SPI--> Black Board --UART--> Arduino UNO --I2C--> Weact Black Pill --UART--> PC
