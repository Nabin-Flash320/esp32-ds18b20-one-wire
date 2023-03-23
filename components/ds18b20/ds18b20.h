

// Technical document for DS18B20: https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf

#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "esp_err.h"
#include "stdbool.h"


#define DS18B20_TAG __FILE__
#define GET_LINE    __LINE__

#define DS18B20_TEMPERATURE_LSB     0
#define DS18B20_TEMPERATURE_HSB     1
#define DS18B20_TH_REGISTER         2
#define DS18B20_TL_REGISTER         3
#define DS18B20_CONFIG_REGISTER     4
#define DS18B20_CRC_REGISTER        8


#define DS18B20_TEMPERATURE_9_BIT_RESOLUTION        0x1F
#define DS18B20_TEMPERATURE_10_BIT_RESOLUTION       0x3F
#define DS18B20_TEMPERATURE_11_BIT_RESOLUTION       0x5F
#define DS18B20_TEMPERATURE_12_BIT_RESOLUTION       0x7F


#define DS18B20_ROM_COMMAND_SEARCH_ROM              0xF0
#define DS18B20_ROM_COMMAND_READ_ROM                0x33
#define DS18B20_ROM_COMMAND_MATCH_ROM               0x55
#define DS18B20_ROM_COMMAND_SKIP_ROM                0xCC
#define DS18B20_ROM_COMMAND_ALARM_SEARCH            0xEC


#define DS18B20_FUNCTION_COMMAND_CONVERT_TEMP           0x44
#define DS18B20_FUNCTION_COMMAND_WRITE_SCRATCHPAD       0x4E
#define DS18B20_FUNCTION_COMMAND_READ_SCRATCHPAD        0xBE
#define DS18B20_FUNCTION_COMMAND_COPY_SCRATCHPAD        0x48
#define DS18B20_FUNCTION_COMMAND_RECALL_E2              0xB8
#define DS18B20_FUNCTION_COMMAND_READ_POWER_SUPPLY      0xB4



void ds18b20_init();
esp_err_t ds18b20_write_data(uint8_t* data);
esp_err_t ds18b20_read_data(uint8_t* data);
bool ds18b20_reset_line();
esp_err_t ds18b20_set_resolution(uint8_t resolution);

#endif //_DS18B20_H_

