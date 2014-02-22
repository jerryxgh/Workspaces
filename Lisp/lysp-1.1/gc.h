#ifndef _GC_H_
#define _GC_H_

#include <sys/types.h>

struct GC_StackRoot
{
  void **root;
  struct GC_StackRoot *next;
};

#define GC_PROTECT(V)		struct GC_StackRoot _sr_##V;  _sr_##V.root= (void *)&V;  GC_push_root(&_sr_##V)
#define GC_UNPROTECT(V)		GC_pop_root(&_sr_##V)

#define GC_INIT()
#define GC_init()

void   *GC_malloc(size_t nbytes);
void   *GC_malloc_atomic(size_t nbytes);
void   	GC_free(void *ptr);
void   	GC_add_root(void *root);
void   	GC_delete_root(void *root);
void   	GC_mark(void *ptr);
void   	GC_sweep(void);
void   	GC_gcollect(void);
size_t 	GC_count_objects(void);
size_t 	GC_count_bytes(void);

extern struct GC_StackRoot *GC_stack_roots;

static inline void GC_push_root(struct GC_StackRoot *sr)
{
  sr->next= GC_stack_roots;
  GC_stack_roots= sr;
}

static inline void GC_pop_root(struct GC_StackRoot *sr)
{
#if 1
  GC_stack_roots= sr->next;
#else /* paranoid version for broken code warns of mismatched pops with a SEGV */
  struct GC_StackRoot *nr= sr->next;
  while (nr != GC_stack_roots)
    GC_stack_roots= GC_stack_roots->next;
#endif
}

typedef void (*GC_mark_function_t)(void *ptr);
extern GC_mark_function_t GC_mark_function;

typedef void (*GC_free_function_t)(void *ptr);
extern GC_free_function_t GC_free_function;

#endif /* _GC_H_ */
