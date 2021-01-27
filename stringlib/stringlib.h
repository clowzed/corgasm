#ifndef __CORGASM_STRINGLIB_H__
#define __CORGASM_STRINGLIB_H__

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

char * strdup(const char *);

typedef struct string
{
	size_t 	length;
	char *  data;
}string;

/**
 * @brief Deallocates memory of a string
 * @param[in] string * self pointer to string
 * @return string *
 * @warning Always return NULL
 */
void corgasm_stringlib_destroy(string * self);

/**
 * @brief Clears string without deallocating memory
 * @param[in] string * self pointer to string
 * @return void
 */
void corgasm_stringlib_clear(string * self);

/**
 * @brief Wrapped destructor for listlib
 * @param[in] void * self pointer to string
 * @warning Use only if you are sure that pointer is correct
 */
void corgasm_stringlib_destroy_wrapped(void * self);

/**
 * @brief Makes string uppercase
 * @param[in] string * self pointer to string
 * @return string * same string
 */
string * corgasm_stringlib_to_upper(string * self);

/**
 * @brief Makes string lowercase
 * @param[in] string * self pointer to string
 * @return string * same string
 */
string * corgasm_stringlib_to_lower(string * self);

/**
 * @brief Reverse string
 * @param[in] string * self pointer to string
 * @return string * same string
 */
string * corgasm_stringlib_reverse(string * self);

/**
 * @brief Allocates memory for a new string and sets it with the given string
 * @param[in] const char * str String will store strdup of it
 * @return string * pointer to a new string
 * @warning Can return NULL
 * @note With str == NULL it calls stringlib.
 */
string * corgasm_stringlib_new_string_from(const char * str);

/**
 * @brief Allocates memory for an empty string
 * @return string * pointer to a new string
 * @warning Can return NULL
 */
string * corgasm_stringlib_new_string();

/**
 * @brief Concatinates two strings
 * 		This does not allocates memory for a new string
 * 		Just adds chars from second string to a first
 * 		Thats why there is no need in setting new variable for it
 * 		You can just call it without handling a result
 * @param[in] string * self pointer to string
 * @param[in] string * second string for concatination
 * @return string * pointer to a self
 */
string * corgasm_stringlib_concat(string * self, string * second);

/**
 * @brief Adds a char to a string
 * @param[in] string * self pointer to string
 * @param[in] char char to add
 * @return string * pointer to a self
 * @note No memory allocation for new string
 */
string * corgasm_stringlib_add_char(string * self, char c);

/**
 * @brief Checks if a string starts with another string
 * @param[in] string * self pointer to string
 * @param[in] string * prefix
 * @return true if the string starts with another string false otherwise
 */
bool corgasm_stringlib_starts_with_string(string * self, string * prefix);

/**
 * @brief Checks if a string starts with another string
 * @param[in] string * self pointer to string
 * @param[in] const char * prefix
 * @return true if the string starts with another string false otherwise
 * @note stringlib.strats_with_string calls this inside
 */
bool corgasm_stringlib_starts_with(string * self, const char * prefix);

/**
 * @brief Checks if a string is all upper
 * @param[in] string * self pointer to string
 * @return true if the string is all upper
 * @note calls stringlib.all_accepted_by inside
 */
bool corgasm_stringlib_isupper(string * self);

/**
 * @brief Checks if a string is all lower
 * @param[in] string * self pointer to string
 * @return true if the string is all lower
 * @note calls stringlib.all_accepted_by inside
 */
bool corgasm_stringlib_islower(string * self);

/**
 * @brief Checks if a string is digit
 * @param[in] string * self pointer to string
 * @return true if the string is digit
 * @note calls stringlib.all_accepted_by inside
 */
bool corgasm_stringlib_isdigit(string * self);

/**
 * @brief Checks if a string is alpha
 * @param[in] string * self pointer to string
 * @return true if the string is alpha
 * @note calls stringlib.all_accepted_by inside
 */
bool corgasm_stringlib_isalpha(string * self);

