/*
 * Copyright (c) 2014, Stephen Robinson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <system.h>
#include "cJSON.h"
#include "esp_log.h"
#include "endpointJson.h"

char *getJsonRelayEndpoint(int index, MASTER *master)
{
  // NOTE: Returns a heap allocated string, you are required to free it after use.
  char *string = NULL;
  cJSON *payload = cJSON_CreateObject();
  cJSON *timers = NULL;

  static char outString[20];
  sprintf(outString, "%s/relay/%d/", master->CHIPID, index + 1);
  if (cJSON_AddStringToObject(payload, "enpoint", outString) == NULL)
  {
    goto end;
  }
  //Compiler flags were added to dependencies/ESP8266_RTOS_SDK/components/newlib/component.mk to support cJSON_AddNumberToObject without crashing 
  if (cJSON_AddNumberToObject(payload, "state", (int)master->slave.relay[index].current_state) == NULL)
  {
    goto end;
  }
  if (cJSON_AddStringToObject(payload, "type", "tnm.endpoint.switch") == NULL)
  {
    goto end;
  }
  if (cJSON_AddNumberToObject(payload, "clock", master->system.clock.epochtime) == NULL)
  {
    goto end;
  }
  // TODO : Fill clientid correctly
  if (cJSON_AddStringToObject(payload, "clientid", "") == NULL)
  {
    goto end;
  }
  // TODO : Fill userid correctly
  if (cJSON_AddStringToObject(payload, "userid", "") == NULL)
  {
    goto end;
  }
  timers = cJSON_AddArrayToObject(payload, "timers");
  if (timers == NULL)
  {
    goto end;
  }
  for (uint8_t timer = 0; timer < TIMER_NUMBER; timer++)
  {
    char str[30];
    sprintf(str, "%d %d %d %d %d %d", master->slave.relay[index].timer[timer].minute, master->slave.relay[index].timer[timer].hour, master->slave.relay[index].timer[timer].day, master->slave.relay[index].timer[timer].month, master->slave.relay[index].timer[timer].week, master->slave.relay[index].timer[timer].set_value);

    cJSON *arrayitem = cJSON_CreateString(&str);
    cJSON_AddItemToArray(timers, arrayitem);
  }
  if (cJSON_AddFalseToObject(payload, "offline") == NULL)
  {
    goto end;
  }

  string = cJSON_PrintUnformatted(payload);
  // if (string == NULL)
  //{
  //   fprintf(stderr, "Failed to print payload.\n");
  // }

end:
  cJSON_Delete(payload);
  return string;
}

char *getJsonFanEndpoint(int index, MASTER *master)
{
  // NOTE: Returns a heap allocated string, you are required to free it after use.
  char *string = NULL;
  cJSON *payload = cJSON_CreateObject();
  cJSON *timers = NULL;

  static char outString[20];
  sprintf(outString, "%s/fan/%d/", master->CHIPID, index + 1);
  if (cJSON_AddStringToObject(payload, "enpoint", outString) == NULL)
  {
    goto end;
  }
  //Compiler flags were added to dependencies/ESP8266_RTOS_SDK/components/newlib/component.mk to support cJSON_AddNumberToObject without crashing 
  if (cJSON_AddNumberToObject(payload, "state", (int)master->slave.fan[index].current_state) == NULL)
  {
    goto end;
  }
  if (cJSON_AddNumberToObject(payload, "value", (int)master->slave.fan[index].current_value) == NULL)
  {
    goto end;
  }
  if (cJSON_AddStringToObject(payload, "type", "tnm.endpoint.fan") == NULL)
  {
    goto end;
  }
  if (cJSON_AddNumberToObject(payload, "clock", master->system.clock.epochtime) == NULL)
  {
    goto end;
  }
  // TODO : Fill clientid correctly
  if (cJSON_AddStringToObject(payload, "clientid", "") == NULL)
  {
    goto end;
  }
  // TODO : Fill userid correctly
  if (cJSON_AddStringToObject(payload, "userid", "") == NULL)
  {
    goto end;
  }
  timers = cJSON_AddArrayToObject(payload, "timers");
  if (timers == NULL)
  {
    goto end;
  }
  for (uint8_t timer = 0; timer < TIMER_NUMBER; timer++)
  {
    char str[30];
    sprintf(str, "%d %d %d %d %d %d", master->slave.fan[index].timer[timer].minute, master->slave.fan[index].timer[timer].hour, master->slave.fan[index].timer[timer].day, master->slave.fan[index].timer[timer].month, master->slave.fan[index].timer[timer].week, master->slave.fan[index].timer[timer].set_value);

    cJSON *arrayitem = cJSON_CreateString(&str);
    cJSON_AddItemToArray(timers, arrayitem);
  }
  if (cJSON_AddFalseToObject(payload, "offline") == NULL)
  {
    goto end;
  }

  string = cJSON_PrintUnformatted(payload);
  // if (string == NULL)
  //{
  //   fprintf(stderr, "Failed to print payload.\n");
  // }

end:
  cJSON_Delete(payload);
  return string;
}