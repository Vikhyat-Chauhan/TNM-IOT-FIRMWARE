/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#include <stdint.h>
#include <stdbool.h>
#include <sensor.h>
#include <string.h>

/**
 * Initializes @p sensor with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param sensor The sensor structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeSENSOR(SENSOR *sensor)
{
  sensor->type = "";
  sensor->current_value = 0;    //Currently max sensor value by default
  sensor->previous_value = 0;   //Currently max sensor value by default
  sensor->change = false;
  sensor->lastmqttcommand = false;
  sensor->lastslavecommand = false;
  return 1;
}