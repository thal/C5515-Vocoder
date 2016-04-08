/*******************************************************************************
****                            I N C L U D E S
*******************************************************************************/
#include <std.h>
#include <c55.h>
#include "stdint.h"
#include "usbstk5515.h"

/*******************************************************************************
****                         D E F I N I T I O N S
*******************************************************************************/

/*******************************************************************************
****                   S T A T I C   V A R I A B L E S
*******************************************************************************/

/*******************************************************************************
****                    G L O B A L   V A R I A B L E S
*******************************************************************************/

/*******************************************************************************
****          F U N C T I O N   I M P L E M E N T A T I O N
*******************************************************************************/

void EnableI2sPort(void)
{
    /*
     * Setup and enable the I2S port transmit and receive interrupts
     */

    /*
     * FSDIV & CLKDIV = 0; has no effect when I2S is configured as slave device
     */
    I2S0_SRGR = 0x0;

    /*
     * Enable(15) = 1; I2S is enabled
     * MONO(12) = 0; Stereo Mode
     * LOOPBACK(11) = 0; no loopback
     * FSPOL(10) = 0; FS low for left word, high for right word
     * CLKPOL(9) = 0;
     * DATADLY(8) = 0; 1-bit data delay
     * PACK(7) = 0; disabled
     * SIGN_EXT(6) = 0; no sign extension
     * WDLNGTH(5:2) = 4h; 16-bit data word
     * MODE(1) = 0; Serializer is conifg as slaave
     * FRMT(0) = 0; I2S/left-justified format
     */
    I2S0_CR = 0x8010;    // 16-bit word, slave, enable I2C

    /*
     * XMITST(5) = 1; enable stero TX data interrupt
     * RCVST(3) = 1; enable stero RX data interrupt
     */
    I2S0_ICMR = 0x28;    // Enable stereo tx & rx interrupts
}

