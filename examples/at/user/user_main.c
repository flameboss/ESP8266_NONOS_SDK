/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "osapi.h"
#include "at_custom.h"
#include "user_interface.h"

#if ((SPI_FLASH_SIZE_MAP == 0) || (SPI_FLASH_SIZE_MAP == 1))
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 2)
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 3)
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 4)
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 5)
#define SYSTEM_PARTITION_OTA_SIZE							0xE0000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
#define SYSTEM_PARTITION_AT_PARAMETER_ADDR					0xfd000
#define SYSTEM_PARTITION_SSL_CLIENT_CERT_PRIVKEY_ADDR		0xfc000
#define SYSTEM_PARTITION_SSL_CLIENT_CA_ADDR					0xfb000
#define SYSTEM_PARTITION_WPA2_ENTERPRISE_CERT_PRIVKEY_ADDR	0xfa000
#define SYSTEM_PARTITION_WPA2_ENTERPRISE_CA_ADDR			0xf9000
#elif (SPI_FLASH_SIZE_MAP == 6)
#define SYSTEM_PARTITION_OTA_SIZE							0xE0000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#define SYSTEM_PARTITION_AT_PARAMETER_ADDR					0xfd000
#define SYSTEM_PARTITION_SSL_CLIENT_CERT_PRIVKEY_ADDR		0xfc000
#define SYSTEM_PARTITION_SSL_CLIENT_CA_ADDR					0xfb000
#define SYSTEM_PARTITION_WPA2_ENTERPRISE_CERT_PRIVKEY_ADDR	0xfa000
#define SYSTEM_PARTITION_WPA2_ENTERPRISE_CA_ADDR			0xf9000
#else
#error "The flash map is not supported"
#endif

#ifdef CONFIG_ENABLE_IRAM_MEMORY
uint32 user_iram_memory_is_enabled(void)
{
    return CONFIG_ENABLE_IRAM_MEMORY;
}
#endif

#define FLAMEBOSS_VERSION "FLAMEBOSS_VER: 3.1.2\r\n"

void ICACHE_FLASH_ATTR
at_setupCmdFbVer(uint8_t id, char *pPara)
{
    at_port_print(FLAMEBOSS_VERSION);
    at_response_ok();
}

void ICACHE_FLASH_ATTR
at_testCmdFbVer(uint8_t id)
{
    at_port_print(FLAMEBOSS_VERSION);
    at_response_ok();
}

void ICACHE_FLASH_ATTR
at_queryCmdFbVer(uint8_t id)
{
    at_port_print(FLAMEBOSS_VERSION);
    at_response_ok();
}

void ICACHE_FLASH_ATTR
at_exeCmdFbVer(uint8_t id)
{
    at_port_print(FLAMEBOSS_VERSION);
    at_response_ok();
}

#ifdef AT_UPGRADE_SUPPORT
extern void at_exeCmdCiupdate(uint8_t id);
#endif

at_funcationType at_custom_cmd[] = {
    {"+FBVER", 6, at_testCmdFbVer, at_queryCmdFbVer, at_setupCmdFbVer, at_exeCmdFbVer},
#ifdef AT_UPGRADE_SUPPORT
    {"+CIUPDATE", 9,       NULL,            NULL,            NULL, at_exeCmdCiupdate}
#endif
};

static const partition_item_t at_partition_table[] = {
    { SYSTEM_PARTITION_BOOTLOADER, 						0x0, 												0x1000},
    { SYSTEM_PARTITION_OTA_1,   						0x1000, 											SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_OTA_2,   						SYSTEM_PARTITION_OTA_2_ADDR, 						SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_RF_CAL,  						SYSTEM_PARTITION_RF_CAL_ADDR, 						0x1000},
    { SYSTEM_PARTITION_PHY_DATA, 						SYSTEM_PARTITION_PHY_DATA_ADDR, 					0x1000},
    { SYSTEM_PARTITION_SYSTEM_PARAMETER, 				SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 			0x3000},
    { SYSTEM_PARTITION_AT_PARAMETER, 					SYSTEM_PARTITION_AT_PARAMETER_ADDR, 				0x3000},
	{ SYSTEM_PARTITION_SSL_CLIENT_CERT_PRIVKEY, 		SYSTEM_PARTITION_SSL_CLIENT_CERT_PRIVKEY_ADDR, 		0x1000},
	{ SYSTEM_PARTITION_SSL_CLIENT_CA, 					SYSTEM_PARTITION_SSL_CLIENT_CA_ADDR, 				0x1000},
#ifdef CONFIG_AT_WPA2_ENTERPRISE_COMMAND_ENABLE
	{ SYSTEM_PARTITION_WPA2_ENTERPRISE_CERT_PRIVKEY, 	SYSTEM_PARTITION_WPA2_ENTERPRISE_CERT_PRIVKEY_ADDR,	0x1000},
    { SYSTEM_PARTITION_WPA2_ENTERPRISE_CA, 				SYSTEM_PARTITION_WPA2_ENTERPRISE_CA_ADDR, 			0x1000},
#endif
};

void ICACHE_FLASH_ATTR user_pre_init(void)
{
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
	}
}

void ICACHE_FLASH_ATTR
user_init(void)
{
    char buf[128] = {0};
    at_customLinkMax = 5;
    at_init();
#ifdef ESP_AT_FW_VERSION
    if ((ESP_AT_FW_VERSION != NULL) && (os_strlen(ESP_AT_FW_VERSION) < 64)) {
        os_sprintf(buf,"compile time:"__DATE__" "__TIME__"\r\n"ESP_AT_FW_VERSION);
    } else {
        os_sprintf(buf,"compile time:"__DATE__" "__TIME__);
    }
#else
    os_sprintf(buf,"compile time:"__DATE__" "__TIME__);
#endif
    at_set_custom_info(buf);
    at_port_print_irom_str("\r\nready\r\n");
    at_cmd_array_regist(&at_custom_cmd[0], sizeof(at_custom_cmd)/sizeof(at_custom_cmd[0]));
#ifdef CONFIG_AT_SMARTCONFIG_COMMAND_ENABLE
    at_cmd_enable_smartconfig();
#endif
#ifdef CONFIG_AT_WPA2_ENTERPRISE_COMMAND_ENABLE
    at_cmd_enable_wpa2_enterprise();
#endif
}
