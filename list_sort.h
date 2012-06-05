/*
 * Copyright 2001, 2002, 2003 David Mansfield and Cobite, Inc.
 * See COPYING file for license information 
 */

#ifndef LIST_SORT_H
#define LIST_SORT_H

#include <cbtcommon/list.h>

void list_sort(list_head *, int (*)(list_node *, list_node *));

#endif /* LIST_SORT_H */
