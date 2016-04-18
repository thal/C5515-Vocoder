/*
 * Noc.h
 *
 *  Created on: Jan 27, 2016
 */

#ifndef NCO_H_
#define NCO_H_

#include <stdint.h>

void NCO_setAtt(unsigned int att);

void NCO_fillFrame(int16_t * buf, unsigned num);

void NCO_setFreq(float freq);

#endif /* NCO_H_ */
