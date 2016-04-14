/*
 * NOC.c
 *
 *  Created on: Jan 27, 2016
 */
#include <std.h>
#include <tsk.h>
#include "usbstk5515.h"
#include "usbstk5515_i2c.h"
#include "nco.h"
#include "dma.h"


#define NCO_FS 48000

short ATT = 0;

Uint32 pa = 0;
Uint32 delta;
/*
 * NCO_fillFrame(int16_t * buf)
 *
 * Fill a buffer with output from the NCO
 *
 * Arg: buf = pointer to ouput buffer
 */

void NCO_fillFrame(int16_t * buf, unsigned num) {
	Uint16 j;
	Int16 index;
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
		// Do this if you want to add attack/decay later
		//buf[j] = _smpy(index >> ATT, scale);

		pa += delta;
	}

}

void NCO_setFreq(unsigned freq)
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

/*
 * NCO_close()
 *
 * Clean up after the NCO is not needed
 */
void NCO_close()
{
	// Disable I2S
	I2S0_CR = 0x00;
}

