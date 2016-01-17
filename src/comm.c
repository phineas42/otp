#include <pebble.h>
#include "comm.h"
#include "config.h"
#include "ui.h"

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");
  // Get the first pair
  Tuple *t = dict_read_first(iterator);
  // Process all pairs present
  while (t != NULL) {
    // Process this pair's key
    switch (t->key) {
      case MK_OTPKEY0:
        // Copy value and display
        //text_layer_set_text(s_output_layer, s_buffer);
        APP_LOG(APP_LOG_LEVEL_INFO, "Setting key0");
        config_setkeys(0,t->value->data,t->length);
        break;
      case MK_OTPLABEL0:
        APP_LOG(APP_LOG_LEVEL_INFO, "Setting label0");
        config_setlabels(0,t->value->cstring);
        break;
    }
    // Get next pair, if any
    t = dict_read_next(iterator);
  }
  config_setkeys_commit();
  config_setlabels_commit();
  ui_refresh_timestep();
  ui_refresh_labels();
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  //global_log("Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void init_comm() {
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  //app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  app_message_open(512, 512);
}
