/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef MODULEJSON_H_
#define MODULEJSON_H_

#include <master.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef CONFIG_TIMER_NUMBER
#define TIMER_NUMBER CONFIG_TIMER_NUMBER
#else
#define TIMER_NUMBER 5
#endif

char *getJsonModule(MASTER* master);

#endif /* MODULEJSON_H_ */