/*
 * @brief Implements I2C slave driver for host interface module
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
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
#include "board.h"
#include <string.h>
#include <stdint.h>
#include "sensorhub.h"
#include "hostif.h"
#include "osp-types.h"
#include "common.h"
/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define RX_LENGTH		0x4
#define I2C_SLAVE_XFER_DONE	0x10
#define I2C_OVERREACH_VAL	0xCC// This value is sent for access beyond register area

typedef struct __HOSTIF_Ctrl_t {
	int	magic;
	uint8_t rxCount;			/* Bytes so far received  */
	uint8_t rxLength;			/* Expected Rx buffer length */
	uint16_t txCountSet;
	uint16_t txCount;			/* Bytes so far transmitted */
	uint16_t txLength;			/* Total transfer length of Tx buffer */
	uint8_t *txBuff;			/* Tx buffer pointer */
	uint8_t *oldtxBuff;
	uint8_t rxBuff[RX_LENGTH];	/* Rx buffer */
	uint8_t *txBuff_next;
	uint16_t txLength_next;
} Hostif_Ctrl_t;

static Hostif_Ctrl_t g_hostif;

#if defined(I2CS_DMA_BASED)
/* DMA descriptors must be aligned to 16 bytes */
#if defined(__CC_ARM)
__align(16) static DMA_CHDESC_T dmaI2CSDesc;
#endif /* defined (__CC_ARM) */

/* IAR support */
#if defined(__ICCARM__)
#pragma data_alignment=16
static DMA_CHDESC_T dmaI2CSDesc;
#endif /* defined (__ICCARM__) */

#if defined( __GNUC__ )
static DMA_CHDESC_T dmaI2CSDesc __attribute__ ((aligned(16)));
#endif /* defined (__GNUC__) */
#endif

static void Hostif_TxNext(int magic);

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Handler for slave start callback */
static void processSlaveTransferStart(uint8_t addr)
{
#if 0
	Hostif_StartTx(NULL, 0, -2);
#endif
}

#if defined(I2CS_DMA_BASED)
/* DMA error handler and notification for I2C */
void dev_i2cs_dma_callback(bool error)
{
	if (error) {
		/* Nothing to do here. I2C will terminate the trasnfer. */
		;
	}

	Chip_I2CS_SlaveDisableDMA(I2C_HOSTIF);
#if 0
	if (getLastCommand() == SPI_SH_GET_BROADCAST_DATA) {
		hostCommitDataTx();
		calculate_commited_tx_buffer_size();
	}
#endif
}

/* Start a DMA slave write operation */
static void slaveDMAWriteStart(void *buff, uint32_t bytes)
{
	/* Slave to master */
	dmaI2CSDesc.source = DMA_ADDR(buff) + bytes - 1;
	dmaI2CSDesc.dest = DMA_ADDR(&I2C_HOSTIF->SLVDAT);
	dmaI2CSDesc.next = DMA_ADDR(0);
	dmaI2CSDesc.xfercfg = DMA_XFERCFG_CFGVALID | DMA_XFERCFG_SETINTA |
						 DMA_XFERCFG_SWTRIG | DMA_XFERCFG_WIDTH_8 | DMA_XFERCFG_SRCINC_1 |
						 DMA_XFERCFG_DSTINC_0 | DMA_XFERCFG_XFERCOUNT(bytes);

	/* Setup transfer descriptor and validate it */
	Chip_DMA_SetupTranChannel(LPC_DMA, I2C_HOSTID_DMAID, &dmaI2CSDesc);

	/* Setup data transfer */
	Chip_DMA_SetupChannelTransfer(LPC_DMA, I2C_HOSTID_DMAID, dmaI2CSDesc.xfercfg);

	Chip_DMA_SetValidChannel(LPC_DMA, I2C_HOSTID_DMAID);
}

