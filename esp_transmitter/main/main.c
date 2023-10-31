/* UART asynchronous transmition
 *
 * File: main.c
 * Author: antusystem <aleantunes95@gmail.com>
 * Date: 26th October 2023
 * Description: Example of how to use UART asynchronous transmition.
 * This file is subject to the terms of the Apache License, Version 2.0, January 2004.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "esp_random.h"

static const int RX_BUF_SIZE = 1024;

// Change the values as needed
#define TXD_PIN GPIO_NUM_4
#define RXD_PIN GPIO_NUM_5
#define TX_UART UART_NUM_1

static const char *TX_TASK_TAG = "TX_TASK";

/**
  * @brief  Initialize the UART in TX_UART
  * 
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
    uart_driver_install(TX_UART, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(TX_UART, &uart_config);
    uart_set_pin(TX_UART, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

/**
  * @brief  Send data using the UART's Tx
  * 
  * @param char TAG to use in the logging
  * @param data data to send
  * @param len len of the data to send
  */
int sendData(const char* logName, const char* data, int len){
    // Write the data
    const int txBytes = uart_write_bytes(TX_UART, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

/**
  * @brief  Tx Task
  *
  * Send data using the UART Tx
  */
static void tx_task(void *arg){
    // Initialize UART
    uart_init();
    // Setting the loggin level for the TAG
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
    // Creating variables
    uint32_t random_number;
    int data_len;
    char data_to_send[sizeof(uint32_t)];
    while (1) {
        ESP_LOGI(TX_TASK_TAG, "*** Generating random number ***");
        random_number = esp_random();
        ESP_LOGI(TX_TASK_TAG, "random_number: %ld", random_number);

        // Copy value to string
        sprintf(data_to_send,"%ld", random_number);

        // Identify the number of digits
        data_len = strlen(data_to_send);
        ESP_LOGI(TX_TASK_TAG, "data_len: %d", data_len);

        // Copy the last digit
        sprintf(data_to_send,"%c", data_to_send[data_len - 1]);
        ESP_LOGI(TX_TASK_TAG, "data_to_send: %s", data_to_send);

        if (data_len > 9) {
            ESP_LOGI(TX_TASK_TAG, "Sending data");
            sendData(TX_TASK_TAG, data_to_send, data_len);
            ESP_LOGI(TX_TASK_TAG, "Data sent");
        } else {
            ESP_LOGW(TX_TASK_TAG, "Data will not be send");
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
}

