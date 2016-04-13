/*
 * FIR.c
 *
 *  Created on: Feb 10, 2016
 *      Author: tom
 */
#include "FIR.h"
#include "dma.h"
#include "filters.h"

#define ROUND_SHIFT_32_TO_16(a) (_lshrs(_sround(a),16))
#define FRAME_SIZE 8

static unsigned numberOfFilterCoeffs = FILTER_LENGTH;

void Filter(
			int16_t * input,
			int32_t * output,
			int16_t * delayLine,
			const int16_t * filterCoeffs)
{
	unsigned i,j;

	// Static because values must be preserved between function calls
	static int xPos = 0;
	static int hPos = 0;

		for(i = 0; i < FRAME_SIZE; i++)
	{
		int32_t sample = 0;
		delayLine[xPos] = input[i];

		for(j = 0; j < numberOfFilterCoeffs; j++)
		{
			hPos = j - (xPos+1);
			if(hPos < 0)
				hPos = numberOfFilterCoeffs + hPos;
			hPos = (numberOfFilterCoeffs - 1) - hPos;

			int16_t x = delayLine[j];
			int16_t h = filterCoeffs[hPos];

			// Use saturated MAC with rounding intrinsic
			sample = _smacr(sample, x, h);
		}

		xPos++;
		if(xPos == numberOfFilterCoeffs)
			xPos = 0;

		output[i] = sample;
	}
}

void GetEnvelope(
			int16_t * input,
			int32_t * output,
			int16_t * delayLine,
			const int16_t * filterCoeffs)
{
	unsigned i,j;

	// Static because values must be preserved between function calls
	static int xPos = 0;
	static int hPos = 0;


	int16_t tmpOut[FRAME_SIZE];
	for(i = 0; i < FRAME_SIZE; i++)
	{
		int32_t sample = 0;
		delayLine[xPos] = input[i];

		for(j = 0; j < numberOfFilterCoeffs; j++)
		{
			hPos = j - (xPos+1);
			if(hPos < 0)
				hPos = numberOfFilterCoeffs + hPos;
			hPos = (numberOfFilterCoeffs - 1) - hPos;

			int16_t x = delayLine[j];
			int16_t h = filterCoeffs[hPos];

			// Use saturated MAC with rounding intrinsic
			sample = _smacr(sample, x, h);
		}

		xPos++;
		if(xPos == numberOfFilterCoeffs)
			xPos = 0;

		// Rectify with absolute value
		tmpOut[i] = _abss(_lsshl(sample,16));
	}
	// Lowpass the rectified signal to get the envelope
	Filter(tmpOut, output, delayLine, lpfEnvelope);
}
