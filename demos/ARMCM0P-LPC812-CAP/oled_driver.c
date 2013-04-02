/*
  (c) 2013: meldes ltd
*/
//=====================================
//=====================================
//  OLED Driver
//=====================================
//=====================================
#include "ch.h"
#include "hal.h"

#include "oled_driver.h"

//==========================================================================
#define NUM_PAGES       (DISP_HEIGHT/8)

#if (DISP_WIDTH != 128)
#error "xxx"
#endif

#if (NUM_PAGES != 4)
#error "xxx"
#endif



//==========================================================================
// Driver Commands
//==========================================================================

#define     SSD1305_CMD_COL_LO(x)        (0x00|((x)&0x0f))
#define     SSD1305_CMD_COL_HI(x)        (0x10|(((x)>>4)&0x0f))
#define     SSD1305_CMD_COL_ADDR         (0x21)
#define     SSD1305_CMD_PAGE_ADDR        (0x22)
#define     SSD1305_CMD_START_LINE(x)    (0x40|((x)&0x3F))
#define     SSD1305_CMD_BANK0_CONTRAST   (0x81)
#define     SSD1305_CMD_LUT              (0x91)
#define     SSD1305_CMD_SEG_DIR(x)       (0xA0|((x)&0x01))
#define     SSD1305_CMD_ALL_ON(x)        (0xA4|((x)&0x01))
#define     SSD1305_CMD_INV_DISP(x)      (0xA6|((x)&0x01))
#define     SSD1305_CMD_MUX_RATIO        (0xA8)
#define     SSD1305_CMD_DIM_CONTRAST     (0xAB)
#define     SSD1305_CMD_MASTER_CONFIG    (0xAD)
#define     SSD1305_CMD_DISP_ON(x)       (0xAC|((x)&0x03))
#define     SSD1305_CMD_PAGE(x)          (0xB0|((x)&0x0f))
#define     SSD1305_CMD_COM_DIR(x)       (0xC0|((x)&0x08))
#define     SSD1305_CMD_DISPLAY_OFFSET   (0xD3)
#define     SSD1305_CMD_SET_CLOCK        (0xD5)
#define     SSD1305_CMD_AREA_COLOUR_MODE (0xD8)
#define     SSD1305_CMD_PRE_CHARGE       (0xD9)
#define     SSD1305_CMD_COM_PINS_CFG     (0xDA)
#define     SSD1305_CMD_VCOMH_LEVEL      (0xDB)
#define     SSD1305_CMD_RMW              (0xE0)
#define     SSD1305_CMD_NOP              (0xE3)
#define     SSD1305_CMD_END              (0xEE)

/*===========================================================================*/
/* Driver private functions.                                                */
/*===========================================================================*/
static SPIDriver *SPIp;

//=====================================
static __inline void write_data( uint8_t *data, int num )
{
  spiSend( SPIp, num, data );
}

//=====================================
static __inline void write_cmd( uint8_t *data, int num )
{
  palClearPad(GPIO0, DISP_DNC);
  spiSend( SPIp, num, data );
  palSetPad(GPIO0, DISP_DNC);;
}

//=====================================
static __inline void set_page( uint8_t page )
{
  uint8_t cmd = SSD1305_CMD_PAGE(page);
  write_cmd( &cmd, 1 );
}

//=====================================
static __inline void set_column( uint8_t column )
{
  uint8_t cmd[2] = 
  {
    SSD1305_CMD_COL_HI(column),
    SSD1305_CMD_COL_LO(column)
  };
  
  write_cmd( &(cmd[0]), 2 );
}

//=====================================
//=====================================
static const  SPIConfig OLED_SPI_config = {
  NULL,
  SPI_CFG_MASTER,                       // cfg;
  SPI_DLY_FRAME(1),                     // dly;
  SPI_TXCTRL_EOF|SPI_TXCTRL_FLEN(8)     // txctrl;
};

