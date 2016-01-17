#include <pebble.h>
#include "progressbar.h"

static void progress_bar_layer_update_callback(Layer *layer, GContext *ctx) {
  ProgressBarLayerData * pbld = layer_get_data(layer);
  GRect bounds = layer_get_bounds(layer);
  GRect progressrect = layer_get_bounds(layer);
  progressrect.size.w = (int16_t) ((pbld->progress) * bounds.size.w);
  //APP_LOG(APP_LOG_LEVEL_DEBUG,"setting progress size=%d of %d", progressrect.size.w,bounds.size.w);
#ifndef PBL_SDK_2
  graphics_context_set_antialiased(ctx, false);
#endif
  graphics_context_set_stroke_color(ctx, pbld->bgcolor);
  graphics_context_set_fill_color(ctx, pbld->bgcolor);
  graphics_fill_rect(ctx, bounds,0, GCornerNone);
  graphics_context_set_stroke_color(ctx, pbld->fgcolor);
  graphics_context_set_fill_color(ctx, pbld->fgcolor);
  graphics_fill_rect(ctx, progressrect,0, GCornerNone);
#ifndef PBL_SDK_2
  graphics_context_set_antialiased(ctx, true);
#endif
}

ProgressBarLayer * progress_bar_layer_create(GRect frame) {
  Layer * layer = layer_create_with_data(frame, sizeof(ProgressBarLayerData));
  ProgressBarLayerData * pbld = layer_get_data(layer);
  pbld->bgcolor=GColorWhite;
  pbld->fgcolor=GColorBlack;
  layer_set_update_proc(layer, progress_bar_layer_update_callback);
  return (ProgressBarLayer *) layer;
}

void progress_bar_layer_destroy(ProgressBarLayer * pblayer) {
  layer_destroy((Layer *) pblayer);
  return;
}

Layer * progress_bar_layer_get_layer(const ProgressBarLayer * pblayer) {
  return (Layer *) pblayer;
}

void progress_bar_layer_set_progress(ProgressBarLayer * pblayer, float progress) {
  ProgressBarLayerData * pbld = layer_get_data((Layer *) pblayer);
  //APP_LOG(APP_LOG_LEVEL_DEBUG,"setting progress=%d%%", (int) (progress*100));
  if (progress != pbld->progress) {
    pbld->progress=progress;
    layer_mark_dirty((Layer *) pblayer);
  }
}

void progress_bar_layer_set_bgcolor(ProgressBarLayer * pblayer, GColor color) {
  ProgressBarLayerData * pbld = layer_get_data((Layer *) pblayer);
  pbld->bgcolor=color;
  return;
}

void progress_bar_layer_set_fgcolor(ProgressBarLayer * pblayer, GColor color) {
  ProgressBarLayerData * pbld = layer_get_data((Layer *) pblayer);
  pbld->fgcolor=color;
  return;
}
