/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include <stdbool.h>
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "driver/i2c.h"

#define I2C_EXAMPLE_MASTER_SCL_IO 4         // 5                /*!< gpio number for I2C master clock */
#define I2C_EXAMPLE_MASTER_SDA_IO 5         // 4               /*!< gpio number for I2C master data  */
#define I2C_EXAMPLE_MASTER_NUM I2C_NUM_0    /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE 0 /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE 0 /*!< I2C master do not need buffer */

#define MPU6050_SENSOR_ADDR 0x68   /*!< slave address for MPU6050 sensor */
#define MPU6050_CMD_START 0x41     /*!< Command to set measure mode */
#define MPU6050_WHO_AM_I 0x75      /*!< Command to read WHO_AM_I reg */
#define WRITE_BIT I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ   /*!< I2C master read */
#define ACK_CHECK_EN 0x1           /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0          /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                /*!< I2C ack value */
#define NACK_VAL 0x1               /*!< I2C nack value */
#define LAST_NACK_VAL 0x2          /*!< I2C last_nack value */

#define MAX_SLAVE_RESPONSE_BUFFER 20

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define I2C_CONNECTED_BIT BIT0
#define I2C_FAIL_BIT BIT1


/**
 * Initializes @p master with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param master The master structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
static esp_err_t i2c_init();

void i2c_master_init(void);

esp_err_t i2c_slave_write(char *inputString);

esp_err_t i2c_slave_read(char *data);

int equals(char *a,char *b, size_t len);

void i2c_task_example(void *arg);

#endif /* I2C_H_ */