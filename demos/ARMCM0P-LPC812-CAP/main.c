/*
  (c) 2013: meldes ltd
*/

#include "ch.h"
#include "hal.h"

#include "gps_parser.h"
#include "oled_driver.h"

//===========================================================================
#define DISP_UPDATE_INTERVAL       250   //ms
#define BUTTON_POLL_INTERVAL        100   //ms

//===========================================================================
#define EV_REDRAW                   0x0200
#define EV_UPDATE                   0x0100
#define EV_BUTTON_1_PRESS           0x0001
#define EV_BUTTON_1_RELEASE         0x0010

#define DISP_MODE_HEAD              0
#define DISP_MODE_SPEED             1
#define DISP_MODE_WP                2
#define DISP_MODE_END               3
#define DISP_MODE_STATUS            4
//#define DISP_MODE_MENU              10
//#define DISP_MODE_SLEEP             20
#define DISP_MODE_AUTO              31

//===========================================================================
static uint8_t disp_mode;

//===========================================================================
// FIXME: Bodge
static Thread *main_tp;

static void gpt1cb(GPTDriver *gptp)
{
  (void)gptp;
  chSysLockFromIsr();
  if( disp_mode == DISP_MODE_STATUS )
    chEvtSignalI( main_tp, EV_BUTTON_1_PRESS );
  chSysUnlockFromIsr();
}


//===========================================================================
// Button Thread - poll buttons

static WORKING_AREA(waButtonThread, 32);

//=====================================
static msg_t ButtonThread( void *thread_p )
{
  uint32_t last_buttons = palReadPad(GPIO0, BUTTON_0);

  while( true )
  {
    uint32_t new_buttons;
    eventmask_t emButton;
    
    chThdSleepMilliseconds( BUTTON_POLL_INTERVAL );

    new_buttons = palReadPad(GPIO0, BUTTON_0) ^ last_buttons;
    if( !new_buttons )
    {
      continue;
    }

    emButton  =  new_buttons &  last_buttons;     // pressed buttons
    emButton |= (new_buttons & ~last_buttons)<<4; // released buttons
    chEvtSignal( (Thread *)thread_p, emButton );
    last_buttons ^= new_buttons;
  }

  return 0;
}


//=====================================
const char const *qual_str[] = 
{
  "No Fix : ",
  "Fix    : ",
  "D-Fix  : ",
  "No GPS : "
};

static const GPTConfig gpt1cfg = {
  1000,    /* 1kHz timer clock.*/
  gpt1cb    /* Timer callback.*/
};

//=====================================
static void screen_splash( void )
{
  DISP_display_string( &font_7x5, 0,  3, "www.TorqueRacing.net" );

  DISP_display_string( &font_7x5, 2,  3, "(c) 2013:" );
  DISP_display_string( &font_7x5, 3,  3, "theShedWorks.org.uk" );

  chThdSleepSeconds(5);
  DISP_clear();
}

//=====================================
static void screen_waypoint( void )
{
  int32_t offset;
  uint32_t distance;

// distance = ((rmb.dest_range*1852)/1609);     // miles
  distance = ((rmb.dest_range*1852)/100);       // metres

  if( distance > 1000 )
  {
    distance /= 10;
    DBUFF_float( NULL, distance, 5, 2, false );   // km
    DISP_display_buffer( &font_24x16, 1, 20 );
    DISP_display_string(   &font_7x5, 3, 100, "km" );
  }
  else
  {
    DBUFF_decimal( NULL, distance, 5, false );
    DISP_display_buffer( &font_24x16, 1, 12 );
    DISP_display_string(   &font_7x5, 3, 100, "m " );
  }

  offset = ((rmb.dest_bearing+5)/10) - ((vtg.track_true+5)/10);
  if( offset > 180 )
    offset -= 360;
  if( offset < -180 )
    offset += 360;
  DISP_display_offset( &font_7x5, 0, 1, offset );
}

//=====================================
//=====================================
int main(void)
{

  halInit();
  chSysInit();

  gptStart(&GPTD1, &gpt1cfg);
  // FIXME:Bodge for now - timer callback
  main_tp = chThdSelf();

  GPS_Parser_Start( &SD1 );

  chThdCreateStatic( waButtonThread, sizeof(waButtonThread),
                     NORMALPRIO, ButtonThread, chThdSelf() );

  DISP_Start( &SPID1 );
  DISP_clear();

  screen_splash();

  // and GO!!!
  chEvtGetAndClearEvents( ALL_EVENTS );

  // Now we are the main display thread....
  disp_mode = DISP_MODE_AUTO;

  do {
    eventmask_t evMask;

    evMask = chEvtWaitAnyTimeout(ALL_EVENTS,  MS2ST(DISP_UPDATE_INTERVAL) );
    if( !evMask )
    {
      evMask = EV_UPDATE;
    }

    // do Menu/Sleep here

    if( evMask & EV_BUTTON_1_PRESS )
    {
      disp_mode++;
      if( disp_mode >= DISP_MODE_END )
      {
        disp_mode = DISP_MODE_HEAD;
      }
      chEvtAddEvents( EV_REDRAW );
      continue;
    }

    // Lost fix? => Auto Status Mode
    if((gga.qual!=GPS_STAT_FIX) &&
       (gga.qual!=GPS_STAT_D_FIX))
    {
      if( disp_mode != DISP_MODE_AUTO )
      {
        disp_mode = DISP_MODE_AUTO;
        chEvtAddEvents( EV_REDRAW );
      }
    }
    else if( disp_mode == DISP_MODE_AUTO )
    {
      gptStartOneShot(&GPTD1, 5000);          // 5 sec
      disp_mode = DISP_MODE_STATUS;
    }

    if( evMask & EV_REDRAW )
    {
      DISP_clear();    
    }
    if( evMask & (EV_UPDATE|EV_REDRAW) )
    {
      switch(disp_mode)
      {

      case DISP_MODE_SPEED:
        DBUFF_decimal( NULL, ((vtg.speed+5)/10), 3, false );
        DISP_display_buffer( &font_30x20, 0, 30 );

        DISP_display_string(   &font_7x5, 3, 94, "kmh" );
        break;

      case DISP_MODE_WP:
        screen_waypoint();
        break;

      case DISP_MODE_STATUS:
      case DISP_MODE_AUTO:
        DISP_display_string(   &font_7x5, 0, 0, (char *)qual_str[gga.qual] );
        DBUFF_decimal( NULL, gga.sats, 2, true );
        DISP_display_buffer(   &font_7x5, 0, 48 );

        DISP_display_datetime( &font_7x5, 1, 0, rmc.date, rmc.time );
        DISP_display_latlon(   &font_7x5, 2, 0, rmc.lat,  rmc.N, true );
        DISP_display_latlon(   &font_7x5, 3, 0, rmc.lon,  rmc.E, false );
        break;

      case DISP_MODE_HEAD:
      default:
        DBUFF_decimal( NULL, (vtg.track_true+5)/10, 3, true );
        DISP_display_buffer( &font_30x20, 0, 30 );

        DISP_display_string(   &font_7x5, 0, 94, "$T" );
        break;

      }
    }
  

  } while (TRUE);

  return 0;
}

//=====================================
// End.
//=====================================

