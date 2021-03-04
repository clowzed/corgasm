#include "./dictlib.h"

size_t corgasm_dictlib_get_prime_nearby(size_t size)
{
    for (size_t i = 0; i < 500; i++)
    {
        if (primes[i] >= size)
            return primes[i];
    }
    return 0;
}

size_t hash(const char * key, size_t size)
{
    size_t hashval = 0;
    for (hashval = 0; *key != '\0'; key++)
      hashval = *key + 31 * hashval;
    return hashval % size;
}

dict * corgasm_dictlib_new_dict(size_t size)
{
    dict * new_dict = NULL;
    if (size <= 3571)
    {
        new_dict                = malloc(sizeof(dict));
        new_dict->hash_function = hash;
        new_dict->size          = corgasm_dictlib_get_prime_nearby(size);
        new_dict->data          = arraylib.new_array(NULL);
        new_dict->keys          = listlib.new_list(stringlib.destroy_wrapped);
        for (size_t i = 0; i < new_dict->size; i++)
            arraylib.append(new_dict->data, NULL);
        if (!new_dict->data || !new_dict->size || !new_dict->keys)
            new_dict = dictlib.destroy(new_dict, NULL);
    }
    return new_dict;
}

dict * corgasm_dictlib_destroy(dict * self, void (*destructor)(void *))
{
    if (self)
    {
        if (destructor)
            dictlib.destroy_data(self, destructor);
        arraylib.destroy(self->data);
        listlib.destroy(self->keys);
        free(self);
    }
    return NULL;
}

void corgasm_dictlib_destroy_data(dict * self, void (*destructor)(void *))
{
    if (self && destructor)
    {
        foreach(string *, key, self->keys)
        {
            void * inner_data = dictlib.get(self, stringlib.extract(key));
            destructor(inner_data);
        }
    }
}

void * corgasm_dictlib_set(dict * self, const char * key, void * value)
{
    void * previous_data = NULL;
    if (self && key && value)
    {
        size_t index = self->hash_function(key, self->size);
        previous_data = arraylib.get(self->data, index);
        arraylib.set(self->data, index, value);
    }
    return previous_data;
}

void * corgasm_dictlib_get(dict * self, const char * key)
{
    void * value = NULL;
    if (self && key)
    {
        size_t index = self->hash_function(key, self->size);
        value = arraylib.get(self->data, index);
    }
    return value;
}


#ifdef LIB_BUILD_DICTLIB
int main()
{
    char   name[]       = "Kate";
    char   surname[]    = "Dilinger";
    dict * persons_info = dictlib.new_dict(3571);

    dictlib.set(persons_info, "name",    name);
    dictlib.set(persons_info, "surname", surname);

    printf("%s\n", dictlib.get(persons_info, "name"));
    printf("%s\n", dictlib.get(persons_info, "surname"));

    dictlib.destroy(persons_info, NULL);
    return 0;
}
#endif
