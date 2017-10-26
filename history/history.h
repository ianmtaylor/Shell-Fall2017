#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/myUtils.h"
#include "../linkedlist/linkedList.h"
#include "../linkedlist/listUtils.h"
#include "../msshrcHandler/rcHandler.h"

struct history
{
	LinkedList * theList;
	int histCount;
	int histFileCount;
};
typedef struct history History;

//Constructor

History * history(int Count, int FileCount);

//cleanup
void cleanHistory(History * history, void (*removeData)(void *));

//makeHistoryFile

void checkHistory(History * history);

void loadHistory(History * hist, void * (*buildData)(char * cmd), Node * (*buildNode)(void * passedIn), void (*cleanCmd)(void * cmd));

void addToHistory(History * hist, void * (*buildData)(char * cmd), Node * (*buildNode)(void * passedIn), void (*cleanCmd)(void * cmd), char * cmdstr);

void addToHistCount(History * hist, LinkedList * cmdList, char * cmdstr, void * (*buildData)(char * cmd), Node * (*buildNode)(void * passedIn), void (*cleanCmd)(void * cmd));

void loadIntoHistory(History * hist, LinkedList * cmdLL, void (*removeData)(void *));

void getHistInfo(History * hist, FILE * fp);

int checkEOF(char * filename);

#endif




