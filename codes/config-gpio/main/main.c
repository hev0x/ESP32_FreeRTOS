#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h" 
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

#define PIN 2


void app_main(void)
{
    bool status = 0;
    esp_rom_gpio_pad_select_gpio(PIN);
    gpio_set_direction(PIN,GPIO_MODE_OUTPUT);

    while(1)
    {
        status = !status;
        gpio_set_level(PIN,status);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    

}