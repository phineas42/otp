#pragma once
#include <pebble.h>

typedef Layer ProgressBarLayer;
typedef struct ProgressBarLayerData {
  float progress;
  GColor fgcolor;
  GColor bgcolor;
} ProgressBarLayerData;

extern ProgressBarLayer * progress_bar_layer_create(GRect);
extern void progress_bar_layer_destroy(ProgressBarLayer *);
extern Layer * progress_bar_layer_get_layer(const ProgressBarLayer *);
extern void progress_bar_layer_set_progress(ProgressBarLayer *, float);
extern void progress_bar_layer_set_bgcolor(ProgressBarLayer *, GColor);
extern void progress_bar_layer_set_fgcolor(ProgressBarLayer *, GColor);