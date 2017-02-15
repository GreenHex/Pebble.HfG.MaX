//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#include <pebble.h>
#include "global.h"
#include "clock.h"
#include "utils.h"

tm tm_time;
GColor background_colour;
GColor foreground_colour;

static Layer *window_layer = 0;
static Layer *dial_layer = 0;
static Layer *hours_layer = 0;
static Layer *minutes_layer = 0;
static Layer *date_layer = 0;

#define NUM_DIGITS 12
static RotBitmapLayer *m_layer[ NUM_DIGITS ];
static RotBitmapLayer *h_layer[ NUM_DIGITS ];
static GBitmap *m_bitmap[ NUM_DIGITS ];
static GBitmap *h_bitmap[ NUM_DIGITS ];

uint32_t M_BITMAPS[ NUM_DIGITS ] = {
  RESOURCE_ID_IMAGE_M_00,
  RESOURCE_ID_IMAGE_M_05,
  RESOURCE_ID_IMAGE_M_10,
  RESOURCE_ID_IMAGE_M_15,
  RESOURCE_ID_IMAGE_M_20,
  RESOURCE_ID_IMAGE_M_25,
  RESOURCE_ID_IMAGE_M_30,
  RESOURCE_ID_IMAGE_M_35,
  RESOURCE_ID_IMAGE_M_40,
  RESOURCE_ID_IMAGE_M_45,
  RESOURCE_ID_IMAGE_M_50,
  RESOURCE_ID_IMAGE_M_55
};

uint32_t H_BITMAPS[ NUM_DIGITS ] = {
  RESOURCE_ID_IMAGE_H_12,
  RESOURCE_ID_IMAGE_H_01,
  RESOURCE_ID_IMAGE_H_02,
  RESOURCE_ID_IMAGE_H_03,
  RESOURCE_ID_IMAGE_H_04,
  RESOURCE_ID_IMAGE_H_05,
  RESOURCE_ID_IMAGE_H_06,
  RESOURCE_ID_IMAGE_H_07,
  RESOURCE_ID_IMAGE_H_08,
  RESOURCE_ID_IMAGE_H_09,
  RESOURCE_ID_IMAGE_H_10,
  RESOURCE_ID_IMAGE_H_11
};

static uint32_t const two_segments[] = { 200, 200, 200 };
VibePattern double_vibe_pattern = {
  .durations = two_segments,
  .num_segments = ARRAY_LENGTH( two_segments ),
};

static void handle_clock_tick( struct tm *tick_time, TimeUnits units_changed ) {
  tm_time = *tick_time; // copy to global
  
  #ifdef DEBUG
  APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: handle_clock_tick(): %02d:%02d:%02d", tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec );
  #endif
  
  layer_mark_dirty( dial_layer );
  if ( ( units_changed & HOUR_UNIT ) && ( !quiet_time_is_active() ) ) vibes_enqueue_custom_pattern( double_vibe_pattern );
}

static void dial_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &bounds );
  graphics_context_set_antialiased( ctx, true );
  #ifdef SHOW_LAYER_OUTLINES
  graphics_context_set_stroke_color( ctx, GColorLightGray );
  graphics_draw_rect( ctx, bounds );
  #endif
  graphics_context_set_fill_color( ctx, background_colour );
  graphics_fill_radial( ctx, grect_inset( bounds, GEdgeInsets( 1 ) ), GOvalScaleModeFitCircle, bounds.size.w / 2, 0, TRIG_MAX_ANGLE );
  
  draw_ticks( & (DRAW_TICKS_PARAMS) { 
    .layer = layer, 
    .ctx = ctx, 
    .p_gpath_info = &PATH_TICK, 
    .increment = 1, 
    .tick_thk = 1, 
    .tick_length = 19, 
    .tick_colour = GColorDarkGray, 
    .bg_colour = background_colour
  } );
  draw_ticks( & (DRAW_TICKS_PARAMS) {
    .layer = layer,
    .ctx = ctx,
    .p_gpath_info = &PATH_TICK,
    .increment = 5,
    .tick_thk = 1,
    .tick_length = 26,
    .tick_colour = GColorDarkGray, 
    .bg_colour = background_colour
  } );
  graphics_context_set_fill_color( ctx, background_colour );
  graphics_fill_radial( ctx, grect_inset( bounds, GEdgeInsets( 0 ) ), GOvalScaleModeFitCircle, 13, 0, TRIG_MAX_ANGLE );
  graphics_context_set_fill_color( ctx, GColorLightGray );
  graphics_fill_radial( ctx, grect_inset( bounds, GEdgeInsets( 0 ) ), GOvalScaleModeFitCircle, 3, 0, TRIG_MAX_ANGLE );
}

static void hours_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &bounds );
  graphics_context_set_antialiased( ctx, true );
  #ifdef SHOW_LAYER_OUTLINES
  graphics_context_set_stroke_color( ctx, GColorLightGray );
  graphics_draw_rect( ctx, bounds );
  #endif
  draw_gpath_hand( & (GPATH_HAND_PARAMS) { 
    .ctx = ctx, 
    .center_pt = center_pt, 
    .angle = ( TRIG_MAX_ANGLE * ( ( ( tm_time.tm_hour % 12 ) * 6 ) + ( tm_time.tm_min / 10 ) ) ) / ( 12 * 6 ), 
    .gpath_hand = &MAX_BILL_HOUR_HAND, 
    .gpath_hand_highlight = 0,
    .hand_colour = GColorDarkGray,
    .hand_highlight_colour = GColorDarkGray,
    .hand_outline_colour = GColorBlack,
    .center_dot_radius = 6,
    .center_dot_colour = GColorDarkGray
  } );
}

