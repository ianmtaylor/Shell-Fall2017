#include "makeArgs.h"

void clean(int argc, char **argv)
{
	int x = 0;
	for(; x < argc; x++)
	{
		free(argv[x]);
		argv[x] = NULL;
	}
	free(argv);
	argv = NULL;


}// end clean

void printargs(int argc, char **argv)
{
	int x;
	for(x = 0; x < argc; x++)
		printf("%s\n", argv[x]);

}// end printargs

int makeargs(char *s, char *** argv)
{

	char copy[MAX];
	strncpy(copy, s, strlen(s)+1);
	copy[strlen(s)+1] = '\0';
	char * reserve;
	char * token = strtok_r(s, " ", &reserve);
	int count = 0;

	while(token != NULL)
	{
		count++;
		token = strtok_r(NULL, " ", &reserve);
	}

	if(count <= 0)
		return -1;

	(*argv) = (char**)calloc(count+1, sizeof(char*));
	token = strtok_r(copy," ", &reserve);
	strip(token);
	(*argv)[0] = (char*)calloc(strlen(token)+1, sizeof(char));
	strncpy((*argv)[0], token, strlen(token));
	int x = 1;
	for(; x < count; x++)
	{
		token = strtok_r(NULL," ", &reserve);
	        strip(token);
	        (*argv)[x] = (char*)calloc(strlen(token)+1, sizeof(char));
	        strncpy((*argv)[x], token, strlen(token));

	}

	return count;



}// end makeArgs




void removeSpaces(char *str)
{
    	char *str1 = str;
    	while(*str) 
	{
		if(*str != ' ')
            	*str1++ = *str;  
        	++str; 
    	}
    	*str1 = '\0';  
}

void removeBang(char * str)
{
    	char *str1 = str;
    	while(*str) 
	{
        	if(*str != '!')
            	*str1++ = *str;  
        	++str; 
    	}
    	*str1 = '\0'; 
}







