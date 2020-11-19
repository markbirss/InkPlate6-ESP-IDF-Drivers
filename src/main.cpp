#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "logging.hpp"

#include <stdio.h>

#include "inkplate6_ctrl.hpp"
#include "Graphics.hpp"
#include "mcp.hpp"

extern "C" {

static const char * TAG = "Main";

void mainTask(void * params) 
{
  for (int i = 10; i > 0; i--) {
    printf("\r%02d ...", i);
    fflush(stdout);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  printf("\n"); fflush(stdout);

  if (e_ink.setup()) {
    ESP_LOGI(TAG, "EInk initialized!");

    static Graphics graphics;

    e_ink.clean();

    ESP_LOGI(TAG, "Set Display Mode...");
    graphics.selectDisplayMode(3);
    ESP_LOGI(TAG, "writeLine...");
    graphics.writeLine(10, 10, 200, 200, 0);
    graphics.writeFillRect(400, 400, 150, 150, 0);
    ESP_LOGI(TAG, "Show...");
    graphics.show();
  }
  
  while (1) {
    printf("Job completed...\n");
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

#define STACK_SIZE 10000

void app_main()
{
  TaskHandle_t xHandle = NULL;

  xTaskCreate(mainTask, "mainTask", STACK_SIZE, (void *) 1, tskIDLE_PRIORITY, &xHandle);
  configASSERT(xHandle);
}

} // extern "C"