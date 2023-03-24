
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
    do
    {
        if(ds18b20_reset_line(DEMO_PIN))
        {
            // printf("Present.\n");
            if(ds18b20_recognize_device(DEMO_PIN))
            {
                printf("DS18B20 Found!!\n");
                uint8_t temp_data = 0;
                ds18b20_write_to_scratchpad(DS18B20_TH_HIGHER_THRESHOLD, DS18B20_TL_LOWER_THRESHOLD, 12, DEMO_PIN);
                ds18b20_get_temperature_data(&temp_data, DEMO_PIN);
            }
            else 
            {
                printf("No DS18B20 device found!!");
            }
        }  
        else 
        {
            printf("Not present\n");
        } 
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }while (1);
    
    // xTaskCreate(reset_task, "reset_task", 2*2048, NULL, 0, NULL);
    
}
