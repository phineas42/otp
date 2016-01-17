#include <pebble.h>
#include <string.h>
#include "clock.h"
#include "config.h"
#include "ui.h"
  
char clockstring[16]="";
char secsstring[3]="";
time_t timestep=0;

void init_clock() {
  clock_copy_time_string(clockstring, sizeof(clockstring));
}

void update_clock() {
  char new_clockstring[16];
  char new_secsstring[3];
  time_t new_unixtime;
  time_t new_timestep;
  clock_copy_time_string(new_clockstring, sizeof(new_clockstring));
  snprintf(new_secsstring,3,"%ld",time(NULL)%60);
  if (strcmp(clockstring,new_clockstring) || strcmp(secsstring,new_secsstring)) {
    strcpy(clockstring,new_clockstring);
    if (! clock_is_24h_style()) {
      clockstring[strlen(new_clockstring) - 3] = '\0';
    }
    APP_LOG(APP_LOG_LEVEL_DEBUG,"Clock string: %s", clockstring);
    strcpy(secsstring,new_secsstring);
    //APP_LOG(APP_LOG_LEVEL_DEBUG,"Refreshing time to %s",new_clockstring);
    ui_refresh_time();
  }
//#ifdef PBL_PLATFORM_APLITE
  new_unixtime = time(NULL);
  new_unixtime = mktime(gmtime(&new_unixtime));
  new_timestep = new_unixtime/30;
  if (new_timestep!=timestep) {
    timestep=new_timestep;
    ui_refresh_timestep();
  }
  ui_refresh_second(new_unixtime%60);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_clock();
  //redraw()  
}

void start_clock() {
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

void deinit_clock() {
  tick_timer_service_unsubscribe();
}