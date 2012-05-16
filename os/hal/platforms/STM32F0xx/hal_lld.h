/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    STM32F0xx/hal_lld.h
 * @brief   STM32F0xx HAL subsystem low level driver header.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - STM32_LSECLK.
 *          - STM32_HSECLK.
 *          - STM32_HSE_BYPASS (optionally).
 *          .
 *          One of the following macros must also be defined:
 *          - STM32F0XX for Entry Level devices.
 *          .
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "stm32.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Defines the support for realtime counters in the HAL.
 */
#define HAL_IMPLEMENTS_COUNTERS FALSE

/**
 * @name    Platform identification
 * @{
 */
#define PLATFORM_NAME           "STM32F0 Entry Level"
/** @} */

/**
 * @name    Absolute Maximum Ratings
 * @{
 */
/**
 * @brief   Maximum system clock frequency.
 */
#define STM32_SYSCLK_MAX        48000000

/**
 * @brief   Maximum HSE clock frequency.
 */
#define STM32_HSECLK_MAX        32000000

/**
 * @brief   Minimum HSE clock frequency.
 */
#define STM32_HSECLK_MIN        1000000

/**
 * @brief   Maximum LSE clock frequency.
 */
#define STM32_LSECLK_MAX        1000000

/**
 * @brief   Minimum LSE clock frequency.
 */
#define STM32_LSECLK_MIN        32768

/**
 * @brief   Maximum PLLs input clock frequency.
 */
#define STM32_PLLIN_MAX         25000000

/**
 * @brief   Maximum PLLs input clock frequency.
 */
#define STM32_PLLIN_MIN         1000000

/**
 * @brief   Maximum PLL output clock frequency.
 */
#define STM32_PLLOUT_MAX        48000000

/**
 * @brief   Maximum PLL output clock frequency.
 */
#define STM32_PLLOUT_MIN        16000000

/**
 * @brief   Maximum APB clock frequency.
 */
#define STM32_PCLK_MAX          48000000

/**
 * @brief   Maximum ADC clock frequency.
 */
#define STM32_ADCCLK_MAX        14000000
/** @} */

/**
 * @name    Internal clock sources
 * @{
 */
#define STM32_HSICLK            8000000     /**< High speed internal clock. */
#define STM32_HSI14CLK          14000000    /**< 14MHz speed internal clock.*/
#define STM32_LSICLK            40000       /**< Low speed internal clock.  */
/** @} */

/**
 * @name    PWR_CR register bits definitions
 * @{
 */
#define STM32_PLS_MASK          (7 << 5)    /**< PLS bits mask.             */
#define STM32_PLS_LEV0          (0 << 5)    /**< PVD level 0.               */
#define STM32_PLS_LEV1          (1 << 5)    /**< PVD level 1.               */
#define STM32_PLS_LEV2          (2 << 5)    /**< PVD level 2.               */
#define STM32_PLS_LEV3          (3 << 5)    /**< PVD level 3.               */
#define STM32_PLS_LEV4          (4 << 5)    /**< PVD level 4.               */
#define STM32_PLS_LEV5          (5 << 5)    /**< PVD level 5.               */
#define STM32_PLS_LEV6          (6 << 5)    /**< PVD level 6.               */
#define STM32_PLS_LEV7          (7 << 5)    /**< PVD level 7.               */
/** @} */

/**
 * @name    RCC_CFGR register bits definitions
 * @{
 */
#define STM32_SW_HSI            (0 << 0)    /**< SYSCLK source is HSI.      */
#define STM32_SW_HSE            (1 << 0)    /**< SYSCLK source is HSE.      */
#define STM32_SW_PLL            (2 << 0)    /**< SYSCLK source is PLL.      */

