#include "bytebuffer.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    bytebuffer_t bb;

    bb_init(&bb, 6);
    bb_print(bb);
    printf("size: %zd\n", bb_get_size(bb));
    printf("%d\n", bb_get_bytes(bb)[0]);
    printf("pos: %zd\n", bb.pos);
    printf("byte: %d\n", bb_get(&bb));
    printf("pos: %zd\n", bb.pos);
    bb_reset(&bb);
    printf("pos: %zd\n", bb.pos);
    printf("byte: %d\n", bb_get_at(bb, 1));
    printf("short: %d\n", bb_get_short(&bb));
    bb_destroy(&bb);

    printf("\n");
    bb_init_str_order(&bb, "55FF03", BB_BIG_ENDIAN);
    bb_print(bb);
    printf("size: %zd\n", bb_get_size(bb));
    printf("%d\n", bb_get_bytes(bb)[0]);
    printf("pos: %zd\n", bb.pos);
    printf("byte: %d\n", bb_get(&bb));
    printf("pos: %zd\n", bb.pos);
    bb_reset(&bb);
    printf("pos: %zd\n", bb.pos);
    printf("byte: %d\n", bb_get_at(bb, 1));
    printf("short: %d\n", bb_get_short(&bb));
    bb_destroy(&bb);

    bb_init_order(&bb, 3, 3);
    bb_destroy(&bb);

    return 0;
}