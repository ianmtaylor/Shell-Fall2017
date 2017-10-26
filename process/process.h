#ifndef PROCESS_H
#define PROCESS_H

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../tokenize/makeArgs.h"

int forkIt(char ** argv, int argc);

void forkItRedirect(char ** argv);

#endif
