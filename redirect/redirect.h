#ifndef REDIRECT_H
#define REDIRECT_H

#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../pipes/pipes.h"
#include "../process/process.h"



char ** parsePreRed(char *s, int * preCount, int c);

char ** parsePostRed(char *s, int * postCount, int c);


void redTo(int c, char ** preRed, char ** postRed);

int containsDirect(char * cmd); 

int countRedirect(char * cmd);





#endif
