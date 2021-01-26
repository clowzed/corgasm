#ifndef __CORGASM_STRINGLIB_H__
#define __CORGASM_STRINGLIB_H__

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

char * strdup(const char *);
char * strrev (char *);

typedef struct string
{
	size_t 	length;
	char *  data;
}string;


char *   corgasm_stringlib_extract           (string *);
bool     corgasm_stringlib_isupper           (string *);
bool     corgasm_stringlib_isalpha           (string *);
bool     corgasm_stringlib_isalnum           (string *);
bool     corgasm_stringlib_isdigit           (string *);
bool     corgasm_stringlib_islower           (string *);
void     corgasm_stringlib_clear             (string *);
string * corgasm_stringlib_concat            (string *, string *);
string * corgasm_stringlib_new_string        ();
void     corgasm_stringlib_destroy           (string *str);
size_t   corgasm_stringlib_length            (string *);
int      corgasm_stringlib_compare           (string *, string *);
bool     corgasm_stringlib_are_same          (string *, string *);
string * corgasm_stringlib_to_lower          (string *);
string * corgasm_stringlib_to_upper          (string *);
string * corgasm_stringlib_reverse           (string *);
string * corgasm_stringlib_new_string_from   (const char *);
bool     corgasm_stringlib_starts_with       (string *, const char *);
bool     corgasm_stringlib_starts_with_string(string *, string *);
bool     corgasm_stringlib_all_accepted_by   (string * , int (*)(int));
string * corgasm_stringlib_add_char          (string * , char );
void 	corgasm_stringlib_destroy_wrapped    (void *);


typedef struct corgasm_stringlib_functions
{
	void				(*clear)			(string *);
	string *			(*new_string)		();
	string *			(*concat)			(string *, string *);
	void 				(*destroy)			(string *);
	size_t              (*length)			(string *);
	int 				(*compare)			(string *, string *);
	bool 				(*are_same)			(string *, string *);
	string *			(*to_lower)			(string *);
	string *			(*to_upper)			(string *);
	string *			(*reverse)			(string *);
	string *			(*new_string_from)	(const char *);
	bool 				(*isupper)			(string *);
	bool 				(*islower)			(string *);
	bool 				(*isdigit)			(string *);
	bool 				(*isalnum)			(string *);
	bool 				(*isalpha)			(string *);
	char * 				(*extract)			(string *);
	bool                (*startswith)	    (string *, const char *);
	bool                (*startswith_string)(string *, string *);
	bool     			(*all_accepted_by)  (string * , int (*)(int));
	string *            (*add_char)         (string * , char);
	void 	            (*destroy_wrapped)  (void *);



}corgasm_stringlib_functions;


static const corgasm_stringlib_functions stringlib = {
									.clear             = corgasm_stringlib_clear,
									.new_string        = corgasm_stringlib_new_string,
									.concat            = corgasm_stringlib_concat,
									.destroy           = corgasm_stringlib_destroy,
									.length            = corgasm_stringlib_length,
									.compare           = corgasm_stringlib_compare,
									.are_same          = corgasm_stringlib_are_same,
									.to_lower          = corgasm_stringlib_to_lower,
									.to_upper          = corgasm_stringlib_to_upper,
									.reverse           = corgasm_stringlib_reverse,
									.new_string_from   = corgasm_stringlib_new_string_from,
									.isupper           = corgasm_stringlib_isupper,
									.islower           = corgasm_stringlib_islower,
									.isdigit           = corgasm_stringlib_isdigit,
									.isalpha           = corgasm_stringlib_isalpha,
									.isalnum           = corgasm_stringlib_isalnum,
									.extract           = corgasm_stringlib_extract,
									.startswith        = corgasm_stringlib_starts_with,
									.startswith_string = corgasm_stringlib_starts_with_string,
									.all_accepted_by   = corgasm_stringlib_all_accepted_by,
									.add_char          = corgasm_stringlib_add_char,
									.destroy_wrapped   = corgasm_stringlib_destroy_wrapped,
};

#endif /* __CORGASM_STRINGLIB_H__ */
