/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#include <stdint.h>
#include <stdbool.h>
#include <timer.h>

/**
 * Initializes @p timer with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param timer The timer structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeTIMER(TIMER *timer)
{
  timer->current_state = false;
  timer->active = false;
  timer->set_value = 0;
  timer->minute = 0;
  timer->hour = 12;
  timer->day = 1;
  timer->month = 1;
  timer->week = 1;
  return 1;
}