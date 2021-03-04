#ifndef __CORGASM_CONFIGLIB_H__
#define __CORGASM_CONFIGLIB_H__

#include "../listlib/listlib.h"
#include "../stringlib/stringlib.h"
#include "../dictlib/dictlib.h"

#define splitter '='
#define uu __attribute__((unused))

#define alphabet_contains(c, state) (strchr(state##_alphabet, c) != NULL)


#define find_left_bracket                0
#define find_left_bracket_alphabet       " \t\n["



#define reading_section_name 1
#define reading_section_name_alphabet "[qwertyuiopasdfghjklzxcvbnm_- ]1234567890QWERTYUIOPASDFGHJKLZXCVBNM\n"

#define find_key_name        2
#define find_key_name_alphabet reading_section_name_alphabet ":="

#define find_end_of_value    3
#define find_end_of_value_alphabet find_key_name_alphabet "\n"

#define error_occured        4

#define while_reading(char_name, stream, condition) char char_name = '\0'; while (condition && ((char_name = fgetc(stream)) != EOF))


typedef struct corgasm_configlib_configparser
{
    list   * sections;
} configparser;


typedef struct corgasm_configlib_section
{
    string * name;
    dict   * data;
}section;


typedef struct corgasm_configlib_functions
{
    section *      (*new_section)(const char * name);
    section *      (*destroy_section)(section * self);
    configparser * (*new_configparser)(const char * filename);
    configparser * (*destroy)(configparser * self);
    string       * (*get)(configparser * self, const char * section_name, const char * key);
    void           (*represent)(configparser * self);
} corgasm_configlib_functions;



section *      corgasm_configlib_new_section            (const char * name);
section *      corgasm_configlib_destroy_section        (section * self);
configparser * corgasm_configlib_new_configparser       (const char * filename);
configparser * corgasm_configlib_destroy                (configparser * self);
string *       corgasm_configlib_get                    (configparser * self, const char * section_name, const char * key);
void           corgasm_configlib_destroy_section_wrapped(void * self);
void           corgasm_configlib_represent              (configparser * self);


static const corgasm_configlib_functions configlib = {
    .new_section      = corgasm_configlib_new_section,
    .destroy_section  = corgasm_configlib_destroy_section,
    .new_configparser = corgasm_configlib_new_configparser,
    .destroy          = corgasm_configlib_destroy,
    .get              = corgasm_configlib_get,
    .represent        = corgasm_configlib_represent,
};

#endif /* __CORGASM_CONFIGLIB_H__ */
