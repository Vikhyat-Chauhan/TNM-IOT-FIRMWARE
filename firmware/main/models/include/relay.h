/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef RELAY_H_
#define RELAY_H_

#include <timer.h>
#include <stdint.h>
#include <stdbool.h>

/* These are added from the configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define RELAY_MAX_NUMBER "5"
*/
#ifdef CONFIG_TIMER_NUMBER
#define TIMER_NUMBER CONFIG_TIMER_NUMBER
#else
#define TIMER_NUMBER 5
#endif

typedef struct{
    bool current_state;
    bool previous_state;
    bool change;
    bool lastmqttcommand;
    bool lastslavecommand;
    TIMER timer[TIMER_NUMBER];
}RELAY;

/**
 * Initializes @p relay with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param relay The relay structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeRELAY(RELAY *relay);

#endif /* RELAY_H_ */