#define STM32_HPRE_DIV1         (0 << 4)    /**< SYSCLK divided by 1.       */
#define STM32_HPRE_DIV2         (8 << 4)    /**< SYSCLK divided by 2.       */
#define STM32_HPRE_DIV4         (9 << 4)    /**< SYSCLK divided by 4.       */
#define STM32_HPRE_DIV8         (10 << 4)   /**< SYSCLK divided by 8.       */
#define STM32_HPRE_DIV16        (11 << 4)   /**< SYSCLK divided by 16.      */
#define STM32_HPRE_DIV64        (12 << 4)   /**< SYSCLK divided by 64.      */
#define STM32_HPRE_DIV128       (13 << 4)   /**< SYSCLK divided by 128.     */
#define STM32_HPRE_DIV256       (14 << 4)   /**< SYSCLK divided by 256.     */
#define STM32_HPRE_DIV512       (15 << 4)   /**< SYSCLK divided by 512.     */

#define STM32_PPRE_DIV1         (0 << 8)    /**< HCLK divided by 1.         */
#define STM32_PPRE_DIV2         (4 << 8)    /**< HCLK divided by 2.         */
#define STM32_PPRE_DIV4         (5 << 8)    /**< HCLK divided by 4.         */
#define STM32_PPRE_DIV8         (6 << 8)    /**< HCLK divided by 8.         */
#define STM32_PPRE_DIV16        (7 << 8)    /**< HCLK divided by 16.        */

#define STM32_ADCPRE_DIV2       (0 << 14)   /**< PCLK divided by 2.         */
#define STM32_ADCPRE_DIV4       (1 << 14)   /**< PCLK divided by 4.         */

#define STM32_PLLSRC_HSI        (0 << 16)   /**< PLL clock source is HSI.   */
#define STM32_PLLSRC_HSE        (1 << 16)   /**< PLL clock source is HSE.   */

#define STM32_PLLXTPRE_DIV1     (0 << 17)   /**< HSE divided by 1.          */
#define STM32_PLLXTPRE_DIV2     (1 << 17)   /**< HSE divided by 2.          */

#define STM32_MCOSEL_NOCLOCK    (0 << 24)   /**< No clock on MCO pin.       */
#define STM32_MCOSEL_HSI14      (3 << 24)   /**< HSI14 clock on MCO pin.    */
#define STM32_MCOSEL_SYSCLK     (4 << 24)   /**< SYSCLK on MCO pin.         */
#define STM32_MCOSEL_HSI        (5 << 24)   /**< HSI clock on MCO pin.      */
#define STM32_MCOSEL_HSE        (6 << 24)   /**< HSE clock on MCO pin.      */
#define STM32_MCOSEL_PLLDIV2    (7 << 24)   /**< PLL/2 clock on MCO pin.    */
/** @} */

/**
 * @name    RCC_BDCR register bits definitions
 * @{
 */
#define STM32_RTCSEL_MASK       (3 << 8)    /**< RTC clock source mask.     */
#define STM32_RTCSEL_NOCLOCK    (0 << 8)    /**< No clock.                  */
#define STM32_RTCSEL_LSE        (1 << 8)    /**< LSE used as RTC clock.     */
#define STM32_RTCSEL_LSI        (2 << 8)    /**< LSI used as RTC clock.     */
#define STM32_RTCSEL_HSEDIV     (3 << 8)    /**< HSE divided by 32 used as
                                                 RTC clock.                 */
