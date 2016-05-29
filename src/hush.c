#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "hushlib.h"
#include "globals.h"
#include "builtin.h"

#define ANSI_COLOR_BLUE    "\e[0;48;05;11;38;05;16m"
#define ANSI_COLOR_ARROW   "\e[0;38;05;11m"
#define ANSI_COLOR_RESET   "\e[0;01;38;05;11m"

uint8_t calculate_argc(int8_t* line);
int8_t get_next_separator(int8_t** args);
int32_t exec_pipe(int8_t** args);
int32_t exec_simple_cmd(int8_t** args, int8_t background);
int32_t exec_file_io(int8_t** args, int8_t* input, int8_t* output);

void init()
{
    GBSH_PID = getpid();
    GBSH_IS_INTERACTIVE = isatty(STDIN_FILENO);  

    if (GBSH_IS_INTERACTIVE)
    {
	while (tcgetpgrp(STDIN_FILENO) != (GBSH_PGID = getpgrp()))
	    kill(GBSH_PID, SIGTTIN);             
	              
	act_child.sa_handler = signal_handler_child;
	act_int.sa_handler = signal_handler_int;			
			
	sigaction(SIGCHLD, &act_child, 0);
	sigaction(SIGINT, &act_int, 0);
			
	GBSH_PGID = getpgrp();
	if (GBSH_PID != GBSH_PGID)
	{
	    printf("Error, the shell is not process group leader");
	    exit(EXIT_FAILURE);
	}
	tcsetpgrp(STDIN_FILENO, GBSH_PGID);  
			
	tcgetattr(STDIN_FILENO, &GBSH_TMODES);

	current_directory = (char*) calloc(1024, sizeof(char));
    } else {
	fputs("Sorry, we could not make the shell interactive yet :(", stderr);
	exit(EXIT_FAILURE);
    }
}

void signal_handler_child(int p) {
	while (waitpid(-1, NULL, WNOHANG) > 0)
	    ;
	printf("\n");
}

void signal_handler_int(int p){
	if (kill(pid, SIGTERM) == 0)
	{
		printf("\nProcess %d received a SIGINT signal\n",pid);
		no_reprint_prmpt = 1;			
	} else 
	    printf("\n");
}

int8_t* greeting()
{
    char* result = NULL;

    int8_t* wd = get_current_dir_name();
    if (wd == NULL)
    {
	perror("getwd");
	exit(EXIT_FAILURE);
    }
    result = (char*) calloc (1, strlen(wd) + 100);

    sprintf(result,
	    ANSI_COLOR_BLUE "%s "
	    ANSI_COLOR_ARROW " "
	    ANSI_COLOR_RESET,
	    wd);

    return result;
}

int8_t** parse_args(int8_t* input_line)
{
    if (input_line == NULL)
	return NULL;
    int8_t bufsize = 0, pos = 0;
    int8_t **tokens = NULL; 
    int8_t *token;
    int8_t *delim = " \t\r\n\a";

    bufsize = calculate_argc(input_line);
    if (bufsize == 0)
	return NULL;

    tokens = (int8_t**) calloc ( ++bufsize, sizeof(int8_t*));
    if (!tokens) {
	perror("parsing: malloc");
	exit(EXIT_FAILURE);
    }

    token = strtok(input_line, delim);
    while (token != NULL) {
	tokens[pos] = token;
	pos++;
	token = strtok(NULL, delim);
    }
    tokens[pos] = NULL;
    return tokens;
}

uint8_t calculate_argc(int8_t* line)
{
    if (line == NULL || *line == '\0')
	return 0;

    uint8_t argc = (*line == ' ') ? 0 : 1;

    for (int8_t pos = 1; line[pos] != '\0'; pos++) 
	if (line[pos] != ' ' && line[pos-1] == ' ')
	    argc++;

    return argc;
}

int8_t get_next_separator(int8_t** args)
{
    int8_t i;
    for (i = 0; args[i] != NULL; i++)
	if (strcmp(args[i], ">") == 0 ||
	    strcmp(args[i], "|") == 0 ||
	    strcmp(args[i], "&") == 0 ||
	    strcmp(args[i], "<") == 0)
	    return i;
    return i;
}

int32_t handle_command(int8_t** args)
{
    if (is_builtin( args ))
	return run_builtin (args);

    int8_t separator = get_next_separator(args);
    int8_t* curr_args [256];
    
    if (args[separator] == NULL ||
	strcmp(args[separator], "&") == 0)

	return exec_simple_cmd(args, separator); 

    if (strcmp(args[separator], "|") == 0)
	return exec_pipe(args);
    
    for (int32_t i = 0; i < separator; i++)
	curr_args[i] = args[i];
    curr_args[separator] = NULL;

    if (strcmp(args[separator], "<") == 0)
    {
	if (args[separator + 1] == NULL ||
	    ( args[separator + 2] != NULL &&
	      ( strcmp(args[separator + 2], ">") != 0 ||
		strcmp(args[separator + 2], ">") == 0 &&
		args[separator + 3] == NULL )))
	{
	    fputs("Error\nUsage: command < inputfile [> outputfile]\n", stderr);
	    return -1;
	}

	if (args[separator + 2] != NULL)
	    return exec_file_io(curr_args,
				args[separator+1],
				args[separator+3]);
	else
	    return exec_file_io(curr_args,
				args[separator+1],
				NULL);
    }
    if (strcmp(args[separator], ">") == 0)
    {
	if (args[separator + 1] == NULL)
	{
	    fputs("Not enough input arguments", stderr);
	    return -1;
	}
	return exec_file_io(curr_args,
			    NULL,
			    args[separator + 1]);
    }
    return 0;
}

