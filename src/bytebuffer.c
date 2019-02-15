#include "bytebuffer.h"
#include <stdio.h>
#include <string.h>

static bb_order_t bb_get_machine_endianness(void) {
    uint32_t   i = 1;
    char *     c = (char *) &i;
    bb_order_t order;

    if (*c) {
        printf("This machine is little endian\n");
        order = BB_LITTLE_ENDIAN;
    } else {
        printf("This machine is big endian");
        order = BB_BIG_ENDIAN;
    }

    return order;
}

static float bb_int_bits_to_float(uint32_t value) {
    FloatB valueB;

    if (bb_get_machine_endianness() == BB_LITTLE_ENDIAN) {
        valueB.bytes[0] = value & 0xFF;
        valueB.bytes[1] = (value >> 8) & 0xFF;
        valueB.bytes[2] = (value >> 16) & 0xFF;
        valueB.bytes[3] = (value >> 24) & 0xFF;
    } else {
        valueB.bytes[3] = value & 0xFF;
        valueB.bytes[2] = (value >> 8) & 0xFF;
        valueB.bytes[1] = (value >> 16) & 0xFF;
        valueB.bytes[0] = (value >> 24) & 0xFF;
    }

    return valueB.value;
}

/*
 * Initialization and destruction
 */

void bb_init(bytebuffer_t *bytebuffer, size_t size) {
    printf("Init\n");
    bytebuffer->size      = size;
    bytebuffer->pos       = 0;
    bytebuffer->buff      = (uint8_t *) malloc(size * sizeof(uint8_t));
    bytebuffer->bigEndian = FALSE;
}

void bb_init_order(bytebuffer_t *bytebuffer, size_t size, bb_order_t order) {
    printf("Init order\n");
    bytebuffer->size = size;
    bytebuffer->pos  = 0;
    bytebuffer->buff = (uint8_t *) malloc(size * sizeof(uint8_t));

    if (order == BB_LITTLE_ENDIAN) {
        bytebuffer->bigEndian = FALSE;
    } else {
        bytebuffer->bigEndian = TRUE;
    }
}

void bb_init_str(bytebuffer_t *bytebuffer, const char *hexstring) {
    bb_get_machine_endianness();
    printf("Init str\n");
    bytebuffer->size      = strlen(hexstring) / 2;
    bytebuffer->pos       = 0;
    bytebuffer->buff      = (uint8_t *) malloc(bytebuffer->size * sizeof(uint8_t));
    bytebuffer->bigEndian = FALSE;

    for (size_t i = 0; i < bytebuffer->size * 2; i += 2) {
        int  c = 0;
        char cb[3];
        cb[0] = *(hexstring + i);
        cb[1] = hexstring[i + 1];
        cb[2] = '\0';
        sscanf(cb, "%x", &c);
        bytebuffer->buff[i / 2] = c;
    }
}

void bb_init_str_order(bytebuffer_t *bytebuffer, const char *hexstring, bb_order_t order) {
    printf("Init str\n");
    bytebuffer->size = strlen(hexstring) / 2;
    bytebuffer->pos  = 0;
    bytebuffer->buff = (uint8_t *) malloc(bytebuffer->size * sizeof(uint8_t));

    if (order == BB_LITTLE_ENDIAN) {
        bytebuffer->bigEndian = FALSE;
    } else {
        bytebuffer->bigEndian = TRUE;
    }

    for (size_t i = 0; i < bytebuffer->size * 2; i += 2) {
        int  c = 0;
        char cb[3];
        cb[0] = *(hexstring + i);
        cb[1] = hexstring[i + 1];
        cb[2] = '\0';
        sscanf(cb, "%x", &c);
        bytebuffer->buff[i / 2] = c;
    }
}

void bb_destroy(bytebuffer_t *bytebuffer) {
    printf("Destroy\n");
    bytebuffer->size = 0;
    bytebuffer->pos  = 0;
    free(bytebuffer->buff);
}

void bb_print(bytebuffer_t bytebuffer) {
    for (int i = 0; i < bytebuffer.size; i++) {
        printf("%d ", bytebuffer.buff[i]);
    }
    printf("\n");
}

size_t bb_get_size(bytebuffer_t bytebuffer) {
    return bytebuffer.size;
}

void bb_reset(bytebuffer_t *bytebuffer) {
    bytebuffer->pos = 0;
}

