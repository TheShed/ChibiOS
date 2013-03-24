/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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

#include "ch.h"
#include "hal.h"
//#include "test.h"
#include "sct_fsm.h"



/*
 * RGB LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);

static msg_t Thread1(void *arg) {
  (void)arg;
  chRegSetThreadName("blinker");
  
  palTogglePort(GPIO0, PAL_PORT_BIT(LED_GREEN));

  while (TRUE) {
    palTogglePort(GPIO0, PAL_PORT_BIT(LED_GREEN));
    palTogglePort(GPIO0, PAL_PORT_BIT(LED_RED));
    chThdSleepMilliseconds(500);
    
    palTogglePort(GPIO0, PAL_PORT_BIT(LED_RED));
    palTogglePort(GPIO0, PAL_PORT_BIT(LED_BLUE));
    chThdSleepMilliseconds(500);
    
    palTogglePort(GPIO0, PAL_PORT_BIT(LED_BLUE));
    palTogglePort(GPIO0, PAL_PORT_BIT(LED_GREEN));
    chThdSleepMilliseconds(500);
  }
  
  return 0;
}

int main(void){
  uint16_t cntrlRegL;
  uint16_t cntrlRegH;
  uint32_t cntrlRegU;


  halInit();
  chSysInit();


  sdStart(&SD1, NULL);                  /* Default: 9600,8,N,1. */

  chThdCreateStatic(waThread1, sizeof(waThread1),
                    NORMALPRIO, Thread1, NULL);

  chnWrite( &SD1, (const uint8_t *)"\nhello\n", 8 ); 




  // SCT Clock
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<8);


  LPC_SCT->CONFIG = 0x00000000;                       // Configure as two separate 16-bit timers, clocked internally

  // Create settings for L counter
  cntrlRegL = 0x000C + (0xff << 5);        // L counter: Set prescaler = 256. Clear counter. Keep halted
  cntrlRegL &= ~((1 << 2));                   // Add the bit mask for starting L when needed
  
  // Create settings for H counter
  cntrlRegH = 0x000A + (0xff << 5) ;       // H counter: Set prescaler = 256. Clear counter. Release from halt but keep stopped
                                              // Bit 2 and 1 must be changed at the same time to stay stopped!
      
  // Create the global mask for the control register needed for starting both halves at the same time 
  // AND for configuring the stopped state of side H
  cntrlRegU = ((cntrlRegL) + (cntrlRegH << 16));                                                                              


  // Now use the FSM code to configure the state machine 
  sct_fsm_init();

  // Other manual configuration and overrides 
  // Conflict resolution: CLR override for all
  // LPC_SCT->RES = (2 << (2*0)) | (2 << (2*1)) | (2 << (2*2)) | (2 << (2*3));  

  // Start the SCT with the register settings created above
  LPC_SCT->CTRL_U = cntrlRegU;


  do {
    chThdSleepMilliseconds(500);
  } while (TRUE);
  
  return 0;
}

