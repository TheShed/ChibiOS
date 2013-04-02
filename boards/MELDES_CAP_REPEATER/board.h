/*
  (c) 2013: meldes ltd
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for theShedWorks.org.uk Cap-Repeater board.
 */

/*
 * Board identifiers.
 */
#define BOARD_MELDES_CAP
#define BOARD_NAME "theShedWorks.org.uk Cap-Repeater"

/*
 * Board frequencies.
 */
#define SYSOSCCLK               12000000

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the LPC8xx Reference Manual for details.
 */
/* Pull-up/down  */
#define PIN_MODE_NOPULL               (0<<3)
#define PIN_MODE_PULLDOWN             (1<<3)
#define PIN_MODE_PULLUP               (2<<3)
#define PIN_MODE_REPEATER             (3<<3)
/* Hysteresis */
#define PIN_HYS_EN                    (1<<5)
/* Invert Input */
#define PIN_INV_INPUT                 (1<<6)
/* Reserved bits */
#define PIN_RSVD                      (1<<7)
/* I2C Mode */
#define PIN_I2CMODE_STD               (0<<8)
#define PIN_I2CMODE_STDIO             (1<<8)
#define PIN_I2CMODE_FAST              (2<<8)
/* Open Drain */
#define PIN_OPEN_DRAIN                (1<<10)
/* Input Filter Sample Clocks */
#define PIN_SMODE_FILTER(n)           ((n)<<11)
/* Input Filter clock divider */
#define PIN_CLKDIV_FILTER(n)          ((n)<<13)

/*
 * Pin definitions.
 */
#define GPS_RX                         0        // UART0
#define BUTTON_0                       1        // IN
#define SWDIO                          2        // SWD
#define SWCLK                          3        // SWD
#define BUTTON_1                       4        // IN
#define RESET                          5        // RESET_IN
#define DISP_RST                       6        // OUT(0)
#define DISP_DNC                       7        // OUT(1)
#define DISP_SCLK                      8        // SPI0
#define DISP_MOSI                      9        // SPI0
#define UNASSIGNED_10                 10        // I2C?
#define UNASSIGNED_11                 11        // I2C?
#define UNASSIGNED_12                 12        // ---
#define UNASSIGNED_13                 13        // ---
#define DISP_SSEL                     14        // SPI0

// ** Dev Board only **
//#define LED_RED                       15        // OUT(1)
#define LED_BLUE                      16        // OUT(1)
#define LED_GREEN                     17        // OUT(1)


/*
 * GPIO 0 initial setup.
 */
/*#define VAL_PIO0_0              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_1              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_2              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_3              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_4              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_5              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_6              PIN_MODE_PULLUP*/
#define VAL_PIO0_7              PIN_MODE_NOPULL
#define VAL_PIO0_8              PIN_MODE_NOPULL
#define VAL_PIO0_9              PIN_MODE_NOPULL
/*#define VAL_PIO0_10             PIN_MODE_PULLUP*/
/*#define VAL_PIO0_11             PIN_MODE_PULLUP*/
/*#define VAL_PIO0_12             PIN_MODE_PULLUP*/
/*#define VAL_PIO0_13             PIN_MODE_PULLUP*/
#define VAL_PIO0_14             PIN_MODE_NOPULL
/*#define VAL_PIO0_15             PIN_MODE_PULLUP*/
#define VAL_PIO0_16             PIN_MODE_NOPULL
#define VAL_PIO0_17             PIN_MODE_NOPULL


    /* UART0: RXD = P0.0  */
#define VAL_PINASSIGN0            ((0xFFFF00FF) | (0<<8))

/*#define VAL_PINASSIGN1            0xFFFFFFFF*/
/*#define VAL_PINASSIGN2            0xFFFFFFFF*/

    /* SPI0: SCLK = P0.8  */
#define VAL_PINASSIGN3            ((0x00FFFFFF) | (8<<24))
    /* SPI0: MOSI = P0.9, SSEL = P0.14  */
#define VAL_PINASSIGN4            ((0xFF00FF00) | (14<<16) | (9))

/*#define VAL_PINASSIGN5            0xFFFFFFFF*/
/*#define VAL_PINASSIGN6            0xFFFFFFFF*/
/*#define VAL_PINASSIGN7            0xFFFFFFFF*/
/*#define VAL_PINASSIGN8            0xFFFFFFFF*/


#define VAL_GPIO0DIR            (PAL_PORT_BIT(DISP_RST)   |          \
                                 PAL_PORT_BIT(DISP_DNC)   |          \
                                 PAL_PORT_BIT(LED_BLUE)   |          \
                                 PAL_PORT_BIT(LED_GREEN))
                                
#define VAL_GPIO0DATA           (PAL_PORT_BIT(DISP_DNC)   |          \
                                 PAL_PORT_BIT(LED_BLUE)   |          \
                                 PAL_PORT_BIT(LED_GREEN))


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */


#endif /* _BOARD_H_ */
