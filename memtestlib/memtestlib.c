#include "memtestlib.h"

memanager * corgasm_memtest_new_memanager(const char * block_name)
{
    memanager * new_manager = NULL;
    if (block_name && (new_manager = malloc(sizeof(memanager))))
    {
        new_manager->name         = strdup(block_name);
        new_manager->all_pointers = listlib.new_list(memtestlib.destroy_pointer_wrapped);

        // * if any error occured after initializing - free memory
        // * memtestlib.destroy_manager returns NULL always
        // * this helps to reduce amount of lines
        if (!new_manager->all_pointers || !new_manager->name)
            new_manager = memtestlib.destroy_memanager(new_manager);
    }
    return new_manager;
}

memanager * corgasm_memtest_destroy_memanager(memanager * manager)
{
    if (manager)
    {
        listlib.destroy(manager->all_pointers);
        if (manager->name) free(manager->name);
        free(manager);
    }
    return NULL;
}

pointer_action * corgasm_memtest_new_pointer_action(void *       new_pointer  ,
                                                    size_t       new_size     ,
                                                    short        action_type  ,
                                                    const char * filename     ,
                                                    const char * function_name,
                                                    int          line_number  )
{
    pointer_action * new_pointer_action  = NULL;

    if (is_correct_action_type(action_type) &&\
        filename                            &&\
        function_name                       &&\
        line_number > 0                     )
    {
        new_pointer_action = malloc(sizeof(pointer_action));
        if (new_pointer_action)
        {
            new_pointer_action->new_pointer   = new_pointer;
            new_pointer_action->action_type   = action_type;
            new_pointer_action->filename      = strdup(filename);
            new_pointer_action->function_name = strdup(function_name);
            new_pointer_action->line_number   = line_number;
            new_pointer_action->new_size      = new_size;

            // * if any error occures - free memory
            // * memtestlib.destroy_pointer_action always returns NULL
            if (!new_pointer_action->filename|| !new_pointer_action->function_name)
                new_pointer_action = memtestlib.destroy_pointer_action(new_pointer_action);
        }
    }
    return new_pointer_action;
}


pointer_action * corgasm_memtest_destroy_pointer_action(pointer_action * action)
{
    if (action)
    {
        if (action->function_name) free(action->function_name);
        if (action->filename)      free(action->filename);
        free(action);
    }
    return NULL;
}

/**
 * ! Call this only if you are sure that void * action == pointer_action * action
 * ! It was written as a wrapped destructor for listlib
 * * void corgasm_listlib_destroy_node(node *, void (*)(void *))
 * * pointer to a destructor function must be of type void (*)(void*)
*/
void corgasm_memtest_destroy_pointer_action_wrapped(void * action)
{
    if (action)
        memtestlib.destroy_pointer_action((pointer_action*)action);
}

pointer * corgasm_memtest_new_pointer(void * active_pointer, size_t size)
{
    pointer * new_pointer = malloc(sizeof(pointer));
    if (new_pointer)
    {
        new_pointer->active      = active_pointer;
        new_pointer->active_size = size;
        new_pointer->history     = listlib.new_list(memtestlib.destroy_pointer_action_wrapped);
        new_pointer->is_freed    = false;

        if (!new_pointer->history)
            new_pointer = memtestlib.destroy_pointer(new_pointer);
    }
    return new_pointer;
}

pointer * corgasm_memtest_destroy_pointer(pointer * pointer_to_destroy)
{
    if (pointer_to_destroy)
    {
        listlib.destroy(pointer_to_destroy->history);
        free(pointer_to_destroy);
    }
    return NULL;
}

void corgasm_memtest_destroy_pointer_wrapped(void * pointer_to_destroy)
{
    if (pointer_to_destroy)
        memtestlib.destroy_pointer((pointer *)pointer_to_destroy);
}

pointer * corgasm_memtest_find_by_active_pointer(memanager * manager, void * ptr)
{
    if (manager)
    {
        for (node * current_node  = listlib.begin(manager->all_pointers);
                    current_node != listlib.end(manager->all_pointers);
                    current_node  = listlib.next(current_node))
        {
            pointer * current_pointer = (pointer*)(current_node->data);
            if (current_pointer->active == ptr && !current_pointer->is_freed)
                return current_pointer;
        }
    }
    return NULL;
}

void * corgasm_memtest_malloc(memanager * manager, size_t memsize, const char * filename, const char * function_name, int line_number)
{
    void * new_pointer_data = NULL;
    if (manager         &&\
        memsize > 0     &&\
        filename        &&\
        function_name   &&\
        line_number > 0 &&\
        (new_pointer_data = malloc(memsize)))
    {
        pointer * new_pointer = memtestlib.new_pointer(new_pointer_data, memsize);
        if (new_pointer)
        {
            new_pointer->active      = new_pointer_data;
            new_pointer->active_size = memsize;
            pointer_action * new_pointer_action = memtestlib.new_pointer_action(new_pointer_data,
                                                                                memsize,
                                                                                memtest_called_malloc,
                                                                                filename,
                                                                                function_name,
                                                                                line_number);
            if (new_pointer_action)
                listlib.append(new_pointer->history, (void*)new_pointer_action);
            else
                new_pointer = memtestlib.destroy_pointer(new_pointer);

        }
        if (!new_pointer)
        {
            free(new_pointer_data);
            new_pointer_data = NULL;
        }
        else
            listlib.append(manager->all_pointers, (void *)new_pointer);
    }
    return new_pointer_data;
}

