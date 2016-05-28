#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "hushlib.h"
#include "builtin.h"

int main(int argc, char *argv[])
{
    int32_t status = 0;
    int8_t *input_line = NULL,
	 *prompt = NULL;
    int8_t **args = NULL;

    init();

    do
    {
	prompt = greeting();
	
	input_line = readline (prompt);

	if (input_line == NULL)
	{
	    free(prompt);
	    break;
	}

	args = parse_args(input_line);

	if (args == NULL)
	{
	    free(prompt);
	    free(input_line);
	    input_line = NULL;
	    continue;
	}

	status = handle_command(args);
	
	free(prompt);
	free(input_line);
	input_line = NULL;
	free(args);
	
    } while (status != EXIT);
    
    return EXIT_SUCCESS;
}
