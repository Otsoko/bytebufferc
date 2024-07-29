# ByteBuffer #
ByteBuffer implementation in C

## How to use ##
To include header
```c
#include "bytebuffer.h"
```
To create bytebuffers, must give the size in bytes. Default byte order is Little Endian.
```c
bytebuffer_t bb;

bb_init      (&bb, 9);
bb_init_order(&bb, 9, BB_BIG_ENDIAN);
bb_init_order(&bb, 9, BB_LITTLE_ENDIAN);
```
Or directly create a bytebuffer from a hex string
```c
bytebuffer_t bb;

bb_init_str      (&bb, "12345678");
bb_init_str_order(&bb, "12345678", BB_BIG_ENDIAN);
bb_init_str_order(&bb, "12345678", BB_LITTLE_ENDIAN);
```
At the end, the bytebuffers must be destroyed in order to avoid memory leaks
```c
bb_destroy(&bb);
```
Insert data at the current position
```c
bb_put       (&bb, 64);           // 1 byte
bb_put_short (&bb, 256);          // 2 bytes
bb_put_int   (&bb, 3645);         // 4 bytes
bb_put_float (&bb, 123.45f);      // 4 bytes
bb_put_long  (&bb, 123456789LL);  // 8 bytes
bb_put_double(&bb, 45678.456123); // 8 bytes
bb_put_string(&bb, "Kaixop", 6);  // 6 bytes
```
Insert data at a certain position
```c
bb_put_at       (&bb, 0, 64);            // 1 byte
bb_put_short_at (&bb, 1, 256);           // 2 bytes
bb_put_int_at   (&bb, 3, 3645);          // 4 bytes
bb_put_float_at (&bb, 7, 123.45f);       // 4 bytes
bb_put_long_at  (&bb, 11, 123456789LL);  // 8 bytes
bb_put_double_at(&bb, 19, 45678.456123); // 8 bytes
bb_put_string_at(&bb, 27, "Kaixop", 6);  // 6 bytes
```
Read data at the current position
```c
bb_get       (&bb);    // 1 byte
bb_get_short (&bb);    // 2 bytes
bb_get_int   (&bb);    // 4 bytes
bb_get_float (&bb);    // 4 bytes
bb_get_long  (&bb);    // 8 bytes
bb_get_double(&bb);    // 8 bytes
bb_get_string(&bb, 6); // 6 bytes
```
Read data at a certain position
```c
bb_get_at       (bb, 0);     // 1 byte
bb_get_short_at (bb, 1);     // 2 bytes
bb_get_int_at   (bb, 3);     // 4 bytes
bb_get_float_at (bb, 7);     // 4 bytes
bb_get_long_at  (bb, 11);    // 8 bytes
bb_get_double_at(bb, 19);    // 8 bytes
bb_get_string_at(bb, 27, 6); // 6 bytes
```
Use of bb_get_string* functions
```c
char *str = bb_get_string(bb, 6);
printf("Get string: [%s]\n", str);
free(str); // Don't forget to free memory

char *str = bb_get_string_at(bb, 2, 6);
printf("Get string at position 2: [%s]\n", str);
free(str); // Don't forget to free memory
```
Reset the current position to the beggining
```c
bb_reset(&bb);
```
Get the bytes array
```c
uint8_t *bytes = bb_get_bytes(bb);
```
Print the hex string
```c
char *hex_string = bb_get_hex_string(bb);
printf("Hex string: [%s]\n", hex_string);
free(hex_string); // Don't forget to free memory
```
To clone a bytebuffer
```c
bytebuffer_t cloned;

bb_clone(bb, &cloned); // Clone bb into cloned

bb_destroy(&bb);       // Don't forget to free memory
bb_destroy(&cloned);   // Don't forget to free memory
```
Print the content of the bytebuffer
```c
bb_print(bb);
```

## Build

To build this project run

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Run

To run this project run

```bash
$ ./app
```

## License

[Apache License 2.0](https://choosealicense.com/licenses/apache-2.0/)
