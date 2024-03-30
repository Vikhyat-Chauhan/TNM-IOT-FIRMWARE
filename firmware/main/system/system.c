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
#include "system.h"
#include "master.h"

const static char *SYSTEM_LOG_TAG = "SYSTEM_HANDLER";

char *getChipId(void)
{
    uint8_t mac[6];
    char *id_string = calloc(1, MAX_ID_STRING);
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    sprintf(id_string, "%02x%02X%02X", mac[3], mac[4], mac[5]);
    return id_string;
}

char *getMqttClientID(void)
{
    uint8_t mac[6];
    char *id_string = calloc(1, MAX_ID_STRING);
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    sprintf(id_string, "TNMESP12%02x%02X%02X", mac[3], mac[4], mac[5]);
    return id_string;
}

MASTER *initilize_master(void)
{
    master.CHIPID = getChipId();
    master.MQTTCLIENTID = getMqttClientID();
    // TODO : Fill these values dynamically after i2c slave handshake
    master.NAME = "MASTERWIFI12T";
    master.VERSION = "12.0";
    master.slave.NAME = "SLAVE2R1FMS4B";
    master.slave.VERSION = "18";
    master.slave.RELAY_NUMBER = 2;
    master.slave.FAN_NUMBER = 1;
    master.slave.SENSOR_NUMBER = 1;
    for (int index = 0; index < master.slave.RELAY_NUMBER; index++)
    {
        master.slave.relay[index].current_state = true;
        master.slave.relay[index].timer[2].hour = 65;
    }
    return &master;
}