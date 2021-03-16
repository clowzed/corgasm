#ifndef __CORGASM_QUEUELIB_H__
#define __CORGASM_QUEUELIB_H__

#include "../listlib/listlib.h"

typedef struct corgasm_queuelib_queue
{
    list * data;
}queue;

typedef struct corgasm_queuelib_functions
{
    queue * (*new_queue)    ();
    bool    (*put)          (      queue * self, const void * data);
    void  * (*get)          (const queue * self);
    bool    (*empty)        (      queue * self);
    bool    (*destroy)      (queue * self);
    bool    (*destroy_with) (queue * self, void (*destructor)(void *))

}corgasm_queuelib_functions;


queue * corgasm_queuelib_new_queue    ();
bool    corgasm_queuelib_put          (      queue * self, const void * data);
void  * corgasm_queuelib_get          (const queue * self);
bool    corgasm_queuelib_empty        (      queue * self);
bool    corgasm_queuelib_destroy_with (      queue * self, void (*destructor)(void *));
bool    corgasm_queuelib_destroy      (      queue * self);




static const corgasm_queuelib_functions queuelib = {
    .new_queue     = corgasm_queuelib_new_queue,
    .put           = corgasm_queuelib_put,
    .get           = corgasm_queuelib_get,
    .empty         = corgasm_queuelib_empty,
    .destroy       = corgasm_queuelib_destroy,
    .destroy_with  = corgasm_queuelib_destroy_with,
};

#endif
