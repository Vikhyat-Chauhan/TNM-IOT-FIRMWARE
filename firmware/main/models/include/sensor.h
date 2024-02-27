/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef SENSOR_H_
#define SENSOR_H_

#include <timer.h>
#include <stdint.h>
#include <stdbool.h>

/* These are added from the configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define SENSOR_MAX_NUMBER "5"
*/
#ifdef CONFIG_TIMER_NUMBER
#define TIMER_NUMBER CONFIG_TIMER_NUMBER
#else
#define TIMER_NUMBER 5
#endif

typedef struct{
    char *type;
    int current_value;
    int previous_value;
    bool change;
    bool lastmqttcommand;
    bool lastslavecommand;
}SENSOR;

/**
 * Initializes @p sensor with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param sensor The sensor structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeSENSOR(SENSOR *sensor);

#endif /* SENSOR_H_ */