/*
 * @brief Basic CMSIS include file
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
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

#ifndef __CMSIS_H_
#define __CMSIS_H_

#include "lpc_types.h"
#include "sys_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup CMSIS_412X_ALL CHIP: LPC412X CMSIS include file
 * @ingroup CHIP_412X_Drivers
 * @{
 */

#if defined(__ARMCC_VERSION)
// Kill warning "#pragma push with no matching #pragma pop"
  #pragma diag_suppress 2525
#if 0
  #pragma push
#endif
  #pragma anon_unions
#elif defined(__CWCC__)
  #pragma push
  #pragma cpp_extensions on
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#elif defined(__IAR_SYSTEMS_ICC__)
//  #pragma push // FIXME not usable for IAR
  #pragma language=extended
#else
  #error Not supported compiler type
#endif

#if !defined(CORE_M4)
#if !defined(CORE_M0PLUS)
#error Please #define CORE_M4 or CORE_M0PLUS
#endif
#endif

/** @defgroup CMSIS_412X CHIP: LPC412X Cortex CMSIS definitions
 * @ingroup CMSIS_412X_ALL
 * @{
 */

#if defined(CORE_M4)
/* Configuration of the Cortex-M0+ Processor and Core Peripherals */
// FIXME - it is highly suspect these settings are wrong
#define __CM4_REV                 0x0000	/*!< Cortex-M4 Core Revision                          */
#define __MPU_PRESENT             0			/*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          3			/*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0			/*!< Set to 1 if different SysTick Config is used     */
#define __FPU_PRESENT             1
#define __VTOR_PRESENT            1
#else 
#if defined(CORE_M0PLUS)
/* Configuration of the Cortex-M0+ Processor and Core Peripherals */
#define __CM0PLUS_REV             0x0000	/*!< Cortex-M0PLUS Core Revision                          */
#define __MPU_PRESENT             0			/*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          2			/*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0			/*!< Set to 1 if different SysTick Config is used     */
#define __VTOR_PRESENT            1
#endif
#endif

/**
 * @}
 */

/** @defgroup CMSIS_412X_IRQ CHIP: LPC412X peripheral interrupt numbers
 * @ingroup CMSIS_412X_ALL
 * @{
 */

typedef enum {
	/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
	Reset_IRQn                    = -15,	/*!< 1 Reset Vector, invoked on Power up and warm reset */
	NonMaskableInt_IRQn           = -14,	/*!< 2 Non Maskable Interrupt                           */
	HardFault_IRQn                = -13,	/*!< 3 Cortex-M0 Hard Fault Interrupt                   */
	SVCall_IRQn                   = -5,		/*!< 11 Cortex-M0 SV Call Interrupt                     */
	PendSV_IRQn                   = -2,		/*!< 14 Cortex-M0 Pend SV Interrupt                     */
	SysTick_IRQn                  = -1,		/*!< 15 Cortex-M0 System Tick Interrupt                 */
	
	/******  LPC412X Specific Interrupt Numbers ********************************************************/
	WWDT_IRQn                     = 0,		/*!< WWDT                                             */
	BOD_IRQn                      = 1,		/*!< BOD                                              */
	Reserved0_IRQn                = 2,		/*!< Reserved Interrupt                               */
	DMA_IRQn                      = 3,		/*!< DMA                                              */
	GINT0_IRQn                    = 4,		/*!< GINT0                                            */
	PININT0_IRQn                  = 5,		/*!< PININT0                                          */
	PININT1_IRQn                  = 6,		/*!< PININT1                                          */
	PININT2_IRQn                  = 7,		/*!< PININT2                                          */
	PININT3_IRQn                  = 8,		/*!< PININT3                                          */
	UTICK_IRQn                    = 9,		/*!< UTICK                                            */
	MRT_IRQn                      = 10,		/*!< MRT                                              */
	CTIMER0_IRQn                  = 11,		/*!< CTMR0                                            */
	CTIMER1_IRQn                  = 12,		/*!< CTMR1                                            */
	CTIMER2_IRQn                  = 13,		/*!< CTMR2                                            */
	CTIMER3_IRQn                  = 14,		/*!< CTMR3                                            */
	CTIMER4_IRQn                  = 15,		/*!< CTMR4                                            */

	SCT_IRQn                      = 16,		/*!< SCT                                              */
	UART0_IRQn                    = 17,		/*!< UART0                                            */
	UART1_IRQn                    = 18,		/*!< UART1                                            */
	UART2_IRQn                    = 19,		/*!< UART2                                            */
	UART3_IRQn                    = 20,		/*!< UART3                                            */
	I2C0_IRQn                     = 21,		/*!< I2C0                                             */
	I2C1_IRQn                     = 22,		/*!< I2C1                                             */
	I2C2_IRQn                     = 23,		/*!< I2C2                                             */
	SPI0_IRQn                     = 24,		/*!< SPI0                                             */
	SPI1_IRQn                     = 25,		/*!< SPI1                                             */
	ADC_SEQA_IRQn                 = 26,		/*!< ADC SEQA                                         */
	ADC_SEQB_IRQn                 = 27,		/*!< ADC SEQB                                         */
	ADC_THCMP_OVR_IRQn            = 28,		/*!< ADC THCMP OVERRUN                                */
	RTC_IRQn                      = 29,		/*!< RTC                                              */
	MBOX_IRQn                     = 31,		/*!< MBOX                                             */
  /* Below is for CM4 only */
#if defined(CORE_M4)	
	GINT1_IRQn                    = 32,		/*!< GINT1                                            */
	PININT4_IRQn                  = 33,		/*!< External Interrupt 4                             */
	PININT5_IRQn                  = 34,		/*!< External Interrupt 5                             */
	PININT6_IRQn                  = 35,		/*!< External Interrupt 6                             */
	PININT7_IRQn                  = 36,		/*!< External Interrupt 7                             */
	OSTIMER_IRQn                  = 40,		/*!< OSTIMER or RIT                                   */
#endif
} IRQn_Type;
/**
 * @}
 */

#if defined(CORE_M4)	
#include "core_cm4.h"				/*!< Cortex-M4 processor and core peripherals        */
#else
#if defined(CORE_M0PLUS)	
#include "core_cm0plus.h"				/*!< Cortex-M0 Plus processor and core peripherals        */
#endif
#endif
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CMSIS_H_ */
