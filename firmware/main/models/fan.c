/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#include <stdint.h>
#include <stdbool.h>
#include <fan.h>

/**
 * Initializes @p fan with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param fan The fan structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeFAN(FAN *fan)
{
  fan->current_state = false;
  fan->previous_state = true;
  fan->current_value = 4;    //Currently max fan speed by default
  fan->previous_value = 4;   //Currently max fan speed by default
  fan->change = false;
  fan->lastmqttcommand = false;
  fan->lastslavecommand = false;
  for(uint8_t fanindex=0;fanindex<TIMER_NUMBER;fanindex++){
    initilizeTIMER((&(fan->timer[fanindex]))) ;
  }
  return 1;
}