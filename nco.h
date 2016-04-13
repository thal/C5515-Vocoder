/*
 * Noc.h
 *
 *  Created on: Jan 27, 2016
 */

#ifndef NCO_H_
#define NCO_H_

#include <stdint.h>
#include "usbstk5515.h"

#define AIC3204_I2C_ADDR 0x18
#define XmitL 0x10
#define XmitR 0x20

extern Int16 USBSTK5515_I2C_write( Uint16 i2c_addr, Uint8* data, Uint16 len );

void NCO_setAtt(unsigned int att);

void NCO_fillFrame(int16_t * buf, unsigned num);


void NCO_setFreq(unsigned freq);


void NCO_close();

#endif /* NCO_H_ */
