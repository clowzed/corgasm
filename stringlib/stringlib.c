#include "stringlib.h"

char * corgasm_stringlib_strrev(char * str)
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

void corgasm_stringlib_clear(string * self)
{
	if (self && self->data)
	{
		free(self->data);
		self->data = NULL;
		self->length = 0;
	}
}

void corgasm_stringlib_destroy(string * self)
{
	if (self)
	{
		stringlib.clear(self);
		free(self);
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

string * corgasm_stringlib_add_char(string * self, char c)
{
	char buffer[2] = {c};
	string * str2  = stringlib.new_string_from(buffer);
	         self   =  stringlib.concat(self, str2);
	stringlib.destroy(str2);
	return self;
}

string * corgasm_stringlib_concat(string * self, string * second)
{
	if (self && second && second->length)
	{
		char *new_data_for_self = malloc((self->length + second->length + 1) * sizeof(char));
		if (new_data_for_self)
		{
			if (self->data)
			{
				strcpy(new_data_for_self, self->data);
				if (second->data)
					strcat(new_data_for_self, second->data);
			}
			else
				if (second->data)
					strcpy(new_data_for_self, second->data);
			self->data = new_data_for_self;
			self->length = self->length + second->length;
		}
	}
	return self;
}

size_t corgasm_stringlib_length(string * self)
{
	return self ? self->length : 0;
}

int corgasm_stringlib_compare(string * self, string * other)
{
	int result = -1;
	if (self && other && self->data && other->data)
		result = strcmp(self->data, other->data);
	return result;
}

bool corgasm_stringlib_are_same(string * self, string * second)
{
	return !(stringlib.compare(self, second));
}

string * corgasm_stringlib_to_lower(string * self)
{
	if (self)
		for (size_t i = 0; i < self->length; i++)
			self->data[i] = (char)tolower(self->data[i]);
	return self;
}

string * corgasm_stringlib_to_upper(string * self)
{
	if (self)
		for (size_t i = 0; i < self->length; i++)
			self->data[i] = (char)toupper(self->data[i]);
	return self;
}

string * corgasm_stringlib_reverse(string *self)
{
	if (self && self->data)
		corgasm_stringlib_strrev(self->data);
	return self;
}

string * corgasm_stringlib_new_string_from(const char * str)
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

bool corgasm_stringlib_all_accepted_by(string * self, int (*f)(int))
{
	bool result = false;
	if (self)
	{
		result = true;
		for (size_t  i = 0; i < self->length && result; i++)
			result = f(self->data[i]);
	}
	return result;
}

bool corgasm_stringlib_isalpha(string * self)
{
	return corgasm_stringlib_all_accepted_by(self, isalpha);
}

bool corgasm_stringlib_isdigit(string * self)
{	
	return corgasm_stringlib_all_accepted_by(self, isdigit);
}

bool corgasm_stringlib_islower(string * self)
{
	return corgasm_stringlib_all_accepted_by(self, islower);
}

bool corgasm_stringlib_isupper(string * self)
{
	return corgasm_stringlib_all_accepted_by(self, isupper);
}

bool corgasm_stringlib_isalnum(string * self)
{
	return corgasm_stringlib_all_accepted_by(self, isalnum);
}

char * corgasm_stringlib_extract(string * self)
{
	return self ? self->data : NULL;
}

bool corgasm_stringlib_starts_with(string * self, const char * start)
{
	bool res = 0;
	if (self && start)
	{
		size_t len_start = strlen(start);
		if (len_start < self->length && self->data)
			res = !memcmp(start, self->data, len_start);
	}
	return res;
}

bool corgasm_stringlib_starts_with_string(string * self, string * prefix)
{
	bool result = false;
	if (self && prefix && prefix->data)
		result = stringlib.startswith(self, prefix->data);
	return result;
}

void corgasm_stringlib_destroy_wrapped(void * self)
{
	if (self)
		stringlib.destroy((string*)self);
}
