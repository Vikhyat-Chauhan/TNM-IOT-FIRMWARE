/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <stdbool.h>

/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

typedef struct {
  bool current_state;
  bool active;
  int8_t set_value;
  int8_t minute;
  int8_t hour;
  int8_t day;
  int8_t month;
  int8_t week;
}TIMER;  

/**
 * Initializes @p timer with default values and the function returns the value
 * @c 1. Otherwise, @c 0 is returned.
 *
 * @param timer The timer structure to initilize.
 *
 * @return      @c 1 on success, @c 0 otherwise.
 */
int initilizeTIMER(TIMER *timer);

#endif /* TIMER_H_ */