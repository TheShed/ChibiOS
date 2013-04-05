/*
  (c) 2013: meldes ltd
*/

/*
 * LPC812 drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * **and halconf.h **
 * Note that the settings for each driver only have effect if the driver
 * is enabled.
 *
 * IRQ priorities:
 * 3...0        Lowest...highest.
 */

/*
 * HAL driver system settings.
 */

/* Default: Run PLL @24MHz from 12MHz IRC
  #define LPC8xx_PLLCLK_SOURCE               SYSPLLCLKSEL_IRCOSC
  #define LPC8xx_SYSPLL_MUL                  4
  #define LPC8xx_SYSPLL_DIV                  4
  #define LPC8xx_MAINCLK_SOURCE              SYSMAINCLKSEL_PLLOUT
  #define LPC8xx_SYSABHCLK_DIV               1
*/

/*run directly from internal 12M osc...*/
#define LPC8xx_MAINCLK_SOURCE              SYSMAINCLKSEL_IRCOSC

/*
 * PAL driver system settings.
 */
/* Defaults:
  #define HAL_USE_PAL                      FALSE
*/

#define HAL_USE_PAL                        TRUE

/*
 * GPT driver system settings.
 */
/* Defaults:
  #define HAL_USE_GPT                      FALSE
  #define LPC8xx_GPT_USE_MRT0              TRUE
  #define LPC8xx_GPT_USE_MRT1              FALSE
  #define LPC8xx_GPT_USE_MRT2              FALSE
  #define LPC8xx_GPT_USE_MRT3              FALSE
  #define LPC8xx_GPT_MRT_IRQ_PRIORITY      2
*/

/*
 * EXT driver system settings.
 */
/* Defaults:
  #define HAL_USE_EXT                       FALSE

  #define LPC8xx_EXT_EXTI0_IRQ_PRIORITY     3
  #define LPC8xx_EXT_EXTI1_IRQ_PRIORITY     3
  #define LPC8xx_EXT_EXTI2_IRQ_PRIORITY     3
  #define LPC8xx_EXT_EXTI3_IRQ_PRIORITY     3
  #define LPC8xx_EXT_EXTI4_IRQ_PRIORITY     3
  #define LPC8xx_EXT_EXTI5_IRQ_PRIORITY     3
  #define LPC8xx_EXT_EXTI6_IRQ_PRIORITY     3
  #define LPC8xx_EXT_EXTI7_IRQ_PRIORITY     3
*/


/*
 * SERIAL driver system settings.
 */
/* Defaults:
  #define HAL_USE_SERIAL                     FALSE
  #define SERIAL_DEFAULT_BITRATE             38400
  #define SERIAL_BUFFERS_SIZE                16
  
  #define LPC8xx_SERIAL_USE_UART0            TRUE
  #define LPC8xx_SERIAL_USE_UART1            FALSE
  #define LPC8xx_SERIAL_USE_UART2            FALSE
  #define LPC8xx_SERIAL_UART0_IRQ_PRIORITY   3
  #define LPC8xx_SERIAL_UART1_IRQ_PRIORITY   3
  #define LPC8xx_SERIAL_UART2_IRQ_PRIORITY   3

  #define LPC8xx_SERIAL_U_PCLK              11059200
  #define LPC8xx_SERIAL_UARTCLKDIV          !!Calculated!!
  #define LPC8xx_SERIAL_UARTFRGMUL          !!Calculated!!
  #define LPC8xx_SERIAL_UARTFRGDIV          !!Calculated!!
*/

#define HAL_USE_SERIAL                     TRUE
#define SERIAL_DEFAULT_BITRATE              9600

/*
 * SPI driver system settings.
 */
/* Defaults:
  #define HAL_USE_SPI                        FALSE
  #define SPI_USE_WAIT                       TRUE
  #define SPI_USE_MUTUAL_EXCLUSION           TRUE

  #define LPC8xx_SPI_USE_SPI0                TRUE
  #define LPC8xx_SPI_USE_SPI1                FALSE
  #define LPC8xx_SPI_SPI0CLKDIV              1
  #define LPC8xx_SPI_SPI1CLKDIV              1
  #define LPC8xx_SPI_SPI0_IRQ_PRIORITY       1
  #define LPC8xx_SPI_SPI1_IRQ_PRIORITY       1
  #define LPC8xx_SPI_ERROR_HOOK(spip)        chSysHalt()
  */





