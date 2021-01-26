#include "stringlib.h"
#include <string.h>

char *   corgasm_stringlib_strrev(char *str)
{
	char *p1, *p2 = NULL;

	if (!str || !*str)
		return str;

	for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
	{
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	return str;
}

void     corgasm_stringlib_clear(string * str)
{
	if (str && str->data)
	{
		free(str->data);
		str->data = NULL;
		str->length = 0;
	}
}

void     corgasm_stringlib_destroy(string *str)
{
	if (str)
	{
		stringlib.clear(str);
		free(str);
		str = NULL;
	}
}

string * corgasm_stringlib_new_string()
{
	string *str = malloc(sizeof(string));
	if (str)
	{
		str->data = NULL;
		str->length = 0;
	}
	return str;
}

string * corgasm_stringlib_add_char(string * str, char c)
{
	char buffer[2] = {c};
	string * str2  = stringlib.new_string_from(buffer);
	         str   =  stringlib.concat(str, str2);
	stringlib.destroy(str2);
	return str;
}

string * corgasm_stringlib_concat(string *str1, string *str2)
{
	if (str1 && str2)
	{
		char *new_data_for_str1 = malloc((str1->length * str2->length + 1) * sizeof(char));
		if (new_data_for_str1)
		{
			if (str1->data)
			{
				strcpy(new_data_for_str1, str1->data);
				if (str2->data)
					strcat(new_data_for_str1, str2->data);
			}
			else
				if (str2->data)
					strcpy(new_data_for_str1, str2->data);
			str1->data = new_data_for_str1;
			str1->length = str1->length + str2->length;
		}
	}
	return str1;
}

size_t   corgasm_stringlib_length(string *str)
{
	return str ? str->length : 0;
}

int      corgasm_stringlib_compare(string *str1, string *str2)
{
	int result = -1;
	if (str1 && str2 && str1->data && str2->data)
		result = strcmp(str1->data, str2->data);
	return result;
}

bool     corgasm_stringlib_are_same(string *str1, string *str2)
{
	return !(stringlib.compare(str1, str2));
}

string * corgasm_stringlib_to_lower(string *str)
{
	if (str)
		for (size_t i = 0; i < str->length; i++)
			str->data[i] = (char)tolower(str->data[i]);
	return str;
}

string * corgasm_stringlib_to_upper(string *str)
{
	if (str)
		for (size_t i = 0; i < str->length; i++)
			str->data[i] = (char)toupper(str->data[i]);
	return str;
}

string * corgasm_stringlib_reverse(string *str)
{
	if (str && str->data)
		strrev(str->data);
	return str;
}

string * corgasm_stringlib_new_string_from(const char *str)
{
	string *new_str = stringlib.new_string();
	if (new_str)
	{
		char * data = strdup(str);
		if (data)
		{
			new_str->data = data;
			new_str->length = strlen(str);
		}
		else
			stringlib.destroy(new_str);
	}
	return new_str;
}

bool     corgasm_stringlib_all_accepted_by(string * str, int (*func)(int))
{
	bool result = false;
	if (str)
	{
		result = true;
		for (size_t  i = 0; i < str->length && result; i++)
			result = func(str->data[i]);
	}
	return result;
}

bool     corgasm_stringlib_isalpha(string * str)
{
	return corgasm_stringlib_all_accepted_by(str, isalpha);
}

bool     corgasm_stringlib_isdigit(string * str)
{	
	return corgasm_stringlib_all_accepted_by(str, isdigit);
}

bool     corgasm_stringlib_islower(string * str)
{
	return corgasm_stringlib_all_accepted_by(str, islower);
}

bool     corgasm_stringlib_isupper(string * str)
{
	return corgasm_stringlib_all_accepted_by(str, isupper);
}

bool     corgasm_stringlib_isalnum(string * str)
{
	return corgasm_stringlib_all_accepted_by(str, isalnum);
}

char *   corgasm_stringlib_extract(string * str)
{
	return str ? str->data : NULL;
}

bool 	 corgasm_stringlib_starts_with(string * str, const char * start)
{
	bool res = 0;
	if (str && start)
	{
		size_t len_start = strlen(start);
		if (len_start < str->length && str->data)
			res = !memcmp(start, str->data, len_start);
	}
	return res;
}

bool     corgasm_stringlib_starts_with_string(string * str, string * start)
{
	bool result = false;
	if (str && start && start->data)
		result = stringlib.startswith(str, start->data);
	return result;
}

void corgasm_stringlib_destroy_wrapped(void * str)
{
	if (str)
		stringlib.destroy((string*)str);
}