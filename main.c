#include <tskcfg.h>
#include <stdint.h>
#include "usbstk5515.h"
#include "i2s.h"
#include "dma.h"
#include "aic3204.h"

#define FRAME_SIZE DMA_PING_PONG_BUFFER_LENGTH
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

	while(1)
	{
		MBX_pend(&MBX_Dma, &msg, SYS_FOREVER);
		volatile int whichBuf = msg[0];
		SEM_pendBinary(&dmaSEM, SYS_FOREVER);
		int i;
		for( i = 0; i < FRAME_SIZE; i++)
		{
			g_dmaOutputBuffer[i+whichBuf] = _lsshl(msg[i+1], 16);
		}
	}
}