uint8_t *bb_get_bytes(bytebuffer_t bytebuffer) {
    return bytebuffer.buff;
}

/*
 * Get methods
 */

uint8_t bb_get(bytebuffer_t *bytebuffer) {
    return bytebuffer->buff[bytebuffer->pos++];
}

uint8_t bb_get_at(bytebuffer_t bytebuffer, size_t index) {
    return bytebuffer.buff[index];
}

static uint16_t bb_get_short_l(bytebuffer_t *bytebuffer) {
    uint8_t b1 = bytebuffer->buff[bytebuffer->pos + 1];
    uint8_t b0 = bytebuffer->buff[bytebuffer->pos];

    bytebuffer->pos += 2;

    return ((b1 & 0xFF) << 8) | (b0 & 0xFF);
}

static uint16_t bb_get_short_b(bytebuffer_t *bytebuffer) {
    uint8_t b0 = bytebuffer->buff[bytebuffer->pos + 1];
    uint8_t b1 = bytebuffer->buff[bytebuffer->pos];

    bytebuffer->pos += 2;

    return ((b1 & 0xFF) << 8) | (b0 & 0xFF);
}

uint16_t bb_get_short(bytebuffer_t *bytebuffer) {
    uint16_t res = 0;
    if (bytebuffer->bigEndian) {
        printf("get short big endian\n");
        res = bb_get_short_b(bytebuffer);
    } else {
        printf("get short little endian\n");
        res = bb_get_short_l(bytebuffer);
    }
    return res;
}

static uint16_t bb_get_short_l_at(bytebuffer_t bytebuffer, size_t index) {
    uint8_t b1 = bytebuffer.buff[index + 1];
    uint8_t b0 = bytebuffer.buff[index];

    return ((b1 & 0xFF) << 8) | (b0 & 0xFF);
}

static uint16_t bb_get_short_b_at(bytebuffer_t bytebuffer, size_t index) {
    uint8_t b0 = bytebuffer.buff[index + 1];
    uint8_t b1 = bytebuffer.buff[index];

    return ((b1 & 0xFF) << 8) | (b0 & 0xFF);
}

uint16_t bb_get_short_at(bytebuffer_t bytebuffer, size_t index) {
    uint16_t res = 0;
    if (bytebuffer.bigEndian) {
        printf("get short big endian at\n");
        res = bb_get_short_b_at(bytebuffer, index);
    } else {
        printf("get short little endian at\n");
        res = bb_get_short_l_at(bytebuffer, index);
    }
    return res;
}

static uint32_t bb_get_int_l(bytebuffer_t *bytebuffer) {
    uint8_t b3 = bytebuffer->buff[bytebuffer->pos + 3];
    uint8_t b2 = bytebuffer->buff[bytebuffer->pos + 2];
    uint8_t b1 = bytebuffer->buff[bytebuffer->pos + 1];
    uint8_t b0 = bytebuffer->buff[bytebuffer->pos];

    bytebuffer->pos += 4;

    return (b3 << 24) | ((b2 & 0xFF) << 16) | ((b1 & 0xFF) << 8) | (b0 & 0xFF);
}

static uint32_t bb_get_int_b(bytebuffer_t *bytebuffer) {
    uint8_t b0 = bytebuffer->buff[bytebuffer->pos + 3];
    uint8_t b1 = bytebuffer->buff[bytebuffer->pos + 2];
    uint8_t b2 = bytebuffer->buff[bytebuffer->pos + 1];
    uint8_t b3 = bytebuffer->buff[bytebuffer->pos];

    bytebuffer->pos += 4;

    return (b3 << 24) | ((b2 & 0xFF) << 16) | ((b1 & 0xFF) << 8) | (b0 & 0xFF);
}

uint32_t bb_get_int(bytebuffer_t *bytebuffer) {
    uint32_t res = 0;
    if (bytebuffer->bigEndian) {
        printf("get int big endian\n");
        res = bb_get_int_b(bytebuffer);
    } else {
        printf("get int little endian\n");
        res = bb_get_int_l(bytebuffer);
    }
    return res;
}

static uint32_t bb_get_int_l_at(bytebuffer_t bytebuffer, size_t index) {
    uint8_t b3 = bytebuffer.buff[index + 3];
    uint8_t b2 = bytebuffer.buff[index + 2];
    uint8_t b1 = bytebuffer.buff[index + 1];
    uint8_t b0 = bytebuffer.buff[index];

    return (b3 << 24) | ((b2 & 0xFF) << 16) | ((b1 & 0xFF) << 8) | (b0 & 0xFF);
}

