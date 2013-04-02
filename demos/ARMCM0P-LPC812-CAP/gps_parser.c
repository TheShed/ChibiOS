/*
  (c) 2013: meldes ltd
*/
//=====================================
//=====================================
//  GPS Parser
//=====================================
//=====================================
#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#include "ctype.h"
#include "string.h"

#include "gps_parser.h"

//=====================================
gps_rmb_t rmb;
gps_rmc_t rmc;
gps_gga_t gga;
gps_hdg_t hdg;
gps_vtg_t vtg;

//=====================================
//=====================================
static const SerialConfig GPS_config = {
  4800,
  (CFG_DL8 | CFG_NOPARITY | CFG_STOP1)
};

//=====================================
//=====================================
static uint8_t buffer[16];
//=====================================
static bool gps_nextToken( SerialDriver *sdp, uint8_t *buffer, int len )
{
  len--;
  *buffer = '\0';

  do
  {
    int32_t ch;

    ch = sdGetTimeout( sdp, S2ST(2) );
    if( ch == Q_TIMEOUT )
      return false;

    if( !((ch == ',') || (ch == '*')) )
    {
      *buffer++ = ch;
      len--;
      if( len )
        continue;
    }
    *buffer = '\0';
    break;

  }while(1);

  return true;
}

//=====================================
static bool gps_read_bool( SerialDriver *sdp, uint8_t ch_true )
{
  if( gps_nextToken(sdp, buffer, sizeof(buffer)) )
  {
    if( buffer[0] == ch_true )
    {
      return true;
    }
  }

  return false;
}

//=====================================
static inline bool gps_read_str( SerialDriver *sdp, uint8_t *buffer, int len )
{
  return gps_nextToken(sdp, buffer, len);
}

//=====================================
static uint32_t gps_read_uint( SerialDriver *sdp, uint32_t dec_place )
{
  uint32_t ret = 0;
  
  if( gps_nextToken(sdp, buffer, sizeof(buffer)) )
  {
    uint8_t *p = &(buffer[0]);
    uint8_t ch;
    bool dec = false;
    
    while( (ch = *p++) )
    {
      if( ch == '.' )
      {
        dec = true;
        continue;
      }

      if( dec )
      {
        if( !dec_place )
        {
          break;
        }
        dec_place--;
      }

      if( !isdigit(ch) )
      {
        break;
      }
      ret = (ret*10) + (ch-'0');
    }
    
    while( dec_place-- )
    {
      ret = (ret*10);
    }
  }

  return ret;
}

//=====================================
//=====================================

static WORKING_AREA(waGPS_Parser, 64);

//=====================================
static msg_t GPS_Parser_Task(void *arg)
{
  SerialDriver *sdp = (SerialDriver *)arg;

  sdStart(sdp, &GPS_config);
  gga.qual    = 3;        // NO GPS
  gga.sats    = 0;

  do {
    int32_t ch;

    //palSetPad(GPIO0, LED_GREEN);
    ch = sdGetTimeout( sdp, S2ST(2) );
    if( ch == Q_TIMEOUT )
    {
      gga.qual    = GPS_STAT_NO_GPS;
      gga.sats    = 0;
      rmb.fix     = false;
      rmc.fix     = false;
      continue;
    }

    if( ch != '$' )
      continue;

    //palClearPad(GPIO0, LED_GREEN);
    if( !gps_read_str( sdp, buffer, sizeof(buffer) ) )
    {
      continue;
    }

    if( !strcmp((const char *)buffer,"GPGGA") )
    {
      gga.time    = gps_read_uint( sdp,  0 );
      gga.lat     = gps_read_uint( sdp,  3 );
      gga.N       = gps_read_bool( sdp, 'N' );
      gga.lon     = gps_read_uint( sdp,  3 );
      gga.E       = gps_read_bool( sdp, 'E' );
      gga.qual    = gps_read_uint( sdp,  0 );
      gga.sats    = gps_read_uint( sdp,  0 );
    }
    else if( !strcmp((const char *)buffer,"GPHDG") )
    {
      hdg.head    = gps_read_uint( sdp,  2 );
      hdg.mag_dev = gps_read_uint( sdp,  3 );
      hdg.E       = gps_read_bool( sdp, 'E' );
      hdg.mag_var = gps_read_uint( sdp,  3 );
      hdg.E       = gps_read_bool( sdp, 'E' );
    }
    else if( !strcmp((const char *)buffer,"GPRMB") )
    {
      rmb.fix          = gps_read_bool( sdp, 'A' );
      rmb.error        = gps_read_uint( sdp,  2 );
      rmb.L            = gps_read_bool( sdp, 'L' );
      gps_read_str( sdp, rmb.src, sizeof(rmb.src) );
      gps_read_str( sdp, rmb.dest, sizeof(rmb.dest) );
      rmb.dest_lat     = gps_read_uint( sdp,  3 );
      rmb.dest_N       = gps_read_bool( sdp, 'N' );
      rmb.dest_lon     = gps_read_uint( sdp,  3 );
      rmb.dest_E       = gps_read_bool( sdp, 'E' );
      rmb.dest_range   = gps_read_uint( sdp,  2 );
      rmb.dest_bearing = gps_read_uint( sdp,  1 );
      rmb.dest_vel     = gps_read_uint( sdp,  2 );
      rmb.dest_A       = gps_read_bool( sdp, 'A' );
    }
    else if( !strcmp((const char *)buffer,"GPRMC") )
    {
      rmc.time    = gps_read_uint( sdp,  0 );
      rmc.fix     = gps_read_bool( sdp, 'A' );
      rmc.lat     = gps_read_uint( sdp,  3 );
      rmc.N       = gps_read_bool( sdp, 'N' );
      rmc.lon     = gps_read_uint( sdp,  3 );
      rmc.E       = gps_read_bool( sdp, 'E' );
      rmc.speed   = gps_read_uint( sdp,  3 );
      rmc.track   = gps_read_uint( sdp,  2 );
      rmc.date    = gps_read_uint( sdp,  0 );
      rmc.mag_var = gps_read_uint( sdp,  2 );
      rmc.var_E   = gps_read_bool( sdp, 'E' );
    }
    else if( !strcmp((const char *)buffer,"GPVTG") )
    {
      vtg.track_true = gps_read_uint( sdp,  1 );
                       gps_read_bool( sdp, 'T' );     // 'T' - True
      vtg.track_mag  = gps_read_uint( sdp,  1 );
                       gps_read_bool( sdp, 'M' );     // 'M' - Magnetic
                       gps_read_uint( sdp,  1 );      // speed in knots/h
                       gps_read_bool( sdp, 'N' );     // 'N' - kNots
      vtg.speed      = gps_read_uint( sdp,  1 );      // speed in km/h
    }

  } while (TRUE);
  
  return 0;
}

//=====================================
//=====================================
void GPS_Parser_Start( SerialDriver *sdp )
{
  chThdCreateStatic( waGPS_Parser, sizeof(waGPS_Parser),
                    NORMALPRIO,
                    GPS_Parser_Task, sdp );
}

//=====================================
// The end
//=====================================

