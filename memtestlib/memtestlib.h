#ifndef __CORGASM_MEMTEST_H__
#define __CORGASM_MEMTEST_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../listlib/listlib.h"
#include "../tablelib/tablelib.h"

#define  memtest_called_malloc  0
#define  memtest_called_calloc  1
#define  memtest_called_realloc 2
#define  memtest_called_free    3

// ?   strdup is not in string.h :(
char * strdup(const char *);

#define file_information __FILE__, __func__, __LINE__

typedef struct pointer_action
{
    int     line_number;
    char *  function_name;
    char *  filename;
    short   action_type;
    void *  new_pointer;
    size_t  new_size;
}pointer_action;

typedef struct pointer
{
    list * history;
    void * active;
    size_t active_size;
    bool   is_freed;
}pointer;

typedef struct corgasm_memanager
{
    char * name;
    list * all_pointers;
}memanager;

void *           corgasm_memtest_malloc                        (memanager *, size_t, const char *, const char *, int        );
void *           corgasm_memtest_calloc                        (memanager *, size_t, size_t, const char *, const char *, int);
void             corgasm_memtest_free                          (memanager *, void *, const char *, const char *, int        );
void *           corgasm_memtest_realloc                       (memanager *, void *, size_t, const char *, const char *, int);
memanager *      corgasm_memtest_new_memanager                 (const char *                                                );
memanager *      corgasm_memtest_destroy_memanager             (memanager *                                                 );
pointer_action * corgasm_memtest_new_pointer_action            (void *, size_t, short, const char *, const char *, int      );
pointer_action * corgasm_memtest_destroy_pointer_action        (pointer_action *                                            );
void             corgasm_memtest_destroy_pointer_action_wrapped(void *                                                      );
pointer *        corgasm_memtest_new_pointer                   (void *, size_t                                              );
pointer *        corgasm_memtest_destroy_pointer               (pointer *                                                   );
void             corgasm_memtest_destroy_pointer_wrapped       (void *                                                      );
void             corgasm_memtest_short_report                  (memanager *                                                 );
void             corgasm_memtest_full_report                   (memanager *                                                 );
pointer *        corgasm_memtest_find_by_active_pointer        (memanager *, void *                                         );
bool             is_correct_action_type                        (short                                                       );
size_t           find_still_reachable                          (memanager *                                                 );
void             print_pointer                                 (pointer *, const char *                                     );


typedef struct corgasm_memanager_functions
{
    void      *      (*malloc )                       (memanager *, size_t , const char * , const char * , int         );
    void      *      (*calloc )                       (memanager *, size_t , size_t, const char * , const char * , int );
    void             (*free   )                       (memanager *, void * , const char * , const char * , int         );
    void      *      (*realloc)                       (memanager *, void *, size_t, const char *, const char *, int    );
    memanager *      (*new_memanager)                 (const char *                                                    );
    memanager *      (*destroy_memanager)             (memanager *                                                     );
    pointer_action * (*new_pointer_action)            (void *, size_t, short, const char *, const char *, int          );
    pointer_action * (*destroy_pointer_action)        (pointer_action *                                                );
    void             (*destroy_pointer_action_wrapped)(void *                                                          );
    pointer *        (*new_pointer)                   (void *, size_t                                                  );
    pointer *        (*destroy_pointer)               (pointer *                                                       );
    void             (*destroy_pointer_wrapped)       (void *                                                          );
    pointer *        (*find_by_active_pointer)        (memanager *, void *                                             );
    void             (*short_report)                  (memanager *                                                     );
    void             (*full_report)                   (memanager *                                                     );
}corgasm_memanager_functions;


static const corgasm_memanager_functions memtestlib = {
    .malloc                          = corgasm_memtest_malloc,
    .calloc                          = corgasm_memtest_calloc,
    .free                            = corgasm_memtest_free,
    .realloc                         = corgasm_memtest_realloc,
    .new_memanager                   = corgasm_memtest_new_memanager,
    .destroy_memanager               = corgasm_memtest_destroy_memanager,
    .new_pointer_action              = corgasm_memtest_new_pointer_action,
    .destroy_pointer_action          = corgasm_memtest_destroy_pointer_action,
    .destroy_pointer_action_wrapped  = corgasm_memtest_destroy_pointer_action_wrapped,
    .new_pointer                     = corgasm_memtest_new_pointer,
    .destroy_pointer                 = corgasm_memtest_destroy_pointer,
    .destroy_pointer_wrapped         = corgasm_memtest_destroy_pointer_wrapped,
    .find_by_active_pointer          = corgasm_memtest_find_by_active_pointer,
    .short_report                    = corgasm_memtest_short_report,
    .full_report                     = corgasm_memtest_full_report
};


#endif // __CORGASM_MEMTEST_H__
