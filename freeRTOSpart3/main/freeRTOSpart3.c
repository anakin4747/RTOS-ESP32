// Couldnt get this to demo properly, I think I'm having issues with setting
// the baud rate of the communication or flushing the buffer to print out the
// message



#include <stdio.h>
#include <string.h>

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


// Built-in LED
#define LED 2

const char msg[] = "I am your Father";

// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;


void startTask1(void *parameter);
void startTask2(void *parameter);

void app_main(void){

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    xTaskCreatePinnedToCore(startTask1, "Task1",
            2048, NULL, 1, &task_1, app_cpu);
    xTaskCreatePinnedToCore(startTask2, "Task2",
            2048, NULL, 1, &task_2, app_cpu);

    while(1){
        for(int i = 0; i < 3; i++){
            vTaskSuspend(task_2);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            vTaskResume(task_2);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        if(task_1 != NULL){
            vTaskDelete(task_1);
            task_1 = NULL;
        }
    }
}

void startTask1(void *parameter){
    
    char *task1Name = pcTaskGetName(NULL);
    int msg_len = strlen(msg);

    while(1){
        ESP_LOGI(task1Name, "\n");
        for(int i = 0; i < msg_len; i++){
            ESP_LOGI(task1Name, "%c", msg[i]);
        }
        ESP_LOGI(task1Name, "\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}

void startTask2(void *parameter){
    char *task2Name = pcTaskGetName(NULL);

    while(1){
        ESP_LOGI(task2Name, "*");
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

