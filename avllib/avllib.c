#include "avllib.h"

avl_tree *corgasm_treelib_avllib_new_avl_tree(short (*comparator)(const void *first, const void *second),
                                              void (*destructor)(void *data))
{
    avl_tree *self = NULL;
    if (comparator)
    {
        self = (avl_tree *)memtestlib.malloc(manager, sizeof(avl_tree), file_information);
        if (self)
        {
            self->comparator = comparator;
            self->destructor = destructor;
            self->root = NULL;
        }
    }
    return self;
}

short corgasm_treelib_avllib_height(avl_node *node)
{
    short height = 0;
    if (node)
        height = node->height;
    return height;
}

avl_node * corgasm_treelib_avllib_new_node()
{
    avl_node * new_node = (avl_node *)memtestlib.malloc(manager, sizeof(avl_node), file_information);
    if (new_node)
    {
        new_node->left   = NULL;
        new_node->right  = NULL;
        new_node->data   = NULL;
        new_node->height = 1;

    }
    return new_node;
}

avl_node * corgasm_treelib_avllib_rotate_right(avl_node * node)
{
    avl_node * x   = node->left;
    avl_node * tmp = x->right;

    x->right   = node;
    node->left = tmp;

    short node_left_height  = avllib.height(node->left);
    short node_right_height = avllib.height(node->right);

    short x_left_height  = avllib.height(x->left);
    short x_right_height = avllib.height(x->right);

    node->height = max(node_left_height, node_right_height) + 1;
    x->height    = max(x_left_height,     x_right_height)   + 1;
    return x;
}

avl_node * corgasm_treelib_avllib_rotate_left(avl_node * node)
{
    avl_node * y   = node->right;
    avl_node * tmp = y->left;

    y->left = node;
    node->right = tmp;

    short node_left_height  = avllib.height(node->left);
    short node_right_height = avllib.height(node->right);

    short y_left_height  = avllib.height(y->left);
    short y_right_height = avllib.height(y->right);

    node->height = max(node_left_height, node_right_height) + 1;
    y->height    = max(y_left_height,     y_right_height)   + 1;

    return y;
}

avl_node * corgasm_treelib_avllib_destroy_node(avl_node * node, void (*destructor)(void *data))
{
    if (node)
    {
        if (destructor && node->data)
        {
            if (destructor == free)
                memtestlib.free(manager, node->data, file_information);
            else
                destructor(node->data);
        }
        memtestlib.free(manager, node, file_information);
    }
    return NULL;
}

short corgasm_treelib_avllib_balance_factor(avl_node * node)
{
    short factor = 0;
    if (node)
        factor = avllib.height(node->left) - avllib.height(node->right);
    return factor;
}

avl_node * corgasm_treelib_avllib_insert_node(avl_node * node, void * data, short (*comparator)(const void * first, const void * second))
{
    if (!node)
    {
        avl_node * new_node = corgasm_treelib_avllib_new_node();
        if (new_node)
            new_node->data = data;
        return new_node;
    }

    short cmp_result = comparator(node->data, data);

    if (cmp_result > 0)
        node->left = corgasm_treelib_avllib_insert_node(node->left, data, comparator);
    else if (cmp_result < 0)
        node->right = corgasm_treelib_avllib_insert_node(node->right, data, comparator);
    else
        return node;



    short node_left_height  = avllib.height(node->left);
    short node_right_height = avllib.height(node->right);

    node->height = 1 + (max(node_left_height, node_right_height));

    short balance_factor = corgasm_treelib_avllib_balance_factor(node);


    if (balance_factor > 1 && comparator(data, node->left->data) > 0)
        return corgasm_treelib_avllib_rotate_right(node);

    if (balance_factor < -1 && comparator(data, node->right->data) < 0)
        return corgasm_treelib_avllib_rotate_left(node);

    if (balance_factor > 1 && comparator(data, node->left->data) > 0)
    {
        node->left = corgasm_treelib_avllib_rotate_left(node->left);
        return corgasm_treelib_avllib_rotate_right(node);
    }

    if (balance_factor < -1 && comparator(data, node->right->data) < 0)
    {
        node->right = corgasm_treelib_avllib_rotate_right(node->right);
        return corgasm_treelib_avllib_rotate_left(node);
    }

    return node;
}

