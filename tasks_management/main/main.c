#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_chip_info.h"
#include "esp_flash.h"


void hardware_info(void *pvParameter)
{
    printf("Hi, here is some info about hardware: \n");
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%lu %s flash\n", flash_size / (1024 * 1024), (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %ld bytes\n\n", esp_get_minimum_free_heap_size());

    vTaskDelete(NULL);
}

void vtask_blink(void *pvParameter)
{
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

    printf("Calling blink led task...\n");
      
    int uiCounter = 0;

    for (;;)
    {
        gpio_set_level(GPIO_NUM_2, (uiCounter % 2));
        vTaskDelay(1000/portTICK_PERIOD_MS);

        uiCounter++;
    }
}

void app_main(void)
{
    xTaskCreatePinnedToCore(hardware_info,"hardware_info",4098,NULL,2,NULL,0);
    xTaskCreatePinnedToCore(vtask_blink,"vtask_blink",1024,NULL,2,NULL,0);
    printf("Created blink led task...\n");

}



