/*
 * FIR.h
 *
 *  Created on: Feb 10, 2016
 *      Author: tom
 */
#include <stdint.h>
#include "usbstk5515_i2c.h"

#define Rcv 0x08
#define Xmit 0x20

#ifndef FIR_H_
#define FIR_H_

void Filter(
			int16_t * input,
			int32_t * output,
			int16_t * delayLine,
			unsigned numberOfInputSamples);

void SetFilter(const int16_t * filter, unsigned num);

void SetMute(int val);

int GetMute();

#endif /* FIR_H_ */