/* Handler for slave send callback */
static uint8_t processSlaveTransferSend(uint8_t *data)
{
	uint8_t ret = I2C_SLVCTL_SLVCONTINUE;

	/* In transmit we will do not want to send data beyond specified region */
	if ((g_hostif.txBuff != NULL) && (g_hostif.txLength > 0)) {
		slaveDMAWriteStart(g_hostif.txBuff, g_hostif.txLength);
		Hostif_StartTx(NULL, 0, -1);
		ret = I2C_SLVCTL_SLVDMA;
	} else {
		*data = I2C_OVERREACH_VAL;	/* This value signifies read beyond allowed range */
		Hostif_StartTx(NULL, 0, -1);
	}
#error Used?
	return ret;
}
#else
/* Handler for slave send callback */
static uint8_t processSlaveTransferSend(uint8_t *data)
{
	uint8_t ret = I2C_SLVCTL_SLVCONTINUE;
	static volatile int finished = 0;

	/* In transmit we will do not want to send data beyond specified region */
	if ((g_hostif.txBuff != NULL) && (g_hostif.txCount < g_hostif.txLength)) {
		*data = g_hostif.txBuff[g_hostif.txCount++];
		finished = 0;
		if (g_hostif.txCount >= g_hostif.txLength) {
			Hostif_TxNext(0x0d000000);
			finished = 1;
		}
	} else {
		*data = I2C_OVERREACH_VAL;	/* This value signifies read beyond allowed range */
	}
	return ret;
}
#endif

/* Handler for slave receive callback */
static uint8_t processSlaveTransferRecv(uint8_t data)
{
	uint8_t ret = I2C_SLVCTL_SLVCONTINUE;

	if ((g_hostif.rxCount == 0) || (g_hostif.rxCount < g_hostif.rxLength)) {
		g_hostif.rxBuff[g_hostif.rxCount++] = data;
		switch (g_hostif.rxCount) {
		case 2:
			D0_printf("2 byte received from i2c\r\n");
		case 1:
			/* D0_printf("1 byte received\n\r"); */
			g_hostif.rxLength = process_command(&g_hostif.rxBuff[0], g_hostif.rxCount) + 1;
			if (g_hostif.rxCount >= g_hostif.rxLength) {
				g_hostif.rxCount = 0;
				g_hostif.rxLength = 0;
			}
			break;
#if 0

		case 2:
		case 3:
		case 4:
			if (g_hostif.rxCount >= g_hostif.rxLength) {
				process_command(&g_hostif.rxBuff[0], g_hostif.rxCount);
				ret = I2C_SLVCTL_SLVNACK;
				g_hostif.rxCount = 0;
				g_hostif.rxLength = 0;
			}
			break;
#endif

		default:
			/* Not supported at this time so just NACK it for now */
			ret = I2C_SLVCTL_SLVNACK;
			break;
		}
	} else {
		/* Not supported at this time so just NACK it for now */
		g_hostif.rxCount = 0;
		ret = I2C_SLVCTL_SLVNACK;
	}
	return ret;
}


volatile int i2cdoneflag = 0;
/* Handler for slave transfer complete callback */
static void processSlaveTransferDone(void)
{
	i2cdoneflag++;
	/* Nothing needed here */
#if 0
	Hostif_TxNext(0x0c000000);
#endif
#if 0
	Hostif_StartTx(NULL, 0, -3);
#endif
}

