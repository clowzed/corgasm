#ifndef __CORGASM_ARRAYLIB_H__
#define __CORGASM_ARRAYLIB_H__

#include <stdlib.h>
#include <stdbool.h>


typedef struct corgasm_arraylib_array
{
    void ** data;
    size_t  size;
    void    (*destructor)(void *);
}array;

/**
 * @brief Allocates memory for one new element
 * @param[in] array * self pointer to the array
 * @return bool if operation was successful
 */
bool corgasm_arraylib_update_capacity(array * self);

/**
 * @brief Allocates memory for a new array
 * @param[in] destructor function pointer to a destructor
 * @return array * pointer to a new allocated array
 * @warning Can return NULL
 * @note destructor can be NULL
 */
array * corgasm_arraylib_new_array(void (*destructor)(void *));

/**
 * @brief Destructor for an array
 * @param[in] array * self pointer to the array
 * @return array *
 * @note Always return NULL
 */
array * corgasm_arraylib_destroy(array * self);


/**
 * @brief Wrapped destructor for array
 * @param[in] void * self pointer to array
 * @return void
 * @warning Use only if you are sure that pointer is correct
 */
void corgasm_arraylib_destroy_wrapped(void * self);

/**
 * @brief Appends data to an array
 * @param[in] array * self pointer to the array
 * @param[in] const void * data pointer to a data
 * @return bool if operation was successful
 */
bool corgasm_arraylib_append(array * self, void * data);

/**
 * @brief Sets data in array at a given index
 * @param[in] array * self pointer to the array
 * @param[in] size_t index Where to store the data
 * @param[in] const void * data Pointer to a new data
 * @return bool if operation was successful
 * @warning Do not forget to handle previous data
 */
bool corgasm_arraylib_set(array * self, size_t index, void * data);

/**
 * @brief Gets data from array at a given index
 * @param[in] array * self pointer to the array
 * @param[in] size_t index target index
 * @return void * data pointer to a data in the array
 */
void * corgasm_arraylib_get(array * self, size_t index);


typedef struct corgasm_arraylib_functions
{
    array * (*new_array)(void (*destructor)(void *));
    array * (*destroy)(array * self);
    void    (*destroy_wrapped)(void * self);
    bool    (*append)(array * self, void * data);
    bool    (*set)(array * self, size_t index, void * data);
    void *  (*get)(array * self, size_t index);

}corgasm_arraylib_functions;

static const corgasm_arraylib_functions arraylib =
{
    .new_array       = corgasm_arraylib_new_array,
    .destroy         = corgasm_arraylib_destroy,
    .destroy_wrapped = corgasm_arraylib_destroy_wrapped,
    .append          = corgasm_arraylib_append,
    .set             = corgasm_arraylib_set,
    .get             = corgasm_arraylib_get
};

#endif
