#ifndef __CORGASM_LISTLIB_H__
#define __CORGASM_LISTLIB_H__

#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

typedef struct node
{
    struct node * next;
    void *        data;
} node;


typedef struct list
{
    node *     head;
    node *     tail;
    size_t     length;
    void     (*destructor)(void *);

} list;

/**
 * @brief     Allocates memory for new list and initializes it
 * @param[in] destructor your function which will be applied to each void * inside each node
 * @return    list * pointer to a new list
 * @warning   Do not forget to call destructor for list after usage
 * @note destructor can be NULL
 */
list * corgasm_listlib_new_list(void (*destructor)(void *));

/**
 * @brief     Allocates memory for a structure of a node and sets data
 * @param[in] void * pointer to a data to set in the structure
 * @return    Pointer to a new allocated node
 */
node * corgasm_listlib_new_node(void * data);

/**
 * @brief Deallocates memory of a list and each node and each data
 * @param[in] list * pointer to a list to destroy
 * @return NULL always
 */
list * corgasm_listlib_destroy(list * self);

/**
 * @brief Deallocates memory of a node and data inside with a destructor
 * @param[in] node * pointer to a node to destroy
 * @param[in] destructor pointer to a funtion of type void f(void *)
 * @return void
 */
void corgasm_listlib_destroy_node(node * node_to_destroy, void (*destructor)(void *));

/**
 * @brief Adds a node with data to the tail of the list
 * @param[in] list * pointer to a list to append to
 * @param[in] void * pointer to a data to append
 * @return bool if operation was successful
 */
bool corgasm_listlib_append(list * self, void * data);

/**
 * @brief Pops data from a list
 * @param[in] list * pointer to a source list
 * @return void * pointer to a data from the tail of the list
 * @warning Can return NULL beacause list can be empty
 */
void * corgasm_listlib_pop(list * self);

/**
 * @brief Get node from list with the given index
 * @param[in] list * pointer to a source list
 * @param[in] size_t index of the node to get
 * @return node * pointer to the node with a given index
 * @warning Can return NULL beacause list can be empty or index can be out of range
 */
node * corgasm_listlib_get(list * self, size_t index);

/**
 * @brief Inserts data into the list
 * @param[in] list * pointer to a list to insert data in
 * @param[in] void * pointer to the data to be inserted
 * @param[in] size_t index where to insert the data
 * @return bool if operation was successful
 */
bool corgasm_listlib_insert(list * self, void * data, size_t index);

/**
 * @brief Used for iterating over the list
 * @param[in] list * pointer to a list to iterate over
 * @return node * pointer to the first node of the list
 * @warning Can return NULL
 */
node * corgasm_listlib_begin(list * self);

/**
 * @brief Used for iterating over the list
 * @param[in] list * pointer to a list to iterate over
 * @return node * pointer to the last node of the list
 * @warning Can return NULL
 */
node * corgasm_listlib_end(list * self);

/**
 * @brief Used for iterating over the list
 * @param[in] node * pointer to the current node
 * @return node * pointer to the next node of the list
 * @warning Can return NULL
 */
node * corgasm_listlib_next(node * current);

/**
 * @brief Maps a function throught all data in the list
 * @param[in] list * pointer to the list to iterate over
 * @return list * pointer to the mapped list
 * @warning can return NULL
 * @note does not allocates memory for a new list
 */
list * corgasm_listlib_map(list * self, void *(*map_function)(void *));

/**
 * @brief Checks if a list contains a data pointer
 * @param[in] list * pointer to a list to iterate over
 * @param[in] void * pointer to the data to search for
 * @return true if list contains data otherwise false
 */
bool corgasm_listlib_contains(list * self, void * data);

/**
 * @brief Find index of a data in the list
 * @param[in] list * pointer to a list to iterate over
 * @param[in] void * pointer to the data to search for
 * @return size_t index of the data in the list
 * @warning Can return UINT_MAX if data is not found or list is empty or NULL
 */
