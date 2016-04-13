#include <tskcfg.h>
#include <stdint.h>
#include <string.h>
#include "defs.h"
#include "usbstk5515.h"
#include "i2s.h"
#include "dma.h"
#include "aic3204.h"
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

	NCO_setFreq(200);
	NCO_setAtt(5);
}
void TSK_Analysis()
{
	int16_t msg_samples[IN_FRAME_SIZE + 1];
	int16_t envelopes[(IN_FRAME_SIZE * NUM_FILTERS) + 1];

	while(1)
	{
		MBX_pend(&MBX_Dma, &msg_samples, SYS_FOREVER);

		envelopes[0] = msg_samples[0];
		memset(&(envelopes[1]), 0x7fff ,(IN_FRAME_SIZE*NUM_FILTERS));
		MBX_post(&MBX_Env, &envelopes, 0);
	}
}

void TSK_Synthesis()
{
	int16_t msg_envelopes[(IN_FRAME_SIZE * NUM_FILTERS) + 1];

	int16_t carrier[OUT_FRAME_SIZE];
	int32_t outFrame[OUT_FRAME_SIZE];

	while(1)
	{
		MBX_pend(&MBX_Env, &msg_envelopes, SYS_FOREVER);
		SEM_pendBinary(&dmaSEM, SYS_FOREVER);
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
		sFilter(firstBand, outFrame, sDelayLines[0], sFilters[0]);


		// Copy finished frame to DMA output buffer
		// TODO: try to get rid of this copying step
		for(i = 0; i < OUT_FRAME_SIZE; i++)
		{
			// Use only first channel envelope for now
			g_dmaOutputBuffer[whichBuf + i] = outFrame[i];
		}
	}
}
