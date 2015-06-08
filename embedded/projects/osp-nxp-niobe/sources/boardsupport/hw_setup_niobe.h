/* Open Sensor Platform Project
 * https://github.com/sensorplatforms/open-sensor-platform
 *
 * Copyright (C) 2013 Sensor Platforms Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __HW_SETUP_NIOBE_H__
#define __HW_SETUP_NIOBE_H__
/*-------------------------------------------------------------------------*\
 |    I N C L U D E   F I L E S
\*-------------------------------------------------------------------------*/
#include "board.h"
#include "osp-types.h"
#include <stddef.h>

/*--------------------------------------------------------------------------*\
 |    C O N S T A N T S   &   M A C R O S
\*--------------------------------------------------------------------------*/
#define RAM_START                               0x02000000

/* ######################################################################## */
/* #    T I M I N G S                                                     # */
/* ######################################################################## */
/** System clock & tick configuration */
#define SYSTEM_CLOCK_FREQ                       84000000  //Make sure this matches the settings in system_stm32f10x.c
#define USEC_PER_TICK                           10000       ///< (in uS) = 200Hz
#define TICS_PER_SEC                            ((uint32_t)(1000000/USEC_PER_TICK))
#define MSEC_PER_TICK                           ((uint32_t)(USEC_PER_TICK/1000))

#define US_PER_RTC_TICK                        32  //micro-seconds
#define RTC_PRESCALAR                          (((US_PER_RTC_TICK * HSE_VALUE)/(128000000))-1) ///< RTC period = 125KHz/(RTC_PRESCALAR+1)

/* Tick conversion macros */
#define TICS_TO_SEC(T)                          ((uint32_t)(((T) + (TICS_PER_SEC/2))/TICS_PER_SEC))
#define SEC_TO_TICS(S)                          ((uint32_t)((S) * TICS_PER_SEC))
#define MSEC_TO_TICS(M)                         ((uint32_t)(((M) + (MSEC_PER_TICK-1))/MSEC_PER_TICK))


/* ########################################################################## */
/* #    F L A S H  S T O R A G E                                            # */
/* ########################################################################## */

/* **** NOT PRESENT **** */


/* ########################################################################## */
/* #    D I A G N O S T I C  (LED/GPIOs) I N T E R F A C E                  # */
/* ########################################################################## */
/* Diagnostic GPIOs */

enum {
	LED_RED,
	LED_GREEN=1,
	LED_BLUE=2,
};
#define LED_On(led) do {Board_LED_Set(led, true);} while(0)
#define LED_Off(led) do {Board_LED_Set(led, false);} while(0)
#define LED_Toggle(led) do {Board_LED_Toggle(led);} while(0)
/* Assert LED assignment */
#define AssertIndication()	do {\
			LED_On(LED_RED);\
			LED_Off(LED_GREEN);\
			LED_Off(LED_BLUE);\
		} while(0)
/* ########################################################################## */
/* #    K E Y P A D  I N T E R F A C E                                      # */
/* ########################################################################## */

/* **** NOT PRESENT **** */

/* ########################################################################## */
/* #    A C C E L E R O M E T E R  I N T E R F A C E                        # */
/* ########################################################################## */
/* Interrupt input from Accel (INT1) */

/* ########################################################################## */
/* #    M A G N E T O M E T E R  I N T E R F A C E  (D I G I T A L)         # */
/* ########################################################################## */

/* ########################################################################## */
/* #    G Y R O S C O P E  I N T E R F A C E                                # */
/* ########################################################################## */
/* Gyroscope */


/* ########################################################################## */
/* #    B A R O M E T E R  I N T E R F A C E                                # */
/* ########################################################################## */

/* **** NOT PRESENT **** */

/* ########################################################################## */
/* #    M I S C  I N T E R F A C E                                          # */
/* ########################################################################## */

/* define SENSORHUB_INT_ACTIVE_HIGH for active high INT. Leave undefined.
 * for default active low behavior.
 */
