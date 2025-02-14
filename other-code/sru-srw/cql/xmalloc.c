/*
 * Copyright (C) 1995-2005, Index Data ApS
 * All rights reserved.
 *
 * $Id: xmalloc.c 77 2005-07-11 19:55:55Z warnock $
 */
/**
 * \file xmalloc.c
 * \brief Implements malloc interface.
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef YAZ_NOLOG
#include <yaz/log.h>
#endif

#include <yaz/xmalloc.h>

#ifndef TRACE_XMALLOC
#define TRACE_XMALLOC 1
#endif

static int log_level=0;
static int log_level_initialized=0;

#if TRACE_XMALLOC > 1

static const unsigned char head[] = {88, 77, 66, 55, 44, 33, 22, 11};
static const unsigned char tail[] = {11, 22, 33, 44, 55, 66, 77, 88};
static const unsigned char freed[] = {11, 22, 33, 44, 55, 66, 77, 88};

struct dmalloc_info {
    int len;
    char file[16];
    int line;
    struct dmalloc_info *next;
    struct dmalloc_info *prev;
};

struct dmalloc_info *dmalloc_list = 0;


void *xmalloc_d(size_t nbytes, const char *file, int line)
{
    char *res;
    struct dmalloc_info *dinfo;
    
    if (!log_level_initialized)
    {
#ifndef YAZ_NOLOG
        log_level=yaz_log_module_level("malloc");
#endif
        log_level_initialized=1;
    }

    if (!(res = (char*) malloc(nbytes + sizeof(*dinfo)+16*sizeof(char))))
	return 0;
    dinfo = (struct dmalloc_info *) res;
    strncpy (dinfo->file, file, sizeof(dinfo->file)-1);
    dinfo->file[sizeof(dinfo->file)-1] = '\0';
    dinfo->line = line;
    dinfo->len = nbytes;
    
    dinfo->prev = 0;
    dinfo->next = dmalloc_list;
    if (dinfo->next)
	dinfo->next->prev = dinfo;
    dmalloc_list = dinfo;
    
    memcpy(res + sizeof(*dinfo), head, 8*sizeof(char));
    res += sizeof(*dinfo) + 8*sizeof(char);
    memcpy(res + nbytes, tail, 8*sizeof(char));
    return res;
}

void xfree_d(void *ptr, const char *file, int line)
{
    struct dmalloc_info *dinfo;

    if (!ptr)
	return;
    dinfo = (struct dmalloc_info *)
	((char*)ptr - 8*sizeof(char) - sizeof(*dinfo));
    if (memcmp(head, (char*) ptr - 8*sizeof(char), 8*sizeof(char)))
    {
#ifndef YAZ_NOLOG
	yaz_log(YLOG_FATAL, "xfree_d bad head, %s:%d, %p", file, line, ptr);
#endif
        abort();
    }
    if (memcmp((char*) ptr + dinfo->len, tail, 8*sizeof(char)))
    {
#ifndef YAZ_NOLOG
	yaz_log(YLOG_FATAL, "xfree_d bad tail, %s:%d, %p", file, line, ptr);
#endif
        abort();
    }
    if (dinfo->prev)
	dinfo->prev->next = dinfo->next;
    else
	dmalloc_list = dinfo->next;
    if (dinfo->next)
	dinfo->next->prev = dinfo->prev;
    memcpy ((char*) ptr - 8*sizeof(char), freed, 8*sizeof(char));
    free(dinfo);
    return;
}

void *xrealloc_d(void *p, size_t nbytes, const char *file, int line)
{
    struct dmalloc_info *dinfo;
    char *ptr = (char*) p;
    char *res;
    
    if (!log_level_initialized)
    {
#ifndef YAZ_NOLOG
        log_level=yaz_log_module_level("malloc");
#endif
        log_level_initialized=1;
    }

    if (!ptr)
    {
	if (!nbytes)
	    return 0;
	res = (char *) malloc(nbytes + sizeof(*dinfo) + 16*sizeof(char));
    }
    else
    {
	if (memcmp(head, ptr - 8*sizeof(char), 8*sizeof(char)))
	{
#ifndef YAZ_NOLOG
	    yaz_log(YLOG_FATAL, "xrealloc_d bad head, %s:%d, %p",
		    file, line, ptr);
#endif
	    abort();
	}
	dinfo = (struct dmalloc_info *) (ptr-8*sizeof(char) - sizeof(*dinfo));
	if (memcmp(ptr + dinfo->len, tail, 8*sizeof(char)))
	{
#ifndef YAZ_NOLOG
	    yaz_log(YLOG_FATAL, "xrealloc_d bad tail, %s:%d, %p",
		    file, line, ptr);
#endif
	    abort();
	}
	if (dinfo->prev)
	    dinfo->prev->next = dinfo->next;
	else
	    dmalloc_list = dinfo->next;
	if (dinfo->next)
	    dinfo->next->prev = dinfo->prev;
	
	if (!nbytes)
	{
	    free (dinfo);
	    return 0;
	}
	res = (char *)
	    realloc(dinfo, nbytes + sizeof(*dinfo) + 16*sizeof(char));
    }
    if (!res)
	return 0;
    dinfo = (struct dmalloc_info *) res;
    strncpy (dinfo->file, file, sizeof(dinfo->file)-1);
    dinfo->file[sizeof(dinfo->file)-1] = '\0';
    dinfo->line = line;
    dinfo->len = nbytes;

    dinfo->prev = 0;
    dinfo->next = dmalloc_list;
    if (dmalloc_list)
	dmalloc_list->prev = dinfo;
    dmalloc_list = dinfo;
    
    memcpy(res + sizeof(*dinfo), head, 8*sizeof(char));
    res += sizeof(*dinfo) + 8*sizeof(char);
    memcpy(res + nbytes, tail, 8*sizeof(char));
    return res;
}

void *xcalloc_d(size_t nmemb, size_t size, const char *file, int line)
{
    char *res;
    struct dmalloc_info *dinfo;
    size_t nbytes = nmemb * size;
    
    if (!log_level_initialized)
    {
#ifndef YAZ_NOLOG
        log_level=yaz_log_module_level("malloc");
#endif
        log_level_initialized=1;
    }

    if (!(res = (char*) calloc(1, nbytes+sizeof(*dinfo)+16*sizeof(char))))
	return 0;
    dinfo = (struct dmalloc_info *) res;
    strncpy (dinfo->file, file, sizeof(dinfo->file)-1);
    dinfo->file[sizeof(dinfo->file)-1] = '\0';
    dinfo->line = line;
    dinfo->len = nbytes;
    
    dinfo->prev = 0;
    dinfo->next = dmalloc_list;
    if (dinfo->next)
	dinfo->next->prev = dinfo;
    dmalloc_list = dinfo;
    
    memcpy(res + sizeof(*dinfo), head, 8*sizeof(char));
    res += sizeof(*dinfo) + 8*sizeof(char);
    memcpy(res + nbytes, tail, 8*sizeof(char));
    return res;
}

void xmalloc_trav_d(const char *file, int line)
{
    size_t size = 0;
    struct dmalloc_info *dinfo = dmalloc_list;
    
    if (!log_level_initialized)
    {
#ifndef YAZ_NOLOG
        log_level=yaz_log_module_level("malloc");
#endif
        log_level_initialized=1;
    }

#ifndef YAZ_NOLOG
    yaz_log (log_level, "malloc_trav %s:%d", file, line);
#endif
    while (dinfo)
    {
#ifndef YAZ_NOLOG
	yaz_log (log_level, " %20s:%d p=%p size=%d", dinfo->file, dinfo->line,
	      ((char*) dinfo)+sizeof(*dinfo)+8*sizeof(char), dinfo->len);
#endif
	size += dinfo->len;
	dinfo = dinfo->next;
    }
#ifndef YAZ_NOLOG
    yaz_log (log_level, "total bytes %ld", (long) size);
#endif
}

#else
/* TRACE_XMALLOC <= 1 */
#define xrealloc_d(o, x, f, l) realloc(o, x)
#define xmalloc_d(x, f, l) malloc(x)
#define xcalloc_d(x,y, f, l) calloc(x,y)
#define xfree_d(x, f, l) free(x)
#define xmalloc_trav_d(f, l) 
#endif

