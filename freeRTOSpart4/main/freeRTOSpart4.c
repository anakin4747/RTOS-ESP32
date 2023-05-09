#include <stdio.h>

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ESP includes
#include "esp_log.h"

// Ensure only one core is used for demos
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


void testTask(void *parameter);

void app_main(void){

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    xTaskCreatePinnedToCore(testTask, "Task1",
            1024, NULL, 1, NULL, app_cpu);

    vTaskDelete(NULL);


}

void testTask(void *parameter){
    while(1){
        int a = 1;
        int b[100];

        for(int i = 0; i < 100; i++){
            b[i] = a + 1;
        }
        ESP_LOGI("WOW", "%d", b[0]);
    }
}


