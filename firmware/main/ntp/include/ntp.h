/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef NTP_H_
#define NTP_H_

#include <stdint.h>
#include <stdbool.h>
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

void initilize_time(void);

void ntp_refresh_task(void *arg);

void ntp_app_start(void);

#endif /* NTP_H_ */