bool corgasm_treelib_avllib_insert(avl_tree * self, void * data)
{
    bool was_inserted = false;
    if (self && data)
    {
        self->root = corgasm_treelib_avllib_insert_node(self->root, data, self->comparator);
        was_inserted = true;
    }
    return was_inserted;
}

void corgasm_treelib_avllib_reqursive_destroy(avl_node * node, void (*destructor) (void *data))
{
    if (node)
    {
        corgasm_treelib_avllib_reqursive_destroy(node->right, destructor);
        corgasm_treelib_avllib_reqursive_destroy(node->left, destructor);
        corgasm_treelib_avllib_destroy_node(node, destructor);
    }
}

void corgasm_treelib_avllib_destroy(avl_tree * self)
{
    if (self)
    {
        corgasm_treelib_avllib_reqursive_destroy(self->root, self->destructor);
        memtestlib.free(manager, self, file_information);
    }
}

avl_node  *corgasm_treelib_avllib_delete_node(avl_node * root, const void *data, short (*comparator)(const void *first, const void *second), void (*destructor)(void *data))
{
    if (!root)
        return root;

    short cmp_key_and_root_key = comparator(data, root->data);

    if (cmp_key_and_root_key < 0)
        root->left = corgasm_treelib_avllib_delete_node(root->left, data, comparator, destructor);

    else if (cmp_key_and_root_key > 0)
        root->right = corgasm_treelib_avllib_delete_node(root->right, data, comparator, destructor);

    else
    {
        if ((!root->left) || (!root->right))
        {
            avl_node * temp = root->left ? root->left : root->right;

            if (!temp)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            corgasm_treelib_avllib_destroy_node(temp, destructor);
        }
        else
        {
            avl_node * temp = corgasm_treelib_avllib_min_node(root->right);
            root->data = temp->data;
            root->right = corgasm_treelib_avllib_delete_node(root->right, temp->data, comparator, destructor);
        }
    }

    if (!root)
        return root;


    short left_height  = avllib.height(root->left);
    short right_height = avllib.height(root->right);

    root->height = 1 + (max(left_height, right_height));

    short balance_factor = corgasm_treelib_avllib_balance_factor(root);
    short left_balance_factor  = corgasm_treelib_avllib_balance_factor(root->left);
    short right_balance_factor = corgasm_treelib_avllib_balance_factor(root->right);

    if (balance_factor > 1 && left_balance_factor >= 0)
        return corgasm_treelib_avllib_rotate_right(root);

    if (balance_factor > 1 && left_balance_factor < 0)
    {
        root->left = corgasm_treelib_avllib_rotate_left(root->left);
        return corgasm_treelib_avllib_rotate_right(root);
    }

    if (balance_factor < -1 && right_balance_factor <= 0)
        return corgasm_treelib_avllib_rotate_left(root);

    if (balance_factor < -1 && right_balance_factor > 0)
    {
        root->right = corgasm_treelib_avllib_rotate_right(root->right);
        return corgasm_treelib_avllib_rotate_left(root);
    }

    return root;
}

void corgasm_treelib_avllib_delete(avl_tree * self, const void * data)
{
    if (self)
        self->root = corgasm_treelib_avllib_delete_node(self->root, data, self->comparator, self->destructor);
}

avl_node * corgasm_treelib_avllib_min_node(avl_node * node)
{
  avl_node * current = node;

  while (current->left != NULL)
    current = current->left;

  return current;
}

void printPreOrder(avl_node * root)
{
    if (root)
    {
        printf("%d ", *(int *)root->data);
        printPreOrder(root->left);
        printPreOrder(root->right);
    }
}

#ifdef LIB_BUILD_AVLLIB

short cmp(const void * a, const void * b)
{
    if (a && b)
        return *(int *)a - *(int *)b;
    return 0;
}

int main()
{
    manager = memtestlib.new_memanager("avllib");
    avl_tree * data = avllib.new_avl_tree(cmp, free);
    for (int i = 0; i < 1000; i++)
    {
        void * a = memtestlib.malloc(manager, sizeof(int), file_information);
        *(int*)a = 1000 - i;
        avllib.insert(data, a);
    }
    //printPreOrder(data->root);
    printf("Start of destruction!\n");
    avllib.destroy(data);
    memtestlib.short_report(manager);
    memtestlib.destroy_memanager(manager);
    printf("Sucess!\n");
    return 0;
}

#endif
