#include "configlib.h"

void corgasm_configlib_destroy_section_wrapped(void * self)
{
    if (self)
        configlib.destroy_section((section *)self);
}

section * corgasm_configlib_new_section(const char * name)
{
    section * new_section = NULL;
    if (name)
    {
        new_section = malloc(sizeof(section));
        if (new_section)
        {
            new_section->name = stringlib.new_string_from(name);
            new_section->data = dictlib.new_dict(300);
        }
        if (new_section && (!new_section->name || !new_section->data))
            new_section = configlib.destroy_section(new_section);
    }
    return new_section;
}

section * corgasm_configlib_destroy_section(section * self)
{
    if (self)
    {
        stringlib.destroy(self->name);
        dictlib.destroy(self->data, stringlib.destroy_wrapped);
        free(self);
    }
    return NULL;
}


bool corgasm_configlib_parse(configparser *self, FILE *stream)
{
    short      current_state        = find_left_bracket;
    bool       was_parsed           = true;
    section *  current_section      = NULL;
    string  *  current_section_name = stringlib.new_string();
    string  *  current_key_name     = stringlib.new_string();
    string  *  current_value        = stringlib.new_string();

    if (self && stream)
    {
        while_reading(current_char, stream, current_state != error_occured)
        {
            switch (current_state)
            {

                case find_left_bracket:
                {
                    if (alphabet_contains(current_char, find_left_bracket))
                    {
                        if (current_char == '[')
                        {
                            stringlib.clear(current_section_name);
                            current_state = reading_section_name;
                        }
                    }
                    else
                        current_state = error_occured;
                break;
                }



                case reading_section_name:
                {
                    if (alphabet_contains(current_char, reading_section_name))
                    {
                        if (current_char == ']')
                        {
                            current_section = configlib.new_section(stringlib.extract(current_section_name));
                            listlib.append(self->sections, (void *)current_section);
                            stringlib.clear(current_key_name);
                            stringlib.clear(current_value);
                        }
                        else if (current_char == '\n')
                        {
                            stringlib.clear(current_key_name);
                            current_state = find_key_name;
                        }
                        else
                            stringlib.add_char(current_section_name, current_char);
                    }
                    else
                        current_state = error_occured;
                break;
                }

                case find_key_name:
                {
                    if (alphabet_contains(current_char, find_key_name))
                    {
                        if (current_char == '[')
                        {
                            stringlib.clear(current_section_name);
                            current_state = reading_section_name;
                        }
                        else if (current_char == splitter)
                        {
                            stringlib.clear(current_value);
                            current_state = find_end_of_value;
                        }
                        else
                            stringlib.add_char(current_key_name, current_char);
                    }
                    else
                        current_state = error_occured;
                    break;
                }

                case find_end_of_value:
                {
                    if (alphabet_contains(current_char, find_end_of_value))
                    {
                        if (current_char == '\n')
                        {
                            dictlib.set(current_section->data,
                                        stringlib.extract(stringlib.trim(current_key_name)),
                                        stringlib.new_string_from(stringlib.extract(stringlib.trim(current_value))));
                            stringlib.clear(current_value);
                            stringlib.clear(current_key_name);
                            current_state = find_key_name;
                        }
                        else
                            stringlib.add_char(current_value, current_char);
                    }
                    else
                        current_state = error_occured;
                    break;
                }

                case error_occured:
                    was_parsed = false;
            }
        }
    }
    if (current_state != error_occured && current_value && current_value->length)
        dictlib.set(current_section->data,
                    stringlib.extract(stringlib.trim(current_key_name)),
                    stringlib.new_string_from(stringlib.extract(stringlib.trim(current_value))));
    stringlib.destroy(current_section_name);
    stringlib.destroy(current_key_name);
    stringlib.destroy(current_value);
    return was_parsed;
}

configparser * corgasm_configlib_new_configparser(const char * filename)
{
    configparser * new_configparser    = NULL;
    bool           is_parsed_correctly = false;
    FILE *         stream              = NULL;

    if (filename)
    {
        new_configparser = malloc(sizeof(configparser));
        if (new_configparser)
        {
            new_configparser->sections = listlib.new_list(corgasm_configlib_destroy_section_wrapped);
            stream                     = fopen(filename, "r");
            if (stream && new_configparser->sections)
                is_parsed_correctly = corgasm_configlib_parse(new_configparser, stream);
            if (!is_parsed_correctly)
                new_configparser = configlib.destroy(new_configparser);
            fclose(stream);
        }
    }
    return new_configparser;
}


configparser * corgasm_configlib_destroy(configparser * self)
{
    if (self)
    {
        listlib.destroy(self->sections);
        free(self);
    }
    return NULL;
}


string * corgasm_configlib_get(configparser * self, const char * section_name, const char * key)
{
    string * result = NULL;
    if (section_name && key)
    {
        string  * section_name_repr   = stringlib.new_string_from(section_name);
        section * result_section      = NULL;

        foreach_condition(section *, current_section, self->sections, !result_section)
        {
            if (stringlib.are_same(section_name_repr, current_section->name))
                result_section = current_section;
        }

        if (result_section)
            result = dictlib.get(result_section->data, key);
        stringlib.destroy(section_name_repr);
    }
    return result;
}

#ifdef LIB_BUILD_CONFIGLIB

int main()
{
    configparser * parser = configlib.new_configparser("./conf.ini");
    printf("libname: %s\n", stringlib.extract(configlib.get(parser, "default", "libname")));
    configlib.destroy(parser);
    return 0;
}

#endif
