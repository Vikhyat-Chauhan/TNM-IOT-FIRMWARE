/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#include <stdint.h>
#include <stdbool.h>
#include <relay.h>

/**
 * Initializes @p relay with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param relay The relay structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeRELAY(RELAY *relay)
{
  relay->current_state = false;
  relay->previous_state = true;
  relay->change = false;
  relay->lastmqttcommand = false;
  relay->lastslavecommand = false;
  for(uint8_t relayindex=0;relayindex<TIMER_NUMBER;relayindex++){
    initilizeTIMER((&(relay->timer[relayindex]))) ;
  }
  return 1;
}