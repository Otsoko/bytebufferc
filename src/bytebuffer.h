#ifndef _BYTEBUFFER_H_
#define _BYTEBUFFER_H_

#include <stdint.h>
#include <stdlib.h>

typedef long long Long;

typedef union {
    float   value;
    uint8_t bytes[4];
} FloatB;

typedef union {
    double  value;
    uint8_t bytes[8];
} DoubleB;

typedef enum { BB_LITTLE_ENDIAN,
               BB_BIG_ENDIAN } bb_order_t;

#define TRUE 1
#define FALSE 0

typedef struct {
    uint8_t *buff;
    size_t   size;
    size_t   pos;
    uint8_t  bigEndian;
} bytebuffer_t;

void     bb_init(bytebuffer_t *bytebuffer, size_t size);
void     bb_init_order(bytebuffer_t *bytebuffer, size_t size, bb_order_t order);
void     bb_init_str(bytebuffer_t *bytebuffer, const char *hexstring);
void     bb_init_str_order(bytebuffer_t *bytebuffer, const char *hexstring, bb_order_t order);
void     bb_destroy(bytebuffer_t *bytebuffer);
void     bb_print(bytebuffer_t bytebuffer);
size_t   bb_get_size(bytebuffer_t bytebuffer);
void     bb_reset(bytebuffer_t *bytebuffer);
uint8_t *bb_get_bytes(bytebuffer_t bytebuffer);

uint8_t  bb_get(bytebuffer_t *bytebuffer);
uint8_t  bb_get_at(bytebuffer_t bytebuffer, size_t index);
uint16_t bb_get_short(bytebuffer_t *bytebuffer);
uint16_t bb_get_short_at(bytebuffer_t bytebuffer, size_t index);
uint32_t bb_get_int(bytebuffer_t *bytebuffer);
uint32_t bb_get_int_at(bytebuffer_t bytebuffer, size_t index);
float    bb_get_float(bytebuffer_t *bytebuffer);
float    bb_get_float_at(bytebuffer_t bytebuffer, size_t index);

void bb_put(bytebuffer_t *bytebuffer, uint8_t value);
void bb_put_at(bytebuffer_t *bytebuffer, size_t index, uint8_t value);
void bb_put_short(bytebuffer_t *bytebuffer, uint16_t value);
void bb_put_short_at(bytebuffer_t *bytebuffer, size_t index, uint16_t value);
void bb_put_int(bytebuffer_t *bytebuffer, uint32_t value);

#endif