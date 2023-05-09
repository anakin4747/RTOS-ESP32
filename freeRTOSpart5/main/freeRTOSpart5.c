#include <stdio.h>

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// ESP includes
#include "esp_log.h"

// Ensure only one core is used for demos
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const uint8_t msg_queue_len = 5;

static QueueHandle_t msg_queue;

void printMessages(void *parameters);

void app_main(void){
    // Wait a second to start
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

    xTaskCreatePinnedToCore(printMessages,
            "Print Messages",
            2048,
            NULL,
            1,
            NULL,
            app_cpu);

    while(1){
        static int num = 0;

        if(xQueueSend(msg_queue, (void *)&num, 10) != pdTRUE){
            ESP_LOGI("WOW", "Queue Full\n");
        }
        num++;

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void printMessages(void *parameters){
    int item;

    while(1){
        if(xQueueReceive(msg_queue, (void *)&item, 0) == pdTRUE){
            ESP_LOGI("WOW", "%d\n", item);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
