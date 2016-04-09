#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "builtin.h"

uint32_t hush_cd(uint8_t** args);
uint32_t hush_exit(uint8_t** args);

static builtin_function functions[] =
{
    { "cd", hush_cd },
    { "exit", hush_exit },
    { NULL, NULL }
};

static uint8_t builtin_number;

bool is_builtin(uint8_t** args)
{
    if (args == NULL)
	return -1;
    uint8_t number = 0;
    while (functions[number].name != NULL) {
	if ( strcmp(args[0], functions[number].name) == 0) {
	    builtin_number = number;
	    return true;
	}
	number++;
    }
    
    return false;
}

uint32_t run_builtin(uint8_t** args)
{
    uint32_t status = functions[builtin_number].func(args);
    return status;
}

uint32_t hush_cd(uint8_t** args)
{
    if (args == NULL || args[1] == NULL)	
	return -1;
    uint8_t status = chdir(args[1]);
    if (status == -1)
	perror("chdir");
    return status;
}

uint32_t hush_exit(uint8_t** args)
{
    return EXIT;
}
