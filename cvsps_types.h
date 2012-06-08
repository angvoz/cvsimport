/*
 * Copyright 2001, 2002, 2003 David Mansfield and Cobite, Inc.
 * See COPYING file for license information 
 */

#ifndef CVSPS_TYPES_H
#define CVSPS_TYPES_H

#include <time.h>

#define LOG_STR_MAX 65536
#define AUTH_STR_MAX 64
#define REV_STR_MAX 64
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct _CvsFile CvsFile;
typedef struct _PatchSet PatchSet;
typedef struct _PatchSetRange PatchSetRange;
typedef struct _Revision Revision;
typedef struct _GlobalSymbol GlobalSymbol;
typedef struct _Tag Tag;

struct _Revision
{
    char * rev;
    CvsFile * file;
    const Tag * branch;
    unsigned dead : 1;
    /*
     * In the cvs cvs repository (ccvs project) there are tagged
     * revisions that don't exist. track 'confirmed' revisions
     * so as to not let them screw us up.
     */
    unsigned present : 1;
    /*
     * bad_funk is only set w.r.t the -r tags
     */
    unsigned bad_funk : 1;

    PatchSet * ps;
    list_node ps_link; /* PatchSet.members */
    
    /*
     * A revision can be part of many PatchSets because it may
     * be the branch point of many branches (as a prev_rev).  
     * It should, however, be the 'next_rev' of only one 
     * Revision.  The 'main line of inheritence' is
     * kept in next_rev, and all 'branch revisions' are kept
     * in a list.
     */
    Revision *prev_rev;
    Revision *next_rev;
    list_head branch_children; /* Revision->branch_link */
    list_node branch_link; /* Revision.branch_children */

    /*
     * A list of all Tag structures tagging this revision
     */
    list_head tags; /* Tag->rev_link */
};

struct _CvsFile
{
    char *filename;
    struct hash_table * revisions;    /* rev_str to revision [Revision*] */
    struct hash_table * symbols;      /* tag to revision [Tag*]     */
    /* 
     * this is a hack. when we initially create entries in the symbol hash
     * we don't have the branch info, so the Revisions get created 
     * with the branch attribute NULL.  Later we need to resolve these.
     */
    int have_branches;
};

/* 
 * these are bit flags for tag flags 
 * they apply to any patchset that
 * has an assoctiated tag
 */
#define TAG_FUNKY   0x1
#define TAG_INVALID 0x2

/* values for funk_factor. they apply
 * only to the -r tags, to patchsets
 * that have an odd relationship to the
 * tag
 */
enum funk_factor {
    FNK_NONE = 0,
    FNK_SHOW_SOME,
    FNK_SHOW_ALL,
    FNK_HIDE_ALL,
    FNK_HIDE_SOME
};

struct _PatchSet
{
    int psid;
    time_t date;
    time_t min_date;
    time_t max_date;
    char *descr;
    char *author;
    list_head tags; /* GlobalSymbol->link */
    const GlobalSymbol *branch;
    const char *ancestor_branch;
    list_head members; /* Revision->ps_link */
    /*
     * A 'branch add' patch set is a bogus patch set created automatically
     * when a 'file xyz was initially added on branch abc'
     * we want to ignore these.  fortunately, there's a way to detect them
     * without resorting to looking at the log message.
     */
    unsigned branch_add : 1;
    /*
     * If the '-r' option specifies a funky tag, we will need to detect the
     * PatchSets that come chronologically before the tag, but are logically
     * after, and vice-versa if a second -r option was specified
     */
    enum funk_factor funk_factor : 4;

    /* for putting onto a list */
    list_node all_link; /* all_patch_sets */
    list_node collision_link; /* collisions */
};

struct _PatchSetRange
{
    int min_counter;
    int max_counter;
    list_node link; /* show_patch_set_ranges */
};

struct _GlobalSymbol
{
    const char * tag;
    PatchSet * ps;
    short flags;
    list_head tags; /* Tag->global_link */
    list_node link; /* PatchSet.tags */
};

struct _Tag
{
    const GlobalSymbol * sym;
    Revision * rev;
    short branch;
    short flags;
    list_node global_link; /* GlobalSymbol.tags */
    list_node rev_link; /* Revision.tags */
};

#endif /* CVSPS_TYPES_H */