static uint32_t bb_get_int_b_at(bytebuffer_t bytebuffer, size_t index) {
    uint8_t b0 = bytebuffer.buff[index + 3];
    uint8_t b1 = bytebuffer.buff[index + 2];
    uint8_t b2 = bytebuffer.buff[index + 1];
    uint8_t b3 = bytebuffer.buff[index];

    return (b3 << 24) | ((b2 & 0xFF) << 16) | ((b1 & 0xFF) << 8) | (b0 & 0xFF);
}

uint32_t bb_get_int_at(bytebuffer_t bytebuffer, size_t index) {
    uint32_t res = 0;
    if (bytebuffer.bigEndian) {
        printf("get int big endian at\n");
        res = bb_get_int_b_at(bytebuffer, index);
    } else {
        printf("get int little endian at\n");
        res = bb_get_int_l_at(bytebuffer, index);
    }
    return res;
}

float bb_get_float(bytebuffer_t *bytebuffer) {
    return bb_int_bits_to_float(bb_get_int(bytebuffer));
}

float bb_get_float_at(bytebuffer_t bytebuffer, size_t index) {
    return bb_int_bits_to_float(bb_get_int_at(bytebuffer, index));
}

static uint64_t bb_get_long_l(bytebuffer_t *bytebuffer) {
    uint8_t b7 = bytebuffer->buff[bytebuffer->pos + 7];
    uint8_t b6 = bytebuffer->buff[bytebuffer->pos + 6];
    uint8_t b5 = bytebuffer->buff[bytebuffer->pos + 5];
    uint8_t b4 = bytebuffer->buff[bytebuffer->pos + 4];
    uint8_t b3 = bytebuffer->buff[bytebuffer->pos + 3];
    uint8_t b2 = bytebuffer->buff[bytebuffer->pos + 2];
    uint8_t b1 = bytebuffer->buff[bytebuffer->pos + 1];
    uint8_t b0 = bytebuffer->buff[bytebuffer->pos];

    bytebuffer->pos += 8;

    uint64_t ll = (((uint64_t) b7 & 0xFF) << 56) | (((uint64_t) b6 & 0xFF) << 48) |
                  (((uint64_t) b5 & 0xFF) << 40) | (((uint64_t) b4 & 0xFF) << 32) |
                  (((uint64_t) b3 & 0xFF) << 24) | (((uint64_t) b2 & 0xFF) << 16) |
                  (((uint64_t) b1 & 0xFF) << 8) | ((uint64_t) b0 & 0xFF);

    return ll;
}

static uint64_t bb_get_long_b(bytebuffer_t *bytebuffer) {
    uint8_t b0 = bytebuffer->buff[bytebuffer->pos + 7];
    uint8_t b1 = bytebuffer->buff[bytebuffer->pos + 6];
    uint8_t b2 = bytebuffer->buff[bytebuffer->pos + 5];
    uint8_t b3 = bytebuffer->buff[bytebuffer->pos + 4];
    uint8_t b4 = bytebuffer->buff[bytebuffer->pos + 3];
    uint8_t b5 = bytebuffer->buff[bytebuffer->pos + 2];
    uint8_t b6 = bytebuffer->buff[bytebuffer->pos + 1];
    uint8_t b7 = bytebuffer->buff[bytebuffer->pos];

    bytebuffer->pos += 8;

    uint64_t ll = (((uint64_t) b7 & 0xFF) << 56) | (((uint64_t) b6 & 0xFF) << 48) |
                  (((uint64_t) b5 & 0xFF) << 40) | (((uint64_t) b4 & 0xFF) << 32) |
                  (((uint64_t) b3 & 0xFF) << 24) | (((uint64_t) b2 & 0xFF) << 16) |
                  (((uint64_t) b1 & 0xFF) << 8) | ((uint64_t) b0 & 0xFF);

    return ll;
}

uint64_t bb_get_long(bytebuffer_t *bytebuffer) {
    uint64_t res = 0;
    if (bytebuffer->bigEndian) {
        printf("get long big endian\n");
        res = bb_get_long_b(bytebuffer);
    } else {
        printf("get long little endian\n");
        res = bb_get_long_l(bytebuffer);
    }
    return res;
}

