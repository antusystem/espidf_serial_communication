# UART Transmitter

This example shows how to use UART asynchronous transmition to send a random number every 5 seconds.


**Note:** This examples might not obtain an actual random number, you can read more about in the [docs](https://docs.espressif.com/projects/esp-idf/en/v5.1.1/esp32/api-reference/system/random.html).

## How to use example

### Configure the project

You can change the values from `TXD_PIN`, `RXD_PIN`, and `TX_UART` in `main.c` to the indicate the pins to use and the UART to use.


## Hardware Required

You need a ESP32 basic Devboard, this was tested with a NodeMCU.

### Pin Assignment:


| ESP32  | DESCRIPTION |
| ------ | ----------- |
| GPIO4  | UART TX     |
| GPIO5  | UART RX     |

## Log

* Last compile: October 30t 2023.
* Last test: October 30t 2023.
* Last compile espidf version: `v 5.1.1`.
* VSCode ESP-IDF Visual Studio Code Extension `v 1.6.5`.


## License

Apache License, Version 2.0, January 2004.

## Version

`v 1.0.0`
