/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*/

#ifndef ENDPOINTJSON_H_
#define ENDPOINTJSON_H_

#include <master.h>
#include <stdint.h>
#include <stdbool.h>

char *getJsonRelayEndpoint(int index, MASTER* master);
char *getJsonFanEndpoint(int index, MASTER* master);
#endif /* ENDPOINTJSON_H_ */