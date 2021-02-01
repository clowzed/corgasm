#include "listlib.h"

list * corgasm_listlib_new_list(void (*destructor)(void *))
{
    list * new_list = malloc(sizeof(list));
    if (new_list)
    {
        new_list->destructor = destructor;
        new_list->length     = 0;
        new_list->head       = NULL;
        new_list->tail       = NULL;
    }
    return new_list;
}

node * corgasm_listlib_new_node(void * data)
{
    node * new_node = malloc(sizeof(node));
    if (new_node)
    {
        new_node->data = data;
        new_node->next = NULL;
    }
    return new_node;
}

void  corgasm_listlib_destroy_node(node * node_to_destroy, void (*destructor)(void *))
{
    if (node_to_destroy)
    {
        if (destructor && node_to_destroy->data)
            destructor(node_to_destroy->data);

        listlib.destroy_node(node_to_destroy->next, destructor);
        free(node_to_destroy);
    }
}

void corgasm_listlib_clear(list * self)
{
    if (self) listlib.destroy_node(self->head, self->destructor);
}

list * corgasm_listlib_destroy(list * self)
{
    if (self)
    {
        listlib.clear(self);

        self->destructor = NULL;
        self->head       = NULL;
        self->tail       = NULL;
        self->length     = 0;

        free(self);
    }
    return NULL;
}

bool corgasm_listlib_append(list * self, void * data)
{
    bool was_appended = false;
    if (self)
    {
        node * new_node = listlib.new_node(data);
        if (new_node)
        {
            if (!self->length) self->head        = new_node;
            else               self->tail->next  = new_node;

            self->tail    = new_node;
            self->length += 1;
            was_appended  = true;
        }
    }
    return was_appended;
}

void * corgasm_listlib_pop(list * self)
{
    void * popped_data = NULL;
    if (self)
    {
        if (self->length == 1)
        {
            popped_data = self->head->data;
            self->head = NULL;
            self->tail = NULL;
            self->length = 0;
        }
        else if (self->length > 1)
        {
            node * penultimate_node = listlib.get(self,
                                                  self->length - 2);
            popped_data = penultimate_node->next->data;
            listlib.destroy_node(penultimate_node->next,
                                 NULL);
            self->tail      = penultimate_node;
            penultimate_node->next = NULL;
            self->length--;
        }
    }
    return popped_data;
}

node * corgasm_listlib_get(list * self, size_t index)
{
    node * result_node = NULL;
    if (self && index < self->length)
    {
        if (self->head)
        {
            size_t current_index = 0;
            node * current_node = self->head;
            while (current_node->next)
            {
                if (current_index == index)
                    break;
                else
                {
                    current_index++;
                    current_node = current_node->next;
                }
            }
            result_node = current_node;
        }
    }
    return result_node;
}

bool corgasm_listlib_insert(list * self, void * data, size_t index)
{
    bool was_inserted = false;
    if (self && index < self->length + 1)
    {
        if (index == self->length)
            was_inserted = listlib.append(self, data);
        else
        {
            node * new_node    = listlib.new_node(data);
            if (new_node)
            {
                node * before_node = listlib.get(self, index - 1);
                node * current     = listlib.get(self, index);

                before_node->next  = new_node;
                new_node->next     = current;
                was_inserted       = true;
            }
        }
    }
    return was_inserted;
}

node * corgasm_listlib_begin(list * self)
{
    node * begin_node = self ? self->head : NULL;
    return begin_node;
}

node * corgasm_listlib_end(list * self)
{
    node * end_node = self && self->tail ? self->tail->next : NULL;
    return end_node;
}

node * corgasm_listlib_next(node * current)
{
    node * next_node = current ? current->next: NULL;
    return next_node;
}

list * corgasm_listlib_map(list * self, void * (*map_function)(void *))
{
    if (self && map_function)
    {
        fornode(current_node, self)
            current_node->data = map_function(current_node->data);
    }
    return self;
}

size_t corgasm_listlib_index_of(list * self, void * data)
{
    if (self)
    {
        size_t current_index = 0;
        fornode(current_node, self)
        {
            if (current_node->data == data)
                return current_index;
            else
                current_index++;
        }
    }
    return UINT_MAX;
}

bool corgasm_listlib_contains(list * self, void * data)
{
    return self ? listlib.index_of(self, data) != UINT_MAX : false;
}

void corgasm_listlib_destroy_wrapped(void * self)
{
    if (self) listlib.destroy((list *) self);
}

void * corgasm_listlib_set(list * self, size_t index, void * new_ptr)
{
    void * last = NULL;
    node * setting_node = listlib.get(self, index);

    if (setting_node)
    {
        last               = setting_node->data;
        setting_node->data = new_ptr;
    }
    return last;
}

void * corgasm_listlib_extract(node * n)
{
    return n ? n->data : NULL;
}