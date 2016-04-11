#include <unistd.h>
#include <stdint.h>
#include <wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "hushlib.h"
#include "builtin.h"

int main(int argc, char *argv[])
{
    int32_t status;
    int8_t *input_line = NULL,
	 *prompt = NULL;
    int8_t **args = NULL;
    do
    {
	prompt = greeting();
	
	input_line = readline (prompt);

	args = parse_args(input_line);
	
	if (is_builtin( args )) {
	    status = run_builtin (args);
	} else {
	    pid_t pid = fork();
	    switch (pid) {
	    case 0: {
		execvp(args[0], (char**) args);
  		break;
	    }
	    case -1: {
		perror("fork");
		break;
	    }
	    default:
		if (waitpid(pid, &status, 0) != pid)
		    perror("wait");
		break;
	    }
	}
        free_var(input_line, prompt, args);
    } while (status != EXIT);
    
    return EXIT_SUCCESS;
}
