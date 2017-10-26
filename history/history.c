#include "history.h"


History * history(int Count, int FileCount)
{
	History * history =(History*)calloc(1, sizeof(History));
	history->theList = linkedList();
	history->histCount = Count;
	history->histFileCount = FileCount;
	return history; 
}

void cleanHistory(History * history, void (*removeData)(void *))
{
	clearList(history->theList, removeData);
	free(history->theList);
	history->theList = NULL;
	history->histCount = 0;
	history->histFileCount = 0;
	free(history);
	history = NULL;
}

void checkHistory(History * history)
{
	if(!(fileExists(".msshrc_history")))
	{
		FILE * fp;
		fp = fopen(".msshrc_history", "w");
		fclose(fp);
	}
}

void loadHistory(History * hist, void * (*buildData)(char * cmd), Node * (*buildNode)(void * passedIn), void (*cleanCmd)(void * cmd))
{
	if(checkEOF(".msshrc_history") <= 1)
	{
		return;
	}
	FILE * fp;
	fp = fopen(".msshrc_history", "r");
	char s[100];

	fgets(s, 100, fp);
	if(s[0] == '\0')
	{
		
		rewind(fp);
		fclose(fp);
		return;  //file is empty
	}
	strip(s);
	char * str;
	str = (char *)calloc(strlen(s)+1, sizeof(char));
        strncpy(str, s, strlen(s));

	void * ptr = NULL;
	if(hist->theList->size == hist->histFileCount)
	{
		removeFirst(hist->theList, cleanCmd);
		ptr = buildData(str);
		addLast(hist->theList, buildNode(ptr));
	}
	else
	{
		ptr = buildData(str);
		addLast(hist->theList, buildNode(ptr));
	}
	
	while(!feof(fp))
	{
		fgets(s, 100, fp);
		strip(s);
		free(str);
		str = NULL;
		str = (char *)calloc(strlen(s)+1, sizeof(char));
		strncpy(str, s, strlen(s));
		if(!feof(fp))
		{
			ptr = NULL;
			if(hist->theList->size == hist->histFileCount)
			{
				removeFirst(hist->theList, cleanCmd);
				ptr = buildData(str);
				addLast(hist->theList, buildNode(ptr));
			}
			else
			{
				ptr = buildData(str);
				addLast(hist->theList, buildNode(ptr));
			}
		}
	}


	rewind(fp);
	fclose(fp);
	free(str);
	str = NULL;
}

void addToHistory(History * hist, void * (*buildData)(char * cmd), Node * (*buildNode)(void * passedIn), void (*cleanCmd)(void * cmd), char * cmdstr)
{
	void * ptr = buildData(cmdstr);

	if(hist->theList->size == hist->histFileCount)
	{
		removeFirst(hist->theList, cleanCmd);
		addLast(hist->theList, buildNode(ptr)); 
	}
	else
	{
		addLast(hist->theList, buildNode(ptr));
	}
}

void addToHistCount(History * hist, LinkedList * cmdList, char * cmdstr, void * (*buildData)(char * cmd), Node * (*buildNode)(void * passedIn), void (*cleanCmd)(void * cmd))
{
	void * ptr = buildData(cmdstr);

	if(cmdList->size == hist->histCount)
	{
		removeFirst(cmdList, cleanCmd);
		addLast(cmdList, buildNode(ptr));
	}
	else
	{
		addLast(cmdList, buildNode(ptr));
	}
}

void loadIntoHistory(History * hist, LinkedList * cmdLL, void (*removeData)(void *))
{
	Node * nn;
	while(cmdLL->size != 0 && hist->histFileCount > 0)
        {
		nn = NULL;
                if(hist->theList->size == hist->histFileCount)
            	{
           	     removeFirst(hist->theList, removeData);
		     nn = getFirst(cmdLL);
                     addLast(hist->theList, nn);
                }
                else
                {
		     nn = getFirst(cmdLL);
                     addLast(hist->theList, nn);
                }

        }

}

void getHistInfo(History * hist, FILE * fp)
{

	char s[100];
        char * str;
        char * saveptr;
        char * token;
        int histcount;
        int histfilecount;

        fgets(s, 100, fp);
        strip(s);
        str = (char *)calloc(strlen(s)+1, sizeof(char));
        strncpy(str, s, strlen(s));
        token = strtok_r(str, "=", &saveptr);

        if(strcmp(token, "HISTCOUNT") == 0)
        {
                token = strtok_r(NULL, "=", &saveptr);
                histcount = atoi(token);
                //deal with HISTFILECOUNT

                fgets(s, 100, fp);
                strip(s);
                free(str);
                str = NULL;
                str = (char *)calloc(strlen(s)+1, sizeof(char));
                strncpy(str, s, strlen(s));
                token = strtok_r(str, "=", &saveptr);
                token = strtok_r(NULL, "=", &saveptr);
                histfilecount = atoi(token);
                //LOAD IT INTO HIST
		hist->histCount = histcount;
		hist->histFileCount = histfilecount;
        }
        else if(strcmp(token, "HISTFILECOUNT") == 0)
        {
                token = strtok_r(NULL, "=", &saveptr);
                histfilecount = atoi(token);
                //deal with HISTCOUNT
                fgets(s, 100, fp);
                strip(s);
                free(str);
                str = NULL;
                str = (char *)calloc(strlen(s)+1, sizeof(char));
                strncpy(str, s, strlen(s));
                token = strtok_r(str, "=", &saveptr);
                token = strtok_r(NULL, "=", &saveptr);
                histcount = atoi(token);
                //LOAD IT INTO HIST
		hist->histCount = histcount;
		hist->histFileCount = histfilecount;
        }


        free(str);
        str = NULL;

}

int checkEOF(char * filename)
{
	FILE * fp;
	long size = 0;

	fp = fopen(filename, "r");

	if(fp)
	{
		while(!feof(fp))
		{
			fgetc(fp);
			size++;
		}
		rewind(fp);
		fclose(fp);
	}
	return size;
}
