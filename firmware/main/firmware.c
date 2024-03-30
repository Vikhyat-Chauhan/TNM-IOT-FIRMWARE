/* WiFi station Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "cJSON.h"

#include "master.h"
#include "wifi.h"
#include "slave.h"
#include "i2c.h"
#include "system.h"
#include "endpointJson.h"
#include "moduleJson.h"
#include "mqtt.h"

const static char *FIRMWARE_TAG = "FIRMWARE";

void app_main() 
{
    MASTER* master = initilize_master();

    ESP_ERROR_CHECK(nvs_flash_init());
    i2c_master_init();
    wifi_init_sta(master);

    ESP_LOGI(FIRMWARE_TAG, "Printing Json ");
    cJSON *root2 = cJSON_Parse("{\"enpoint\":\"11526556/relay/1/\",\"state\":1,\"type\":\"tnm.endpoint.switch\",\"clock\":1709335580,\"clientid\":\"\",\"userid\":\"\",\"timers\":[ ],\"offline\":false}");

	char *value = cJSON_GetObjectItem(root2,"type")->valuestring;
	ESP_LOGI(FIRMWARE_TAG, "value=%s",value);
    
    i2c_slave_write("RSLAVENAME");
    vTaskDelay(200 / portTICK_RATE_MS);ESP_LOGI(FIRMWARE_TAG, "The current date/time is: %lu", master->system.clock.epochtime);

    char data[20];
    i2c_slave_read(&data);
    ESP_LOGI(FIRMWARE_TAG, " data : %s", data);
    ESP_LOGI(FIRMWARE_TAG, " Relay : %s", getJsonRelayEndpoint(0,master));
    ESP_LOGI(FIRMWARE_TAG, " Module : %s", getJsonModule(master));
    publishMqtt("TestRelay",getJsonRelayEndpoint(0,master));
    publishMqtt("TestFan",getJsonFanEndpoint(0,master));
    publishMqtt("TestModule",getJsonModule(master));
}
