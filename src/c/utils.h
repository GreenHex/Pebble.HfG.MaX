//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once

typedef struct {
  Layer *layer;
  GContext *ctx;
  GPathInfo *p_gpath_info;
  int increment;
  int tick_thk;
  int tick_length;
  GColor tick_colour;
  GColor bg_colour;
} DRAW_TICKS_PARAMS;

typedef struct {
  GContext *ctx;
  GPoint center_pt;
  uint32_t angle;
  GPathInfo *gpath_hand;
  GPathInfo *gpath_hand_highlight;
  GColor hand_colour;
  GColor hand_highlight_colour;
  GColor hand_outline_colour;
  uint16_t center_dot_radius;
  GColor center_dot_colour;
} GPATH_HAND_PARAMS;

void log_pt( char *str, GPoint pt );
void log_rect( char *str, GRect rect );
void change_layer_colours( GContext *ctx, Layer *layer, GColor old_bg_colour, GColor new_bg_colour, GColor old_fg_colour, GColor new_fg_colour );
void make_outline( GContext *ctx, Layer *layer, GColor fgColour, GColor outlineColor );
void draw_gpath_hand( GPATH_HAND_PARAMS *pGP );
void draw_ticks( DRAW_TICKS_PARAMS *pDTP );
