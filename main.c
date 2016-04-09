#include <tskcfg.h>
#include <stdint.h>
#include "usbstk5515.h"
#include "i2s.h"
#include "dma.h"
#include "aic3204.h"
#include "FIR.h"
#include "filters.h"

#define FRAME_SIZE 8
extern int32_t g_dmaOutputBuffer[DMA_OUTPUT_BUFFER_LENGTH];

int main()
{
    USBSTK5515_init();

	DmaInitialize();
	StartDma();


	ConfigureAic3204();
	EnableI2sPort();
}

void TSK_Filter()
{
	int16_t msg[FRAME_SIZE + 1];
	int16_t delayLine[FILTER_LENGTH];
	while(1)
	{
		MBX_pend(&MBX_Dma, &msg, SYS_FOREVER);
		volatile int whichBuf = msg[0];
		int32_t outBuf[8];

		SetFilter(bpf4, FILTER_LENGTH);
		Filter(&(msg[1]), outBuf, delayLine, 8);
		SEM_pendBinary(&dmaSEM, SYS_FOREVER);
		int i;
		for( i = 0; i < 8; i++)
		{
			int j;
			for(j = 0; j < 6; j++)
			{
				g_dmaOutputBuffer[(i*6)+j+whichBuf] = outBuf[i];
			}

		}
	}
}
