/*
 * myprintf.h
 *
 *  Created on: Dec 18, 2025
 *      Author: iman
 */

#include "stm32f401xe.h"
#include <stdint.h>
#include <stdarg.h>
#include "usart.h"
#include "gpio.h"

#ifndef MYPRINTF_H_
#define MYPRINTF_H_

void myprintf_init(void);

void myprintf(char *sr, ...);

#endif /* MYPRINTF_H_ */
