#include <stdint.h>

uint8_t* greeting();

uint8_t** parse_args(uint8_t* input_line);

void free_var(uint8_t* line, uint8_t* prompt, uint8_t** args);