int32_t exec_simple_cmd(int8_t** args, int8_t separator)
{
    pid_t pid;

    args[separator] = NULL;
    
    pid = fork();
    if (pid == -1)
    {
	perror("fork");
	return -1;
    }
    if (pid == 0)
    {
	signal(SIGINT, SIG_IGN);
	
	setenv("parent", getcwd(current_directory, 1024), 1);
	
	if ( execvp(args[0], (char**)args) == -1)
	{
	    fprintf(stderr, "hush: %s: command not found", args[0]);
	    kill (getpid(), SIGTERM);
	}
    }

    if (args[separator] == NULL)
    {
	int32_t status;
	waitpid(pid, &status, 0);
	return status;
    } else 
	printf("Process created with PID: %d\n", pid);

    return 0;
}

int32_t exec_pipe(int8_t** args)
{
    int32_t fildes_odd[2]; 
    int32_t fildes_even[2];
	
    int32_t num_cmds = 0;
	
    char* command[256];
	
    for (int32_t l = 0; args[l] != NULL; l++)
	if (strcmp(args[l], "|") == 0)
	    num_cmds++;
    num_cmds++;

    int32_t end = 0;

    for (int32_t j = 0, i = 0; args[j] != NULL && end != 1; i++)
    {
	int32_t c = 0;
	while (strcmp(args[j], "|") != 0)
	{
	    command[c] = args[j];
	    j++;	
	    if (args[j] == NULL)
	    {
		end = 1;
		c++;
		break;
	    }
	    c++;
	}
	command[c] = NULL;
	j++;		

	if (i % 2 != 0)
	    pipe(fildes_odd); 
	else
	    pipe(fildes_even); 

	pid_t pid = fork();
		
	if (pid == -1)
	{
	    perror("hush: fork");
	    if (i != num_cmds - 1)
	    {
		if (i % 2 != 0)
		    close(fildes_odd[1]); 
		else
		    close(fildes_even[1]); 
	    }
	    return -1;
	}
	if (pid == 0)
	{
	    if (i == 0)
		dup2(fildes_even[1], STDOUT_FILENO);
	    else if (i == num_cmds - 1)
	    {
		if (num_cmds % 2 != 0) 
		    dup2(fildes_odd[0], STDIN_FILENO);
		else
		    dup2(fildes_even[0], STDIN_FILENO);
	    } else { 
		if (i % 2 != 0)
		{
		    dup2(fildes_even[0], STDIN_FILENO); 
		    dup2(fildes_odd[1], STDOUT_FILENO);
		} else { 
		    dup2(fildes_odd[0], STDIN_FILENO); 
		    dup2(fildes_even[1], STDOUT_FILENO);
		} 
	    }

	    if (execvp(command[0], command) == -1)
	    {
		fprintf(stderr, "hush: %s: command not found", command[0]);
		kill(getpid(), SIGTERM);
	    }		
	}
		 
	if (i == 0)
	    close(fildes_even[1]);
	else if (i == num_cmds - 1)
	{
	    if (num_cmds % 2 != 0)					
		close(fildes_odd[0]);
	    else			
		close(fildes_even[0]);
	} else {
	    if (i % 2 != 0)
	    {					
		close(fildes_even[0]);
		close(fildes_odd[1]);
	    } else {					
		close(fildes_odd[0]);
		close(fildes_even[1]);
	    }
	}

	waitpid(pid, NULL, 0);
    }
    return 0;
}

int32_t exec_file_io(int8_t** args, int8_t* input, int8_t* output)
{
    int32_t input_fd, output_fd;

    pid_t pid = fork();

    if (pid == -1)
    {
	perror("fork");
	return -1;
    }
    else if (pid == 0)
    {
	if (input != NULL)
	{
	    input_fd = open(input, O_RDONLY, 0600);
	    if (input_fd == -1)
	    {
		perror("hush");
		exit(EXIT_FAILURE);
	    }
	    dup2(input_fd, STDIN_FILENO);
	    close(input_fd);
	}
	if (output != NULL)
	{
	    output_fd = open(output, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	    if (output_fd == -1)
	    {
		perror("hush");
		exit(EXIT_FAILURE);
	    }
	    dup2(output_fd, STDOUT_FILENO);
	    close(output_fd);	
	}

	setenv("parent", getcwd(current_directory, 1024), 1);
		
	if (execvp(args[0], (char**)args) == -1)
	{
	    perror("hush: execvp");
	    kill(getpid(), SIGTERM);
	}
    }
    int32_t status;
    waitpid(pid, &status, 0);
    return status;    
}

