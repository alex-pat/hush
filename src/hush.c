#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hushlib.h"

uint8_t calculate_argc(uint8_t* line);

uint8_t* greeting()
{
    uint8_t* result = NULL;
    uint8_t* wd = get_current_dir_name();
    if (wd == NULL)
    {
	perror("getwd");
	exit(EXIT_FAILURE);
    }
    result = (uint8_t*) calloc (1, strlen(wd) + 1);
    if (result == NULL)
    {
	perror("calloc");
	exit(EXIT_FAILURE);
    }
    result = strcpy(result, wd);
    free(wd);
    result = strcat(result, ">");
    return result;
}

uint8_t** parse_args(uint8_t* input_line)
{
    if (input_line == NULL)
	return NULL;
    uint8_t bufsize = 0, pos = 0;
    uint8_t **tokens = NULL; 
    uint8_t *token;
    uint8_t *delim = " \t\r\n\a";

    bufsize = calculate_argc(input_line);
    if (bufsize == 0)
	return NULL;
    tokens = malloc(bufsize * sizeof(char*));
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

uint8_t calculate_argc(uint8_t* line)
{
    if (line == NULL || *line == '\0')
	return 0;
    uint8_t argc = (*line == ' ') ? 0 : 1;
    uint8_t pos = 1;
    while (line[pos] != '\0') {
	if (line[pos] != ' ' && line[pos-1] == ' ')
	    argc++;
	pos++;
    }
    return argc;
}

void free_var(uint8_t* line, uint8_t* prompt, uint8_t** args)
{
    free(line);
    free(prompt);
    free(args);
}
