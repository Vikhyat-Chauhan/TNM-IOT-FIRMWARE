/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef MQTT_H_
#define MQTT_H_

#include <stdint.h>
#include <stdbool.h>
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "mqtt_client.h"
#include "system.h"

/* The examples use MQTT configuration that you can set via project configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#ifdef CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_SSID CONFIG_ESP_WIFI_SSID
#else
#define EXAMPLE_ESP_WIFI_SSID ""
#endif

#ifdef CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_WIFI_PASS CONFIG_ESP_WIFI_PASSWORD
#else
#define EXAMPLE_ESP_WIFI_PASS ""
#endif

#ifdef CONFIG_ESP_MAXIMUM_RETRY
#define EXAMPLE_ESP_MAXIMUM_RETRY CONFIG_ESP_MAXIMUM_RETRY
#else
#define EXAMPLE_ESP_MAXIMUM_RETRY 2
#endif


/* FreeRTOS event group to signal when we are connected*/
EventGroupHandle_t s_mqtt_event_group;

esp_mqtt_client_handle_t client;

int publishMqtt(char* topic, char* payload);

/**
 * Initializes @p master with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param master The master structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event);

/**
 * Initializes @p master with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param master The master structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

/**
 * Initializes @p master with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param master The master structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
void mqtt_app_start(MASTER* master);

#endif /* MQTT_H_ */