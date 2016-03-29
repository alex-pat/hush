#include <readline/readline.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    int32_t status;
    uint8_t *input_line = NULL,
	 *prompt = NULL;
    uint8_t **args = NULL;
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
		execvp(args[0], args);
  		break;
	    }
	    case -1: {
		perror("hush");
		break;
	    }
	    default:
		status = waitpid(pid, 0, 0);
		// TODO: get return status 
		break;
	    }
	}
	// TODO: background process
    } while (status != 0);
    
    return 0;
}
