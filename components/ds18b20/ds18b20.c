

#include "ds18b20.h"
#include "onewire.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "string.h"


void ds18b20_init()
{
    ESP_LOGI(DS18B20_TAG, "(%d, func: %s)", GET_LINE, __func__);
    onewire_init();
}

esp_err_t ds18b20_write_data(uint8_t* data, uint32_t gpio_pin)
{
    esp_err_t error = ESP_OK;
    error = one_wire_write_byte_to_line(data, gpio_pin);
    return error;
}

esp_err_t ds18b20_read_data(uint8_t* data, uint32_t gpio_pin)
{
    esp_err_t error = ESP_OK;
    error = one_wire_read_byte_from_line(data, gpio_pin);
    return error;
}

esp_err_t ds18b20_write_to_scratchpad(uint8_t th_val, uint8_t tl_val, uint8_t resolution, uint8_t gpio_pin)
{
    esp_err_t error = ESP_OK;
    uint8_t ds18b20_skip_rom = DS18B20_ROM_COMMAND_SKIP_ROM;
    uint8_t ds18b20_write_scratchpad = DS18B20_FUNCTION_COMMAND_WRITE_SCRATCHPAD;
    uint8_t ds18b20_th_val = th_val;
    uint8_t ds18b20_tl_vla = tl_val;
    uint8_t ds18b20_resolution = 0;

    switch (resolution)
    {
        case 9:
        {
            ds18b20_resolution = DS18B20_TEMPERATURE_9_BIT_RESOLUTION;
            break;
        }
        case 10:
        {
            ds18b20_resolution = DS18B20_TEMPERATURE_10_BIT_RESOLUTION;
            break;
        }
        case 11:
        {
            ds18b20_resolution = DS18B20_TEMPERATURE_11_BIT_RESOLUTION;
            break;
        }
        case 12:
        default:
        {
            ds18b20_resolution = DS18B20_TEMPERATURE_12_BIT_RESOLUTION;
            break;
        }
    }

    if(ds18b20_reset_line(gpio_pin))
    {
        ds18b20_write_data(&ds18b20_skip_rom, gpio_pin);
        // write to the scratch pad. (TH, TL, Configuration) = (42, -10, 12-bit resolution.)
        ds18b20_write_data(&ds18b20_write_scratchpad, gpio_pin);
        ds18b20_write_data(&ds18b20_th_val, gpio_pin);
        ds18b20_write_data(&ds18b20_tl_vla, gpio_pin);
        ds18b20_write_data(&ds18b20_resolution, gpio_pin);
    }
    else 
    {
        error = ESP_FAIL;
    }
    return error;
}


bool ds18b20_reset_line(uint32_t gpio_pin)
{
    bool present = false;
    present = one_wire_reset_line(gpio_pin);
    return present;
}

bool ds18b20_recognize_device(uint32_t gpio_pin)
{
    uint8_t read_rom_command = 0x33;
    ds18b20_write_data(&read_rom_command, gpio_pin);
    uint8_t data_from_ds18b20 = 0;
    ds18b20_read_data(&data_from_ds18b20, gpio_pin);
    return (data_from_ds18b20 == DS18B20_FAMILY_CODE) ? true : false;
}

esp_err_t ds18b20_get_temperature_data(uint8_t* temperature_data, uint32_t gpio_pin)
{
    esp_err_t error = ESP_OK;
    uint8_t ds18b20_skip_rom = DS18B20_ROM_COMMAND_SKIP_ROM;
    uint8_t ds18b20_convert_temperature = DS18B20_FUNCTION_COMMAND_CONVERT_TEMP;
    uint8_t ds18b20_read_scratchpad = DS18B20_FUNCTION_COMMAND_READ_SCRATCHPAD;
    
    uint8_t ds18b20_temperature_lsb = 0;
    uint8_t ds18b20_temperature_msb = 0;
    uint8_t ds18b20_th_value = 0;
    uint8_t ds18b20_tl_data = 0;
    uint8_t ds18b20_config_data = 0;
    uint8_t ds18b20_reserved_reg_1 = 0;
    uint8_t ds18b20_reserved_reg_2 = 0;
    uint8_t ds18b20_reserved_reg_3 = 0;
    uint8_t ds18b20_crc = 0;

    if(ds18b20_reset_line(gpio_pin))
    {
        ds18b20_write_data(&ds18b20_skip_rom, gpio_pin);
        ds18b20_write_data(&ds18b20_convert_temperature, gpio_pin);
        vTaskDelay(750 / portTICK_RATE_MS);
        ds18b20_reset_line(gpio_pin);
        ds18b20_write_data(&ds18b20_skip_rom, gpio_pin);
        ds18b20_write_data(&ds18b20_read_scratchpad, gpio_pin);

        // Now read the data;
        ds18b20_read_data(&ds18b20_temperature_lsb, gpio_pin);
        ds18b20_read_data(&ds18b20_temperature_msb, gpio_pin);
        ds18b20_read_data(&ds18b20_th_value, gpio_pin);
        ds18b20_read_data(&ds18b20_tl_data, gpio_pin);
        ds18b20_read_data(&ds18b20_config_data, gpio_pin);
        ds18b20_read_data(&ds18b20_reserved_reg_1, gpio_pin);
        ds18b20_read_data(&ds18b20_reserved_reg_2, gpio_pin);
        ds18b20_read_data(&ds18b20_reserved_reg_3, gpio_pin);
        ds18b20_read_data(&ds18b20_crc, gpio_pin);

        ESP_LOGI(DS18B20_TAG, "(%d, func: %s) The ds18b20_temperature_lsb is %d", GET_LINE, __func__, ds18b20_temperature_lsb);
        ESP_LOGI(DS18B20_TAG, "(%d, func: %s) The ds18b20_temperature_msb is %d", GET_LINE, __func__, ds18b20_temperature_msb);
        ESP_LOGI(DS18B20_TAG, "(%d, func: %s) The ds18b20_th_value is %d", GET_LINE, __func__, ds18b20_th_value);
        ESP_LOGI(DS18B20_TAG, "(%d, func: %s) The ds18b20_tl_data is %d", GET_LINE, __func__, ds18b20_tl_data);
        ESP_LOGI(DS18B20_TAG, "(%d, func: %s) The ds18b20_config_data is 0x%X", GET_LINE, __func__, ds18b20_config_data);
        ESP_LOGI(DS18B20_TAG, "(%d, func: %s) The CRC is 0x%X", GET_LINE, __func__, ds18b20_crc);

        float temp=0;
        temp=(float)(ds18b20_temperature_lsb+(ds18b20_temperature_msb*256.0))/16.0;

        ESP_LOGI(DS18B20_TAG, "(%d, func: %s) The CRC is %f", GET_LINE, __func__, temp);

    }
    else 
    {
        error = ESP_FAIL;
    }
    return error;
}
