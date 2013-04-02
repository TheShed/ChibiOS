//=====================================
//=====================================
//  GPS Parser
//=====================================
//=====================================

#define GPS_STAT_NO_FIX             0
#define GPS_STAT_FIX                1
#define GPS_STAT_D_FIX              2
#define GPS_STAT_NO_GPS             3

//=====================================
//=====================================
//=====================================
typedef struct
{
  uint32_t    time;
  uint32_t    lat;
  bool        N;
  uint32_t    lon;
  bool        E;
  uint32_t    qual;
  uint32_t    sats;
} gps_gga_t;

//=====================================
typedef struct
{
  uint32_t    head;
  uint32_t    mag_dev;
  bool        E;
  uint32_t    mag_var;
  bool        var_E;
} gps_hdg_t;

//=====================================
typedef struct
{
  uint32_t    track_true;
  uint32_t    track_mag;
  uint32_t    speed;
} gps_vtg_t;

//=====================================
typedef struct
{
  bool        fix;
  uint32_t    error;
  bool        L;
  uint8_t     src[16];
  uint8_t     dest[16];
  uint32_t    dest_lat;
  bool        dest_N;
  uint32_t    dest_lon;
  bool        dest_E;
  uint32_t    dest_range;
  uint32_t    dest_bearing;
  uint32_t    dest_vel;
  bool        dest_A;
} gps_rmb_t;

//=====================================
typedef struct
{
  uint32_t    time;
  bool        fix;
  uint32_t    lat;
  bool        N;
  uint32_t    lon;
  bool        E;
  uint32_t    speed;
  uint32_t    track;
  uint32_t    date;
  uint32_t    mag_var;
  bool        var_E;
} gps_rmc_t;

//=====================================
//=====================================
extern gps_rmb_t rmb;
extern gps_rmc_t rmc;
extern gps_gga_t gga;
extern gps_hdg_t hdg;
extern gps_vtg_t vtg;

extern void GPS_Parser_Start( SerialDriver *sdp );

//=====================================
//  The End
//=====================================

