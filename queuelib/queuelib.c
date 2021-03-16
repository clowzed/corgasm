#include "queuelib.h"

queue * corgasm_queuelib_new_queue()
{
    queue * self = malloc(sizeof(queue));
    if (self)
    {
        self->data = listlib.new_list(NULL);
        if (!self->data)
            queuelib.destroy(self);
    }
    return self;
}

bool corgasm_queuelib_put(queue * self, void * data)
{
    bool was_putted = false;
    if (self)
        was_putted = listlib.append(self->data, data);
    return was_putted;
}

void * corgasm_queuelib_get(const queue * self)
{
    void * data = NULL;
    if (self && self->data->length)
    {
        node * data_node = listlib.get(self->data, 0);
        data = data_node->data;
        listlib.remove(self->data, 0);
    }
    return data;
}

bool corgasm_queuelib_empty(const queue * self)
{
    return self ? !self->data->length : true;
}

bool corgasm_queuelib_destroy_with (queue * self, void (*destructor)(void *))
{
    bool was_destroyed = false;
    if (self)
        listlib.destroy_node(self->data->head, destructor);
    return was_destroyed;
}

bool corgasm_queuelib_destroy(queue * self)
{
    return  queuelib.destroy_with(self, NULL);
}


#ifdef LIB_BUILD_QUEULIB

int main()
{
    queue * data = queuelib.new_queue();
    for (int i = 0; i < 5 ; i++)
        queuelib.put(data, NULL);
    for (int i = 0; i < 5; i++)
        printf("data: %p\n", queuelib.get(data));
    queuelib.destroy(data);
}

#endif