//=====================================
static const uint8_t OLED_config[] = 
{
  1, SSD1305_CMD_DISP_ON(2),                // Display Off
  2, SSD1305_CMD_SET_CLOCK, 0x10,           // Clk divide, Osc Freq
  2, SSD1305_CMD_MUX_RATIO, 0x1F,
  2, SSD1305_CMD_DISPLAY_OFFSET, 0x00,
  3, SSD1305_CMD_COL_ADDR, 0x00, 0x7f,
  3, SSD1305_CMD_PAGE_ADDR, 0x00, 0x1f,
  1, SSD1305_CMD_START_LINE(0),
  2, SSD1305_CMD_AREA_COLOUR_MODE, 0x05,    // Monochrome, low-power mode
  2, SSD1305_CMD_COM_PINS_CFG, 0x12,
  5, SSD1305_CMD_LUT, 0x3F, 0x3F, 0x3F, 0x3F,
  2, SSD1305_CMD_BANK0_CONTRAST, 0xBF,
  4, SSD1305_CMD_DIM_CONTRAST, 0x00, 0x80, 0x00,
  2, SSD1305_CMD_PRE_CHARGE, 0xD2,
  2, SSD1305_CMD_VCOMH_LEVEL, 0x08,
  1, SSD1305_CMD_ALL_ON(0),                 // All Off
//  1, SSD1305_CMD_ALL_ON(1),                 // All Off
  1, SSD1305_CMD_INV_DISP(0),               // Not Inverted
//  1, SSD1305_CMD_INV_DISP(1),               // Inverted
  1, SSD1305_CMD_DISP_ON(3),                // Display On
  0                                         // Terminate Config Table
};

//=====================================
void DISP_Start( SPIDriver *spip )
{
  uint8_t *cfg_p = (uint8_t *)&(OLED_config[0]);
  uint8_t len;
  
  
  //palClearPad(GPIO0, DISP_RST);
  spiStart( spip, &OLED_SPI_config );

  palSetPad(GPIO0, DISP_RST);
  spiSelect( spip );
  SPIp = spip;

  while( (len = *cfg_p++) )
  {
    write_cmd( cfg_p, len );
    cfg_p += len;
  }

  DISP_clear();
}

//=====================================
void DISP_clear( void )
{
  int i,j;
  uint8_t data = 0x00;

  set_column( 0 );
  for( i=0; i<NUM_PAGES; i++ )
  {
    set_page( i );
    for( j=0; j<DISP_WIDTH; j++ )
    {
      write_data( &data, 1 );
    }
  }
  
  return;
}

#if 0
//===========================================================================
void DISP_set_pixel( uint32_t x, uint32_t y )
{
  uint32_t page = (y/8);
  uint8_t  bit = (0x01<<(y&0x07));
  uint8_t  data;

  set_page( page );
  set_column( x );
  //data = read_data();
  data = 0x00;
  
  data |= bit;
  write_data( &data, 1 );

  return;
}
#endif

//===========================================================================
static void DISP_set_pos( uint8_t page, uint8_t col )
{
  set_page(   page );
  set_column( col );
}

//===========================================================================
int DISP_display_char( const FONT_INFO *font, uint8_t page, uint8_t col, char ch )
{
  static const uint8_t spacing[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  };
  
  int idx;
  const uint8_t *p;
  int i;
  int ch_width = font->widthPixels;

  if( (ch < font->startChar) || (ch > font->endChar) )
  {
    if( font->digit_only && (ch == '.') )
    {
      p = &(font->data[10*(ch_width*font->heightPages)]);
      ch_width /= 2;
    }
    else
      p = &(spacing[0]);
  }
  else
  {
    idx = (ch - font->startChar);
    p = &(font->data[idx*(ch_width*font->heightPages)]);
  }

                            
  for( i=0; i<font->heightPages; i++ )
  {
    DISP_set_pos( page, col );
    page++;
    write_data( (uint8_t *)p, ch_width );
    if( p != &(spacing[0]) )
      p += ch_width;
    if( font->spacing )
      write_data( (uint8_t *)spacing, font->spacing );
  }
  
  return ch_width+font->spacing;
}

//===========================================================================
void DISP_display_string( const FONT_INFO *font, uint8_t page, uint8_t col, char *str )
{
  while(*str != 0)
  {
    col += DISP_display_char( font, page, col, *str );
    str++;
  }
}

//===========================================================================
// DISP Utils....
//===========================================================================
static uint8_t disp_buffer[22];

