#if !defined(GLOBALS_H)
#define GLOBALS_H

#include <termios.h>

#define EXIT 618

static pid_t GBSH_PID;
static pid_t GBSH_PGID;
static int GBSH_IS_INTERACTIVE;
static struct termios GBSH_TMODES;

static char* current_directory;
extern char** environ;

struct sigaction act_child;
struct sigaction act_int;

int no_reprint_prmpt;

pid_t pid;

void signal_handler_child(int p);

void signal_handler_int(int p);


#endif // GLOBALS_H
