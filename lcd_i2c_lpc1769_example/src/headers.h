/*
 * headers.h
 *
 *  Created on: 9 mar. 2021
 *      Author: ealegremendoza
 */

#ifndef HEADERS_H_
#define HEADERS_H_

#include "chip.h"
#include <cr_section_macros.h>
#include <stdio.h>
#include <string.h>
#include "systick.h"
#include "display1602_i2c_lpc1769.h"

/* Comment for stand-alone mode */
#include "debug.h"

#ifndef DEBUG_H_
#define DEBUGOUT(a);
#else
#define DEBUGOUT(a);  printf(a);
#endif


#endif /* HEADERS_H_ */
