/*
 * Copyright 2001, 2002, 2003 David Mansfield and Cobite, Inc.
 * See COPYING file for license information 
 */

#ifndef _COMMON_LIST_H
#define _COMMON_LIST_H

/*
 * Stolen from linux-2.1.131
 * All comments from the original source unless otherwise noted
 * Added: the CLEAR_LIST_NODE macro
 */

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

#include "inline.h"
#include <stddef.h>

typedef struct list_link {
        struct list_link *next, *prev;
} list_head, list_node;

#define LIST_HEAD(name) \
        list_head name = { &name, &name }

#define INIT_LIST_HEAD(ptr)  do { \
        (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

#define CLEAR_LIST_NODE(ptr) do { \
        (ptr)->next = NULL;  (ptr)->prev = NULL; \
} while (0)

/*
 * Insert a new entry between two known consecutive entries. 
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static INLINE void __list_add(struct list_link *li,
        struct list_link * prev,
        struct list_link * next)
{
        next->prev = li;
        li->next = next;
        li->prev = prev;
        prev->next = li;
}

/*
 * Insert a new entry after the specified head..
 */
static INLINE void list_add(list_node *li, list_head *head)
{
        __list_add(li, head, head->next);
}

/*
 * Insert a new entry before the specified head..
 */
static INLINE void list_ins(list_node *li, list_head *head)
{
        __list_add(li, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static INLINE void __list_del(struct list_link * prev,
                                  struct list_link * next)
{
        next->prev = prev;
        prev->next = next;
}

static INLINE void list_del(list_node *entry)
{
        __list_del(entry->prev, entry->next);
}

static INLINE int list_empty(list_head *head)
{
        return head->next == head;
}

/*
 * Splice in "list" into "head"
 */
static INLINE void list_splice(list_head *list, list_head *head)
{
        struct list_link *first = list->next;

        if (first != list) {
                struct list_link *last = list->prev;
                struct list_link *at = head->next;

                first->prev = head;
                head->next = first;

                last->next = at;
                at->prev = last;
        }
}

#define list_entry(ptr, type, member) \
   ((type *)((char *)(ptr)-offsetof(type, member)))

#endif /* _COMMON_LIST_H */
