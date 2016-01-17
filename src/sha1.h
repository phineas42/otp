#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <pebble.h>
  
#define HASH_LENGTH 20
#define BLOCK_LENGTH 64

typedef struct sha1nfo {
        uint32_t buffer[BLOCK_LENGTH/4];
        uint32_t state[HASH_LENGTH/4];
        uint32_t byteCount;
        uint8_t bufferOffset;
        uint8_t keyBuffer[BLOCK_LENGTH];
        uint8_t innerHash[HASH_LENGTH];
} sha1nfo;

void sha1_init(sha1nfo *s);
void sha1_writebyte(sha1nfo *s, uint8_t data);
void sha1_write(sha1nfo *s, const char *data, size_t len);
uint8_t* sha1_result(sha1nfo *s);
void sha1_initHmac(sha1nfo *s, const uint8_t* key, int keyLength);
uint8_t* sha1_resultHmac(sha1nfo *s);

/*
#define BO(buf,o) (*(((uint8_t *)buf)+o))
#define SHA1DUMP(s) APP_LOG(APP_LOG_LEVEL_DEBUG,"sha1nfo"); \
                    APP_LOG(APP_LOG_LEVEL_DEBUG,"  buffer: 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",\
                            BO(s->buffer,0),BO(s->buffer,1),BO(s->buffer,2),BO(s->buffer,3),\
                              BO(s->buffer,4),BO(s->buffer,5),BO(s->buffer,6),BO(s->buffer,7),\
                              BO(s->buffer,8),BO(s->buffer,9),BO(s->buffer,10),BO(s->buffer,11),\
                              BO(s->buffer,12),BO(s->buffer,13),BO(s->buffer,14),BO(s->buffer,15),\
                              BO(s->buffer,16),BO(s->buffer,17),BO(s->buffer,18),BO(s->buffer,19));\
                    APP_LOG(APP_LOG_LEVEL_DEBUG,"  state0: 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",\
                              BO(s->state,0),BO(s->state,1),BO(s->state,2),BO(s->state,3),\
                              BO(s->state,4),BO(s->state,5),BO(s->state,6),BO(s->state,7),\
                              BO(s->state,8),BO(s->state,9),BO(s->state,10),BO(s->state,11),\
                              BO(s->state,12),BO(s->state,13),BO(s->state,14),BO(s->state,15));\
                    APP_LOG(APP_LOG_LEVEL_DEBUG,"  state1: 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",\
                              BO(s->state,16),BO(s->state,17),BO(s->state,18),BO(s->state,19),\
                              BO(s->state,20),BO(s->state,21),BO(s->state,22),BO(s->state,23),\
                              BO(s->state,24),BO(s->state,25),BO(s->state,26),BO(s->state,27),\
                              BO(s->state,28),BO(s->state,29),BO(s->state,30),BO(s->state,31));\
                    APP_LOG(APP_LOG_LEVEL_DEBUG,"  state2: 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",\
                              BO(s->state,32),BO(s->state,33),BO(s->state,34),BO(s->state,35),\
                              BO(s->state,36),BO(s->state,37),BO(s->state,38),BO(s->state,39),\
                              BO(s->state,40),BO(s->state,41),BO(s->state,42),BO(s->state,43),\
                              BO(s->state,44),BO(s->state,45),BO(s->state,46),BO(s->state,47));\
                     APP_LOG(APP_LOG_LEVEL_DEBUG,"  state3: 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",\
                              BO(s->state,48),BO(s->state,49),BO(s->state,50),BO(s->state,51),\
                              BO(s->state,52),BO(s->state,53),BO(s->state,54),BO(s->state,55),\
                              BO(s->state,56),BO(s->state,57),BO(s->state,58),BO(s->state,59),\
                              BO(s->state,60),BO(s->state,61),BO(s->state,62),BO(s->state,63));\
                     APP_LOG(APP_LOG_LEVEL_DEBUG,"  byteCount: %lu", s->byteCount);\
                     APP_LOG(APP_LOG_LEVEL_DEBUG,"  bufferOffset: %u", s->bufferOffset);\
                     APP_LOG(APP_LOG_LEVEL_DEBUG,"  keyBuffer: 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",\
                              BO(s->keyBuffer,0),BO(s->keyBuffer,1),BO(s->keyBuffer,2),BO(s->keyBuffer,3),\
                              BO(s->keyBuffer,4),BO(s->keyBuffer,5),BO(s->keyBuffer,6),BO(s->keyBuffer,7),\
                              BO(s->keyBuffer,8),BO(s->keyBuffer,9),BO(s->keyBuffer,10),BO(s->keyBuffer,11),\
                              BO(s->keyBuffer,12),BO(s->keyBuffer,13),BO(s->keyBuffer,14),BO(s->keyBuffer,15),\
                              BO(s->keyBuffer,16),BO(s->keyBuffer,17),BO(s->keyBuffer,18),BO(s->keyBuffer,19));\
                      APP_LOG(APP_LOG_LEVEL_DEBUG,"  innerHash: 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",\
                              BO(s->innerHash,0),BO(s->innerHash,1),BO(s->innerHash,2),BO(s->innerHash,3),\
                              BO(s->innerHash,4),BO(s->innerHash,5),BO(s->innerHash,6),BO(s->innerHash,7),\
                              BO(s->innerHash,8),BO(s->innerHash,9),BO(s->innerHash,10),BO(s->innerHash,11),\
                              BO(s->innerHash,12),BO(s->innerHash,13),BO(s->innerHash,14),BO(s->innerHash,15),\
                              BO(s->innerHash,16),BO(s->innerHash,17),BO(s->innerHash,18),BO(s->innerHash,19))
                              */
