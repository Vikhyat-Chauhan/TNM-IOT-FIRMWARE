/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef SYSTEM_H_
#define SYSTEM_H_

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
#include <master.h>

/* The examples use SYSTEM configuration that you can set via project configuration menu

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

#define MAX_ID_STRING (32)

static MASTER master;

MASTER* initilize_master(void);
char *getChipId(void);
char *getMqttClientID(void);

#endif /* SYSTEM_H_ */