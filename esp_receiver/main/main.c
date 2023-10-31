/* UART asynchronous reception
 * 
 * File: main.c
 * Author: antusystem <aleantunes95@gmail.com>
 * Date: 31th October 2023
 * Description: Example of how to use UART asynchronous reception.
 * This file is subject to the terms of the Apache License, Version 2.0, January 2004.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

static const int RX_BUF_SIZE = 1024;

// Change the values as needed
#define TXD_PIN GPIO_NUM_4
#define RXD_PIN GPIO_NUM_5
#define RX_UART UART_NUM_1

static const char *RX_TASK_TAG = "RX_UART";

/**
  * @brief  Initialize the UART in TX_UART
  *
  * This function initialize RX_UART.
  */
void uart_init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(RX_UART, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(RX_UART, &uart_config);
    uart_set_pin(RX_UART, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

/**
  * @brief  Rx Task
  *
  * Recieve data using the UART Rx
  */
static void rx_task(void *arg){
    // Initialize UART
    uart_init();
    // Setting the loggin level for the TAG
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    // Create the buffer in HEAP
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE + 1);
    while (1) {
        // Wait until something arrives at the buffer
        ESP_LOGI(RX_TASK_TAG, "***** Waiting buffer data *****");
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 3000 / portTICK_PERIOD_MS);
        if (rxBytes > 0) {
            // Add the last element of the data recieved
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            // Clean buffer
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
    // Free HEAP, though, the code will not reach this section
    free(data);
}

void app_main(void)
{
    xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
}
