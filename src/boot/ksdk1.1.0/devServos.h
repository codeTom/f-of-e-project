#ifndef SERVOS_H
#define SERVOS_H

#include <stdlib.h>

uint8_t initServos();

uint8_t moveServo(uint8_t servo, uint8_t angle);
#endif