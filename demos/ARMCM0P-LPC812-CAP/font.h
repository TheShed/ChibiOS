/*
  (c) 2013: meldes ltd
*/

#ifndef _FONT_H_
#define _FONT_H_

#include "chtypes.h"

// ==========================================================================
// ==========================================================================

// Describes a single font
typedef struct
{
  const uint8_t  heightPages;  // character height, in pages
  const uint8_t  startChar;    // first character in the font
  const uint8_t  endChar;      // last character in the font
  const uint8_t  widthPixels;  // charter width, in pixels
  const uint8_t  spacing;      // inter-character spacing, in pixels
  const uint8_t  digit_only;   // digit only font
  const uint8_t* data;         // pointer to font data array
} FONT_INFO;
// ==========================================================================


extern const FONT_INFO font_7x5;        // Small
extern const FONT_INFO font_24x16;      // Medium
extern const FONT_INFO font_30x20;      // Large

extern int font_idx( uint8_t ch );
extern uint8_t width_of_char( int idx );
extern const uint8_t *char_table_data( int idx );

// ==========================================================================
#endif
// ==========================================================================


