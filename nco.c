/*
 * NOC.c
 *
 *  Created on: Jan 27, 2016
 */
#include "nco.h"


#define NCO_FS 48000

short ATT = 0;

uint32_t pa = 0;
uint32_t delta;
/*
 * NCO_fillFrame(int16_t * buf)
 *
 * Fill a buffer with output from the NCO
 *
 * Arg: buf = pointer to ouput buffer
 */

void NCO_fillFrame(int16_t * buf, unsigned num) {
	uint16_t j;
	int16_t index;
	/* Play Tone */
	for (j = 0; j < num; j++) {

		// Compute the scale factor if attack or decay are active
//		if(attackLeft > 0)
//		{
//			attackLeft--;
//			scale = (attack - attackLeft) * (0x7fff / attack);
//		}
//		else if(decayLeft > 0)
//		{
//			decayLeft--;
//			scale = decayLeft * (0x7fff / decay);
//		}
//		scale = 0x7fff;

		// For a sawtooth wave use the index itself as the sample (16 bits)
		index = pa >> (16);

		buf[j] = _shrs(index, ATT);
		//buf[j] = _smpy(index >> ATT, scale);

		pa += delta;
	}

}

void NCO_setFreq(float freq)
{
	delta = ((float) 0xffffffff / NCO_FS) * freq;
}
/*
 * NCO_setAtt()
 *
 * Set the global attenuation value
 */

void NCO_setAtt(unsigned int att)
{
	ATT = att;
}

