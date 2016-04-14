/*
 * defs.h
 *
 *  Created on: Apr 13, 2016
 *      Author: tom
 */

#ifndef DEFS_H_
#define DEFS_H_

#define ROUND_SHIFT_32_TO_16(a) (_lshrs(_sround(a),16))

#define IN_FRAME_SIZE (8)
#define OUT_FRAME_SIZE DMA_PING_PONG_BUFFER_LENGTH
#define IN_PER_OUT DMA_PING_PONG_BUFFER_LENGTH / IN_FRAME_SIZE

#define NUM_FILTERS (4)




#endif /* DEFS_H_ */
