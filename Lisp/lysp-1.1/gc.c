/* gc.c -- trivial single-threaded stop-world non-moving mark-sweep collector
**
** Copyright (c) 2008 Ian Piumarta
** All Rights Reserved
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the 'Software'),
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, provided that the above copyright notice(s) and this
** permission notice appear in all copies of the Software.  Inclusion of the
** the above copyright notice(s) and this permission notice in supporting
** documentation would be appreciated but is not required.
**
** THE SOFTWARE IS PROVIDED 'AS IS'.  USE ENTIRELY AT YOUR OWN RISK.
**
** Last edited: 2008-10-20 19:45:22 by piumarta on ubuntu.piumarta.com
*/

#include "gc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ALIGN		sizeof(long)
#define QUANTUM		32768
#define ALLOCS_PER_GC	10000

#define VERBOSE	0

typedef struct _gcheader
{
  union {
    int			flags;
    struct {
      unsigned int	used : 1;
      unsigned int	atom : 1;
      unsigned int	mark : 1;
    };
  };
  struct _gcheader *prev;
  struct _gcheader *next;
  size_t	    size;
} gcheader;

static inline void *hdr2ptr(gcheader *hdr)	{ return (void *)(hdr + 1); }
static inline gcheader *ptr2hdr(void *ptr)	{ return (gcheader *)ptr - 1; }

static gcheader  gcbase= { { -1 }, &gcbase, &gcbase, 0 };
static gcheader *gcnext= &gcbase;

static int gcCount= ALLOCS_PER_GC;

void *GC_malloc(size_t lbs)
{
  gcheader *hdr, *org;
  if (!gcCount--) {
    gcCount= ALLOCS_PER_GC;
    GC_gcollect();
  }
  org= hdr= gcnext;
  lbs= (lbs + ALIGN-1) & ~(ALIGN-1);
#if VERBOSE > 1
  printf("malloc %d\n", lbs);
#endif
 again:
#if VERBOSE > 3
  {
    gcheader *h= gcnext;
    do { 
      printf("  %2d %p <- %p -> %p = %x\n", h->flags, h->prev, h, h->next, h->size);
      h= h->next;
    } while (h != gcnext);
  }
#endif
  do {
#if VERBOSE > 2
    printf("? %2d %p <- %p -> %p = %x\n", hdr->flags, hdr->prev, hdr, hdr->next, hdr->size);
#endif
    if ((!hdr->used) && (hdr->size >= lbs))
      {
	void *mem;
	if (hdr->size >= lbs + sizeof(gcheader) + ALIGN)
	  {
	    gcheader *ins= (gcheader *)(hdr2ptr(hdr) + lbs);
	    ins->flags= 0;
	    ins->prev= hdr;
	    ins->next= hdr->next;
	    ins->size= hdr->size - lbs - sizeof(gcheader);
	    hdr->next->prev= ins;
	    hdr->next= ins;
	    hdr->size= lbs;
	  }
	hdr->used= 1;
	gcnext= hdr->next;
#if VERBOSE > 2
	printf("MALLOC %p\n", hdr);
#endif
	mem= hdr2ptr(hdr);
	memset(mem, 0, hdr->size);
	return mem;
      }
    hdr= hdr->next;
  } while (hdr != org);
  {
    size_t incr= QUANTUM;
    size_t req= sizeof(gcheader) + lbs;
    while (incr <= req) incr *= 2;
    hdr= (gcheader *)sbrk(incr);
    if (hdr != (gcheader *)-1)
      {
	hdr->flags= 0;
	hdr->next= &gcbase;
	hdr->prev= gcbase.prev;
	hdr->prev->next= hdr;
	gcbase.prev= hdr;
	hdr->size= incr - sizeof(gcheader);
#if VERBOSE
	printf("extend by %d at %p\n", hdr->size, hdr);
#endif
	goto again;
      }
  }
  printf("out of memory\n");
  return 0;
}

void *GC_malloc_atomic(size_t lbs)
{
  void *mem= GC_malloc(lbs);
  ptr2hdr(mem)->atom= 1;
  return mem;
}

