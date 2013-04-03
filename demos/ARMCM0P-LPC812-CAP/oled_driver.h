/*
  (c) 2013: meldes ltd
*/
//=====================================
//=====================================
//  OLED Driver
//=====================================
//=====================================
#ifndef _OLED_DRIVER_H_
#define _OLED_DRIVER_H_

#include "font.h"

//=====================================
extern void DISP_Start( SPIDriver *spip );
extern void DISP_clear( void );

//extern void DISP_set_pixel( uint32_t x, uint32_t y );
extern int DISP_display_char( const FONT_INFO *font, uint8_t page,
                              uint8_t col, char ch );
extern void DISP_display_string( const FONT_INFO *font, uint8_t page,
                              uint8_t col, char *str );

extern void DISP_display_buffer( const FONT_INFO *font, uint8_t page,
                              uint8_t col );
extern uint8_t *DBUFF_decimal( uint8_t *buf, uint32_t val,
                              uint32_t width, bool zero_pad );
extern uint8_t *DBUFF_float( uint8_t *buf, uint32_t val, uint32_t width,
                              uint32_t dp, bool zero_pad );
extern void DISP_display_datetime( const FONT_INFO *font, uint8_t page, uint8_t col,
                              uint32_t date, uint32_t time );
extern void DISP_display_latlon( const FONT_INFO *font, uint8_t page, uint8_t col,
                              uint32_t l, bool NE, bool lat );
extern void DISP_display_offset( const FONT_INFO *font, uint8_t page, uint8_t col,
                          int32_t offset );

//=====================================
#endif
//=====================================