static const uint8_t hex_chars[] = "0123456789ABCDEF";
//===========================================================================
void DISP_display_buffer( const FONT_INFO *font, uint8_t page, uint8_t col )
{
  DISP_display_string( font, page, col, (char *)disp_buffer );
}

//===========================================================================
uint8_t *DBUFF_float( uint8_t *buf, uint32_t val, uint32_t width, uint32_t dp, bool zero_pad )
{
  int i = width;
  bool first_before_dp = true;

  if( buf == NULL )
    buf = &disp_buffer[0];
    
  buf[i--] = '\0';
  do
  {
    if( dp )
    {
      buf[i] = hex_chars[val%10];
      val /= 10;
      dp--;
      if( !dp )
      {
        i--;
        buf[i] = '.';
        first_before_dp = true;
      }
    }
    else if( val )
    {
      first_before_dp = false;
      buf[i] = hex_chars[val%10];
      val /= 10;
    }
    else
    {
      if( first_before_dp )
      {
        buf[i] = '0';
        first_before_dp = false;
      }
      else
        buf[i] = zero_pad?'0':' ';
    }
  }while(i--);

  return &(buf[width]);
}

//===========================================================================
uint8_t *DBUFF_decimal( uint8_t *buf, uint32_t val, uint32_t width, bool zero_pad )
{
  return DBUFF_float( buf, val, width, 0, zero_pad );
}


//===========================================================================
void DISP_display_datetime( const FONT_INFO *font, uint8_t page, uint8_t col,
                            uint32_t date, uint32_t time )
{
  uint8_t *p;
  
  p = DBUFF_decimal( NULL, date/10000, 2, false );
  *p++ = '/';
  p = DBUFF_decimal( p, date/100, 2, false );
  *p++ = '/';
  p = DBUFF_decimal( p, date, 2, false );

  *p++ = ' ';

  p = DBUFF_decimal( p, time/10000, 2, false );
  *p++ = ':';
  p = DBUFF_decimal( p, time/100, 2, false );
  *p++ = ':';
  p = DBUFF_decimal( p, time, 2, false );
  
  DISP_display_buffer( font, page, col );
}

//===========================================================================
void DISP_display_latlon( const FONT_INFO *font, uint8_t page, uint8_t col,
                          uint32_t l, bool NE, bool lat )
{
  uint8_t *p = &disp_buffer[0];

  if( lat )
  {
    *p++ = (NE?'N':'S');
    *p++ = ' ';
    p = DBUFF_decimal( p, l/100000, 2, true );
  }
  else
  {
    *p++ = (NE?'E':'W');
    p = DBUFF_decimal( p, l/100000, 3, true );
  }
  *p++ = '$';
  p = DBUFF_float( p, l, 6, 3, true );
  *p++ = '\'';
  *p   = '\0';
  
  DISP_display_buffer( font, page, col );
}

//===========================================================================
void DISP_display_offset( const FONT_INFO *font, uint8_t page, uint8_t col,
                          int32_t offset )
{
  uint8_t *p = &disp_buffer[0];

  *p++ = (offset<-90)?'*':'|';
  *p++ = (offset<-70)?'<':' ';
  *p++ = (offset<-50)?'<':' ';
  *p++ = (offset<-40)?'<':' ';
  *p++ = (offset<-30)?'<':' ';
  *p++ = (offset<-20)?'<':' ';
  *p++ = (offset<-15)?'<':' ';
  *p++ = (offset<-10)?'<':' ';
  *p++ = (offset<- 6)?'<':' ';
  *p++ = (offset<- 3)?'<':' ';
  *p++ = '+';
  *p++ = (offset>  3)?'>':' ';
  *p++ = (offset>  6)?'>':' ';
  *p++ = (offset> 10)?'>':' ';
  *p++ = (offset> 15)?'>':' ';
  *p++ = (offset> 20)?'>':' ';
  *p++ = (offset> 30)?'>':' ';
  *p++ = (offset> 40)?'>':' ';
  *p++ = (offset> 50)?'>':' ';
  *p++ = (offset> 70)?'>':' ';
  *p++ = (offset> 90)?'*':'|';
  *p   = '\0';
  
  DISP_display_buffer( font, page, col );
}

//=====================================
//  The End
//=====================================