void *GC_realloc(void *ptr, size_t lbs)
{
  gcheader *hdr= ptr2hdr(ptr);
  void *mem;
  if (lbs <= hdr->size) return ptr;
  mem= GC_malloc(lbs);
  memcpy(mem, ptr, hdr->size);
  ptr2hdr(mem)->atom= hdr->atom;
  GC_free(ptr);
  return mem;
}

gcheader *GC_freeHeader(gcheader *hdr)
{
#if VERBOSE > 1
  printf("FREE %p\n", hdr);
#endif
  hdr->flags= 0;
  if ((!hdr->prev->flags) && (hdr2ptr(hdr->prev) + hdr->prev->size == hdr))
    {
#if VERBOSE > 2
      printf("COALESCE PREV %p\n", hdr->prev);
#endif
      hdr->prev->next= hdr->next;
      hdr->next->prev= hdr->prev;
      hdr->prev->size += sizeof(gcheader) + hdr->size;
      hdr= hdr->prev;
    }
  if ((!hdr->next->used) && (hdr2ptr(hdr) + hdr->size == hdr->next))
    {
#if VERBOSE > 2
      printf("COALESCE NEXT %p\n", hdr->next);
#endif
      hdr->size += sizeof(gcheader) + hdr->next->size;
      hdr->next= hdr->next->next;
      hdr->next->prev= hdr;
    }
#if VERBOSE > 3
  {
    gcheader *h= &gcbase;
    do { 
      printf("  %2d %p <- %p -> %p = %x\n", h->flags, h->prev, h, h->next, h->size);
      h= h->next;
    } while (h != &gcbase);
  }
#endif
  return hdr;
}

void GC_free(void *ptr)
{
  gcnext= GC_freeHeader(ptr2hdr(ptr));
}

void GC_default_mark_function(void *ptr)
{
  gcheader *hdr= ptr2hdr(ptr);
  void	  **pos= ptr;
  void	  **lim= hdr2ptr(hdr) + hdr->size - sizeof(void *);
  while (pos <= lim)
    {
      void *field= *pos;
      if (field && !((long)field & 1))
	GC_mark(field);
      ++pos;
    }
}

GC_mark_function_t GC_mark_function= GC_default_mark_function;

void GC_mark(void *ptr)
{
  gcheader *hdr= ptr2hdr(ptr);
#if VERBOSE > 2
  printf("mark? %p %d\n", hdr, hdr->flags);
#endif
  if (!hdr->mark) {
    hdr->mark= 1;
    if (!hdr->atom)
      GC_mark_function(ptr);
  }
}

GC_free_function_t GC_free_function= 0;

void GC_sweep(void)
{
  gcheader *hdr= gcbase.next;
  do {
#if VERBOSE > 2
    printf("sweep? %p %d\n", hdr, hdr->flags);
#endif
    if (hdr->flags)
      {
	if (hdr->mark)
	  hdr->mark= 0;
	else {
	  if (GC_free_function) GC_free_function(hdr2ptr(hdr));
	  hdr= GC_freeHeader(hdr);
	}
      }
    hdr= hdr->next;
  } while (hdr != &gcbase);
  gcnext= gcbase.next;
}

static void ***roots= 0;
static size_t numRoots= 0;
static size_t maxRoots= 0;

struct GC_StackRoot *GC_stack_roots= 0;

void GC_add_root(void *root)
{
  if (numRoots == maxRoots)
    roots= maxRoots
      ? realloc(roots, sizeof(roots[0]) * (maxRoots *= 2))
      : malloc (       sizeof(roots[0]) * (maxRoots= 128));
  roots[numRoots++]= (void **)root;
}

void GC_delete_root(void *root)
{
  int i;
  for (i= 0;  i < numRoots;  ++i)
    if (roots[i] == (void **)root)
      break;
  if (i < numRoots)
    {
      memmove(roots + i, roots + i + 1, sizeof(roots[0]) * (numRoots - i));
      --numRoots;
    }
}

void GC_gcollect(void)
{
  int i;
  struct GC_StackRoot *sr;
  for (i= 0;  i < numRoots;  ++i)		if (*roots[i])   GC_mark(*roots[i]);
  for (sr= GC_stack_roots;  sr;  sr= sr->next)	if (*(sr->root)) GC_mark(*(sr->root));
  GC_sweep();
}

