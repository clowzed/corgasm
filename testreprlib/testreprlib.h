#ifndef __CORGASM_TESTREPRLIB_H__
#define __CORGASM_TESTREPRLIB_H__

#include "../stringlib/stringlib.h"
#include "../listlib/listlib.h"

#define func_name(func) #func

typedef bool (*test_function_pointer)();


typedef struct corgasm_testreprlib_test_function
{
    test_function_pointer func;
    string *              name;
}test_function;

typedef struct corgasm_testreprlib_test_suit
{
    FILE   * outstream;
    string * name;
    list   * test_functions;
}test_suit;

typedef struct corgasm_testreprlib_functions
{
    test_suit * (*new_suit)(const char * name);
    bool        (*__register_test__)(test_suit * self, test_function_pointer func, const char * func_name);
    bool        (*run)(test_suit * self);
    test_suit * (*destroy)(test_suit * self);
}corgasm_testreprlib_functions;


test_function * corgasm_testreprlib_new_test_function(test_function_pointer func, const char * func_name);
test_function * corgasm_testreprlib_destroy_test_function(test_function * func);
void            corgasm_testreprlib_destroy_test_function_wrapped(void * func);

test_suit * corgasm_testreprlib_new_suit(const char * name);
bool corgasm_testreprlib_register_test_(test_suit * self, test_function_pointer func, const char * func_name);
bool corgasm_testreprlib_run(test_suit * self);
test_suit * corgasm_testreprlib_destroy_suit(test_suit * self);


#define register_test(suit, func) __register_test__(suit, func, func_name(func))

static const corgasm_testreprlib_functions testreprlib = {
    .new_suit          = corgasm_testreprlib_new_suit,
    .__register_test__ = corgasm_testreprlib_register_test_,
    .run               = corgasm_testreprlib_run,
    .destroy           = corgasm_testreprlib_destroy_suit,
};

#endif // __CORGASM_TESTREPRLIB_H__
