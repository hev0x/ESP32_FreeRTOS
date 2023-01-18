# ESP32 - FreeRTOS Projects



## Memory Types

• ESP32 Memory Blocks (Dynamic IRAM-DRAM Split)

![1 J4W46UFrCKbUv_JUSo6QeQ](https://user-images.githubusercontent.com/6265911/213051235-e7638b0e-471d-4f3a-9f68-63b72221324f.png)


### IRAM (Instruction RAM)

![3 J4W46UFrCKbUv_JUSo6QeQ](https://user-images.githubusercontent.com/6265911/213053715-0454d1a9-6a73-4eee-9abf-24d57870c4df.png)

```
• ESP-IDF allocates part of the Internal SRAM0 region for instruction RAM. 
The region is defined in ESP32 Technical Reference Manual > System and Memory > Embedded Memory 
```

### DRAM (Data RAM)

![2 J4W46UFrCKbUv_JUSo6QeQ](https://user-images.githubusercontent.com/6265911/213053708-ff7d52b5-4873-4006-9508-1b86a14fb9f3.png)

```
• Non-constant static data (.data) and zero-initialized data (.bss) is placed by the linker into Internal SRAM as data memory. 
The remaining space in this region is used for the runtime heap.
```


# References

• https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/memory-types.html
