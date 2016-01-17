#include <pebble.h>
#include "config.h"

  
// #define DEFAULT_OTPKEY0_BASE32 "OVEK7TIJ3A3DM3M6"
#define DEFAULT_OTPKEY0 { 0x75, 0x48, 0xaf, 0xcd, 0x09, 0xd8, 0x36, 0x36, 0x6d, 0x9e }
#define DEFAULT_OTPKEY0_LENGTH (10)
#define DEFAULT_OTPKEY0_MODE RFC6238_HMAC_SHA1
#define DEFAULT_OTPKEY0_LABEL "Demo Key"
//#define DEFAULT_TZOFFSET (-300)

otpsettings settings;
otpkeys keys;
otplabels labels;

static otpsettings default_settings = {
  .version    = CONFIG_VERSION,
  .lengths[0] = DEFAULT_OTPKEY0_LENGTH,
  .modes[0]   = DEFAULT_OTPKEY0_MODE,
//  .tzoffset   = DEFAULT_TZOFFSET,
};

static otpkeys default_keys = {
  .version    = CONFIG_VERSION,
  .keys[0]    = DEFAULT_OTPKEY0,
};

static otplabels default_labels = {
  .version    = CONFIG_VERSION,
  .labels[0]  = DEFAULT_OTPKEY0_LABEL,
};

static void use_default_settings() {
  memcpy(&settings,&default_settings,sizeof(settings));
}
static void use_default_keys() {
  memcpy(&keys,&default_keys,sizeof(keys));
}
static void use_default_labels() {
  memcpy(&labels,&default_labels,sizeof(labels));
}

void load_cfg() {
  
  if(persist_exists(PS_OTPSETTINGS)) {
    persist_read_data(PS_OTPSETTINGS, &settings,sizeof(settings));
    if(settings.version!=CONFIG_VERSION) {
      use_default_settings();
    }
  } else {
    use_default_settings();
  }
  
  if(persist_exists(PS_OTPKEYS)) {
    persist_read_data(PS_OTPKEYS, &keys,sizeof(keys));
    if(keys.version!=CONFIG_VERSION) {
      use_default_keys();
    }
  } else {
    use_default_keys();
  }
  
  if(persist_exists(PS_OTPLABELS)) {
    persist_read_data(PS_OTPLABELS, &labels,sizeof(labels));
    if(labels.version!=CONFIG_VERSION) {
      use_default_labels();
    }
  } else {
    use_default_labels();
  }
}

void config_setkey(int index, const uint8_t newkey[], size_t size) {
  config_setkeys(index, newkey, size);
  config_setkeys_commit();
}
void config_setkeys(int index, const uint8_t newkey[], size_t size) {
  memcpy(keys.keys[index],newkey,size);
}
void config_setkeys_commit() {
  if (persist_write_data(PS_OTPKEYS, &keys, sizeof(keys))!=sizeof(keys)) {
    APP_LOG(APP_LOG_LEVEL_DEBUG,"Could not write persistent keys");
  }
}

void config_setlabel(int index, const char* label) {
  config_setlabels(index, label);
  config_setlabels_commit();
}
void config_setlabels(int index, const char* label) {
  strncpy(labels.labels[index],label, sizeof(labels.labels[index]));
}
void config_setlabels_commit() {
  ////////text_layer_set_text(s_otp_layer_label0, labels.labels[index]); //TODO ... this ui call should not be in the config module
  if   (persist_write_data(PS_OTPLABELS, &labels, sizeof(labels))!=sizeof(labels)) {
    APP_LOG(APP_LOG_LEVEL_DEBUG,"Could write persistent labels");
  }
}