/* Assert IRQ */
#define _SensorHubIntHigh() Chip_GPIO_SetPinState(LPC_GPIO, HOSTIF_IRQ_PORT, HOSTIF_IRQ_PIN, 1)
/* Deassert IRQ */
#define _SensorHubIntLow() Chip_GPIO_SetPinState(LPC_GPIO, HOSTIF_IRQ_PORT, HOSTIF_IRQ_PIN, 0)

#ifdef SENSORHUB_INT_ACTIVE_HIGH
/* Active High */
#define SensorHubAssertInt()	_SensorHubIntHigh()
#define SensorHubDeAssertInt()	_SensorHubIntLow()
#else
/* Active Low */
#define SensorHubAssertInt()	_SensorHubIntLow()
#define SensorHubDeAssertInt()	_SensorHubIntHigh()
#endif
/* ########################################################################## */
/* #    S P I   I N T E R F A C E                                           # */
/* ########################################################################## */

/* **** NOT PRESENT **** */

/* ########################################################################## */
/* #    I 2 C   I N T E R F A C E                                           # */
/* ########################################################################## */
/* I2C Master interface for Sensors */
/** Sensor bus (I2C master) defines */  
#define I2C_MASTER_CLOCK_DIV		2
#define I2C_MCLOCK_SPEED			400000 /* ##KW## 400000 generates 375K, use 430000 for 400K */
/** Sensor bus interface defines */
#define I2C_SENSOR_BUS               LPC_I2C0
#define I2C_SENSOR_BUS_IRQn          I2C0_IRQn
#define I2C_SENSOR_BUS_IRQHandler    I2C0_IRQHandler
#define I2C_SENSOR_BUS_DMAID         DMAREQ_I2C0_MASTER
#define I2C_SENSOR_BUS_DMABYTELIM    (1)	/* Use DMA when sending or receiving more bytes than this define,
                                             I2C ADDR byte is never sent using DMA. Use 0 to always use DMA. */


#if 0
/* In sensorhub.h */
#define I2C_SENSOR_BUS                          (LPC_I2C0)
#define I2C_SENSOR_BUS_BASE                     ((U32)I2C1_BASE)
#define I2C_SENSOR_BUS_DR_ADDR                  ((U32)(I2C_SENSOR_BUS_BASE + I2C_Register_DR))
#define I2C_SENSOR_BUS_CLOCK                    400000  //Fast mode
#define I2C_SENSOR_BUS_GPIO_GRP                 GPIOB
#define RCC_Periph_I2C_SENSOR_BUS_GPIO          RCC_APB2Periph_GPIOB
#define RCC_Periph_I2C_SENSOR_BUS               RCC_APB1Periph_I2C1
#define I2C_SENSOR_BUS_CLK_PIN                  GPIO_Pin_6
#define I2C_SENSOR_BUS_SDA_PIN                  GPIO_Pin_7
/* DMA & interrupt Channel assignments */
#define I2C_SENSOR_BUS_EVENT_IRQ_CH             I2C1_EV_IRQn
#define I2C_SENSOR_BUS_ERROR_IRQ_CH             I2C1_ER_IRQn
#endif
/* I2C Slave interface for host communication (e.g. interface with Android host) */
#define I2C_SLAVE_ADDRESS                       0x18
#define I2C_SLAVE_BUS                           (I2C2)
#define I2C_SLAVE_BUS_BASE                      ((U32)I2C2_BASE)
#define I2C_SLAVE_BUS_DR_ADDR                   ((U32)(I2C_SLAVE_BUS_BASE + I2C_Register_DR))
#define I2C_SLAVE_BUS_CLOCK                     400000  //Don't care
#define I2C_SLAVE_BUS_GPIO_GRP                  GPIOB
#define RCC_Periph_I2C_SLAVE_BUS_GPIO           RCC_APB2Periph_GPIOB
#define RCC_Periph_I2C_SLAVE_BUS                RCC_APB1Periph_I2C2
#define I2C_SLAVE_BUS_CLK_PIN                   GPIO_Pin_10
#define I2C_SLAVE_BUS_SDA_PIN                   GPIO_Pin_11

