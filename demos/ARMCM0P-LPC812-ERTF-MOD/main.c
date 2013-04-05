/*
  (c) 2013: meldes ltd
*/

#include "ch.h"
#include "hal.h"

#include "gps_parser.h"

//===========================================================================
#define DISP_UPDATE_INTERVAL       250   //ms

//===========================================================================

uint8_t ertf_str[] = "$ERTF__xxx\r\n";

static void ERTF_update( uint32_t val )
{

  ertf_str[9] = (val%10)+'0';
  val /= 10;
  ertf_str[8] = (val%10)+'0';
  val /= 10;
  ertf_str[7] = val+'0';
  
  chnWrite( &SD1, (const uint8_t *)&(ertf_str[0]), 12 ); 
}

//=====================================
//=====================================
int main(void)
{

  halInit();
  chSysInit();

  GPS_Parser_Start( &SD1 );
  ERTF_update( 999 );

  do {
    chThdSleepMilliseconds(DISP_UPDATE_INTERVAL);

    // Lost fix?
    if((gga.qual!=GPS_STAT_FIX) &&
       (gga.qual!=GPS_STAT_D_FIX))
    {
      ERTF_update( 999 );
      continue;
    }

    ERTF_update( (vtg.track_true+5)/10 );

  } while (TRUE);

  return 0;
}

//=====================================
// End.
//=====================================

