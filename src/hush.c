#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hushlib.h"

uint8_t calculate_argc(int8_t* line);

int8_t* greeting()
{
    int8_t* result = NULL;
    int8_t* wd = get_current_dir_name();
    if (wd == NULL)
    {
	perror("getwd");
	exit(EXIT_FAILURE);
    }
    result = (int8_t*) calloc (1, strlen(wd) + 2);
    if (result == NULL)
    {
	perror("calloc");
	exit(EXIT_FAILURE);
    }
    result = strcpy(result, wd);
    free(wd);
    if (getuid() == 0)
	result = strcat(result, "#>");
    else
	result = strcat(result, "$>");
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
    uint8_t pos = 1;
    while (line[pos] != '\0') {
	if (line[pos] != ' ' && line[pos-1] == ' ')
	    argc++;
	pos++;
    }
    return argc;
}
