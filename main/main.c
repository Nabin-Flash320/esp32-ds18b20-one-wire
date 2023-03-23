
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "ds18b20.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/rom/ets_sys.h"
#elif CONFIG_IDF_TARGET_ESP32
#include "esp32/rom/ets_sys.h"
#endif

#define DEMO_PIN 2

void reset_task(void* params)
{
    uint8_t presence = 0;
    
    while (1)
    {
        gpio_set_direction(DEMO_PIN, GPIO_MODE_OUTPUT);
        gpio_set_pull_mode(DEMO_PIN, GPIO_FLOATING);
        gpio_set_level(DEMO_PIN, 0);
        ets_delay_us(480);
        gpio_set_level(DEMO_PIN, 1); 
        gpio_set_direction(DEMO_PIN, GPIO_MODE_INPUT);
        ets_delay_us(70); 
        presence = gpio_get_level(DEMO_PIN);
        ets_delay_us(410);  
        if(presence == 0)
        {
            printf("Present.\n");
        }  
        else 
        {
            printf("Not present\n");
        }   
    }
    
}

void app_main()
{
    while (1)
    {
        if(ds18b20_reset_line())
        {
            printf("Present.\n");
        }  
        else 
        {
            printf("Not present\n");
        } 
    }
    
    // xTaskCreate(reset_task, "reset_task", 2*2048, NULL, 0, NULL);
    
}