void * corgasm_memtest_calloc(memanager * manager, size_t num, size_t size, const char * filename, const char * function_name, int line_number)
{
    void * new_pointer_data = NULL;
    if (manager         &&\
        num * size > 0  &&\
        filename        &&\
        function_name   &&\
        line_number > 0 &&\
        (new_pointer_data = calloc(num, size)))
    {
        pointer * new_pointer = memtestlib.new_pointer(new_pointer_data, num * size);
        if (new_pointer)
        {
            new_pointer->active      = new_pointer_data;
            new_pointer->active_size = num * size;
            pointer_action * new_pointer_action = memtestlib.new_pointer_action(new_pointer_data,
                                                                                num * size,
                                                                                memtest_called_calloc,
                                                                                filename,
                                                                                function_name,
                                                                                line_number);
            if (new_pointer_action)
                listlib.append(new_pointer->history, (void*)new_pointer_action);
            else
                new_pointer = memtestlib.destroy_pointer(new_pointer);

        }
        if (!new_pointer)
        {
            free(new_pointer_data);
            new_pointer_data = NULL;
        }
        else
            listlib.append(manager->all_pointers, (void *)new_pointer);
    }
    return new_pointer_data;
}

void  corgasm_memtest_free(memanager * manager, void * ptr, const char * filename, const char * function_name, int line_number)
{
    if (manager && ptr && filename && function_name && line_number > 0)
    {
        pointer * active_pointer = memtestlib.find_by_active_pointer(manager, ptr);
        if (active_pointer)
        {
            pointer_action * new_pointer_action = memtestlib.new_pointer_action(ptr,
                                                                                0,
                                                                                memtest_called_free,
                                                                                filename,
                                                                                function_name,
                                                                                line_number);
            if (new_pointer_action)
            {
                listlib.append(active_pointer->history, (void *)new_pointer_action);
                active_pointer->active = ptr;
                active_pointer->active_size = 0;
            }
            free(ptr);
            active_pointer->is_freed = true;
        }
    }
}

void * corgasm_memtest_realloc(memanager * manager, void * ptr, size_t new_size, const char * filename, const char * function_name, int line_number)
{

    if (!ptr)
        return memtestlib.malloc(manager, new_size, filename, function_name, line_number);

    void * new_pointer_data = NULL;
    if (manager  && \
        filename && \
        function_name && \
        line_number > 0 && \
        (new_pointer_data = realloc(ptr, new_size)))
    {
        pointer_action * new_pointer_action = memtestlib.new_pointer_action(new_pointer_data,
                                                                            new_size,
                                                                            memtest_called_realloc,
                                                                            filename,
                                                                            function_name,
                                                                            line_number);
        pointer * realloced_pointer = memtestlib.find_by_active_pointer(manager, ptr);
        listlib.append(realloced_pointer->history, (void *)new_pointer_action);
        realloced_pointer->active = ptr;
        realloced_pointer->active_size = new_size;
    }
    return new_pointer_data;
}

bool is_correct_action_type(short action_type)
{
    return (action_type == memtest_called_malloc  ||\
            action_type == memtest_called_calloc  ||\
            action_type == memtest_called_realloc ||\
            action_type == memtest_called_free        );
}


void print_pointer(pointer * ptr, const char * block_name)
{

    table * data_table = tablelib.new_table(7, "block name", "new pointer", "new size", "action", "filename", "function name", "line_number");

    for (node * current_node  = listlib.begin(ptr->history);
                current_node != listlib.end(ptr->history);
                current_node  = listlib.next(current_node))
    {
            pointer_action * current_action = (pointer_action*)(current_node->data);
            char ptr_buf[20];
            sprintf(ptr_buf, "%p", current_action->new_pointer);

            char size_buff[100];
            sprintf(size_buff, "%zu", current_action->new_size);

            const char * actions[] = {"malloc", "calloc", "realloc", "free"};

            char line_number_buff[1000];
            sprintf(line_number_buff, "%zu", current_action->line_number);

            tablelib.add_line(data_table, block_name, ptr_buf, size_buff, actions[current_action->action_type], current_action->filename, current_action->function_name, line_number_buff);
    }
    tablelib.show(data_table);
    data_table = tablelib.destroy(data_table);
    printf("\n");
}

size_t find_still_reachable(memanager * manager)
{
    size_t result = 0;
    for (node * current_node  = listlib.begin(manager->all_pointers);
                current_node != listlib.end(manager->all_pointers);
                current_node  = listlib.next(current_node))
        {
            pointer * current_pointer     = (pointer*)(current_node->data);
            if (!current_pointer->is_freed)
                result++;
        }
    return result;
}

void corgasm_memtest_short_report(memanager * manager)
{
    if (manager)
    {
        size_t all_pointers_amount = manager->all_pointers->length;
        size_t still_reachable     = find_still_reachable(manager);

        char buffer[1024];
        char buffer2[1024];
        sprintf(buffer,  "%u", all_pointers_amount);
        sprintf(buffer2, "%u", still_reachable);

        table * data_table = tablelib.new_table(4, "block name", "allocated pointers", "still reachable", "all pointers are freed");
                             tablelib.add_line(data_table, manager->name, buffer, buffer2, still_reachable ? "false" : "true");
        tablelib.show(data_table);
        data_table = tablelib.destroy(data_table);
    }
}

void corgasm_memtest_full_report(memanager * manager)
{
    if (manager)
    {
        for (node * current_node  = listlib.begin(manager->all_pointers);
                    current_node != listlib.end(manager->all_pointers);
                    current_node  = listlib.next(current_node))
            {
                pointer * current_pointer = (pointer*)(current_node->data);
                print_pointer(current_pointer, manager->name);
                printf("\n");
            }
        printf("\n");
    }
}
