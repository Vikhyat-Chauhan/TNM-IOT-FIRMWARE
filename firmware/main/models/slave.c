/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#include <stdint.h>
#include <stdbool.h>
#include <slave.h>

/**
 * Initializes @p slave with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param slave The slave structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeSLAVE(SLAVE *slave)
{
  slave->NAME = "";
  slave->VERSION = "";
  slave->BUTTON_NUMBER = BUTTON_MAX_NUMBER;
  slave->RELAY_NUMBER = RELAY_MAX_NUMBER;
  slave->FAN_NUMBER = FAN_MAX_NUMBER;
  slave->SENSOR_NUMBER = SENSOR_MAX_NUMBER;
  slave->all_relay_change = false;
  slave->all_fan_change = false;
  slave->all_sensor_change = false;
  slave->all_button_change = false;
  slave->all_relay_lastslavecommand = false;
  slave->all_fan_lastslavecommand = false;
  slave->all_sensor_lastslavecommand = false;
  slave->all_relay_lastmqttcommand = false;
  slave->all_fan_lastmqttcommand = false;
  slave->all_sensor_lastmqttcommand = false;
  for(uint8_t buttonindex=0;buttonindex<BUTTON_MAX_NUMBER;buttonindex++){
    initilizeBUTTON(&(slave->button[buttonindex]));
  }
  for(uint8_t relayindex=0;relayindex<RELAY_MAX_NUMBER;relayindex++){
    initilizeRELAY(&(slave->relay[relayindex]));
  }
  for(uint8_t fanindex=0;fanindex<FAN_MAX_NUMBER;fanindex++){
    initilizeFAN(&(slave->fan[fanindex]));
  }
  for(uint8_t sensorindex=0;sensorindex<SENSOR_MAX_NUMBER;sensorindex++){
    initilizeSENSOR(&(slave->sensor[sensorindex]));
  }
  return 1;
}