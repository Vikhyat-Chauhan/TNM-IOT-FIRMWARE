/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef FAN_H_
#define FAN_H_

#include <stdint.h>
#include <stdbool.h>
#include <timer.h>

/* These are added from the configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define FAN_MAX_NUMBER "5"
*/
#ifdef CONFIG_TIMER_NUMBER
#define TIMER_NUMBER CONFIG_TIMER_NUMBER
#else
#define TIMER_NUMBER 5
#endif

typedef struct{
    bool current_state;
    bool previous_state;
    int8_t current_value;
    int8_t previous_value;
    bool change;
    bool lastmqttcommand;
    bool lastslavecommand;
    TIMER timer[TIMER_NUMBER];
}FAN;

/**
 * Initializes @p fan with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param fan The fan structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeFAN(FAN *fan);

#endif /* FAN_H_ */