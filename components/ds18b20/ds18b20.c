

#include "ds18b20.h"
#include "onewire.h"
#include "esp_log.h"
#include "driver/gpio.h"




void ds18b20_init()
{
    ESP_LOGI(DS18B20_TAG, "(%d, func: %s)", GET_LINE, __func__);
    onewire_init();
}

esp_err_t ds18b20_write_data(uint8_t* data)
{
    esp_err_t error = ESP_OK;
    error = one_wire_write_byte_to_line(data, 2);
    return error;
}

esp_err_t ds18b20_read_data(uint8_t* data)
{
    esp_err_t error = ESP_OK;
    error = one_wire_read_byte_from_line(data, 2);
    return error;
}

esp_err_t ds18b20_set_resolution(uint8_t resolution)
{
    esp_err_t error = ESP_OK;
    uint8_t ds18b20_resolution = resolution;
    if(resolution < DS18B20_TEMPERATURE_9_BIT_RESOLUTION)
    {
        ds18b20_resolution = DS18B20_TEMPERATURE_9_BIT_RESOLUTION;
    }
    else if(resolution > DS18B20_TEMPERATURE_12_BIT_RESOLUTION)
    {
        ds18b20_resolution = DS18B20_TEMPERATURE_12_BIT_RESOLUTION;
    }
    return error;
}


bool ds18b20_reset_line()
{
    bool present = false;
    present = one_wire_reset_line(2);
    return present;
}




