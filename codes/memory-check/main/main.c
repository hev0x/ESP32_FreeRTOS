#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_heap_caps.h"

void MemoryView();

void app_main(void)
{
    MemoryView();

    while(1){

        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
}

void MemoryView(){

    printf("[-] ESP32 Memory Check \n");

int heapMemory = (xPortGetFreeHeapSize())/1024;
int freeBlock = (heap_caps_get_largest_free_block(MALLOC_CAP_8BIT))/1024;
int DRamMemory = (heap_caps_get_free_size(MALLOC_CAP_8BIT))/1024;
int IRamMemory = (heap_caps_get_free_size(MALLOC_CAP_32BIT))/1024;

    printf("[-] Getting ESP32 memory blocks info... \n\n ");
    printf("[+] Heap: %d kB\n ", heapMemory);
    printf("[+] DRam: %d kB\n ", DRamMemory);
    printf("[+] IRam: %d kB\n ", IRamMemory);
    printf("[+] Maximum Free Block %d kB\n ", freeBlock);

}