/** @} */

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    STM32F0xx capabilities
 * @{
 */
/* ADC attributes.*/
#define STM32_HAS_ADC1          TRUE
#define STM32_HAS_ADC2          FALSE
#define STM32_HAS_ADC3          FALSE

/* CAN attributes.*/
#define STM32_HAS_CAN1          FALSE
#define STM32_HAS_CAN2          FALSE

/* DAC attributes.*/
#define STM32_HAS_DAC           TRUE

/* DMA attributes.*/
#define STM32_ADVANCED_DMA      FALSE
#define STM32_HAS_DMA1          TRUE
#define STM32_HAS_DMA2          FALSE

/* ETH attributes.*/
#define STM32_HAS_ETH           FALSE

/* EXTI attributes.*/
#define STM32_EXTI_NUM_CHANNELS 28

/* GPIO attributes.*/
#define STM32_HAS_GPIOA         TRUE
#define STM32_HAS_GPIOB         TRUE
#define STM32_HAS_GPIOC         TRUE
#define STM32_HAS_GPIOD         TRUE
#define STM32_HAS_GPIOE         FALSE
#define STM32_HAS_GPIOF         TRUE
#define STM32_HAS_GPIOG         FALSE
#define STM32_HAS_GPIOH         FALSE
#define STM32_HAS_GPIOI         FALSE

/* I2C attributes.*/
#define STM32_HAS_I2C1          TRUE
#define STM32_I2C1_RX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 3))
#define STM32_I2C1_RX_DMA_CHN   0x00000000
#define STM32_I2C1_TX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 2))
#define STM32_I2C1_TX_DMA_CHN   0x00000000

#define STM32_HAS_I2C2          TRUE
#define STM32_I2C2_RX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 5))
#define STM32_I2C2_RX_DMA_CHN   0x00000000
#define STM32_I2C2_TX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 4))
#define STM32_I2C2_TX_DMA_CHN   0x00000000

#define STM32_HAS_I2C3          FALSE
#define STM32_I2C3_RX_DMA_MSK   0
#define STM32_I2C3_RX_DMA_CHN   0x00000000
#define STM32_I2C3_TX_DMA_MSK   0
#define STM32_I2C3_TX_DMA_CHN   0x00000000

/* RTC attributes.*/
#define STM32_HAS_RTC           TRUE
#define STM32_RTC_HAS_SUBSECONDS FALSE
#define STM32_RTC_IS_CALENDAR   TRUE

/* SDIO attributes.*/
#define STM32_HAS_SDIO          FALSE

/* SPI attributes.*/
#define STM32_HAS_SPI1          TRUE
#define STM32_SPI1_RX_DMA_MSK   STM32_DMA_STREAM_ID_MSK(1, 2)
#define STM32_SPI1_RX_DMA_CHN   0x00000000
#define STM32_SPI1_TX_DMA_MSK   STM32_DMA_STREAM_ID_MSK(1, 3)
#define STM32_SPI1_TX_DMA_CHN   0x00000000

#define STM32_HAS_SPI2          TRUE
#define STM32_SPI2_RX_DMA_MSK   STM32_DMA_STREAM_ID_MSK(1, 4)
#define STM32_SPI2_RX_DMA_CHN   0x00000000
#define STM32_SPI2_TX_DMA_MSK   STM32_DMA_STREAM_ID_MSK(1, 5)
#define STM32_SPI2_TX_DMA_CHN   0x00000000

#define STM32_HAS_SPI3          FALSE
#define STM32_SPI3_RX_DMA_MSK   0
#define STM32_SPI3_RX_DMA_CHN   0x00000000
#define STM32_SPI3_TX_DMA_MSK   0
#define STM32_SPI3_TX_DMA_CHN   0x00000000

/* TIM attributes.*/
#define STM32_HAS_TIM1          TRUE
#define STM32_HAS_TIM2          TRUE
#define STM32_HAS_TIM3          TRUE
#define STM32_HAS_TIM4          FALSE
#define STM32_HAS_TIM5          FALSE
#define STM32_HAS_TIM6          TRUE
#define STM32_HAS_TIM7          FALSE
#define STM32_HAS_TIM8          FALSE
#define STM32_HAS_TIM9          FALSE
#define STM32_HAS_TIM10         FALSE
#define STM32_HAS_TIM11         FALSE
#define STM32_HAS_TIM12         FALSE
#define STM32_HAS_TIM13         FALSE
#define STM32_HAS_TIM14         TRUE
#define STM32_HAS_TIM15         TRUE
#define STM32_HAS_TIM16         TRUE
#define STM32_HAS_TIM17         TRUE

/* USART attributes.*/
#define STM32_HAS_USART1        TRUE
#define STM32_USART1_RX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 3))
#define STM32_USART1_RX_DMA_CHN 0x00000000
#define STM32_USART1_TX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 2))
#define STM32_USART1_TX_DMA_CHN 0x00000000

#define STM32_HAS_USART2        TRUE
#define STM32_USART2_RX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 5))
#define STM32_USART2_RX_DMA_CHN 0x00000000
#define STM32_USART2_TX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 4))
#define STM32_USART2_TX_DMA_CHN 0x00000000

#define STM32_HAS_USART3        FALSE
#define STM32_USART3_RX_DMA_MSK 0
#define STM32_USART3_RX_DMA_CHN 0x00000000
#define STM32_USART3_TX_DMA_MSK 0
#define STM32_USART3_TX_DMA_CHN 0x00000000

#define STM32_HAS_UART4         FALSE
#define STM32_UART4_RX_DMA_MSK  0
#define STM32_UART4_RX_DMA_CHN  0x00000000
#define STM32_UART4_TX_DMA_MSK  0
#define STM32_UART4_TX_DMA_CHN  0x00000000

#define STM32_HAS_UART5         FALSE
#define STM32_UART5_RX_DMA_MSK  0
#define STM32_UART5_RX_DMA_CHN  0x00000000
#define STM32_UART5_TX_DMA_MSK  0
#define STM32_UART5_TX_DMA_CHN  0x00000000

#define STM32_HAS_USART6        FALSE
#define STM32_USART6_RX_DMA_MSK 0
#define STM32_USART6_RX_DMA_CHN 0x00000000
#define STM32_USART6_TX_DMA_MSK 0
#define STM32_USART6_TX_DMA_CHN 0x00000000

/* USB attributes.*/
#define STM32_HAS_USB           TRUE
#define STM32_HAS_OTG1          FALSE
#define STM32_HAS_OTG2          FALSE
/** @} */

/*===========================================================================*/
/* Platform specific friendly IRQ names.                                     */
/*===========================================================================*/

/**
 * @name  IRQ VECTOR names
 * @{
 */
#define WWDG_IRQHandler         Vector40    /**< Window Watchdog.           */
#define PVD_IRQHandler          Vector44    /**< PVD through EXTI Line
                                                 detect.                    */
#define RTC_IRQHandler          Vector48    /**< RTC.                       */
#define FLASH_IRQHandler        Vector4C    /**< Flash.                     */
#define RCC_IRQHandler          Vector50    /**< RCC.                       */
#define EXTI0_1_IRQHandler      Vector54    /**< EXTI Line 0..1.            */
#define EXTI2_3_IRQHandler      Vector58    /**< EXTI Line 2..3.            */
#define EXTI4_15_IRQHandler     Vector5C    /**< EXTI Line 4..15.           */
#define TS_IRQHandler           Vector60    /**< TS.                        */
#define DMA1_Ch1_IRQHandler     Vector64    /**< DMA1 Channel 1.            */
#define DMA1_Ch2_3_IRQHandler   Vector68    /**< DMA1 Channels 2 and 3.     */
#define DMA1_Ch4_5_IRQHandler   Vector6C    /**< DMA1 Channels 4 and 5.     */
#define ADC1_COMP_IRQHandler    Vector70    /**< ADC1 comparators 1 and 2.  */
#define TIM1_BRK_UP_TRG_COM_IRQHandler Vector74 /**< TIM1 common.           */
#define TIM1_CC_IRQHandler      Vector78    /**< TIM1 Capture Compare.      */
#define TIM2_IRQHandler         Vector7C    /**< TIM2.                      */
#define TIM3_IRQHandler         Vector80    /**< TIM3.                      */
#define TIM6_DAC_IRQHandler     Vector84    /**< TIM6 and DAC.              */
#define TIM14_IRQHandler        Vector8C    /**< TIM14.                     */
#define TIM15_IRQHandler        Vector90    /**< TIM15.                     */
#define TIM16_IRQHandler        Vector94    /**< TIM16.                     */
#define TIM17_IRQHandler        Vector98    /**< TIM17.                     */
#define I2C1_IRQHandler         Vector9C    /**< I2C1.                      */
#define I2C2_IRQHandler         VectorA0    /**< I2C2.                      */
#define SPI1_IRQHandler         VectorA4    /**< SPI1.                      */
#define SPI2_IRQHandler         VectorA8    /**< SPI2.                      */
#define USART1_IRQHandler       VectorAC    /**< USART1.                    */
#define USART2_IRQHandler       VectorB0    /**< USART2.                    */
#define CEC_IRQHandler          VectorB8    /**< CEC.                       */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Disables the PWR/RCC initialization in the HAL.
 */
#if !defined(STM32_NO_INIT) || defined(__DOXYGEN__)
#define STM32_NO_INIT               FALSE
#endif

/**
 * @brief   Enables or disables the programmable voltage detector.
 */
#if !defined(STM32_PVD_ENABLE) || defined(__DOXYGEN__)
#define STM32_PVD_ENABLE            FALSE
#endif

/**
 * @brief   Sets voltage level for programmable voltage detector.
 */
#if !defined(STM32_PLS) || defined(__DOXYGEN__)
#define STM32_PLS                   STM32_PLS_LEV0
#endif

/**
 * @brief   Enables or disables the HSI clock source.
 */
#if !defined(STM32_HSI_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI_ENABLED           TRUE
#endif

/**
 * @brief   Enables or disables the LSI clock source.
 */
#if !defined(STM32_LSI_ENABLED) || defined(__DOXYGEN__)
#define STM32_LSI_ENABLED           FALSE
#endif

/**
 * @brief   Enables or disables the HSE clock source.
 */
#if !defined(STM32_HSE_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSE_ENABLED           TRUE
#endif

/**
 * @brief   Enables or disables the LSE clock source.
 */
#if !defined(STM32_LSE_ENABLED) || defined(__DOXYGEN__)
#define STM32_LSE_ENABLED           FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*
 * HSI related checks.
 */
#if STM32_HSI_ENABLED
#else /* !STM32_HSI_ENABLED */

#if STM32_SW == STM32_SW_HSI
#error "HSI not enabled, required by STM32_SW"
#endif

#if (STM32_SW == STM32_SW_PLL) && (STM32_PLLSRC == STM32_PLLSRC_HSI)
#error "HSI not enabled, required by STM32_SW and STM32_PLLSRC"
#endif

#if (STM32_MCOSEL == STM32_MCOSEL_HSI) ||                                   \
    ((STM32_MCOSEL == STM32_MCOSEL_PLLDIV2) &&                              \
     (STM32_PLLSRC == STM32_PLLSRC_HSI))
#error "HSI not enabled, required by STM32_MCOSEL"
#endif

#endif /* !STM32_HSI_ENABLED */

/*
 * HSE related checks.
 */
#if STM32_HSE_ENABLED

#if STM32_HSECLK == 0
#error "HSE frequency not defined"
#elif (STM32_HSECLK < STM32_HSECLK_MIN) || (STM32_HSECLK > STM32_HSECLK_MAX)
#error "STM32_HSECLK outside acceptable range (STM32_HSECLK_MIN...STM32_HSECLK_MAX)"
#endif

#else /* !STM32_HSE_ENABLED */

#if STM32_SW == STM32_SW_HSE
#error "HSE not enabled, required by STM32_SW"
#endif

#if (STM32_SW == STM32_SW_PLL) && (STM32_PLLSRC == STM32_PLLSRC_HSE)
#error "HSE not enabled, required by STM32_SW and STM32_PLLSRC"
#endif

#if (STM32_MCOSEL == STM32_MCOSEL_HSE) ||                                   \
    ((STM32_MCOSEL == STM32_MCOSEL_PLLDIV2) && (STM32_PLLSRC == STM32_PLLSRC_HSE))
#error "HSE not enabled, required by STM32_MCOSEL"
#endif

#if STM32_RTCSEL == STM32_RTCSEL_HSEDIV
#error "HSE not enabled, required by STM32_RTCSEL"
#endif

#endif /* !STM32_HSE_ENABLED */

/*
 * LSI related checks.
 */
#if STM32_LSI_ENABLED
#else /* !STM32_LSI_ENABLED */

#if STM32_RTCSEL == STM32_RTCSEL_LSI
#error "LSI not enabled, required by STM32_RTCSEL"
#endif

#endif /* !STM32_LSI_ENABLED */

/*
 * LSE related checks.
 */
#if STM32_LSE_ENABLED

#if (STM32_LSECLK == 0)
#error "LSE frequency not defined"
#endif

#if (STM32_LSECLK < STM32_LSECLK_MIN) || (STM32_LSECLK > STM32_LSECLK_MAX)
#error "STM32_LSECLK outside acceptable range (STM32_LSECLK_MIN...STM32_LSECLK_MAX)"
#endif

#else /* !STM32_LSE_ENABLED */

#if STM32_RTCSEL == STM32_RTCSEL_LSE
#error "LSE not enabled, required by STM32_RTCSEL"
#endif

#endif /* !STM32_LSE_ENABLED */

/* PLL activation conditions.*/
#if STM32_USB_CLOCK_REQUIRED ||                                             \
    (STM32_SW == STM32_SW_PLL) ||                                           \
    (STM32_MCOSEL == STM32_MCOSEL_PLLDIV2) ||                               \
    defined(__DOXYGEN__)
/**
 * @brief   PLL activation flag.
 */
#define STM32_ACTIVATE_PLL          TRUE
#else
#define STM32_ACTIVATE_PLL          FALSE
#endif

/* HSE prescaler setting check.*/
#if (STM32_PLLXTPRE != STM32_PLLXTPRE_DIV1) &&                              \
    (STM32_PLLXTPRE != STM32_PLLXTPRE_DIV2)
#error "invalid STM32_PLLXTPRE value specified"
#endif

/**
 * @brief   PLLMUL field.
 */
#if ((STM32_PLLMUL_VALUE >= 2) && (STM32_PLLMUL_VALUE <= 16)) ||            \
    defined(__DOXYGEN__)
#define STM32_PLLMUL                ((STM32_PLLMUL_VALUE - 2) << 18)
#else
#error "invalid STM32_PLLMUL_VALUE value specified"
#endif

/**
 * @brief   PLL input clock frequency.
 */
#if (STM32_PLLSRC == STM32_PLLSRC_HSE) || defined(__DOXYGEN__)
#if STM32_PLLXTPRE == STM32_PLLXTPRE_DIV1
#define STM32_PLLCLKIN              (STM32_HSECLK / 1)
#else
#define STM32_PLLCLKIN              (STM32_HSECLK / 2)
#endif
#elif STM32_PLLSRC == STM32_PLLSRC_HSI
#define STM32_PLLCLKIN              (STM32_HSICLK / 2)
#else
#error "invalid STM32_PLLSRC value specified"
#endif

/* PLL input frequency range check.*/
#if (STM32_PLLCLKIN < STM32_PLLIN_MIN) || (STM32_PLLCLKIN > STM32_PLLIN_MAX)
#error "STM32_PLLCLKIN outside acceptable range (STM32_PLLIN_MIN...STM32_PLLIN_MAX)"
#endif

/**
 * @brief   PLL output clock frequency.
 */
#define STM32_PLLCLKOUT             (STM32_PLLCLKIN * STM32_PLLMUL_VALUE)

/* PLL output frequency range check.*/
#if (STM32_PLLCLKOUT < STM32_PLLOUT_MIN) || (STM32_PLLCLKOUT > STM32_PLLOUT_MAX)
#error "STM32_PLLCLKOUT outside acceptable range (STM32_PLLOUT_MIN...STM32_PLLOUT_MAX)"
#endif

/**
 * @brief   System clock source.
 */
#if (STM32_SW == STM32_SW_PLL) || defined(__DOXYGEN__)
#define STM32_SYSCLK                STM32_PLLCLKOUT
#elif (STM32_SW == STM32_SW_HSI)
#define STM32_SYSCLK                STM32_HSICLK
#elif (STM32_SW == STM32_SW_HSE)
#define STM32_SYSCLK                STM32_HSECLK
#else
#error "invalid STM32_SYSCLK_SW value specified"
#endif

/* Check on the system clock.*/
#if STM32_SYSCLK > STM32_SYSCLK_MAX
#error "STM32_SYSCLK above maximum rated frequency (STM32_SYSCLK_MAX)"
#endif

/**
 * @brief   AHB frequency.
 */
#if (STM32_HPRE == STM32_HPRE_DIV1) || defined(__DOXYGEN__)
#define STM32_HCLK                  (STM32_SYSCLK / 1)
#elif STM32_HPRE == STM32_HPRE_DIV2
#define STM32_HCLK                  (STM32_SYSCLK / 2)
#elif STM32_HPRE == STM32_HPRE_DIV4
#define STM32_HCLK                  (STM32_SYSCLK / 4)
#elif STM32_HPRE == STM32_HPRE_DIV8
#define STM32_HCLK                  (STM32_SYSCLK / 8)
#elif STM32_HPRE == STM32_HPRE_DIV16
#define STM32_HCLK                  (STM32_SYSCLK / 16)
#elif STM32_HPRE == STM32_HPRE_DIV64
#define STM32_HCLK                  (STM32_SYSCLK / 64)
#elif STM32_HPRE == STM32_HPRE_DIV128
#define STM32_HCLK                  (STM32_SYSCLK / 128)
#elif STM32_HPRE == STM32_HPRE_DIV256
#define STM32_HCLK                  (STM32_SYSCLK / 256)
#elif STM32_HPRE == STM32_HPRE_DIV512
#define STM32_HCLK                  (STM32_SYSCLK / 512)
#else
#error "invalid STM32_HPRE value specified"
#endif

/* AHB frequency check.*/
#if STM32_HCLK > STM32_SYSCLK_MAX
#error "STM32_HCLK exceeding maximum frequency (STM32_SYSCLK_MAX)"
#endif

/**
 * @brief   APB1 frequency.
 */
#if (STM32_PPRE1 == STM32_PPRE1_DIV1) || defined(__DOXYGEN__)
#define STM32_PCLK1                 (STM32_HCLK / 1)
#elif STM32_PPRE1 == STM32_PPRE1_DIV2
#define STM32_PCLK1                 (STM32_HCLK / 2)
#elif STM32_PPRE1 == STM32_PPRE1_DIV4
#define STM32_PCLK1                 (STM32_HCLK / 4)
#elif STM32_PPRE1 == STM32_PPRE1_DIV8
#define STM32_PCLK1                 (STM32_HCLK / 8)
#elif STM32_PPRE1 == STM32_PPRE1_DIV16
#define STM32_PCLK1                 (STM32_HCLK / 16)
#else
#error "invalid STM32_PPRE1 value specified"
#endif

/* APB1 frequency check.*/
#if STM32_PCLK1 > STM32_PCLK1_MAX
#error "STM32_PCLK1 exceeding maximum frequency (STM32_PCLK1_MAX)"
#endif

/**
 * @brief   APB2 frequency.
 */
#if (STM32_PPRE2 == STM32_PPRE2_DIV1) || defined(__DOXYGEN__)
#define STM32_PCLK2                 (STM32_HCLK / 1)
#elif STM32_PPRE2 == STM32_PPRE2_DIV2
#define STM32_PCLK2                 (STM32_HCLK / 2)
#elif STM32_PPRE2 == STM32_PPRE2_DIV4
#define STM32_PCLK2                 (STM32_HCLK / 4)
#elif STM32_PPRE2 == STM32_PPRE2_DIV8
#define STM32_PCLK2                 (STM32_HCLK / 8)
#elif STM32_PPRE2 == STM32_PPRE2_DIV16
#define STM32_PCLK2                 (STM32_HCLK / 16)
#else
#error "invalid STM32_PPRE2 value specified"
#endif

/* APB2 frequency check.*/
#if STM32_PCLK2 > STM32_PCLK2_MAX
#error "STM32_PCLK2 exceeding maximum frequency (STM32_PCLK2_MAX)"
#endif

/**
 * @brief   RTC clock.
 */
#if (STM32_RTCSEL == STM32_RTCSEL_LSE) || defined(__DOXYGEN__)
#define STM32_RTCCLK                STM32_LSECLK
#elif STM32_RTCSEL == STM32_RTCSEL_LSI
#define STM32_RTCCLK                STM32_LSICLK
#elif STM32_RTCSEL == STM32_RTCSEL_HSEDIV
#define STM32_RTCCLK                (STM32_HSECLK / 128)
#elif STM32_RTCSEL == STM32_RTCSEL_NOCLOCK
#define STM32_RTCCLK                0
#else
#error "invalid source selected for RTC clock"
#endif

/**
 * @brief   ADC frequency.
 */
#if (STM32_ADCPRE == STM32_ADCPRE_DIV2) || defined(__DOXYGEN__)
#define STM32_ADCCLK                (STM32_PCLK2 / 2)
#elif STM32_ADCPRE == STM32_ADCPRE_DIV4
#define STM32_ADCCLK                (STM32_PCLK2 / 4)
#elif STM32_ADCPRE == STM32_ADCPRE_DIV6
#define STM32_ADCCLK                (STM32_PCLK2 / 6)
#elif STM32_ADCPRE == STM32_ADCPRE_DIV8
#define STM32_ADCCLK                (STM32_PCLK2 / 8)
#else
#error "invalid STM32_ADCPRE value specified"
#endif

/* ADC frequency check.*/
#if STM32_ADCCLK > STM32_ADCCLK_MAX
#error "STM32_ADCCLK exceeding maximum frequency (STM32_ADCCLK_MAX)"
#endif

/**
 * @brief   USB frequency.
 */
#if (STM32_USBPRE == STM32_USBPRE_DIV1P5) || defined(__DOXYGEN__)
#define STM32_USBCLK                ((STM32_PLLCLKOUT * 2) / 3)
#elif (STM32_USBPRE == STM32_USBPRE_DIV1)
#define STM32_USBCLK                STM32_PLLCLKOUT
#else
#error "invalid STM32_USBPRE value specified"
#endif

/**
 * @brief   Timers 2, 3, 4, 5, 6, 7, 12, 13, 14 clock.
 */
#if (STM32_PPRE1 == STM32_PPRE1_DIV1) || defined(__DOXYGEN__)
#define STM32_TIMCLK1               (STM32_PCLK1 * 1)
#else
#define STM32_TIMCLK1               (STM32_PCLK1 * 2)
#endif

/**
 * @brief   Timers 1, 8, 9, 10, 11 clock.
 */
#if (STM32_PPRE2 == STM32_PPRE2_DIV1) || defined(__DOXYGEN__)
#define STM32_TIMCLK2               (STM32_PCLK2 * 1)
#else
#define STM32_TIMCLK2               (STM32_PCLK2 * 2)
#endif

/**
 * @brief   Flash settings.
 */
#if (STM32_HCLK <= 24000000) || defined(__DOXYGEN__)
#define STM32_FLASHBITS             0x00000010
#elif STM32_HCLK <= 48000000
#define STM32_FLASHBITS             0x00000011
#else
#define STM32_FLASHBITS             0x00000012
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* STM32 DMA and RCC helpers.*/
#include "stm32_dma.h"
#include "stm32_rcc.h"

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
  void stm32_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */
