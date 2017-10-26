
#include "listUtils.h"


Node * buildNode(FILE * fin, void *(*buildData)(FILE * in) )
{

	Node * nn = (Node*)calloc(1, sizeof(Node));
	nn->data = buildData(fin);
	return nn;
}


Node * buildNode_Type(void * passedIn)
{
	Node * nn = (Node*)calloc(1, sizeof(Node));
	nn->data = passedIn;
	return nn;
}


void sort(LinkedList * theList, int (*compare)(const void *, const void *))
{
	if(theList == NULL)
		exit(-99);
	Node * start, * search, * min;
	void * temp;
	if(theList->size > 1)
	{
		for(start = theList->head->next; start->next != NULL; start = start->next)
		{
			min = start;
			for(search = start->next; search != NULL; search = search->next)
			{
				if(compare(search->data, min->data)<0)
					min = search;
			}
			temp = start->data;
			start->data = min->data;
			min->data = temp;
		}
	}
}

void buildListTotal(LinkedList * myList, int total, FILE * fin, void * (*buildData)(FILE * in))
{

	if(myList == NULL || total <= 0)
		exit(-99);

	int x;
	for(x = 0; x < total; x++)
	{
		Node * nn = (Node *)calloc(1, sizeof(Node));
		nn->data = buildData(fin);
		addFirst(myList, nn);
	}

}
