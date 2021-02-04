#ifndef __CORGASM_ARGPARSELIB_H__
#define __CORGASM_ARGPARSELIB_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../listlib/listlib.h"
#include "../stringlib/stringlib.h"

bool startswith(const char * pre, const char * str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

#define FLAG 0
#define ARGUMENT 1
#define REQUIRED true
#define NOT_REQUIRED false
#define NO_LONG_NAME NULL
#define NO_DESCRIPTION NULL


#define is_correct_short_name(short_name)   (short_name && startswith("-", short_name))
#define is_correct_long_name(long_name)     ((long_name == NO_LONG_NAME) || (long_name && startswith("--", long_name)))
#define is_correct_description(description) true
#define is_flag(type)                       (type == FLAG)
#define is_argument(type)                   (type == ARGUMENT)
#define is_correct_type(type)               (is_flag(type) || is_argument(type))
#define string_is_argument(str)             (stringlib.startswith(str, "-"))

// * This helps to make code more clear
// * Example:
// * argument * a = NULL;
// * argument * b = NULL;
// * argparselib.destroy_arguments(a, b); This expands to : argparselib.destroy(a, b, NULL);
#define destroy_arguments(...) destroy(__VA_ARGS__, NULL)
#define parse_arguments(...) parse(__VA_ARGS__, NULL)
#define show_arguments(...) show(__VA_ARGS__, NULL)


typedef struct corgasm_argparselib_argument
{
    string * short_name;
    string * long_name;
    string * description;
    list   * values;
    bool     is_flag;
    bool     is_argument;
    bool     is_enabled;
    bool     is_required;
}argument;


/**
 * @brief Allocates memory for a new argument
 * @param[in] short type set type of argumnet (FLAG or ARGUMENT)
 * @param[in] bool  is_required sets if argument is required
 * @param[in] const char * short_name Set short name of argument [startswith '-']
 * @param[in] const char * long_name Set long name of argument   [startswith '--']
 * @param[in] const char * description Set description of argument
 * @note Description and long_name can be NULL
 * @warning Function can return NULL
 * @return argument * Pointer to an new allocated argument
 */
argument * corgasm_argparselib_new_argument(short   type,
                                            bool    is_required,
                                            const char  * short_name,
                                            const char  * long_name,
                                            const char  * description);

/**
 * @brief Deallocates memory of an argument
 * @param[in] argument * self pointer to an argument
 * @return argument *
 * @note Always return NULL
 */
argument * corgasm_argparselib_destroy_argument(argument * self);

/**
 * @brief Wrapped destructor for argument
 * @param[in] void * self pointer to an argument
 * @return void
 */
void corgasm_argparselib_destroy_argument_wrapped(void * self);

/**
 * @brief Deallocates memory of many arguments
 * @param[in] argument * first pointer to a first argument
 * @return void
 * @note Arguments must end with a NULL pointer
 */
void corgasm_argparselib_destroy(argument * first, ...);

/**
 * @brief Parses arguments from argv
 * @param[in] int argc number of arguments
 * @param[in] char ** argv pointer to arguments
 * @param[in] ... any number of argument *
 * @return bool true if arguments were parsed correctly
 */
bool corgasm_argparselib_parse(int argc, char ** argv, ...);

/**
 * @brief Get program name from argv
 * @param[in] char ** argv pointer to arguments
 * @return string *
 * @warning Can return NULL
 */
string * corgasm_argparselib_get_program_name(char ** argv);

/**
 * @brief Parses arguments from argv
 * @param[in] list * cmd_args List with strings
 * @param[in] list * arguments list of arguments
 * @return bool
 * @warning No check for incorrect params. That is why it is static!
 */
static bool corgasm_argparselib_parse_list_arguments(list * cmd_args, list * arguments);

/**
 * @brief Finds given argument in list
 * @param[in] list * arguments pointer to arguments list
 * @param[in] string * arg a to search for
 * @return argument *
 * @warning Can return NULL
 */
static argument * corgasm_argparselib_find_argument(list * arguments, string * arg);

/**
 * @brief checks if all arguments are correct
 * @param[in] list * arguments pointer to arguments list
 * @return true if all arguments are correct
 */
static bool corgasm_argparselib_check_if_args_are_correct(list * arguments);

/**
 * @brief Checks if argumentwas setted by user
 * @param[in] argument * pointer to argument
 * @return true if argument was setted
 */
bool corgasm_argparselib_setted(argument * self);

/**
 * @brief Get list of values given by the user
 * @param[in] argument * pointer to argument
 * @return list * pointer to list of values
 * @warning Can return NULL if argument is FLAG or NULL
 */
list * corgasm_argparselib_get_values(argument * self);


/**
 * @brief Shows all arguments of the program
 * @param[in] argument * pointer to the first argument
 * @return void
 */
void corgasm_argparselib_show(argument * first, ...);

/**
 * @brief Shows argument in stdin
 * @param[in] argument * pointer to the argument
 * @return void
 */
void corgasm_argparselib_show_argument(argument * self);


typedef struct corgasm_argparselib_functions
{
    argument * (*new_argument)             (short         type,
                                            bool          is_required,
                                            const char  * short_name,
                                            const char  * long_name,
                                            const char  * description);

    argument * (*destroy_argument)         (argument * self);
    void       (*destroy)                  (argument * first, ...);

    bool       (*parse)                    (int     argc,
                                            char ** argv,
                                            ...);

    void       (*destroy_argument_wrapped) (void * self);
    string *   (*get_program_name)         (char ** argv);
    bool       (*setted)                   (argument * self);
    list *     (*get_values)               (argument * self);
    void       (*show)                     (argument * first, ...);

} corgasm_argparselib_functions;


static const corgasm_argparselib_functions argparselib = {
    .new_argument             = corgasm_argparselib_new_argument,
    .destroy_argument         = corgasm_argparselib_destroy_argument,
    .destroy                  = corgasm_argparselib_destroy,
    .parse                    = corgasm_argparselib_parse,
    .destroy_argument_wrapped = corgasm_argparselib_destroy_argument_wrapped,
    .get_program_name         = corgasm_argparselib_get_program_name,
    .setted                   = corgasm_argparselib_setted,
    .get_values               = corgasm_argparselib_get_values,
    .show                     = corgasm_argparselib_show
};

#endif /* __CORGASM_ARGPARSELIB_H_ */