/* DMA & interrupt Channel assignments */
#define I2C_SLAVE_BUS_EVENT_IRQ_CH              I2C2_EV_IRQn
#define I2C_SLAVE_BUS_ERROR_IRQ_CH              I2C2_ER_IRQn


/* ########################################################################## */
/* #    U A R T  I N T E R F A C E                                          # */
/* ########################################################################## */
/** UART configuration */
/* TX data register addr for DMA operations */
#define USART1_DR_Base                          ((uint32_t)(USART1_BASE + offsetof(USART_TypeDef, DR)))
#define USART2_DR_Base                          ((uint32_t)(USART2_BASE + offsetof(USART_TypeDef, DR)))
#define USART3_DR_Base                          ((uint32_t)(USART3_BASE + offsetof(USART_TypeDef, DR)))

#define DBG_UART_BAUD                           921600

#define DBG_IF_UART                             USART1
#define RCC_Periph_DBG_UART                     RCC_APB2Periph_USART1
#define DBG_UART_TX_PIN                         GPIO_Pin_9     ///< PA.09
#define DBG_UART_RX_PIN                         GPIO_Pin_10    ///< PA.10
#define DBG_UART_DR_Base                        USART1_DR_Base

#define RCC_Periph_UART_GPIO                    RCC_APB2Periph_GPIOA
#define DBG_UART_GPIO_GRP                       GPIOA

#ifdef UART_DMA_ENABLE
/** DMA & Channel assignments */
# define RCC_UART_Periph_DMA                    RCC_AHBPeriph_DMA1
# define DBG_UART_TX_DMA_Channel                DMA1_Channel4
/* Interrupt channel assignment */
# define DBG_UART_TX_DMA_IRQChannel             DMA1_Channel4_IRQn //< Dependent on UART selection
#endif

/* IRQ Channel for TX and RX interrupts */
#define DBG_UART_IRQChannel                     USART1_IRQn

/* Flush macro used in assert */
#ifdef UART_DMA_ENABLE
# define FlushUart()                            DMA_Cmd(DBG_UART_TX_DMA_Channel, DISABLE)
#else
# define FlushUart()
#endif


/* ########################################################################## */
/* #    I N T E R R U P T  A S S I G N M E N T S                            # */
/* ########################################################################## */
/** Interrupt Channels, Group & Priority assignments:
 Note that lower number = higher priority (subpriority). All priorities are in relationship to
 each other & the priority group so changing one may affect others.
 */
#define SYSTEM_INTERRUPT_PRIORITY_GRP           NVIC_PriorityGroup_2    //2-bit Preemption, 2-bit Sub

#define DBG_UART_DMA_INT_PREEMPT_PRIORITY       3   // Uart DMA channel preemption prio. group
#define DBG_UART_TX_DMA_INT_SUB_PRIORITY        2  // Uart DMA channel subprio within group

#define DBG_UART_INT_PREEMPT_PRIORITY           3   // Lowest group Preemption priority
#define DBG_UART_INT_SUB_PRIORITY               3  // Lowest Priority within group

#define ACCEL_A_INT_PREEMPT_PRIO                1   ///< Accelerometer A preemption priority group
#define ACCEL_A_INT_SUB_PRIORITY                0   ///< Accelerometer A subpriority within group

/* Priorities for I2C Bus interrupts */
#define I2C_SENSOR_BUS_INT_PREEMPT_PRIORITY     0   ///< I2C IRQ, I2C TX DMA and I2C RX DMA priority
#define I2C_SENSOR_BUS_EVENT_INT_SUB_PRIORITY   0   ///< I2C EV IRQ subpriority !!!! IMPORTANT - THIS NEEDS TO BE HIGHEST PRIORITY !!!!
#define I2C_SENSOR_BUS_ERROR_INT_SUB_PRIORITY   1   ///< I2C ER IRQ subpriority