void xmalloc_trav_f(const char *s, const char *file, int line)
{
    if (!log_level_initialized)
    {
#ifndef YAZ_NOLOG
        log_level=yaz_log_module_level("malloc");
#endif
        log_level_initialized=1;
    }

    xmalloc_trav_d(file, line);
}

void *xrealloc_f (void *o, size_t size, const char *file, int line)
{
    void *p = xrealloc_d (o, size, file, line);

    if (!log_level_initialized)
    {
#ifndef YAZ_NOLOG
        log_level=yaz_log_module_level("malloc");
#endif
        log_level_initialized=1;
    }

    if(log_level)
#ifndef YAZ_NOLOG
        yaz_log (log_level,
            "%s:%d: xrealloc(s=%d) %p -> %p", file, line, size, o, p);
#endif
    if (!p)
    {
#ifndef YAZ_NOLOG
    	yaz_log (YLOG_FATAL|YLOG_ERRNO, "Out of memory, realloc (%d bytes)",
		 size);
#endif
    	exit(1);
    }
    return p;
}

void *xmalloc_f (size_t size, const char *file, int line)
{
    void *p = xmalloc_d (size, file, line);
    
    if (!log_level_initialized)
    {
#ifndef YAZ_NOLOG
        log_level=yaz_log_module_level("malloc");
#endif
        log_level_initialized=1;
    }

    if (log_level)
#ifndef YAZ_NOLOG
        yaz_log (log_level, "%s:%d: xmalloc(s=%d) %p", file, line, size, p);
#endif

    if (!p)
    {
#ifndef YAZ_NOLOG
        yaz_log (YLOG_FATAL, "Out of memory - malloc (%d bytes)", size);
#endif
        exit (1);
    }
    return p;
}

