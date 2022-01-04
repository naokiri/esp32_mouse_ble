/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "ble_hid_component.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <stdio.h>

#define CONSOLE_UART_NUM 0
#define UART_TAG "UART"

hid_control_t control;

void hid_demo_task(void *parameters) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        if (control.is_notifiable || control.is_indicatable) {
            send_mouse_event(&control, 0xde, 0xed, 0xbe, 0xef);
        }
    }
}

void uart_console_task(void *pvParameters) {
    char character;
    // Install UART driver, and get the queue.
    uart_driver_install(CONSOLE_UART_NUM, UART_FIFO_LEN * 2, UART_FIFO_LEN * 2,
                        0, NULL, 0);

    ESP_LOGI(UART_TAG, "console UART processing task started");

    while (1) {
        // read single byte
        uart_read_bytes(CONSOLE_UART_NUM, (uint8_t *)&character, 1,
                        portMAX_DELAY);

        uint8_t button = 0;
        int8_t x = 0, y = 0, wheel = 0;
        switch (character) {
        case 'a':
            x = -20;
            ESP_LOGI(UART_TAG, "mouse: a");
            break;
        case 's':
            y = 20;
            ESP_LOGI(UART_TAG, "mouse: s");
            break;
        case 'd':
            x = 20;
            ESP_LOGI(UART_TAG, "mouse: d");
            break;
        case 'w':
            y = -20;
            ESP_LOGI(UART_TAG, "mouse: w");
            break;
        default:
            ESP_LOGI(UART_TAG, "received: %d, no HID action", character);
            break;
        }
        if (control.is_notifiable || control.is_indicatable) {
            send_mouse_event(&control, button, x, y, wheel);
        }
    }
}

void app_main(void) {
    printf("Hello world!\n");
    memset(&control, 0, sizeof control);

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ", CONFIG_IDF_TARGET,
           chip_info.cores, (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded"
                                                         : "external");

    printf("Minimum free heap size: %d bytes\n",
           esp_get_minimum_free_heap_size());

    fflush(stdout);

    init_ble_hid(&control);
    xTaskCreate(&uart_console_task, "uart_console_task", 4096, NULL, 10, NULL);
}
