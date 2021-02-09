#include "arraylib.h"
#include <stdio.h>

/**
 * @brief Allocates memory for one new element
 * @param[in] array * self pointer to the array
 * @return bool if operation was successful
 */
bool corgasm_arraylib_update_capacity(array * self)
{
    bool successfully_updated = false;
    if (self)
    {
        void ** new_data = realloc(self->data, sizeof(void *) * (self->size + 1));
        if (new_data)
        {
            self->data = new_data;
            self->size++;
            successfully_updated = true;
        }
    }
    return successfully_updated;
}


/**
 * @brief Allocates memory for a new array
 * @param[in] destructor function pointer to a destructor
 * @return array * pointer to a new allocated array
 * @warning Can return NULL
 * @note destructor can be NULL
 */
array * corgasm_arraylib_new_array(void (*destructor)(void *))
{
    array * self = (array *)malloc(sizeof(array));
    if (self)
    {
        self->destructor = destructor;
        self->data = NULL;
        self->size = 0;
    }
    return self;
}


/**
 * @brief Destructor for an array
 * @param[in] array * self pointer to the array
 * @return array *
 * @note Always return NULL
 */
array * corgasm_arraylib_destroy(array * self)
{
    if (self)
    {
        if (self->destructor)
            for (size_t i = 0; i < self->size; i++)
                self->destructor(self->data[i]);

        // * prevents memleak warning in debugger
        if (self->data) free(self->data);

        // * for clear view in debugger
        self->data = NULL;
        self->size = 0;

        free(self);
    }
    return NULL;
}


/**
 * @brief Wrapped destructor for array
 * @param[in] void * self pointer to array
 * @return void
 * @warning Use only if you are sure that pointer is correct
 */
void corgasm_arraylib_destroy_wrapped(void * self)
{
    if (self) arraylib.destroy((array *)self);
}


/**
 * @brief Appends data to an array
 * @param[in] array * self pointer to the array
 * @param[in] const void * data pointer to a data
 * @return bool if operation was successful
 */
bool corgasm_arraylib_append(array * self, void * data)
{
    bool was_appended = false;
    if (self)
    {
        was_appended = corgasm_arraylib_update_capacity(self);
        if (was_appended)
            self->data[self->size - 1] = data;
    }
    return was_appended;
}


/**
 * @brief Sets data in array at a given index
 * @param[in] array * self pointer to the array
 * @param[in] size_t index Where to store the data
 * @param[in] const void * data Pointer to a new data
 * @return bool if operation was successful
 * @warning Do not forget to handle previous data
 */
bool corgasm_arraylib_set(array * self, size_t index, void * data)
{
    bool was_setted = false;
    if (self && index < self->size)
    {
        self->data[index] = data;
        was_setted = true;
    }
    return was_setted;
}


/**
 * @brief Gets data from array at a given index
 * @param[in] array * self pointer to the array
 * @param[in] size_t index target index
 * @return void * data pointer to a data in the array
 */
void * corgasm_arraylib_get(array * self, size_t index)
{
    void * target_data = NULL;
    if (index < self->size)
        target_data = self->data[index];
    return target_data;
}