size_t GC_count_objects(void)
{
  gcheader *hdr= gcbase.next;
  size_t count= 0;
  do {
    if (hdr->flags)
      ++count;
    hdr= hdr->next;
  } while (hdr != &gcbase);
  return count;
}

size_t GC_count_bytes(void)
{
  gcheader *hdr= gcbase.next;
  size_t count= 0;
  do {
    if (hdr->flags)
      count += hdr->size;
    hdr= hdr->next;
  } while (hdr != &gcbase);
  return count;
}

#if 0

#include <stdlib.h>

#define RAND(N)	(1 + (int)((float)N * (rand() / (RAND_MAX + 1.0))))

struct cell { int tag;  struct cell *next; };

void *mklist(int n)
{
  struct cell *cell;
  if (!n) return 0;
  cell= GC_malloc(8);
  cell->tag= n << 1 | 1;
  cell->next= mklist(n - 1);
  return cell;
}

void delist(struct cell *cell)
{
  if (cell && cell->next && cell->next->next) {
    cell->next= cell->next->next;
    delist(cell->next->next);
  }
}

int main()
{
  int i, j;
  void *a, *b, *c, *d, *e;
  for (i= 0;  i < 1000000;  ++i) {
#if !VERBOSE
# define printf(...)
#endif
    //#define GC_malloc malloc
    //#define GC_free free
    a= GC_malloc(RAND(1));	    printf("%p\n", a);
    b= GC_malloc(RAND(10));	    printf("%p\n", b);
    c= GC_malloc(RAND(100));	    printf("%p\n", c);
    d= GC_malloc(RAND(1000));	    printf("%p\n", d);
    e= GC_malloc(RAND(10000));	    printf("%p\n", e);
    GC_free(a);
    GC_free(b);
    //    GC_free(c);
    GC_free(d);
    GC_free(e);
    a= GC_malloc(RAND(100));	    printf("%p\n", a);
    b= GC_malloc(RAND(200));	    printf("%p\n", b);
    c= GC_malloc(RAND(300));	    printf("%p\n", c);
    d= GC_malloc(RAND(400));	    printf("%p\n", d);
    e= GC_malloc(RAND(500));	    printf("%p\n", e);
    GC_free(e);
    GC_free(d);
    //    GC_free(c);
    GC_free(b);
    GC_free(a);
    a= GC_malloc(RAND(4));	    printf("%p\n", a);
    b= GC_malloc(RAND(16));	    printf("%p\n", b);
    c= GC_malloc(RAND(64));	    printf("%p\n", c);
    d= GC_malloc(RAND(256));	    printf("%p\n", d);
    e= GC_malloc(RAND(1024));	    printf("%p\n", e);
    GC_free(e);
    GC_free(b);
    //    GC_free(c);
    GC_free(d);
    GC_free(a);
    a= GC_malloc(RAND(713));	    printf("%p\n", a);
    b= GC_malloc(RAND(713));	    printf("%p\n", b);
    c= GC_malloc(RAND(713));	    printf("%p\n", c);
    d= GC_malloc(RAND(713));	    printf("%p\n", d);
    e= GC_malloc(RAND(713));	    printf("%p\n", e);
    GC_free(a);
    GC_free(c);
    //    GC_free(e);
    GC_free(d);
    GC_free(b);
#undef printf
    if (i % 100000 == 0) printf("alive: %d bytes in %d objects\n", GC_count_bytes(), GC_count_objects());
    GC_sweep();
    if (i % 100000 == 0) printf("alive: %d bytes in %d objects\n", GC_count_bytes(), GC_count_objects());
  }
  {
    GC_PROTECT(a);
    for (i= 0;  i < 100;  ++i) {
      for (j= 0;  j < 100;  ++j) {
	a= mklist(2000);
	delist(a);
#if VERBOSE
	{
	  struct cell *c= a;
	  printf("----\n");
	  while (c) {
	    printf("%p %d %p\n", c, c->tag >> 1, c->next);
	    c= c->next;
	  }
	}
#endif
      }
      GC_gcollect();
    }
    GC_UNPROTECT(a);
  }
  printf("alive: %d bytes in %d objects\n", GC_count_bytes(), GC_count_objects());
  GC_sweep();
  printf("alive: %d bytes in %d objects\n", GC_count_bytes(), GC_count_objects());
  return 0;
}

#endif
