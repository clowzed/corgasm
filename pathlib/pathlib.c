#include "pathlib.h"

path * corgasm_pathlib_new_path(const char * self)
{
    path * new_path = NULL;
    if (self)
    {
        new_path = malloc(sizeof(path));
        if (new_path)
        {
            new_path->segments  = pathlib.parse_path(self);
            new_path->separator = pathlib.separator();
        }

        if (new_path && !new_path->segments)
            new_path = pathlib.destroy(new_path);
    }
    return new_path;
}

path * corgasm_pathlib_destroy(path * self)
{
    if (self)
    {
        listlib.destroy(self->segments);
        free(self);
    }
    return NULL;
}

bool corgasm_pathlib_exists(const path * self)
{
    return self ? pathlib.is_file(self) || pathlib.is_directory(self) : false;
}

bool corgasm_pathlib_is_file(const path * self)
{
    bool is_file = false;

    if (self)
    {
        string * string_path = pathlib.extract(self);
        if (string_path)
        {
            FILE * file     = fopen(stringlib.extract(string_path), "r");
                   is_file  = file != NULL;

            fclose(file);
            stringlib.extract(string_path);
        }
    }

    return is_file;
}

string * corgasm_pathlib_extract(const path * self)
{
    string * path_from_segments = NULL;
    if (self)
    {
        path_from_segments = stringlib.new_string();

        size_t segment_count = 0;
        foreach(string *, segment, self->segments)
        {
            stringlib.concat(path_from_segments, segment);
            if (++segment_count < self->segments->length)
                stringlib.add_char(path_from_segments, self->separator);
        }
    }
    return path_from_segments;
}

bool corgasm_pathlib_is_directory(const path * self)
{
    bool is_directory = false;
    if (self)
    {
        string * path = pathlib.extract(self);
        if (path)
        {
            struct stat statbuf;
            if (!stat(stringlib.extract(path), &statbuf))
                is_directory = S_ISDIR(statbuf.st_mode);
            stringlib.destroy(path);
        }
    }
    return is_directory;
}

path * corgasm_pathlib_join_paths(path * self, ...)
{
    if (self)
    {
        va_list args;
        va_start(args, self);

        path * next = NULL;

        while ((next = va_arg(args, path *)))
        {
            foreach(string *, segment, next->segments)
                    listlib.append(self->segments, segment);
        }

        va_end(args);
    }
    return self;
}

char corgasm_pathlib_separator()
{
    #if defined(WIN32) || defined(_WIN32)
        return windows_separator;
    #else
        return unix_separator;
    #endif
}

list * corgasm_pathlib_parse_path(uu const char * self)
{
    list   * segments    = listlib.new_list(stringlib.destroy_wrapped);
    string * new_segment = stringlib.new_string();
    string * double_dot = stringlib.new_string_from("..");

    for (size_t i = 0; i < strlen(self); i++)
    {
        char current_char = self[i];

        if (current_char == windows_separator ||
            current_char == unix_separator)
        {
            if (new_segment->length)
            {
                if (stringlib.are_same(double_dot, new_segment))
                {
                    string * unused = listlib.pop(segments);
                    stringlib.destroy(unused);
                }
                else
                {
                    listlib.append(segments, new_segment);
                    new_segment = stringlib.new_string();
                }
            }
        }
        else
            stringlib.add_char(new_segment, current_char);
    }
    if (new_segment->length)
        listlib.append(segments, new_segment);

    stringlib.destroy(double_dot);

    foreach(string *, segment, segments)
        printf("%s ", stringlib.extract(segment));

    return segments;
}


#ifdef LIB_BUILD_PATHLIB

int main()
{
    path * path_to_check = pathlib.new_path("./pathlib");
    printf("Path: %s\n", stringlib.extract(pathlib.extract(path_to_check)));
    printf("path exists: %s\n",       pathlib.exists(path_to_check)       ? "yes" : "no");
    printf("path is file: %s\n",      pathlib.is_file(path_to_check)      ? "yes" : "no");
    printf("path is directory: %s\n", pathlib.is_directory(path_to_check) ? "yes" : "no");
    pathlib.destroy(path_to_check);
    return 0;
}

#endif
