#include <stdint.h>

#define EXIT 618

int8_t* greeting();

int8_t handle_command(int8_t** args);

int8_t** parse_args(int8_t* input_line);
