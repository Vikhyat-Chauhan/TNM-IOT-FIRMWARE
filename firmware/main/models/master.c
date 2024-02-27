/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#include <stdint.h>
#include <stdbool.h>
#include <master.h>

/**
 * Initializes @p clock with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param clock The clock structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeCLOCK(CLOCK *clock)
{
  clock->date = 1;
  clock->month = 1;
  clock->year = 1;
  clock->week = 1;
  clock->hour = 12;
  clock->minute = 1;
  clock->second = 1;
  clock->temp = 1;
  clock->error_code = 1;
  clock->error = false;
  clock->h12 = false;
  clock->century = false;
  clock->pmam = false;
  return 1;
}

/**
 * Initializes @p flag with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param flag The flag structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeFLAG(FLAG *flag)
{
  flag->check_update = false;
  flag->slave_handshake= false;
  flag->boot_check_update = true;
  for(uint8_t wifiindex=0;wifiindex<MULTI_WIFI_LIST;wifiindex++){
    flag->saved_wifi_present[wifiindex] = false;
  }
  flag->rom_external = true;
  flag->set_time = false;
  flag->receiving_json = false;
  flag->received_json = false;
  flag->wifi_setup = false;
  flag->wifi_connected = false;
  flag->slave_resetting = false;
  return 1;
}

/**
 * Initializes @p functionstate with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param functionstate The functionstate structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeFUNCTIONSTATE(FUNCTIONSTATE *functionstate)
{
  functionstate->initiate_memory = false;
  functionstate->wifi_handler = false;
  functionstate->initiate_mqtt = false;
  functionstate->wifi_setup = false;
  return 1;
}

/**
 * Initializes @p system with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param system The system structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeSYSTEM(SYSTEM *system)
{
  initilizeCLOCK(&(system->clock));
  initilizeFLAG(&(system->flag));
  return 1;
}

/**
 * Initializes @p master with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param master The master structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeMASTER(MASTER *master)
{
  master->NAME = MODEL_NAME;
  master->VERSION = MODEL_VERSION;
  master->change = false;
  initilizeSYSTEM(&(master->system));
  initilizeSLAVE(&(master->slave));
  return 1;
}