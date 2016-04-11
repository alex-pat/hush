#include <stdint.h>

int8_t* greeting();

int8_t** parse_args(int8_t* input_line);

void free_var(int8_t* line, int8_t* prompt, int8_t** args);
