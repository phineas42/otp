
#pragma once
#include <pebble.h>
  
//App Message Ids
#define MK_OTPKEY0 0
#define MK_OTPLABEL0 10
//#define MK_TZ 21
  
//static void inbox_received_callback(DictionaryIterator *iterator, void *context);
//static void inbox_dropped_callback(AppMessageResult reason, void *context);
//static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
//static void outbox_sent_callback(DictionaryIterator *iterator, void *context);
extern void init_comm();
