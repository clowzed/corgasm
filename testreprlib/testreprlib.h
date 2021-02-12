#ifndef __CORGASM_TESTREPRLIB_H__
#define __CORGASM_TESTREPRLIB_H__

#include "../stringlib/stringlib.h"
#include "../listlib/listlib.h"
#include "../tablelib/tablelib.h"

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
    test_suit * (*new_suit)         (const char * name);
    bool        (*__register_test__)(test_suit * self, test_function_pointer func, const char * func_name);
    bool        (*run)              (test_suit * self);
    test_suit * (*destroy)          (test_suit * self);
    int         (*return_code)      (bool were_passed);
}corgasm_testreprlib_functions;


/**
 * @brief Allocates memory for a new test function
 * @param[in] test_function_pointer pointer to a test function
 * @param[in] const char * func_name name of the test function
 * @return test_function * pointer to the test function
 */
test_function * corgasm_testreprlib_new_test_function(test_function_pointer func, const char * func_name);


/**
 * @brief Deaalocates memory of a test function
 * @param[in] test_function * func pointer to a test function
 * @return test_function * pointer to the test function
 * @note Always returns NULL
 */
test_function * corgasm_testreprlib_destroy_test_function(test_function * func);


/**
 * @brief Wraaped destuctor of a test function
 * @param[in] void * func pointer to a test function
 * @return void
 * @warning Run only if you are sure that pointer is valid
 */
void corgasm_testreprlib_destroy_test_function_wrapped(void * func);


/**
 * @brief Allocates memory for a new test suit
 * @param[in] const char * name name of the test suit
 * @return test_suit * pointer to the new test suit
 */
test_suit * corgasm_testreprlib_new_suit(const char * name);


/**
 * @brief Registers new test function
 * @param[in] test_suit * self pointer to a test suit
 * @param[in] test_function_pointer func your test function
 * @param[in] const char * name name of the test func
 * @return bool if operation was successful
 */
bool corgasm_testreprlib_register_test_(test_suit * self, test_function_pointer func, const char * func_name);

/**
 * @brief Runc all tests and shows the results
 * @param[in] test_suit * self pointer to the test suit
 * @return bool if all tests were passed
 */
bool corgasm_testreprlib_run(test_suit * self);


/**
 * @brief Deallocates memory of a test suit
 * @param[in] test_suit * self pointer to the test suit
 * @return test_suit *
 * @note Always returns NULL
 */
test_suit * corgasm_testreprlib_destroy_suit(test_suit * self);


/**
 * @brief Generates return code for main
 * @param[in] bool were_passed result of the tests
 * @return int return code
 */
int corgasm_testreprlib_return_code(bool were_passed);

#define register_test(suit, func) __register_test__(suit, func, func_name(func))

static const corgasm_testreprlib_functions testreprlib = {
    .new_suit          = corgasm_testreprlib_new_suit,
    .__register_test__ = corgasm_testreprlib_register_test_,
    .run               = corgasm_testreprlib_run,
    .destroy           = corgasm_testreprlib_destroy_suit,
    .return_code       = corgasm_testreprlib_return_code
};

#endif // __CORGASM_TESTREPRLIB_H__
