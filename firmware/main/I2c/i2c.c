/*
 * Copyright (c) 2014, Stephen Robinson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_log.h"

#include "i2c.h"
#include "include/i2c.h"

const static char *I2C_LOG_TAG = "I2C_HANDLER";
const char * I2C_EVENT = "wifi_event";
static int s_retry_num = 0;

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_init()
{
    int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;
    conf.sda_pullup_en = 1;
    conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;
    conf.scl_pullup_en = 1;
    conf.clk_stretch_tick = 100000; // 300 ticks, Clock stretch is about 210us, you can make changes according to the actual situation.
    ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, conf.mode));
    ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));
    return ESP_OK;
}

esp_err_t i2c_slave_write(char *inputString)
{   
    ESP_LOGI(I2C_LOG_TAG, " Writing : %s", inputString);
    size_t size = strlen(inputString);
    int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, MPU6050_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, (uint8_t *)inputString, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK)
    {
        return ret;
    }

    return ret;
}

esp_err_t i2c_slave_read(char *data)
{
    ESP_LOGI(I2C_LOG_TAG, " Reading : %s", data);
    int ret;
    char incomingDataBuffer[MAX_SLAVE_RESPONSE_BUFFER];
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, MPU6050_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read(cmd, (uint8_t *)incomingDataBuffer, sizeof(incomingDataBuffer) - 1, LAST_NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    incomingDataBuffer[sizeof(incomingDataBuffer)] = 0; // Null-terminate whatever we received and treat like a string...
    ESP_LOGD(I2C_LOG_TAG, " incomingDataBuffer: %s", incomingDataBuffer);
    int end = 0;
    while (end < MAX_SLAVE_RESPONSE_BUFFER)
    {
        if (incomingDataBuffer[end] == 255)
        {
            break;
        }
        end++;
    }
    memcpy(data, incomingDataBuffer, end);
    return ret;
}

/**
 * Compares \p len bytes from @p a with @p b in constant time. This
 * functions always traverses the entire length to prevent timing
 * attacks.
 *
 * \param a Byte sequence to compare
 * \param b Byte sequence to compare
 * \param len Number of bytes to compare.
 * \return \c 1 if \p a and \p b are equal, \c 0 otherwise.
 */
int equals(char *a, char *b, size_t len)
{
    int result = 1;
    while (len--)
    {
        result &= (*a++ == *b++);
    }
    return result;
}

void i2c_task_example(void *arg)
{
    vTaskDelay(10000 / portTICK_RATE_MS);
    //i2c_example_master_init();
    vTaskDelay(5000 / portTICK_RATE_MS);
    i2c_slave_write("RSLAVENAME");
    vTaskDelay(200 / portTICK_RATE_MS);

    char data[MAX_SLAVE_RESPONSE_BUFFER];
    i2c_slave_read(&data);
    ESP_LOGI(I2C_LOG_TAG, " data : %s", data);

    i2c_slave_write("RRELAY");
    vTaskDelay(200 / portTICK_RATE_MS);

    i2c_slave_read(&data);
    ESP_LOGI(I2C_LOG_TAG, " data : %s", data);

    char compare[12] = "MASLAVE4R4B";
    if (equals(&data, &compare, sizeof(compare)))
    {
        ESP_LOGI(I2C_LOG_TAG, "Matched");
    }

    i2c_slave_write("RRELAYNUMBER");
    vTaskDelay(200 / portTICK_RATE_MS);

    i2c_slave_read(&data);
    ESP_LOGI(I2C_LOG_TAG, " data : %s", data);

    i2c_slave_write("WRELAY=0000"); 
    vTaskDelay(200 / portTICK_RATE_MS);
    i2c_slave_write("WRELAY=1000");
    vTaskDelay(200 / portTICK_RATE_MS);
    i2c_slave_write("WRELAY=1100");
    vTaskDelay(200 / portTICK_RATE_MS);
    i2c_slave_write("WRELAY=1110");
    vTaskDelay(200 / portTICK_RATE_MS);
    i2c_slave_write("WRELAY=1111");
    while (1)
    {
        // i2c_example_master_mpu6050_custom(I2C_EXAMPLE_MASTER_NUM);
        ESP_LOGI(I2C_LOG_TAG, " Hello");vTaskDelay(10000 / portTICK_RATE_MS);
    }
    i2c_driver_delete(I2C_EXAMPLE_MASTER_NUM);
}

void i2c_master_init(void)
{

  ESP_ERROR_CHECK(i2c_init());

  ESP_LOGI(I2C_LOG_TAG, "i2c_init_sta finished.");

}

