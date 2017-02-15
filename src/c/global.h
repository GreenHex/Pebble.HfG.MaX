//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>

// #define DEBUG
// #define SHOW_LAYER_OUTLINES

#define BACKGROUND_COLOUR PBL_IF_COLOR_ELSE( GColorWhite, GColorWhite )
#define FOREGROUND_COLOUR PBL_IF_COLOR_ELSE( GColorDarkGray, GColorBlack )

#define CLOCK_DIAL_SIZE_W ( PBL_DISPLAY_WIDTH - 4 )
#define CLOCK_DIAL_SIZE_H CLOCK_DIAL_SIZE_W
#define CLOCK_DIAL_POS_X ( PBL_DISPLAY_WIDTH / 2 - CLOCK_DIAL_SIZE_W / 2 )
#define CLOCK_DIAL_POS_Y CLOCK_DIAL_POS_X
#define CLOCK_DIAL_BOUNDS ( GRect( 0, 0, CLOCK_DIAL_SIZE_W, CLOCK_DIAL_SIZE_H ) )
#define CLOCK_DIAL_FRAME ( GRect( CLOCK_DIAL_POS_X, CLOCK_DIAL_POS_Y, CLOCK_DIAL_SIZE_W, CLOCK_DIAL_SIZE_H ) )

#define HOURS_FRAME ( GRect( 0, 0, CLOCK_DIAL_SIZE_W, CLOCK_DIAL_SIZE_H ) )
#define MINUTES_FRAME ( GRect( 0, 0, CLOCK_DIAL_SIZE_W, CLOCK_DIAL_SIZE_H ) )

#define DATE_SIZE_W CLOCK_DIAL_SIZE_W
#define DATE_SIZE_H ( PBL_DISPLAY_HEIGHT - CLOCK_DIAL_SIZE_H - ( 4 * CLOCK_DIAL_POS_Y ) )
#define DATE_POS_X CLOCK_DIAL_POS_X
#define DATE_POS_Y ( 3 * CLOCK_DIAL_POS_Y + CLOCK_DIAL_SIZE_H )
#define DATE_FRAME ( GRect( DATE_POS_X, DATE_POS_Y, DATE_SIZE_W, DATE_SIZE_H ) )

static GPathInfo PATH_TICK = {
  2, (GPoint []) {
    { 0, - ( CLOCK_DIAL_SIZE_W / 2 - 6 ) },
    { 0, ( CLOCK_DIAL_SIZE_W / 2 - 6 ) }
  }
};

static GPathInfo MAX_BILL_HOUR_HAND = {
  5, (GPoint []) {
    { -3, 0 },
    { -3, -28 },
    { 0, -33 },
    { 3, -25 },
    { 3, 0 }
  }
};

static GPathInfo MAX_BILL_MINUTE_HAND = {
  5, (GPoint []) {
     { -3, 0 },
    { -3, -50 },
    { 0, -56 },
    { 3, -50 },
    { 3, 0 }
  }
};

static GPathInfo MAX_BILL_HOUR_HAND_15 = {
  5, (GPoint []) {
    { -2, 0 },
    { -2, -28 },
    { 0, -33 },
    { 2, -25 },
    { 2, 0 }
  }
};

static GPathInfo MAX_BILL_MINUTE_HAND_15 = {
  5, (GPoint []) {
     { -2, 0 },
    { -2, -50 },
    { 0, -56 },
    { 2, -50 },
    { 2, 0 }
  }
};

extern tm tm_time;
extern GColor background_colour;
extern GColor foreground_colour;
