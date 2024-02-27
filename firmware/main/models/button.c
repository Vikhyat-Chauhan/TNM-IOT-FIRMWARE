/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#include <stdint.h>
#include <stdbool.h>
#include <button.h>

/**
 * Initializes @p button with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param button The button structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeBUTTON(BUTTON *button)
{
  button->change = false;
  button->sync = true;
  button->longpress = false;
  button->current_trigger = false;
  button->previous_trigger = false;
  for(uint8_t relayindex=0;relayindex<RELAY_MAX_NUMBER;relayindex++){
    button->set_relay[relayindex] = false;
  }
  for(uint8_t fanindex=0;fanindex<FAN_MAX_NUMBER;fanindex++){
    button->set_fan[fanindex] = false;
  }
  return 1;
}