/**
 * @brief Checks if a string is alnum
 * @param[in] string * self pointer to string
 * @return true if the string is alnum
 * @note calls stringlib.all_accepted_by inside
 */
bool corgasm_stringlib_isalnum(string * self);

/**
 * @brief Checks if strings are same
 * @param[in] string * self pointer to string
 * @param[in] string * other pointer to string
 * @return true if the strings are equal
 */
bool corgasm_stringlib_are_same(string * self, string * second);

/**
 * @brief Checks if all chars in a string are accepted by a function
 * @param[in] string * self pointer to string
 * @param[in] f pointer to a function
 * @return bool
 */
bool corgasm_stringlib_all_accepted_by(string * self, int (*f)(int));

/**
 * @brief Get length of a string
 * @param[in] string * self pointer to string
 * @return size_t string length
 * @note operation is O(1) because of storing lenght in a struct
 */
size_t corgasm_stringlib_length(string * self);

/**
 * @brief Compares two strings
 * @param[in] string * self pointer to string
 * @param[in] string * other pointer to string
 * @return int
 * @note Calls strcmp inside
 */
int corgasm_stringlib_compare(string * self, string * other);

/**
 * @brief Get pointer where chars are stored
 * @param[in] string * self pointer to string
 * @return char * pointer where chars are stored
 * @warning Can return NULL
 */
char * corgasm_stringlib_extract(string *);


typedef struct corgasm_stringlib_functions
{
	string * (*new_string)		 ();
	string * (*new_string_from)	 (const char * str);
	string * (*concat)			 (string * self, string *     second);
	string * (*add_char)         (string * self, char c);
	string * (*to_upper)		 (string * self);
	string * (*to_lower)		 (string * self);
	string * (*reverse)			 (string * self);
	size_t   (*length)			 (string * self);
	int 	 (*compare)			 (string * self, string *     other);
	char * 	 (*extract)			 (string * self);
	bool     (*startswith)	     (string * self, const char * prefix);
	bool     (*startswith_string)(string * self, string *     prefix);
	bool     (*all_accepted_by)  (string * self, int (*f)(int)      );
	bool 	 (*isupper)			 (string * self);
	bool 	 (*islower)			 (string * self);
	bool 	 (*isdigit)			 (string * self);
	bool 	 (*isalpha)			 (string * self);
	bool 	 (*isalnum)			 (string * self);
	bool 	 (*are_same)		 (string * self, string * second);
	void 	 (*destroy_wrapped)  (void *   self);
	void 	 (*destroy)			 (string * self);
	void	 (*clear)			 (string * self);
}corgasm_stringlib_functions;


static const corgasm_stringlib_functions stringlib = {
		.add_char          = corgasm_stringlib_add_char,
		.all_accepted_by   = corgasm_stringlib_all_accepted_by,
		.are_same          = corgasm_stringlib_are_same,
		.clear             = corgasm_stringlib_clear,
		.compare           = corgasm_stringlib_compare,
		.concat            = corgasm_stringlib_concat,
		.destroy           = corgasm_stringlib_destroy,
		.destroy_wrapped   = corgasm_stringlib_destroy_wrapped,
		.extract           = corgasm_stringlib_extract,
		.isalnum           = corgasm_stringlib_isalnum,
		.isalpha           = corgasm_stringlib_isalpha,
		.isdigit           = corgasm_stringlib_isdigit,
		.islower           = corgasm_stringlib_islower,
		.isupper           = corgasm_stringlib_isupper,
		.length            = corgasm_stringlib_length,
		.new_string        = corgasm_stringlib_new_string,
		.new_string_from   = corgasm_stringlib_new_string_from,
		.reverse           = corgasm_stringlib_reverse,
		.startswith        = corgasm_stringlib_starts_with,
		.startswith_string = corgasm_stringlib_starts_with_string,
		.to_lower          = corgasm_stringlib_to_lower,
		.to_upper          = corgasm_stringlib_to_upper,
};

#endif /* __CORGASM_STRINGLIB_H__ */
