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
			const int16_t *  filterCoeffs);

void GetEnvelope(
			int16_t * input,
			int32_t * output,
			int16_t * delayLine,
			const int16_t *  filterCoeffs);

#endif /* FIR_H_ */
