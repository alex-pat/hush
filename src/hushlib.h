#if !defined(HUSHLIB_H)
#define HUSHLIB_H

#include <stdint.h>

#define EXIT 618

void init();

int8_t* greeting();

int32_t handle_command(int8_t** args);

int8_t** parse_args(int8_t* input_line);

#endif // HUSHLIB_H
