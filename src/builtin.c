#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "builtin.h"

int32_t hush_cd(int8_t** args);
int32_t hush_exit(int8_t** args);
int32_t hush_help(int8_t** args);

static builtin_function functions[] =
{
    { "cd", hush_cd },
    { "exit", hush_exit },
    { "help", hush_help },
    { NULL, NULL }
};

static uint8_t builtin_number;

bool is_builtin(int8_t** args)
{
    if (args == NULL)
	return false;
    
    for (int8_t i = 0; args[i] != NULL; i++)
        if (strcmp(args[i], "|") == 0 ||
            strcmp(args[i], ">") == 0 )
            return false;
    
    uint8_t number = 0;
    while (functions[number].name != NULL)
    {
	if ( strcmp(args[0], functions[number].name) == 0)
	{
	    builtin_number = number;
	    return true;
	}
	number++;
    }
    
    return false;
}

int32_t run_builtin(int8_t** args)
{
    int32_t status = functions[builtin_number].func(args);
    return status;
}

int32_t hush_cd(int8_t** args)
{
    if (args == NULL)
	return -1;

    int8_t* dir;
    if (NULL == args[1]	|| 0 == strcmp(args[1], "\x7E"))
	dir = getenv("HOME");
    else
	dir = args[1];

    int32_t status = chdir(dir);
    if (status == -1)
	perror("cd");
    return status;
}

int32_t hush_exit(int8_t** args)
{
    return EXIT;
}

int32_t hush_help(int8_t** args)
{
    printf("\
 _               _     \n\
| |             | |    \n\
| |__  _   _ ___| |__  \n\
| '_ \\| | | / __| '_ \\ \n\
| | | | |_| \\__ \\ | | |\n\
|_| |_|\\__,_|___/_| |_|, v.0.01\n\
\n\
Commands:\n\
===BUILTINS===\n\
help\n\
cd [path]\n\
exit\n\
===OTHER===\n\
command &\n\
command < inputfile [> outputfile]\n\
cmd1 [| cmd2 [|cmd3 ... ]]\n\n");
    return 0;
}
