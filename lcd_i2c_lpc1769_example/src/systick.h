/*
 * systick.h
 *
 *  Created on: 9 mar. 2021
 *      Author: ealegremendoza
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#define TICKRATE_HZ1 (1000)	/* 1000 ticks per second */

void SysTick_Init(uint32_t freq);

#endif /* SYSTICK_H_ */