/* I2C slavecallback function list */
const static I2CS_XFER_T i2csCallBacks = {
	&processSlaveTransferStart,	/* Do nothing */
	&processSlaveTransferSend,
	&processSlaveTransferRecv,	/* From master */
	&processSlaveTransferDone	/* Do Nothing */
};

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/** Initialize the Sensor Hub host/AP interface */
void Hostif_Init(void)
{
	/* reset host IF control data structure */
	memset(&g_hostif, 0, sizeof(Hostif_Ctrl_t));

	/* I2C0 Slave mode initialization */
	Chip_I2C_Init(I2C_HOSTIF);	/* Enables clock and resets the peripheral */
	Chip_I2C_SetClockDiv(I2C_HOSTIF, I2C_HOSTIF_CLOCK_DIV);

#if defined(I2CS_DMA_BASED)
	/* Setup DMA channels for I2C slave send and receive */
	/* Setup I2C0 slave channel for the following configuration:
	   - Peripheral DMA request
	   - Single transfer
	   - High channel priority */
	Chip_DMA_EnableChannel(LPC_DMA, I2C_HOSTID_DMAID);
	Chip_DMA_EnableIntChannel(LPC_DMA, I2C_HOSTID_DMAID);
	Chip_DMA_SetupChannelConfig(LPC_DMA, I2C_HOSTID_DMAID,
								(DMA_CFG_PERIPHREQEN | DMA_CFG_TRIGBURST_SNGL | DMA_CFG_CHPRIORITY(0)));
#endif

	Chip_I2CS_SetSlaveAddr(I2C_HOSTIF, 0, I2C_HOSTIF_ADDR);
#if 0
	/* initlize host buffers */
	init_android_broadcast_buffers();
#endif
	/* init host interrupt pin */
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, HOSTIF_IRQ_PORT, HOSTIF_IRQ_PIN);
	
	/* Configure TWI interrupts */
	NVIC_SetPriority(I2C_HOSTIF_IRQn, HOSTIF_IRQ_PRIORITY);

	/* Clear interrupt status and enable slave interrupts */
	Chip_I2CS_ClearStatus(I2C_HOSTIF, I2C_STAT_SLVDESEL);
	Chip_I2C_EnableInt(I2C_HOSTIF, I2C_INTENSET_SLVPENDING | I2C_INTENSET_SLVDESEL);

	/* Enable I2C slave interface */
	Chip_I2CS_Enable(I2C_HOSTIF);
	/* enable I2C IRQ */
	NVIC_EnableIRQ(I2C_HOSTIF_IRQn);
	/* enable I2C hostif to wake-up the sensor hub */
	Chip_SYSCTL_EnableWakeup(STARTERP0_I2C2);	
}

void Hostif_StartTx(uint8_t *pBuf, uint16_t size, int magic)
{
	if (magic != -1) 
		g_hostif.magic = magic;
	if (magic == -1)
		g_hostif.magic &= 0xffff0000;
	g_hostif.txBuff = pBuf;
	if (size != 0)
		g_hostif.txCountSet = size;
	if (pBuf != NULL) 
		g_hostif.oldtxBuff = pBuf;
	g_hostif.txCount = 0;
	g_hostif.txLength = size;
	g_hostif.txLength_next = 0;
	g_hostif.txBuff_next = 0;
}


void Hostif_TxNext(int magic)
{
	g_hostif.magic |= magic;
	g_hostif.txCountSet = g_hostif.txLength_next;
	g_hostif.txCount = 0;

	if (g_hostif.txBuff_next) {
		g_hostif.txBuff = g_hostif.txBuff_next;
		g_hostif.txLength = g_hostif.txLength_next;
		g_hostif.txBuff_next = NULL;
		g_hostif.txLength_next = 0;
	} else {
		g_hostif.txBuff = NULL;
		g_hostif.txLength = 0;
	}

}

void CHostif_StartTxChained(uint8_t *pBuf, uint16_t size, uint8_t *pBuf_next, uint16_t size_next, int magic)
{
	if (magic != -1) 
		g_hostif.magic = magic;
	if (magic == -1)
		g_hostif.magic &= 0xffff0000;
	g_hostif.txBuff = pBuf;
	if (size != 0)
		g_hostif.txCountSet = size;
	if (pBuf != NULL) 
		g_hostif.oldtxBuff = pBuf;
	g_hostif.txCount = 0;
	g_hostif.txLength = size;
	g_hostif.txBuff_next = pBuf_next;
	g_hostif.txLength_next = size_next;

}

void I2C_HOSTIF_IRQHandler(void)
{
	uint32_t state = Chip_I2C_GetPendingInt(I2C_HOSTIF);
	
	/* I2C slave related interrupt */
	while (state & (I2C_INTENSET_SLVPENDING | I2C_INTENSET_SLVDESEL)) {
		
		Chip_I2CS_XferHandler(I2C_HOSTIF, &i2csCallBacks);

		/* Update state */
		state = Chip_I2C_GetPendingInt(I2C_HOSTIF);
	}
}
