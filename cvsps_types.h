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
typedef struct _PatchSetMember PatchSetMember;
typedef struct _PatchSetRange PatchSetRange;
typedef struct _CvsFileRevision CvsFileRevision;
typedef struct _GlobalSymbol GlobalSymbol;
typedef struct _Tag Tag;
typedef struct _TagName TagName;

struct _CvsFileRevision
{
    char * rev;
    int dead;
    CvsFile * file;
    char * branch;
    /*
     * In the cvs cvs repository (ccvs project) there are tagged
     * revisions that don't exist. track 'confirmed' revisions
     * so as to not let them screw us up.
     */
    int present;

    /*
     * A revision can be part of many PatchSets because it may
     * be the branch point of many branches (as a pre_rev).  
     * It should, however, be the 'post_rev' of only one 
     * PatchSetMember.  The 'main line of inheritence' is
     * kept in pre_psm, and all 'branch revisions' are kept
     * in a list.
     */
    PatchSetMember * pre_psm;
    PatchSetMember * post_psm;
    list_head branch_children; /* CvsFileRevision->link */
    
    /* 
     * for linking this 'first branch rev' into the parent branch_children
     */
    list_node link; /* CvsFileRevision.branch_children */

    /*
     * A list of all Tag structures tagging this revision
     */
    list_head tags; /* Tag->rev_link */
};

struct _CvsFile
{
    char *filename;
    struct hash_table * revisions;    /* rev_str to revision [CvsFileRevision*] */
    struct hash_table * branches;     /* branch to branch_sym [char*]           */
    struct hash_table * branches_sym; /* branch_sym to branch [char*]           */
    struct hash_table * symbols;      /* tag to revision [CvsFileRevision*]     */
    /* 
     * this is a hack. when we initially create entries in the symbol hash
     * we don't have the branch info, so the CvsFileRevisions get created 
     * with the branch attribute NULL.  Later we need to resolve these.
     */
    int have_branches;
};

struct _PatchSetMember
{
    CvsFileRevision * pre_rev;
    CvsFileRevision * post_rev;
    PatchSet * ps;
    CvsFile * file;
    /*
     * bad_funk is only set w.r.t the -r tags
     */
    int bad_funk;
    list_node link; /* PatchSet.members */
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
#define FNK_SHOW_SOME  1
#define FNK_SHOW_ALL   2
#define FNK_HIDE_ALL   3
#define FNK_HIDE_SOME  4

struct _PatchSet
{
    int psid;
    time_t date;
    time_t min_date;
    time_t max_date;
    char *descr;
    char *author;
    list_head tags; /* TagName->link */
    char *branch;
    char *ancestor_branch;
    list_head members; /* PatchSetMember->link */
    /*
     * A 'branch add' patch set is a bogus patch set created automatically
     * when a 'file xyz was initially added on branch abc'
     * we want to ignore these.  fortunately, there's a way to detect them
     * without resorting to looking at the log message.
     */
    int branch_add;
    /*
     * If the '-r' option specifies a funky tag, we will need to detect the
     * PatchSets that come chronologically before the tag, but are logically
     * after, and vice-versa if a second -r option was specified
     */
    int funk_factor;

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
    char * tag;
    PatchSet * ps;
    list_head tags; /* Tag->global_link */
};

struct _Tag
{
    GlobalSymbol * sym;
    CvsFileRevision * rev;
    char * tag;
    list_node global_link; /* GlobalSymbol.tags */
    list_node rev_link; /* CvsFileRevision.tags */
};

struct _TagName
{
    char * name;
    int flags;
    list_node link; /* PatchSet.tags */
};

#endif /* CVSPS_TYPES_H */