#define I2C_SLAVE_BUS_INT_PREEMPT_PRIORITY      2   ///< I2C IRQ, I2C TX DMA and I2C RX DMA priority
#define I2C_SLAVE_BUS_EVENT_INT_SUB_PRIORITY    0   ///< I2C EV IRQ subpriority
#define I2C_SLAVE_BUS_ERROR_INT_SUB_PRIORITY    1   ///< I2C ER IRQ subpriority

/* Priorities for Mag DRDY interrupt */
#define MAG_DRDY_INT_PREEMPT_PRIORITY         1   ///< Mag DRDY IRQ preemption priority
#define MAG_DRDY_INT_SUB_PRIORITY             1   ///< Mag DRDY IRQ subpriority

/* Priorities for Mag DRDY interrupt */
#define GYRO_DRDY_INT_PREEMPT_PRIORITY        1   ///< Gyro DRDY IRQ preemption priority
#define GYRO_DRDY_INT_SUB_PRIORITY            2   ///< Gyro DRDY IRQ subpriority

#define NVIC_CH_ENABLE(IRQCh)                   NVIC_EnableIRQ(IRQCh)
#define NVIC_CH_DISABLE(IRQCh)                  NVIC_DisableIRQ(IRQCh)

/* ########################################################################## */
/* #    P O W E R  C O N T R O L                                            # */
/* ########################################################################## */

/* **** NOT PRESENT **** */


/* ########################################################################## */
/* #    M I S C E L L A N E O U S                                           # */
/* ########################################################################## */
/* Device Unique ID register for STM32L series devices */
#define DEV_UID_OFFSET                          0x1FFFF7E8

/* ########################################################################## */
/* #    U S B  I N T E R F A C E                                            # */
/* ########################################################################## */

/* **** NOT PRESENT **** */

/*-------------------------------------------------------------------------------------------------*\
 |    T Y P E   D E F I N I T I O N S
\*-------------------------------------------------------------------------------------------------*/
typedef enum MsgContextTag
{
    CTX_THREAD,     ///< Message sent from within a thread context
    CTX_ISR         ///< Message sent from ISR
} MsgContext;

typedef union DeviceUidTag
{
    uint32_t uidWords[3];
    uint8_t  uidBytes[12];
} DeviceUid_t;

/*---------------------------------------------------------------------------*\
 |    E X T E R N A L   V A R I A B L E S   &   F U N C T I O N S
\*---------------------------------------------------------------------------*/
extern DeviceUid_t *gDevUniqueId;

/*---------------------------------------------------------------------------*\
 |    P U B L I C   V A R I A B L E S   D E F I N I T I O N S
\*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*\
 |    P U B L I C   F U N C T I O N   D E C L A R A T I O N S
\*---------------------------------------------------------------------------*/
/*****************************************************************************
 * @fn      GetContext
 *          Identifies if we are currently in ISR or Thread context and
 *          returns the corresponding enum value for it. The logic is
 *          based on the fact that ISR uses system stack and Thread
 *          use their allocated stack. We only need to read the current
 *          value of SP to figure out whether we are in a Thread or ISR.
 *          (Maybe there is a better way... but this is good enough for now)
 *
 * @param   none
 *
 * @return  CTX_THREAD or CTX_ISR
 *
 *****************************************************************************/
static __inline MsgContext GetContext( void )
{
    extern uint32_t gStackMem;

    return (__current_sp() < (uint32_t)&gStackMem)? CTX_THREAD : CTX_ISR;
}



/*--------------------------------------------------------------------------*\
 |    E N D   O F   F I L E
\*--------------------------------------------------------------------------*/
#endif