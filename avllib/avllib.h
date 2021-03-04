#ifndef __CORGASM_TREELIB_AVLLIB_H__
#define __CORGASM_TREELIB_AVLLIB_H__

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "../memtestlib/memtestlib.h"

typedef struct avl_node
{
    struct avl_node * right;
    struct avl_node * left;
    void     * data;
    short      height;
}avl_node;

typedef struct corgasm_treelib_avllib
{
    avl_node * root;
    short (*comparator)(const void * first, const void * second);
    void  (*destructor)(void * data);
}avl_tree;

#ifndef min
    #define min(x, y) x > y ? y : x
#endif

#ifndef max
    #define max(x, y) x < y ? y : x
#endif

typedef struct corgasm_treelib_avllib_functions
{
    avl_tree * (*new_avl_tree)(short (*comparator)(const void * first, const void * second),
                               void  (*destructor)(void * data));
    void       (*destroy)(avl_tree * self);
    bool       (*insert) (avl_tree * self, void * data);
    short      (*height) (avl_node * node);
    void       (*delete_) (avl_tree * self, const void * data);
}corgasm_treelib_avllib_functions;

avl_tree * corgasm_treelib_avllib_new_avl_tree  (short (*comparator)(const void *first, const void *second), void (*destructor)(void *data));
avl_node * corgasm_treelib_avllib_new_node      ();
avl_node * corgasm_treelib_avllib_rotate_right  (avl_node * node);
avl_node * corgasm_treelib_avllib_rotate_left   (avl_node * node);
avl_node * corgasm_treelib_avllib_destroy_node  (avl_node * node, void (*destructor)(void *data));
avl_node * corgasm_treelib_avllib_insert_node   (avl_node * node, void * data, short (*comparator)(const void * first, const void * second));
avl_node * corgasm_treelib_avllib_min_node      (avl_node * node);

short      corgasm_treelib_avllib_balance_factor(avl_node * node);
short      corgasm_treelib_avllib_height        (avl_node * node);
bool       corgasm_treelib_avllib_insert(avl_tree * self, void * data);
void       corgasm_treelib_avllib_delete        (avl_tree * self, const void * data);
void       corgasm_treelib_avllib_destroy       (avl_tree * self);



static memanager * manager = NULL;

static const corgasm_treelib_avllib_functions avllib = {
    .new_avl_tree = corgasm_treelib_avllib_new_avl_tree,
    .destroy      = corgasm_treelib_avllib_destroy,
    .insert       = corgasm_treelib_avllib_insert,
    .height       = corgasm_treelib_avllib_height,
    .delete_       = corgasm_treelib_avllib_delete,
};

#endif
