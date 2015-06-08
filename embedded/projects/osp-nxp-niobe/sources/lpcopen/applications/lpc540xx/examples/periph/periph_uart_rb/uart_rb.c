/*
 * @brief UART interrupt example with ring buffers
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "chip.h"
#include "board.h"
#include "string.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Transmit and receive ring buffers */
STATIC RINGBUFF_T txring, rxring;

/* Ring buffer size */
#define UART_RB_SIZE 64

#define AUTO_BAUD_ENABLE			0

/* Set the default UART, IRQ number, and IRQ handler name */
#define LPC_USART       LPC_USART0
#define LPC_IRQNUM      UART0_IRQn
#define LPC_UARTHNDLR   UART0_IRQHandler

/* Default baudrate for testing */
#define UART_TEST_DEFAULT_BAUDRATE 115200

/* Transmit and receive buffers */
static uint8_t rxbuff[UART_RB_SIZE], txbuff[UART_RB_SIZE];

const char inst1[] = "LPC540xx UART example using ring buffers\r\n";
const char inst2[] = "Press a key to echo it back or ESC to quit\r\n";

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* UART Pin mux function - note that SystemInit() may already setup your
   pin muxing at system startup */
static void Init_UART_PinMux(void)
{
#if defined(BOARD_NXP_LPCXPRESSO_54000)
	Chip_Clock_SetAsyncSysconClockDiv(1);	/* divided by 1 */

	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 0, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 1, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
#else
	/* Configure your own UART pin muxing here if needed */
#warning No UART pin muxing defined
#endif
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
/**
 * @brief	UART interrupt handler using ring buffers
 * @return	Nothing
 */
void LPC_UARTHNDLR(void)
{
	/* Want to handle any errors? Do it here. */

	/* Use default ring buffer handler. Override this with your own
	   code if you need more capability. */
	Chip_UART_IRQRBHandler(LPC_USART, &rxring, &txring);
}

/**
 * @brief	Main UART program body
 * @return	Always returns 1
 */
int main(void)
{
	uint8_t key;
	int bytes;

	SystemCoreClockUpdate();
	Board_Init();
	Init_UART_PinMux();
	Board_LED_Set(0, false);

	/* Configure PIN0.21 with pull-up */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 21, IOCON_MODE_PULLUP | IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
	Chip_Clock_SetCLKOUTSource(SYSCTL_CLKOUTSRC_MAINSYSCLK, 1);

	/* Setup UART */
	Chip_UART_Init(LPC_USART);
	Chip_UART_ConfigData(LPC_USART, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1);
#if AUTO_BAUD_ENABLE
	/* Not BRG or FRG need to be set, but, FRG clock needs to be turned on. */
	Chip_Clock_EnableAsyncPeriphClock(ASYNC_SYSCTL_CLOCK_FRG);
#else
	if ( Chip_Clock_GetMainClockSource() == SYSCTL_MAINCLKSRC_RTC )
	{
		/* If the main clock is 32K RTC, UART is running in 32K MODE, the baudrate is set to 9600 only. */
		LPC_USART->BRG = 0;
		LPC_USART->CFG |= UART_CFG_MODE32K; 
		/* FRG in ASYSCON is always needed. */
		Chip_Clock_EnableAsyncPeriphClock(ASYNC_SYSCTL_CLOCK_FRG);
	}
	else 
	{
		Chip_UART_SetBaud(LPC_USART, UART_TEST_DEFAULT_BAUDRATE);
	}
#endif
	Chip_UART_Enable(LPC_USART);
	Chip_UART_TXEnable(LPC_USART);

#if AUTO_BAUD_ENABLE
	/* If auto baud is enabled, the previous baud rate setting may not matter. */
	/* On byte arriving on RX with LSB bit set will trigger the auto-bauding. */
	if ( Chip_UART_AutoBaud(LPC_USART) == false )
	{
		/* Need to understand why */
		while ( 1 );
	}
	Chip_UART_ClearStatus( LPC_USART, UART_STAT_OVERRUNINT|UART_STAT_DELTARXBRK|UART_STAT_FRM_ERRINT|UART_STAT_PAR_ERRINT|UART_STAT_RXNOISEINT);
#endif

	/* Before using the ring buffers, initialize them using the ring
	   buffer init function */
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_RB_SIZE);

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(LPC_USART, UART_INTEN_RXRDY);
	Chip_UART_IntDisable(LPC_USART, UART_INTEN_TXRDY);	/* May not be needed */

	/* preemption = 1, sub-priority = 1 */
	NVIC_EnableIRQ(LPC_IRQNUM);

	/* Initial message sent using blocking method to prevent ring
	   buffer overflow */
	Chip_UART_SendBlocking(LPC_USART, inst1, sizeof(inst1) - 1);
	Chip_UART_SendRB(LPC_USART, &txring, inst2, sizeof(inst2) - 1);

	/* Poll the receive ring buffer for the ESC (ASCII 27) key */
	key = 0;
	while (key != 27) {
		bytes = Chip_UART_ReadRB(LPC_USART, &rxring, &key, 1);
		if (bytes > 0) {
			/* Wrap value back around */
			if (Chip_UART_SendRB(LPC_USART, &txring, (const uint8_t *) &key, 1) != 1) {
				Board_LED_Toggle(0);/* Toggle LED if the TX FIFO is full */
			}
		}
	}

	/* DeInitialize UART peripheral */
	NVIC_DisableIRQ(LPC_IRQNUM);
	Chip_UART_DeInit(LPC_USART);

	while (1) {
		__WFI();
	}
	return 0;
}
