/*
 * Copyright (c) 2014, Stephen Robinson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include "lwip/apps/sntp.h"
#include "ntp.h"
#include "system.h"

const static char *NTP_LOG_TAG = "NTP_HANDLER";
int s_retry_num = 0;

void initilize_time(void)
{
  ESP_LOGI(NTP_LOG_TAG, "Initializing SNTP");
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "pool.ntp.org");
  sntp_init();

  // wait for time to be set
  time_t now = 0;
  struct tm timeinfo = {0};
  int retry = 0;
  const int retry_count = 10;

  while (timeinfo.tm_year < (2016 - 1900) && retry < retry_count)
  {
    ESP_LOGI(NTP_LOG_TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    time(&now);
    localtime_r(&now, &timeinfo);
  }
}

void ntp_refresh_task(void *arg)
{
  time_t now;
  struct tm timeinfo;
  char strftime_buf[64];

  time(&now);
  localtime_r(&now, &timeinfo);

  // Is time set? If not, tm_year will be (1970 - 1900).
  if (timeinfo.tm_year < (2016 - 1900))
  {
    ESP_LOGI(NTP_LOG_TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
    initilize_time();
  }

  // Set timezone to Eastern Standard Time and print local time
  // setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
  // tzset();

  // Set timezone to China Standard Time
  // setenv("TZ", "CST-8", 1);
  // tzset();

  while (1)
  {
    // update 'now' variable with current time
    time(&now);
    localtime_r(&now, &timeinfo);

    if (timeinfo.tm_year < (2016 - 1900))
    {
      ESP_LOGE(NTP_LOG_TAG, "The current date/time error");
    }
    else
    {
      strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
      master.system.clock.epochtime = now;
      //ESP_LOGI(NTP_LOG_TAG, "The current date/time in Shanghai is: %s", strftime_buf);
      //ESP_LOGI(NTP_LOG_TAG, "The current date/time is: %lu", now);
    }

    //ESP_LOGI(NTP_LOG_TAG, "Free heap size: %d\n", esp_get_free_heap_size());
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}

void ntp_app_start(void)
{
  ESP_ERROR_CHECK(esp_netif_init());

  // SNTP service uses LwIP, please allocate large stack space.
  xTaskCreate(ntp_refresh_task, "ntp_refresh_task", 2048, NULL, 9, NULL);
}