#include <tskcfg.h>
#include <stdint.h>
#include "usbstk5515.h"
#include "i2s.h"
#include "dma.h"
#include "aic3204.h"
#include "FIR.h"
#include "filters.h"
#include "nco.h"

#define ROUND_SHIFT_32_TO_16(a) (_lshrs(_sround(a),16))

#define IN_FRAME_SIZE 8
#define OUT_FRAME_SIZE DMA_PING_PONG_BUFFER_LENGTH
#define IN_PER_OUT DMA_PING_PONG_BUFFER_LENGTH / IN_FRAME_SIZE

#define NUM_FILTERS 4

extern int32_t g_dmaOutputBuffer[DMA_OUTPUT_BUFFER_LENGTH];

int main()
{
    USBSTK5515_init();

	DmaInitialize();
	StartDma();


	ConfigureAic3204();
	EnableI2sPort();
}
void TSK_Analysis()
{
	int16_t msg_samples[IN_FRAME_SIZE + 1];
	int16_t envelopes[(IN_FRAME_SIZE * NUM_FILTERS) + 1];

	while(1)
	{
		MBX_pend(&MBX_Dma, &msg_samples, SYS_FOREVER);

		memcpy(&(envelopes[0]), &(msg_samples[0]), IN_FRAME_SIZE + 1);
		MBX_post(&MBX_Env, &envelopes, 0);
	}
}

void TSK_Synthesis()
{
	int16_t msg_envelopes[(IN_FRAME_SIZE * NUM_FILTERS) + 1];

	while(1)
	{
		MBX_pend(&MBX_Env, &msg_envelopes, SYS_FOREVER);
		SEM_pendBinary(&dmaSEM, SYS_FOREVER);
		// TODO: properly the incoming envelopes to 48k
		volatile int whichBuf = msg_envelopes[0];
		int i;
		for(i = 0; i < IN_FRAME_SIZE; i++)
		{
			int j;
			for(j = 0; j < IN_PER_OUT; j++)
			{
				g_dmaOutputBuffer[whichBuf + (i * IN_PER_OUT) + j] = _lsshl(msg_envelopes[i+1], 15);
			}

		}
	}
}
