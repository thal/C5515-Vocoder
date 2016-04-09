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

static const int16_t * filterCoeffs;
static unsigned numberOfFilterCoeffs = 0;
static int mute = 0;

void Filter(
			int16_t * input,
			int32_t * output,
			int16_t * delayLine,
			unsigned numberOfInputSamples)
{
	unsigned i,j;

	// Static because values must be preserved between function calls
	static int xPos = 0;
	static int hPos = 0;

	// If the filter is muted, just write out zeros
	if(mute)
	{
		for(i = 0; i < numberOfInputSamples; i++)
		{
			output[i] = 0;
		}
		return;
	}

	// If the filter is not set, just pass the samples through
	if(numberOfFilterCoeffs == 0)
	{
		for(i = 0; i < numberOfInputSamples; i++)
		{
			output[i] = _lsshl(input[i], 16);
		}
		return;
	}

	for(i = 0; i < numberOfInputSamples; i++)
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

void SetFilter(const int16_t * filter, unsigned num)
{
	numberOfFilterCoeffs = num;
	filterCoeffs = filter;
}

void SetMute(int val)
{
	mute = val;
}

int GetMute()
{
	return mute;
}

