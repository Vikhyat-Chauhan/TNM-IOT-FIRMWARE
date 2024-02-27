/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>
#include <stdbool.h>
#include <slave.h>

/* These are added from the configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define RELAY_MAX_NUMBER "5"
*/
#ifdef CONFIG_MODEL_NAME
#define MODEL_NAME CONFIG_MODEL_NAME
#else
#define MODEL_NAME "MASTERWIFI12T"
#endif

#ifdef CONFIG_MODEL_VERSION
#define MODEL_VERSION CONFIG_MODEL_VERSION
#else
#define MODEL_VERSION "1.0"
#endif

#ifdef CONFIG_MULTI_WIFI_LIST
#define MULTI_WIFI_LIST CONFIG_MULTI_WIFI_LIST
#else
#define MULTI_WIFI_LIST 2
#endif

typedef struct{
  //Time Sensor Variables
  int8_t date,month,year,week,hour,minute,second;
  //Time Sensor Variables
  int8_t temp;
  int8_t error_code;
  bool error;
  bool h12;
  bool century;
  bool pmam;
}CLOCK;

typedef struct{
    //async Task control variables
    bool check_update;
    bool slave_handshake;
    bool boot_check_update;
    bool saved_wifi_present[MULTI_WIFI_LIST];
    bool rom_external;
    bool set_time;
    bool receiving_json;
    bool received_json;
    bool wifi_setup;
    bool wifi_connected;
    bool slave_resetting;
}FLAG;

typedef struct{
  bool initiate_memory;
  bool wifi_handler;
  bool initiate_mqtt;
  bool wifi_setup;
}FUNCTIONSTATE;

typedef struct{
  CLOCK clock;
  FLAG flag;
}SYSTEM;

typedef struct{
    char* NAME;
    char* VERSION;
    bool change;
    SYSTEM system;
    SLAVE slave;
}MASTER;

/**
 * Initializes @p clock with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param clock The clock structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeCLOCK(CLOCK *clock);

/**
 * Initializes @p flag with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param flag The flag structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeFLAG(FLAG *flag);

/**
 * Initializes @p functionstate with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param functionstate The functionstate structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeFUNCTIONSTATE(FUNCTIONSTATE *functionstate);

/**
 * Initializes @p system with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param system The system structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeSYSTEM(SYSTEM *system);

/**
 * Initializes @p master with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param master The master structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeMASTER(MASTER *master);

#endif /* CLOCK_H_ */