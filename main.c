#include <tskcfg.h>
#include <stdint.h>
#include <string.h>
#include <dsplib.h>
#include "defs.h"
#include "i2s.h"
#include "dma.h"
#include "sar.h"
#include "aic3204.h"
#include "a_Filters.h"
#include "s_Filters.h"
#include "nco.h"
#include "midi.h"

#pragma DATA_ALIGN(thisABand, 4)
#pragma DATA_ALIGN(thisSBand, 4)
#pragma DATA_ALIGN(outFrame, 4)
int16_t thisABand[IN_FRAME_SIZE];
int16_t thisSBand[OUT_FRAME_SIZE];
int16_t outFrame[OUT_FRAME_SIZE];

extern int32_t g_dmaOutputBuffer[DMA_OUTPUT_BUFFER_LENGTH];

int main()
{
    SetDcBias(0);

	DmaInitialize();
	StartDma();

	ConfigureAic3204();
	EnableI2sPort();

	Init_SAR();

	NCO_setFreq(MIDI_freq[C3]);
	NCO_setAtt(0);

	rand16init();
}



void TSK_Analysis()
{
	int16_t msg_samples[IN_FRAME_SIZE + 1];
	int16_t envelopes[(IN_FRAME_SIZE * NUM_FILTERS) + 1];
	int16_t carrier[OUT_FRAME_SIZE];
	int16_t noise[OUT_FRAME_SIZE];

	while(1)
	{
		MBX_pend(&MBX_Dma, &msg_samples, SYS_FOREVER);
		envelopes[0] = msg_samples[0];
		int16_t * samples = &(msg_samples[1]);

		int n;
		for(n = 0; n < NUM_FILTERS; n++)
		{
			// Get the envelope for each channel
			// Bandpass filter
			fir2((DATA*)samples,
				(DATA*)aBpfFilters[n],
				(DATA*)thisABand,
				(DATA*)aBpfDelayLines[n],
				IN_FRAME_SIZE, A_FILTER_LENGTH);

			// Rectifier
			int i;
			for(i = 0; i < IN_FRAME_SIZE; i++)
			{
				thisABand[i] = _abss(thisABand[i]);
			}
			// Lowpass filter
			fir2((DATA*)thisABand,
				(DATA*)aLpfFilters[n],
				(DATA*)thisABand,
				(DATA*)aLpfDelayLines[n],
				IN_FRAME_SIZE, A_FILTER_LENGTH);

			// Copy envelope to mailbox
			// TODO: eliminate this copy step?
			unsigned envStart = 1 + (n * IN_FRAME_SIZE);
			for(i = 0; i < IN_FRAME_SIZE; i++)
			{
				// Scale up envelope
				//envelopes[envStart + i] = _lsshl(thisABand[i], 5);
				envelopes[envStart + i] = thisABand[i];
			}

		}

		// TODO: properly upsample the incoming envelopes to 48k
		volatile int whichBuf = envelopes[0];

		NCO_fillFrame(carrier, OUT_FRAME_SIZE);
		// Add noise to carrier

		rand16((DATA*)noise, OUT_FRAME_SIZE);
		int i;
		for(i = 0; i < OUT_FRAME_SIZE; i++)
		{
			carrier[i] += _lshrs(noise[i], 6);
		}
		// Filter first channel
		int16_t firstBand[OUT_FRAME_SIZE];
		// Multiply envelope by carrier
		for(i = 0; i < OUT_FRAME_SIZE; i++)
		{
		    firstBand[i] = _smpy(envelopes[1 + (i/IN_PER_OUT)], carrier[i]);
		}
		fir2((DATA*)firstBand,
			(DATA*)sFilters[0],
			(DATA*)outFrame,
			(DATA*)sDelayLines[0],
			OUT_FRAME_SIZE, S_FILTER_LENGTH);

		// Filter the remaining n-1 channels
		//int n;
		for(n = 1; n < NUM_FILTERS; n++)
		{
			for(i = 0; i < OUT_FRAME_SIZE; i++)
			{
				unsigned envStart = 1 + (n * IN_FRAME_SIZE);
				thisSBand[i] = _smpy(envelopes[envStart + (i/IN_PER_OUT)], carrier[i]);
			}
			fir2((DATA*)thisSBand,
				(DATA*)sFilters[n],
				(DATA*)thisSBand,
				(DATA*)sDelayLines[n],
				OUT_FRAME_SIZE, S_FILTER_LENGTH);
			// Add the filtered channel to the output frame
			for(i = 0; i < OUT_FRAME_SIZE; i++)
			{
				outFrame[i] += thisSBand[i];
			}
		}

		SEM_pendBinary(&dmaSEM, SYS_FOREVER);
		// Copy finished frame to DMA output buffer
		// TODO: try to get rid of this copying step
		for(i = 0; i < OUT_FRAME_SIZE; i++)
		{
			g_dmaOutputBuffer[whichBuf + i] = _lsshl(outFrame[i], 20);
		}
	}
}

void IDL_ButtonPress()
{
	static const MIDI_note scale[8] =
	{
		C3, D3, E3, F3, G3, A3, B3, C4
	};
	static int i = 0;

	int key = Get_Sar_Key();

	if(key == SW2)   // If SW2 pressed
	{
		MIDI_note newNote = scale[i++];
		NCO_setFreq(MIDI_freq[newNote]);
		if(i == 8) i = 0;
	}
}
