#include <pebble.h>
#include "ui.h"
#include "clock.h"
//#include <stddef.h>
//#include "global.h"
#include "config.h"
#include "comm.h"
//#include "sha1.h"

// App icon taken from https://openclipart.org/detail/190821/cles-de-serrure-lock-keys

static void init() {
  APP_LOG(APP_LOG_LEVEL_DEBUG,"Load config");
  load_cfg();
  APP_LOG(APP_LOG_LEVEL_DEBUG,"Init clock");
  init_clock(); //initializes clockstring, secsstring, timestep
  APP_LOG(APP_LOG_LEVEL_DEBUG,"Init UI");
  init_ui(); //initializes code_buffer[]
  APP_LOG(APP_LOG_LEVEL_DEBUG,"Init comm");
  init_comm();
  APP_LOG(APP_LOG_LEVEL_DEBUG,"Start clock");
  start_clock();
  APP_LOG(APP_LOG_LEVEL_DEBUG,"Ready");
}

static void deinit() {
  deinit_clock();
  deinit_ui();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
