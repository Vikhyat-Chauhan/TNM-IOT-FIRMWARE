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
#include "moduleJson.h"

char *getJsonModule(MASTER *master)
{
  // NOTE: Returns a heap allocated string, you are required to free it after use.
  char *string = NULL;
  cJSON *payload = cJSON_CreateObject();
  cJSON *endpoints = NULL;

  static char outString[22];
  sprintf(outString, "%s", master->CHIPID);
  if (cJSON_AddStringToObject(payload, "chipid", outString) == NULL)
  {
    goto end;
  }
  sprintf(outString, "%s", master->NAME);
  if (cJSON_AddStringToObject(payload, "type", outString) == NULL)
  {
    goto end;
  }
  sprintf(outString, "%s", master->VERSION);
  if (cJSON_AddStringToObject(payload, "version", outString) == NULL)
  {
    goto end;
  }
  if (cJSON_AddNumberToObject(payload, "clock", master->system.clock.epochtime) == NULL)
  {
    goto end;
  }
  //TODO : Fill it manually from environment variable
  if (cJSON_AddNumberToObject(payload, "timer_number", TIMER_NUMBER) == NULL)
  {
    goto end;
  }
  if (cJSON_AddFalseToObject(payload, "offline") == NULL)
  {
    goto end;
  }
  endpoints = cJSON_AddArrayToObject(payload, "endpoints");
  if (endpoints == NULL)
  {
    goto end;
  }
  /*char str[30];
  for (uint8_t timer = 0; timer < TIMER_NUMBER; timer++)
  {
    
    sprintf(str, "%d %d %d %d %d %d", master->slave.relay[index].timer[timer].minute, master->slave.relay[index].timer[timer].hour, master->slave.relay[index].timer[timer].day, master->slave.relay[index].timer[timer].month, master->slave.relay[index].timer[timer].week, master->slave.relay[index].timer[timer].set_value);

    cJSON *arrayitem = cJSON_CreateString(&str);
    cJSON_AddItemToArray(endpoints, arrayitem);
  }*/
  if (master->slave.RELAY_NUMBER > 0)
  {
    for (int index = 0; index < master->slave.RELAY_NUMBER; index++)
    {
      sprintf(outString, "%s/relay/%d/", master->CHIPID,index+1);
      cJSON *arrayitem = cJSON_CreateString(&outString);
      cJSON_AddItemToArray(endpoints, arrayitem);
    }
  }
  if (master->slave.FAN_NUMBER > 0)
  {
    for (int index = 0; index < master->slave.FAN_NUMBER; index++)
    {
      sprintf(outString, "%s/fan/%d/", master->CHIPID,index+1);
      cJSON *arrayitem = cJSON_CreateString(&outString);
      cJSON_AddItemToArray(endpoints, arrayitem);
    }
  }
  if (master->slave.SENSOR_NUMBER > 0)
  {
    for (int index = 0; index < master->slave.SENSOR_NUMBER; index++)
    {
      sprintf(outString, "%s/sensor/%d/", master->CHIPID,index+1);
      cJSON *arrayitem = cJSON_CreateString(&outString);
      cJSON_AddItemToArray(endpoints, arrayitem);
    }
  }
  cJSON *hardware = cJSON_AddObjectToObject(payload, "hardware");
  if (cJSON_AddStringToObject(hardware, "name", master->slave.NAME) == NULL)
  {
    goto end;
  }
  if (cJSON_AddStringToObject(hardware, "version", master->slave.VERSION) == NULL)
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