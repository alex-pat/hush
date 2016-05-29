#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "hushlib.h"
#include "builtin.h"

int main(int argc, char *argv[])
{
    int32_t status = 0;
    int8_t *input_line = NULL,
	 *prompt = NULL;
    int8_t **args = NULL;

    init();

    int8_t* history_path = get_history_path();
    read_history(history_path);
    
    do
    {
	prompt = greeting();
	
	input_line = readline (prompt);

	printf(ANSI_COLOR_NULL);
	fflush(stdout);
	
	if (input_line == NULL)
	{
	    free(prompt);
	    free(history_path);
	    break;
	}

	if (input_line[0] != '\0' &&
	    input_line[0] != ' ')
	{
	    add_history(input_line);
	    write_history(history_path);
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
