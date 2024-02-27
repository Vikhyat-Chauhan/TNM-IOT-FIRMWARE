/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef SLAVE_H_
#define SLAVE_H_

#include <stdint.h>
#include <stdbool.h>
#include <button.h>
#include <relay.h>
#include <fan.h>
#include <sensor.h>

/* These are added from the configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define RELAY_MAX_NUMBER "5"
*/
#ifdef CONFIG_RELAY_MAX_NUMBER
#define RELAY_MAX_NUMBER CONFIG_RELAY_MAX_NUMBER
#else
#define RELAY_MAX_NUMBER 2
#endif

#ifdef CONFIG_FAN_MAX_NUMBER
#define FAN_MAX_NUMBER CONFIG_FAN_MAX_NUMBER
#else
#define FAN_MAX_NUMBER 2
#endif

#ifdef CONFIG_BUTTON_MAX_NUMBER
#define BUTTON_MAX_NUMBER CONFIG_BUTTON_MAX_NUMBER
#else
#define BUTTON_MAX_NUMBER 4
#endif

#ifdef CONFIG_SENSOR_MAX_NUMBER
#define SENSOR_MAX_NUMBER CONFIG_SENSOR_MAX_NUMBER
#else
#define SENSOR_MAX_NUMBER 1
#endif

typedef struct{
    char * NAME;
    char * VERSION;
    int8_t BUTTON_NUMBER;
    int8_t RELAY_NUMBER;
    int8_t FAN_NUMBER;
    int8_t SENSOR_NUMBER;
    //int8_t BUTTON_PIN[BUTTON_FIX_NUMBER] = {12};
    //int8_t RELAY_PIN[RELAY_FIX_NUMBER] = {13,12,14,0};
    bool all_relay_change;
    bool all_fan_change;
    bool all_sensor_change;
    bool all_button_change;
    bool all_relay_lastslavecommand;
    bool all_fan_lastslavecommand;
    bool all_sensor_lastslavecommand;
    bool all_relay_lastmqttcommand;
    bool all_fan_lastmqttcommand;
    bool all_sensor_lastmqttcommand;
    BUTTON button[BUTTON_MAX_NUMBER];
    RELAY relay[RELAY_MAX_NUMBER];
    FAN fan[FAN_MAX_NUMBER];
    SENSOR sensor[SENSOR_MAX_NUMBER];
    bool change;
    bool sync;
}SLAVE;


/**
 * Initializes @p slave with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param slave The slave structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeSLAVE(SLAVE *slave);

#endif /* SLAVE_H_ */