void *xcalloc_f (size_t nmemb, size_t size, const char *file, int line)
{
    void *p = xcalloc_d (nmemb, size, file, line);
    if (!log_level_initialized)
    {
#ifndef YAZ_NOLOG
        log_level=yaz_log_module_level("malloc");
#endif
        log_level_initialized=1;
    }

#ifndef YAZ_NOLOG
    if (log_level)
        yaz_log (log_level, "%s:%d: xcalloc(s=%d) %p", file, line, size, p);
#endif

    if (!p)
    {
#ifndef YAZ_NOLOG
        yaz_log (YLOG_FATAL, "Out of memory - calloc (%d, %d)", nmemb, size);
#endif
        exit (1);
    }
    return p;
}

char *xstrdup_f (const char *s, const char *file, int line)
{
    char *p = (char *)xmalloc_d (strlen(s)+1, file, line);
    if (!log_level_initialized)
    {
#ifndef YAZ_NOLOG
        log_level=yaz_log_module_level("malloc");
#endif
        log_level_initialized=1;
    }

#ifndef YAZ_NOLOG
    if (log_level)
        yaz_log (log_level, "%s:%d: xstrdup(s=%d) %p", file, line, strlen(s)+1, p);
#endif

    strcpy (p, s);
    return p;
}

void xfree_f(void *p, const char *file, int line)
{
    if (!p)
        return ;
#ifndef YAZ_NOLOG
    if (log_level)
        yaz_log (log_level, "%s:%d: xfree %p", file, line, p);
#endif
    xfree_d(p, file, line);
}