static uint64_t bb_get_long_l_at(bytebuffer_t bytebuffer, size_t index) {
    uint8_t b7 = bytebuffer.buff[index + 7];
    uint8_t b6 = bytebuffer.buff[index + 6];
    uint8_t b5 = bytebuffer.buff[index + 5];
    uint8_t b4 = bytebuffer.buff[index + 4];
    uint8_t b3 = bytebuffer.buff[index + 3];
    uint8_t b2 = bytebuffer.buff[index + 2];
    uint8_t b1 = bytebuffer.buff[index + 1];
    uint8_t b0 = bytebuffer.buff[index];

    uint64_t ll = (((uint64_t) b7 & 0xFF) << 56) | (((uint64_t) b6 & 0xFF) << 48) |
                  (((uint64_t) b5 & 0xFF) << 40) | (((uint64_t) b4 & 0xFF) << 32) |
                  (((uint64_t) b3 & 0xFF) << 24) | (((uint64_t) b2 & 0xFF) << 16) |
                  (((uint64_t) b1 & 0xFF) << 8) | ((uint64_t) b0 & 0xFF);

    return ll;
}

static uint64_t bb_get_long_b_at(bytebuffer_t bytebuffer, size_t index) {
    uint8_t b0 = bytebuffer.buff[index + 7];
    uint8_t b1 = bytebuffer.buff[index + 6];
    uint8_t b2 = bytebuffer.buff[index + 5];
    uint8_t b3 = bytebuffer.buff[index + 4];
    uint8_t b4 = bytebuffer.buff[index + 3];
    uint8_t b5 = bytebuffer.buff[index + 2];
    uint8_t b6 = bytebuffer.buff[index + 1];
    uint8_t b7 = bytebuffer.buff[index];

    uint64_t ll = (((uint64_t) b7 & 0xFF) << 56) | (((uint64_t) b6 & 0xFF) << 48) |
                  (((uint64_t) b5 & 0xFF) << 40) | (((uint64_t) b4 & 0xFF) << 32) |
                  (((uint64_t) b3 & 0xFF) << 24) | (((uint64_t) b2 & 0xFF) << 16) |
                  (((uint64_t) b1 & 0xFF) << 8) | ((uint64_t) b0 & 0xFF);

    return ll;
}

uint64_t bb_get_long_at(bytebuffer_t bytebuffer, size_t index) {
    uint64_t res = 0;
    if (bytebuffer.bigEndian) {
        printf("get long big endian at\n");
        res = bb_get_long_b_at(bytebuffer, index);
    } else {
        printf("get long little endian at\n");
        res = bb_get_long_l_at(bytebuffer, index);
    }
    return res;
}

/*
 * Put methods
 */

void bb_put(bytebuffer_t *bytebuffer, uint8_t value) {
    bytebuffer->buff[bytebuffer->pos++] = value;
}

void bb_put_at(bytebuffer_t *bytebuffer, size_t index, uint8_t value) {
    bytebuffer->buff[index] = value;
}

static void bb_put_short_l(bytebuffer_t *bytebuffer, uint16_t value) {
    bytebuffer->buff[bytebuffer->pos++] = value & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 8) & 0xFF;
}

static void bb_put_short_b(bytebuffer_t *bytebuffer, uint16_t value) {
    bytebuffer->buff[bytebuffer->pos++] = (value >> 8) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = value & 0xFF;
}

void bb_put_short(bytebuffer_t *bytebuffer, uint16_t value) {
    if (bytebuffer->bigEndian) {
        printf("put short big endian\n");
        bb_put_short_b(bytebuffer, value);
    } else {
        printf("put short little endian\n");
        bb_put_short_l(bytebuffer, value);
    }
}

static void bb_put_short_l_at(bytebuffer_t *bytebuffer, size_t index, uint16_t value) {
    bytebuffer->buff[index++] = value & 0xFF;
    bytebuffer->buff[index]   = (value >> 8) & 0xFF;
}

static void bb_put_short_b_at(bytebuffer_t *bytebuffer, size_t index, uint16_t value) {
    bytebuffer->buff[index++] = (value >> 8) & 0xFF;
    bytebuffer->buff[index]   = value & 0xFF;
}

void bb_put_short_at(bytebuffer_t *bytebuffer, size_t index, uint16_t value) {
    if (bytebuffer->bigEndian) {
        printf("put short at big endian\n");
        bb_put_short_b_at(bytebuffer, index, value);
    } else {
        printf("put short at little endian\n");
        bb_put_short_l_at(bytebuffer, index, value);
    }
}

