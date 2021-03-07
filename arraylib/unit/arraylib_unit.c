#include "arraylib_unit.h"

bool test_update_capacity()
{
    bool passed = false;
    array * new_array = arraylib.new_array(NULL);
    size_t previous_len = new_array->size;

    corgasm_arraylib_update_capacity(new_array);

    size_t new_len = new_array->size;
    passed = new_len - previous_len == 1 &&
             new_array->data != NULL;

    arraylib.destroy(new_array);
    return passed;
}

bool test_new_array()
{
    array * new_array = arraylib.new_array(NULL);
    bool passed = new_array != NULL;
    arraylib.destroy(new_array);
    return passed;
}

bool test_destroy()
{
    array * new_array = arraylib.new_array(NULL);
    new_array = arraylib.destroy(new_array);
    return new_array == NULL;
}

bool test_append()
{
    int a = 10;
    array * new_array = arraylib.new_array(NULL);
    bool was_appended = arraylib.append(new_array, (void *)&a);
    void * first = arraylib.get(new_array, 0);
    was_appended = first == (void *)&a;
    arraylib.destroy(new_array);
    return was_appended;
}

bool test_set()
{
    int a = 3;
    int b = 4;
    array * new_array = arraylib.new_array(NULL);
    arraylib.append(new_array, (void *)&a);
    arraylib.set(new_array, 0, (void *)&b);
    bool passed = arraylib.get(new_array, 0) == (void *)&b;
    arraylib.destroy(new_array);
    return passed;
}

bool test_get()
{
    array * new_array = arraylib.new_array(NULL);
    int a = 0;
    arraylib.append(new_array, (void *)&a);
    bool passed = arraylib.get(new_array, 0) == (void*)&a;
    arraylib.destroy(new_array);
    return passed;
}

#ifdef LIB_TEST_ARRAYLIB

int main()
{
    test_suit * suit = testreprlib.new_suit("arraylib unit tests");
                       testreprlib.register_test(suit, test_update_capacity);
                       testreprlib.register_test(suit, test_new_array);
                       testreprlib.register_test(suit, test_destroy);
                       testreprlib.register_test(suit, test_append);
                       testreprlib.register_test(suit, test_set);
                       testreprlib.register_test(suit, test_get);
    bool were_passed = testreprlib.run(suit);
                       testreprlib.destroy(suit);
    return testreprlib.return_code(were_passed);
}

#endif