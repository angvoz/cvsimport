/*
 * Copyright 2001, 2002, 2003 David Mansfield and Cobite, Inc.
 * See COPYING file for license information 
 */

#ifndef CVSPS_H
#define CVSPS_H

#ifndef HAVE_CVSSERVERCTX_DEF
#define HAVE_CVSSERVERCTX_DEF
typedef struct _CvsServerCtx CvsServerCtx;
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

extern struct hash_table * file_hash;
extern CvsServerCtx * cvs_direct_ctx;

#endif /* CVSPS_H */