static void bb_put_int_l(bytebuffer_t *bytebuffer, uint32_t value) {
    bytebuffer->buff[bytebuffer->pos++] = value & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 8) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 16) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 24) & 0xFF;
}

static void bb_put_int_b(bytebuffer_t *bytebuffer, uint32_t value) {
    bytebuffer->buff[bytebuffer->pos++] = (value >> 24) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 16) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 8) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = value & 0xFF;
}

void bb_put_int(bytebuffer_t *bytebuffer, uint32_t value) {
    if (bytebuffer->bigEndian) {
        printf("put int big endian\n");
        bb_put_int_b(bytebuffer, value);
    } else {
        printf("put int little endian\n");
        bb_put_int_l(bytebuffer, value);
    }
}

static void bb_put_int_l_at(bytebuffer_t *bytebuffer, size_t index, uint32_t value) {
    bytebuffer->buff[index++] = value & 0xFF;
    bytebuffer->buff[index++] = (value >> 8) & 0xFF;
    bytebuffer->buff[index++] = (value >> 16) & 0xFF;
    bytebuffer->buff[index]   = (value >> 24) & 0xFF;
}

static void bb_put_int_b_at(bytebuffer_t *bytebuffer, size_t index, uint32_t value) {
    bytebuffer->buff[index++] = (value >> 24) & 0xFF;
    bytebuffer->buff[index++] = (value >> 16) & 0xFF;
    bytebuffer->buff[index++] = (value >> 8) & 0xFF;
    bytebuffer->buff[index]   = value & 0xFF;
}

void bb_put_int_at(bytebuffer_t *bytebuffer, size_t index, uint32_t value) {
    if (bytebuffer->bigEndian) {
        printf("put int at big endian\n");
        bb_put_int_b_at(bytebuffer, index, value);
    } else {
        printf("put int at little endian\n");
        bb_put_int_l_at(bytebuffer, index, value);
    }
}

static void bb_put_float_l(bytebuffer_t *bytebuffer, float value) {
    FloatB valueB;
    valueB.value = value;

    bytebuffer->buff[bytebuffer->pos++] = valueB.bytes[0];
    bytebuffer->buff[bytebuffer->pos++] = valueB.bytes[1];
    bytebuffer->buff[bytebuffer->pos++] = valueB.bytes[2];
    bytebuffer->buff[bytebuffer->pos++] = valueB.bytes[3];
}

static void bb_put_float_b(bytebuffer_t *bytebuffer, float value) {
    FloatB valueB;
    valueB.value = value;

    bytebuffer->buff[bytebuffer->pos++] = valueB.bytes[3];
    bytebuffer->buff[bytebuffer->pos++] = valueB.bytes[2];
    bytebuffer->buff[bytebuffer->pos++] = valueB.bytes[1];
    bytebuffer->buff[bytebuffer->pos++] = valueB.bytes[0];
}

void bb_put_float(bytebuffer_t *bytebuffer, float value) {
    if (bytebuffer->bigEndian) {
        bb_put_float_b(bytebuffer, value);
    } else {
        bb_put_float_l(bytebuffer, value);
    }
}

static void bb_put_float_l_at(bytebuffer_t *bytebuffer, size_t index, float value) {
    FloatB valueB;
    valueB.value = value;

    bytebuffer->buff[index++] = valueB.bytes[0];
    bytebuffer->buff[index++] = valueB.bytes[1];
    bytebuffer->buff[index++] = valueB.bytes[2];
    bytebuffer->buff[index]   = valueB.bytes[3];
}

static void bb_put_float_b_at(bytebuffer_t *bytebuffer, size_t index, float value) {
    FloatB valueB;
    valueB.value = value;

    bytebuffer->buff[index++] = valueB.bytes[3];
    bytebuffer->buff[index++] = valueB.bytes[2];
    bytebuffer->buff[index++] = valueB.bytes[1];
    bytebuffer->buff[index]   = valueB.bytes[0];
}

void bb_put_float_at(bytebuffer_t *bytebuffer, size_t index, float value) {
    if (bytebuffer->bigEndian) {
        printf("put float at big endian\n");
        bb_put_float_b_at(bytebuffer, index, value);
    } else {
        printf("put float at little endian\n");
        bb_put_float_l_at(bytebuffer, index, value);
    }
}

