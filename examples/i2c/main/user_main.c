/* I2C example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"

#include "driver/i2c.h"

static const char *TAG = "main";

/**
 * TEST CODE BRIEF
 *
 * This example will show you how to use I2C module by running two tasks on i2c bus:
 *
 * - read external i2c sensor, here we use a MPU6050 sensor for instance.
 * - Use one I2C port(master mode) to read or write the other I2C port(slave mode) on one ESP8266 chip.
 *
 * Pin assignment:
 *
 * - master:
 *    GPIO14 is assigned as the data signal of i2c master port
 *    GPIO2 is assigned as the clock signal of i2c master port
 *
 * Connection:
 *
 * - connect sda/scl of sensor with GPIO14/GPIO2
 * - no need to add external pull-up resistors, driver will enable internal pull-up resistors.
 *
 * Test items:
 *
 * - read the sensor data, if connected.
 */

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

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_example_master_init()
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

static esp_err_t i2c_slave_write(char *inputString)
{   
    ESP_LOGI(TAG, " Writing : %s", inputString);
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

static esp_err_t i2c_slave_read(char *data)
{
    ESP_LOGI(TAG, " Reading : %s", data);
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
    ESP_LOGD(TAG, " incomingDataBuffer: %s", incomingDataBuffer);
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
static int
equals(unsigned char *a, unsigned char *b, size_t len)
{
    int result = 1;
    while (len--)
    {
        result &= (*a++ == *b++);
    }
    return result;
}

static void
i2c_task_example(void *arg)
{
    vTaskDelay(10000 / portTICK_RATE_MS);
    i2c_example_master_init();
    vTaskDelay(5000 / portTICK_RATE_MS);
    i2c_slave_write("RSLAVENAME");
    vTaskDelay(200 / portTICK_RATE_MS);

    char data[MAX_SLAVE_RESPONSE_BUFFER];
    i2c_slave_read(&data);
    ESP_LOGI(TAG, " data : %s", data);

    i2c_slave_write("RRELAY");
    vTaskDelay(200 / portTICK_RATE_MS);

    i2c_slave_read(&data);
    ESP_LOGI(TAG, " data : %s", data);

    char compare[12] = "MASLAVE4R4B";
    if (equals(&data, &compare, sizeof(compare)))
    {
        ESP_LOGI(TAG, "Matched");
    }

    i2c_slave_write("RRELAYNUMBER");
    vTaskDelay(200 / portTICK_RATE_MS);

    i2c_slave_read(&data);
    ESP_LOGI(TAG, " data : %s", data);

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
        ESP_LOGI(TAG, " Hello");vTaskDelay(10000 / portTICK_RATE_MS);
    }
    i2c_driver_delete(I2C_EXAMPLE_MASTER_NUM);
}

void app_main(void)
{
    // start i2c task
    xTaskCreate(i2c_task_example, "i2c_task_example", 4096, NULL, 10, NULL);
}