size_t corgasm_listlib_index_of(list * self, void * data);

/**
 * @brief Wrapped destructor for a list
 *        If you want to store a list inside a list you need a destructor for it
 *        listlib.destroy takes void(*)(void *) to destroy a data but itself if of
 *        type void(*)(list *) thats why we need a wrapper.
 * @param[in] void * pointer to a list to destroy
 * @warning Can call segfault if you give incorrect pointer to a function
 * @return void
 */
void corgasm_listlib_destroy_wrapped(void * self);

/**
 * @brief Sets data in the node wit a given pointer
 * @param[in] list * pointer to a destination list
 * @param[in] size_t index of the node to set data into
 * @param[in] void * pointer to a data to set
 * @return void * last data pointer
 */
void * corgasm_listlib_set(list * self, size_t index, void * data);

/**
 * @brief Clears list without deallocating memory of iteself
 * @param[in] list * source list to clear
 * @return void
 */
void corgasm_listlib_clear(list * self);

/**
 * @brief Extracts data from the given node
 * @param[in] node * node to extract from
 * @return void * data
 * @warning Can return NULL if node pointer is NULL or data is NULL
 */
void * corgasm_listlib_extract(node * n);

typedef struct corgasm_listlib_functions
{
    void    (*destroy_wrapped) (void * self);
    list *  (*destroy)         (list * self);
    void *  (*pop)             (list * self);
    node *  (*begin)           (list * self);
    node *  (*end)             (list * self);
    void    (*clear)           (list * self);
    node *  (*get)             (list * self, size_t index);
    bool    (*append)          (list * self, void * data);
    size_t  (*index_of)        (list * self, void * data);
    bool    (*contains)        (list * self, void * data);
    void *  (*set)             (list * self, size_t index, void * data);
    bool    (*insert)          (list * self, void * data, size_t index);
    list *  (*map)             (list * self, void *(*map_function)(void *));
    void    (*destroy_node)    (node *, void (*destructor)(void *));
    list *  (*new_list)        (void (*destructor)(void *));
    node *  (*next)            (node * current);
    node *  (*new_node)        (void * data);
    void *  (*extract)         (node * n);
} corgasm_listlib_functions;



static const corgasm_listlib_functions listlib =
    {
        .new_list        = corgasm_listlib_new_list,
        .new_node        = corgasm_listlib_new_node,
        .destroy_node    = corgasm_listlib_destroy_node,
        .destroy         = corgasm_listlib_destroy,
        .append          = corgasm_listlib_append,
        .pop             = corgasm_listlib_pop,
        .get             = corgasm_listlib_get,
        .insert          = corgasm_listlib_insert,
        .begin           = corgasm_listlib_begin,
        .end             = corgasm_listlib_end,
        .next            = corgasm_listlib_next,
        .map             = corgasm_listlib_map,
        .contains        = corgasm_listlib_contains,
        .index_of        = corgasm_listlib_index_of,
        .destroy_wrapped = corgasm_listlib_destroy_wrapped,
        .set             = corgasm_listlib_set,
        .clear           = corgasm_listlib_clear,
        .extract         = corgasm_listlib_extract,
};

#define foreach_condition(type, varname, iterable, condition)    node * varname##_current = listlib.begin(iterable);   for (type varname = listlib.extract(varname##_current); varname##_current != listlib.end(iterable) && condition; varname##_current  = listlib.next(varname##_current), varname = (type)(listlib.extract(varname##_current)))

#define foreach(type, varname, iterable)  foreach_condition(type, varname, iterable, true)


#define fornode_condition(varname, iterable, condition)  for (node *varname = listlib.begin(iterable); varname != listlib.end(iterable) && condition; varname = listlib.next(varname))

#define fornode(varname, iterable)   foreach_condition(varname, iterable, true)


#endif /* __CORGASM_LISTLIB_H__ */
