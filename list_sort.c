/*
 * Copyright 2001, 2002, 2003 David Mansfield and Cobite, Inc.
 * See COPYING file for license information 
 */

#include <stdio.h>
#include <stdlib.h>
#include "list_sort.h"

void list_sort(list_head * list, int (*node_compare)(list_node *, list_node *))
{
    struct list_link *p, *q, *t;
    list_head tmp;
    int merges = 0;
    int k = 1;
    int psize, qsize; 

    if (list_empty(list))
	return;

    do
    {
	INIT_LIST_HEAD(&tmp);
	p = list->next;
	merges = 0;
	psize = qsize = 0;

	while (p != list)
	{
	    merges++;
	    q = p;

	    while (q != list && psize < k)
	    {
		q = q->next;
		psize++;
	    }
		
	    qsize = k;

	    while (psize || (qsize && q != list))
	    {
		if (psize && (qsize == 0 || q == list || node_compare(p, q) <= 0))
		{
		    t = p;
		    p = p->next;
		    psize--;
		}
		else if (qsize == 0)
		{
		    printf("whoaa. qsize is zero\n");
		    exit (1);
		}
		else
		{
		    t = q;
		    q = q->next;
		    qsize--;
		}
		
		list_del(t);
		
		list_ins(t, &tmp);
	    }

	    p = q;
	}

	if (!list_empty(list))
	{
	    printf("whoaa. initial list not empty\n");
	    exit (1);
	}
	    
	list_splice(&tmp, list);
	k *= 2;

	//printf("done w sort pass %d %d\n", k, merges);
    }
    while (merges > 1);
}

