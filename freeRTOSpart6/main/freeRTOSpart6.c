#include <stdio.h>
#include <stdlib.h>

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semaphr.h"

// ESP includes
#include "esp_log.h"

// Ensure only one core is used for demos
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static int shared_var = 0;
static SemaphoreHandle_t mutex;

void incTask(void *parameters){
    int local_var;

    while(1){
        local_var = shared_var;
        local_var++;
        vTaskDelay(rand(100, 500) / portTICK_PERIOD_MS);
        shared_var = local_var;

        ESP_LOGI("WOW", "%d", shared_var);
    }
}

void app_main(void){

    srand(3);

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    xTaskCreatePinnedToCore(incTask,
            "inc1",
            2048,
            NULL,
            1,
            NULL,
            app_cpu);

    xTaskCreatePinnedToCore(incTask,
            "inc2",
            2048,
            NULL,
            1,
            NULL,
            app_cpu);

    vTaskDelete(NULL);
}
