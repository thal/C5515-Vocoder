#include <tskcfg.h>
#include <stdint.h>
#include <string.h>
#include <dsplib.h>
#include "defs.h"
#include "usbstk5515.h"
#include "i2s.h"
#include "dma.h"
#include "aic3204.h"
#include "a_Filters.h"
#include "s_Filters.h"
#include "nco.h"

extern int32_t g_dmaOutputBuffer[DMA_OUTPUT_BUFFER_LENGTH];

int main()
{
    USBSTK5515_init();

	DmaInitialize();
	StartDma();


	ConfigureAic3204();
	EnableI2sPort();

	NCO_setFreq(220);
	NCO_setAtt(0);
}
void TSK_Analysis()
{
	int16_t msg_samples[IN_FRAME_SIZE + 1];
	int16_t envelopes[(IN_FRAME_SIZE * NUM_FILTERS) + 1];

//	static int16_t test_samples[400];
//	static int16_t test_envelopes[400];
//	unsigned samp_cnt,env_cnt = 0;

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
			int16_t thisBand[IN_FRAME_SIZE];
			fir((DATA*)samples,
				(DATA*)aBpfFilters[n],
				(DATA*)thisBand,
				(DATA*)aBpfDelayLines[n],
				IN_FRAME_SIZE, A_FILTER_LENGTH);
			// Rectifier
			int i;
			for(i = 0; i < IN_FRAME_SIZE; i++)
			{
//				if(n == 3)
//				{
//					test_samples[samp_cnt++]= thisBand[i];
//					if(samp_cnt == 400)
//						samp_cnt = 0;
//				}
				thisBand[i] = _abss(thisBand[i]);

			}
			// Lowpass filter
			fir((DATA*)thisBand,
				(DATA*)aLpfFilters[n],
				(DATA*)thisBand,
				(DATA*)aLpfDelayLines[n],
				IN_FRAME_SIZE, A_FILTER_LENGTH);

			// Copy envelope to mailbox
			// TODO: eliminate this copy step?
			unsigned envStart = 1 + (n * IN_FRAME_SIZE);
			for(i = 0; i < IN_FRAME_SIZE; i++)
			{
				// Scale envelope by 5
				envelopes[envStart + i] = _lsshl(thisBand[i], 3);
//				if(n == 3)
//				{
//					test_envelopes[env_cnt++] = thisBand[i];
//					if(env_cnt == 400)
//					{
//						env_cnt = 0;
//						asm(" NOP ");
//					}
//
//				}
			}

		}
		MBX_post(&MBX_Env, &envelopes, 0);
	}
}

void TSK_Synthesis()
{
	int16_t msg_envelopes[(IN_FRAME_SIZE * NUM_FILTERS) + 1];

	int16_t carrier[OUT_FRAME_SIZE];
	int16_t outFrame[OUT_FRAME_SIZE];


	while(1)
	{
		MBX_pend(&MBX_Env, &msg_envelopes, SYS_FOREVER);
		// TODO: properly upsample the incoming envelopes to 48k

		volatile int whichBuf = msg_envelopes[0];

		NCO_fillFrame(carrier, OUT_FRAME_SIZE);

		// Filter first channel
		int16_t firstBand[OUT_FRAME_SIZE];
		// Multiply envelope by carrier
		int i;
		for(i = 0; i < OUT_FRAME_SIZE; i++)
		{
		    firstBand[i] = _smpy(msg_envelopes[1 + (i/IN_PER_OUT)], carrier[i]);
		}
		fir((DATA*)firstBand,
			(DATA*)sFilters[0],
			(DATA*)outFrame,
			(DATA*)sDelayLines[0],
			OUT_FRAME_SIZE, S_FILTER_LENGTH);

		// Filter the remaining n-1 channels
		int n;
		for(n = 1; n < NUM_FILTERS; n++)
		{
			int16_t thisBand[OUT_FRAME_SIZE];
			for(i = 0; i < OUT_FRAME_SIZE; i++)
			{
				unsigned envStart = 1 + (n * IN_FRAME_SIZE);
				thisBand[i] = _smpy(msg_envelopes[envStart + (i/IN_PER_OUT)], carrier[i]);
			}
			fir((DATA*)thisBand,
				(DATA*)sFilters[n],
				(DATA*)thisBand,
				(DATA*)sDelayLines[n],
				OUT_FRAME_SIZE, S_FILTER_LENGTH);
			// Add the filtered channel to the output frame
			for(i = 0; i < OUT_FRAME_SIZE; i++)
			{
				outFrame[i] += thisBand[i];
			}
		}

		SEM_pendBinary(&dmaSEM, SYS_FOREVER);
		// Copy finished frame to DMA output buffer
		// TODO: try to get rid of this copying step
		for(i = 0; i < OUT_FRAME_SIZE; i++)
		{
			g_dmaOutputBuffer[whichBuf + i] = _lsshl(outFrame[i], 15);
		}
	}
}
