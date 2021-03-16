#include "queuelib.h"


queue * corgasm_queuelib_new_queue()
{
    queue * self = malloc(sizeof(queue));
    if (self)
    {
        self->data = listlib.new_list(NULL);
        if (!self->data)
            self = queuelib.destroy(self);
    }
    return self;
}


bool corgasm_queuelib_put(queue * self, const void * data)
{
    bool was_putted = false;
    if (self)
        was_putted = listlib.append(self->data, data);
    return was_putted;
}

void * corgasm_queuelib_get(const queue * self)
{
    return listlib.get(self->data, 0);
}
bool    corgasm_queuelib_empty        (      queue * self);
bool    corgasm_queuelib_destroy_with (      queue * self, void (*destructor)(void *));
bool    corgasm_queuelib_destroy      (      queue * self);