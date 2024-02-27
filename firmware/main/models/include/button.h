/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>
#include <stdbool.h>

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

typedef struct{
    bool change;
    bool sync;
    bool longpress;
    bool current_trigger;
    bool previous_trigger;
    bool set_relay[RELAY_MAX_NUMBER];
    bool set_fan[FAN_MAX_NUMBER];
}BUTTON;

/**
 * Initializes @p button with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param button The button structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeBUTTON(BUTTON *button);

#endif /* BUTTON_H_ */