static void minutes_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &bounds );
  graphics_context_set_antialiased( ctx, true );
  #ifdef SHOW_LAYER_OUTLINES
  graphics_context_set_stroke_color( ctx, GColorLightGray );
  graphics_draw_rect( ctx, bounds );
  #endif
  
  draw_gpath_hand( & (GPATH_HAND_PARAMS) { 
    .ctx = ctx, 
    .center_pt = center_pt, 
    .angle = TRIG_MAX_ANGLE * tm_time.tm_min / 60, 
    .gpath_hand = tm_time.tm_min % 15 ? &MAX_BILL_MINUTE_HAND : &MAX_BILL_MINUTE_HAND_15, 
    .gpath_hand_highlight = 0,
    .hand_colour = GColorDarkGray,
    .hand_highlight_colour = GColorDarkGray,
    .hand_outline_colour = GColorBlack,
    .center_dot_radius = 4,
    .center_dot_colour = GColorDarkGray
  } );
}

static void date_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &bounds );
  graphics_context_set_antialiased( ctx, true );
  #ifdef SHOW_LAYER_OUTLINES
  graphics_context_set_stroke_color( ctx, GColorLightGray );
  graphics_draw_rect( ctx, bounds );
  #endif
  bounds.size.w /= 3;
  graphics_context_set_fill_color( ctx, GColorRed );
  graphics_fill_rect( ctx, bounds, 0, GCornerNone );
  bounds.origin.x += 48;
  graphics_context_set_fill_color( ctx, GColorYellow );
  graphics_fill_rect( ctx, bounds, 0, GCornerNone );
  bounds.origin.x += 48;
  graphics_context_set_fill_color( ctx, GColorBlue );
  graphics_fill_rect( ctx, bounds, 0, GCornerNone );
}

void clock_init( Window* window ){ 
  window_layer = window_get_root_layer( window );
  window_set_background_color( window, GColorDarkGray );
  
  background_colour = BACKGROUND_COLOUR;
  foreground_colour = FOREGROUND_COLOUR;
  
  dial_layer = layer_create( CLOCK_DIAL_FRAME );
  layer_set_update_proc( dial_layer, dial_layer_update_proc );
  layer_add_child( window_layer, dial_layer );
  GRect dial_layer_bounds = layer_get_bounds( dial_layer );
  
  for ( int i = 0;  i < NUM_DIGITS ; i++ ) {
    m_bitmap[i] = gbitmap_create_with_resource( M_BITMAPS[i] );
    m_layer[i] = rot_bitmap_layer_create( m_bitmap[i] );
    rot_bitmap_set_compositing_mode( m_layer[i], GCompOpSet );
    layer_add_child( dial_layer, (Layer *) m_layer[i] );
    GRect digit_rect = layer_get_frame( (Layer *) m_layer[i] );
    digit_rect = grect_centered_from_polar( grect_inset( CLOCK_DIAL_BOUNDS, GEdgeInsets( 10 ) ),
                                        GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE ( i * 30 ), GSize( digit_rect.size.w, digit_rect.size.h ) );
    digit_rect.origin.x += 1; 
    digit_rect.origin.y += 1; 
    layer_set_frame( (Layer *) m_layer[i], digit_rect );
  }
  for ( int i = 0;  i < NUM_DIGITS ; i++ ) {
    h_bitmap[i] = gbitmap_create_with_resource( H_BITMAPS[i] );
    h_layer[i] = rot_bitmap_layer_create( h_bitmap[i] );
    rot_bitmap_set_compositing_mode( h_layer[i], GCompOpSet );
    layer_add_child( dial_layer, (Layer *) h_layer[i] );
    GRect digit_rect = layer_get_frame( (Layer *) h_layer[i] );
    digit_rect = grect_centered_from_polar( grect_inset( CLOCK_DIAL_BOUNDS, GEdgeInsets( 32 ) ),
                                        GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE ( i * 30 ), GSize( digit_rect.size.w, digit_rect.size.h ) );
    digit_rect.origin.x += 1; 
    digit_rect.origin.y += 1; 
    layer_set_frame( (Layer *) h_layer[i], digit_rect );
  }
  
  hours_layer = layer_create( HOURS_FRAME );
  layer_set_update_proc( hours_layer, hours_layer_update_proc );
  layer_add_child( dial_layer, hours_layer );
  
  minutes_layer = layer_create( MINUTES_FRAME );
  layer_set_update_proc( minutes_layer, minutes_layer_update_proc );
  layer_add_child( dial_layer, minutes_layer );
  
  date_layer = layer_create( DATE_FRAME );
  layer_set_update_proc( date_layer, date_layer_update_proc );
  layer_add_child( window_layer, date_layer );
  
  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
  time_t now = time( NULL );
  handle_clock_tick( localtime( &now ), 0 );
}

void clock_deinit( void ) {
  if ( date_layer ) layer_destroy( date_layer );
  if ( minutes_layer ) layer_destroy( minutes_layer );
  if ( hours_layer ) layer_destroy( hours_layer );
  if ( dial_layer ) layer_destroy( dial_layer );
}
