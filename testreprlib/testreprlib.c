#include "testreprlib.h"



test_function * corgasm_testreprlib_new_test_function(test_function_pointer func, const char * func_name)
{
    test_function * new_test_function = NULL;
    if (func && func_name)
    {
        new_test_function = malloc(sizeof(test_function));
        if (new_test_function)
        {
            new_test_function->func = func;
            new_test_function->name = stringlib.new_string_from(func_name);
            if (!new_test_function->name)
                new_test_function = corgasm_testreprlib_destroy_test_function(new_test_function);
        }
    }
    return new_test_function;
}

test_function * corgasm_testreprlib_destroy_test_function(test_function * func)
{
    if (func)
    {
        stringlib.destroy(func->name);
        free(func);
    }
    return NULL;
}

test_suit * corgasm_testreprlib_new_suit(const char * name)
{
    test_suit * new_suit = NULL;
    if (name)
    {
        new_suit = malloc(sizeof(test_suit));
        if (new_suit)
        {
            new_suit->name = stringlib.new_string_from(name);
            new_suit->test_functions = listlib.new_list(corgasm_testreprlib_destroy_test_function_wrapped);
            new_suit->outstream = stdout;
            if (!new_suit->name || !new_suit->test_functions)
                new_suit = testreprlib.destroy(new_suit);
        }
    }
    return new_suit;
}

bool corgasm_testreprlib_register_test_(test_suit * self, test_function_pointer func, const char * func_name)
{
    bool was_registered = false;
    if (self && func && func_name)
    {
        test_function * new_test_function = corgasm_testreprlib_new_test_function(func, func_name);
        if (new_test_function)
        {
            was_registered = listlib.append(self->test_functions, (void *)new_test_function);
            if (!was_registered)
                new_test_function = corgasm_testreprlib_destroy_test_function(new_test_function);
        }
    }
    return was_registered;
}

bool corgasm_testreprlib_run(test_suit * self)
{
    fprintf(self->outstream, "Suit name: %s\n", stringlib.extract(self->name));
    foreach(test_function *, test, self->test_functions)
    {
        printf("run test %s: %d\n", stringlib.extract(test->name), test->func());
    }
    return true;
}

test_suit * corgasm_testreprlib_destroy_suit(test_suit * self)
{
    if (self)
    {
        listlib.destroy(self->test_functions);
        stringlib.destroy(self->name);
        free(self);
    }
    return NULL;
}

void corgasm_testreprlib_destroy_test_function_wrapped(void * func)
{
    if (func)
        corgasm_testreprlib_destroy_test_function((test_function *)func);
}


bool test_one()
{
    return true;
}

#ifdef LIB_BUILD_TESTREPRLIB


int main()
{
    test_suit * suit = testreprlib.new_suit("tests");
    testreprlib.register_test(suit, test_one);
    testreprlib.run(suit);
    suit = testreprlib.destroy(suit);
    printf("Correct");
    return 0;
}

#endif