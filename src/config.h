#pragma once
#include <pebble.h>
//version number of the persistent storage structures
#define CONFIG_VERSION (1)

//keys for persistent storage
#define PS_OTPSETTINGS (0)
#define PS_OTPKEYS     (1)
#define PS_OTPLABELS   (2)
  
typedef enum {RFC6238_HMAC_SHA1, RFC6238_HMAC_SHA256, RFC6238_HMAC_SHA512} hash_algorithm;

typedef struct otpsettings {
  uint32_t version; //version number of all settings
  uint8_t lengths[8]; //The size in bytes of each key, also: (length != 0) === (is_defined)
  hash_algorithm modes[8]; //The mode
//  int16_t tzoffset; //minutes off of UTC -- maybe a future Pebble API will eliminate this necessity for APLITE
} __attribute__((__packed__)) otpsettings;

typedef struct otpkeys {
  uint32_t version; //version number of otpkeys
  uint8_t keys[8][20]; //8 keys of length 20
} __attribute__((__packed__)) otpkeys;

typedef struct otplabels {
  uint32_t version; //version number of otplabels
  char labels[8][15]; //8 labels of length 14 + '\0'
} __attribute__((__packed__)) otplabels;

extern otpsettings settings;
extern otpkeys keys;
extern otplabels labels;
extern void load_cfg();
extern void config_setkey(int , const uint8_t[], size_t);
extern void config_setkeys(int, const uint8_t[], size_t);
extern void config_setkeys_commit();
extern void config_setlabel(int, const char*);
extern void config_setlabels(int, const char*);
extern void config_setlabels_commit();
