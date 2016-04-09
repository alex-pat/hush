#include <stdbool.h>
#include <stdint.h>

#define EXIT 618

typedef struct builtin_function
{
    uint8_t* name;
    uint32_t (*func)(uint8_t** args);
} builtin_function;

bool is_builtin(uint8_t** args);

uint32_t run_builtin(uint8_t** args);
