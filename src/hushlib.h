#include <stdint.h>

uint8_t* greeting();

uint8_t** parse_args(uint8_t* input_line);

uint8_t is_builtin(uint8_t** args);

int32_t run_builtin(uint8_t** args);
