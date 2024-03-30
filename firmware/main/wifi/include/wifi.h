/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef WIFI_H_
#define WIFI_H_

#include <stdint.h>
#include <stdbool.h>
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "system.h"

/* The examples use WiFi configuration that you can set via project configuration menu

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

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

/* FreeRTOS event group to signal when we are connected*/
EventGroupHandle_t s_wifi_event_group;

/**
 * Initializes @p master with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param master The master structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data);

/**
 * Initializes @p master with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param master The master structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
void wifi_init_sta(MASTER* master);

#endif /* WIFI_H_ */