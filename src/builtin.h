#include <stdbool.h>
#include <stdint.h>

#define EXIT 618

typedef struct builtin_function
{
    int8_t* name;
    int32_t (*func)(int8_t** args);
} builtin_function;

bool is_builtin(int8_t** args);

int32_t run_builtin(int8_t** args);
