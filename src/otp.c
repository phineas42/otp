#include <pebble.h>
#include "otp.h"
#include "sha1.h"
#include "config.h"

//generate the TOTP code, write it into buffer.
void otp_generate_code(char * buffer, uint8_t codeindex, time_t timestep) {
  sha1nfo s;
  uint8_t *hash;
  uint8_t offset;
  union {
    uint32_t number;
    uint8_t  bytes[4];
  } code;
  uint8_t message[8] = { 0,0,0,0,0,0,0,0};
  APP_LOG(APP_LOG_LEVEL_DEBUG,"epoch div 30 =%16lx", (uint32_t) timestep);
  message[4]=*((uint8_t*) &timestep+3);
  message[5]=*((uint8_t*) &timestep+2);
  message[6]=*((uint8_t*) &timestep+1);
  message[7]=*((uint8_t*) &timestep+0);
  sha1_initHmac(&s,(const uint8_t *) keys.keys[codeindex],settings.lengths[codeindex]);
  sha1_write(&s,(const char *) message,8);
  hash=sha1_resultHmac(&s);
  offset=hash[HASH_LENGTH-1] & 0x0f;
  code.bytes[0]=hash[offset+3];
  code.bytes[1]=hash[offset+2];
  code.bytes[2]=hash[offset+1];
  code.bytes[3]=hash[offset] & 0x7f;
  APP_LOG(APP_LOG_LEVEL_DEBUG,"Extracted 4 bytes from Hmac [%16lx].", code.number);
  APP_LOG(APP_LOG_LEVEL_DEBUG,"Writing new code to buffer [%06lu].", code.number%1000000);
  snprintf(buffer, 7, "%06lu", code.number%1000000);
}