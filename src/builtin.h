#if !defined(BUILTIN_H)
#define BUILTIN_H

#include <stdbool.h>
#include <stdint.h>

#define EXIT 618
#define ANSI_COLOR_PWD     "\e[0;48;05;11;38;05;16m"
#define ANSI_COLOR_ARROW   "\e[0;38;05;11m"
#define ANSI_COLOR_CMD     "\e[0;01;38;05;11m"
#define ANSI_COLOR_NULL    "\e[0m"

typedef struct builtin_function
{
    int8_t* name;
    int32_t (*func)(int8_t** args);
} builtin_function;

bool is_builtin(int8_t** args);

int32_t run_builtin(int8_t** args);

#endif // BUILTIN_H
