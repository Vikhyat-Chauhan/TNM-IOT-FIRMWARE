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
#include "mqtt.h"
#include "i2c.h"
#include "system.h"

const static char *MQTT_LOG_TAG = "MQTT_HANDLER";

int publishMqtt(char* topic, char* payload)
{ 
    int msg_id = esp_mqtt_client_publish(client, topic, payload, 0, 0, 0);
    ESP_LOGI(MQTT_LOG_TAG, "sent publish successful, msg_id=%d", msg_id);
    return msg_id;
}

esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{ 
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_CONNECTED");
        //msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
        //ESP_LOGI(MQTT_LOG_TAG, "sent publish successful, msg_id=%d", msg_id);

        //msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
        //ESP_LOGI(MQTT_LOG_TAG, "sent subscribe successful, msg_id=%d", msg_id);

        //msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
        //ESP_LOGI(MQTT_LOG_TAG, "sent subscribe successful, msg_id=%d", msg_id);

        //msg_id = esp_mqtt_client_subscribe(client, "/endpoints/123456/relay/#", 1);
        msg_id = esp_mqtt_client_subscribe(client, "testing", 1);
        ESP_LOGI(MQTT_LOG_TAG, "sent subscribe successful, msg_id=%d", msg_id);

        //msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
        //ESP_LOGI(MQTT_LOG_TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI(MQTT_LOG_TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        if(equals("WRELAY=0000",event->data,event->data_len)){
            ESP_LOGI(MQTT_LOG_TAG, "we got WRELAY=0000");
            i2c_slave_write("WRELAY=0000"); 
        }
        else if(equals("WRELAY=1111",event->data,event->data_len)){
            ESP_LOGI(MQTT_LOG_TAG, "we got WRELAY=1111");
            i2c_slave_write("WRELAY=1111");
        }
        else if(equals("WRELAY=1111",event->data,event->data_len)){
            ESP_LOGI(MQTT_LOG_TAG, "we got WRELAY=1111");
            i2c_slave_write("WRELAY=1111");
        }
        else if(equals("WRELAY=1000",event->data,event->data_len)){
            i2c_slave_write("WRELAY=1000");
        }
        else if(equals("WRELAY=0100",event->data,event->data_len)){
            i2c_slave_write("WRELAY=0100");
        }
        else if(equals("WRELAY=0010",event->data,event->data_len)){
            i2c_slave_write("WRELAY=0010");
        }
        else if(equals("WRELAY=0001",event->data,event->data_len)){
            i2c_slave_write("WRELAY=0001");
        }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(MQTT_LOG_TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(MQTT_LOG_TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

void mqtt_app_start(MASTER* master)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://iot.thenextmove.in",
        .username = "thenextmove",
        .password = "t1n2m3@TNM",
        .client_id = master->MQTTCLIENTID};

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}