static void bb_put_long_l(bytebuffer_t *bytebuffer, uint64_t value) {
    bytebuffer->buff[bytebuffer->pos++] = value & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 8) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 16) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 24) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 32) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 40) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 48) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 56) & 0xFF;
}

static void bb_put_long_b(bytebuffer_t *bytebuffer, uint64_t value) {
    bytebuffer->buff[bytebuffer->pos++] = (value >> 56) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 48) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 40) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 32) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 24) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 16) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = (value >> 8) & 0xFF;
    bytebuffer->buff[bytebuffer->pos++] = value & 0xFF;
}

void bb_put_long(bytebuffer_t *bytebuffer, uint64_t value) {
    if (bytebuffer->bigEndian) {
        printf("put long big endian\n");
        bb_put_long_b(bytebuffer, value);
    } else {
        printf("put long little endian\n");
        bb_put_long_l(bytebuffer, value);
    }
}

static void bb_put_long_l_at(bytebuffer_t *bytebuffer, size_t index, uint64_t value) {
    bytebuffer->buff[index++] = value & 0xFF;
    bytebuffer->buff[index++] = (value >> 8) & 0xFF;
    bytebuffer->buff[index++] = (value >> 16) & 0xFF;
    bytebuffer->buff[index++] = (value >> 24) & 0xFF;
    bytebuffer->buff[index++] = (value >> 32) & 0xFF;
    bytebuffer->buff[index++] = (value >> 40) & 0xFF;
    bytebuffer->buff[index++] = (value >> 48) & 0xFF;
    bytebuffer->buff[index]   = (value >> 56) & 0xFF;
}

static void bb_put_long_b_at(bytebuffer_t *bytebuffer, size_t index, uint64_t value) {
    bytebuffer->buff[index++] = (value >> 56) & 0xFF;
    bytebuffer->buff[index++] = (value >> 48) & 0xFF;
    bytebuffer->buff[index++] = (value >> 40) & 0xFF;
    bytebuffer->buff[index++] = (value >> 32) & 0xFF;
    bytebuffer->buff[index++] = (value >> 24) & 0xFF;
    bytebuffer->buff[index++] = (value >> 16) & 0xFF;
    bytebuffer->buff[index++] = (value >> 8) & 0xFF;
    bytebuffer->buff[index++] = value & 0xFF;
}

void bb_put_long_at(bytebuffer_t *bytebuffer, size_t index, uint64_t value) {
    if (bytebuffer->bigEndian) {
        printf("put long at big endian\n");
        bb_put_long_b_at(bytebuffer, index, value);
    } else {
        printf("put long at little endian\n");
        bb_put_long_l_at(bytebuffer, index, value);
    }
}

static void bb_put_double_l(bytebuffer_t *bytebuffer, double value) {
    DoubleB valueB;
    valueB.value = value;

    for (int i = 0; i < 8; i++) {
        bytebuffer->buff[bytebuffer->pos++] = valueB.bytes[i];
    }
}

static void bb_put_double_b(bytebuffer_t *bytebuffer, double value) {
    DoubleB valueB;
    valueB.value = value;

    for (int i = 7; i >= 0; i--) {
        bytebuffer->buff[bytebuffer->pos++] = valueB.bytes[i];
    }
}

void bb_put_double(bytebuffer_t *bytebuffer, double value) {
    if (bytebuffer->bigEndian) {
        printf("put double big endian\n");
        bb_put_double_b(bytebuffer, value);
    } else {
        printf("put double little endian\n");
        bb_put_double_l(bytebuffer, value);
    }
}

void bb_put_double_l_at(bytebuffer_t *bytebuffer, size_t index, double value) {
    DoubleB valueB;
    valueB.value = value;

    for (int i = 0; i < 8; i++) {
        bytebuffer->buff[index++] = valueB.bytes[i];
    }
}

void bb_put_double_b_at(bytebuffer_t *bytebuffer, size_t index, double value) {
    DoubleB valueB;
    valueB.value = value;

    for (int i = 7; i >= 0; i--) {
        bytebuffer->buff[index++] = valueB.bytes[i];
    }
}

void bb_put_double_at(bytebuffer_t *bytebuffer, size_t index, double value) {
    if (bytebuffer->bigEndian) {
        printf("put double at big endian\n");
        bb_put_double_b_at(bytebuffer, index, value);
    } else {
        printf("put double at little endian\n");
        bb_put_double_l_at(bytebuffer, index, value);
    }
}