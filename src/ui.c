#include <pebble.h>
#include "ui.h"
#include "clock.h"
#include "otp.h"
#include "config.h"
#include "progressbar.h"

//BITHAM_42_BOLD needs text layer of at least height 43
//In the case of height 44, 1 px margin will be below, and 12 above. size is 31.
//If target height is 168/2==84, then target Y is (84-31)/2-12 ~ 14
#define CLOCK_X (0)
#define CLOCK_Y (15)
#define CLOCK_WIDTH (144)
#define CLOCK_HEIGHT (55)

#define SECS_X (0)
#define SECS_Y (69)
#define SECS_WIDTH (144)
#define SECS_HEIGHT (30)
  
#define PROGRESS_X (0)
#define PROGRESS_Y (109)
#define PROGRESS_WIDTH (144)
#define PROGRESS_HEIGHT (4)

#define LABEL_X (0)
#define LABEL_Y (105)
#define LABEL_WIDTH (144)
#define LABEL_HEIGHT (29)

#define CODE_X (0)
#define CODE_Y (126)
#define CODE_WIDTH (144)
#define CODE_HEIGHT (30)

#define LABELMAX (16)
// long-lived buffer for OTP codes
char code_buffer[8][16] = { "------","------","------","------","------","------","------","------"};
char label_buffer[8][LABELMAX] = { "Label 0","Label 1","Label 2","Label 3","Label 4","Label 5","Label 6","Label 7"};

static uint8_t current_index = 0;

//static char otp_label0[] = "          ";
//static char otp_buffer[] = "0123456789";
//static uint32_t otp_message=0;

static TextLayer *clock_layer = NULL;
static TextLayer *secs_layer = NULL;
static ProgressBarLayer *progress_bar_layer;

static TextLayer *code_layer[8];
static TextLayer *current_code_layer;
char *current_code_buffer;
static TextLayer *label_layer[8];
static TextLayer *current_label_layer;
char *current_label_buffer;
static Window *s_main_window;

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
}
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
}
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void main_window_load(Window *window) {
  int i;
  window_set_click_config_provider(s_main_window, click_config_provider);
  window_set_background_color(s_main_window,GColorWhite);
  // Create Window's child Layers here
  clock_layer = text_layer_create(GRect(CLOCK_X, CLOCK_Y, CLOCK_WIDTH, CLOCK_HEIGHT));
  text_layer_set_font(clock_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(clock_layer, GTextAlignmentCenter);
  text_layer_set_background_color(clock_layer,GColorWhite);
  text_layer_set_text_color(clock_layer,GColorBlack);
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(clock_layer));

  secs_layer = text_layer_create(GRect(SECS_X, SECS_Y, SECS_WIDTH, SECS_HEIGHT));
  text_layer_set_font(secs_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(secs_layer, GTextAlignmentCenter);
  text_layer_set_background_color(secs_layer,GColorWhite);
  text_layer_set_text_color(secs_layer,GColorBlack);
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(secs_layer));

  for (i=0; i<8; i++) {
    code_layer[i] = text_layer_create(GRect(CODE_X + 144*i, CODE_Y, CODE_WIDTH, CODE_HEIGHT));
    text_layer_set_font(code_layer[i], fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
    text_layer_set_text_alignment(code_layer[i], GTextAlignmentCenter);
    text_layer_set_background_color(code_layer[i],GColorWhite);
    text_layer_set_text_color(code_layer[i],GColorBlack);
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(code_layer[i]));
    label_layer[i] = text_layer_create(GRect(LABEL_X + 144*i, LABEL_Y, LABEL_WIDTH, LABEL_HEIGHT));
    text_layer_set_font(label_layer[i], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_text_alignment(label_layer[i], GTextAlignmentCenter);
    text_layer_set_background_color(label_layer[i],GColorWhite);
    text_layer_set_text_color(label_layer[i],GColorBlack);
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(label_layer[i]));
  }
  progress_bar_layer = progress_bar_layer_create(GRect(PROGRESS_X, PROGRESS_Y, PROGRESS_WIDTH, PROGRESS_HEIGHT));
  layer_add_child(window_get_root_layer(s_main_window), progress_bar_layer_get_layer(progress_bar_layer));
  
  current_code_layer=code_layer[0];
  current_code_buffer=code_buffer[0];
  current_label_layer=label_layer[0];
  current_label_buffer=label_buffer[0];
  layer_set_hidden(text_layer_get_layer(code_layer[1]),true);
  text_layer_set_text(current_code_layer,current_code_buffer);
  text_layer_set_text(current_label_layer,current_label_buffer);
}

static void main_window_unload(Window *window) {
  // Destroy Window's child Layers here
  progress_bar_layer_destroy(progress_bar_layer);
}

void ui_refresh_labels() {
  for (int i=0; i<8; i++) {
    strncpy(label_buffer[i],labels.labels[i],LABELMAX);
  }
  text_layer_set_text(current_label_layer,current_label_buffer);
}

void init_ui() {
  
  current_code_buffer = code_buffer[current_index];
  otp_generate_code(current_code_buffer, current_index, timestep);
  
  current_label_buffer = label_buffer[current_index];
  strncpy(current_label_buffer,labels.labels[current_index],LABELMAX);
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });

#ifdef PBL_SDK_2
  // Hide the status bar
  window_set_fullscreen(s_main_window, true);
#endif
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

void deinit_ui() {
  // Destroy Window
  window_destroy(s_main_window);
}

void ui_refresh_time() {
  if (clock_layer != NULL) {
    text_layer_set_text(clock_layer, clockstring);
  }
  if (secs_layer != NULL) {
    text_layer_set_text(secs_layer, secsstring);
  }
}

void ui_refresh_timestep() {
  if (current_code_layer == NULL) {
    current_code_buffer = code_buffer[0];
  }
  //refresh any TOTP displays
  if (current_code_layer != NULL) {
    otp_generate_code(current_code_buffer, current_index, timestep);

    //snprintf(current_code_buffer,16,"%u",(int) timestep % 1000000);
          
    //APP_LOG(APP_LOG_LEVEL_DEBUG,"setting code to %s", current_code_buffer);
    text_layer_set_text(current_code_layer, current_code_buffer);
  }
}
void ui_refresh_second(uint8_t second) {
  //int16_t x;
  if (progress_bar_layer != NULL) {
     //x=(60*(145+144)-second*(145+144))/60-144;
     //APP_LOG(APP_LOG_LEVEL_DEBUG,"progressbar x=%d", x);
     //layer_set_frame(progress_bar_layer_get_layer(progress_bar_layer), GRect(x, PROGRESS_Y, PROGRESS_WIDTH, PROGRESS_HEIGHT));
     if (second<30) {
       progress_bar_layer_set_fgcolor(progress_bar_layer, GColorWhite);
       progress_bar_layer_set_bgcolor(progress_bar_layer, GColorBlack);
     } else {
       progress_bar_layer_set_fgcolor(progress_bar_layer, GColorBlack);
       progress_bar_layer_set_bgcolor(progress_bar_layer, GColorWhite);
     }
     progress_bar_layer_set_progress(progress_bar_layer, (30-second%30)/30.0);
  }
  if (secs_layer != NULL) {
    text_layer_set_text(secs_layer, secsstring);
  }
}