#ifndef __CORGASM_PATHLIB_H__
#define __CORGASM_PATHLIB_H__

#include <sys/stat.h>
#include <stdarg.h>


#include "../listlib/listlib.h"
#include "../stringlib/stringlib.h"

static const char windows_separator = '\\';
static const char unix_separator    = '/';

#define uu __attribute__((unused))

typedef struct corgasm_pathlib_path
{
    list * segments;
    char   separator;
}path;

typedef struct corgasm_pathlib_functions
{
    char     (*separator)    ();
    bool     (*exists)       (const path * self);
    bool     (*is_directory) (const path * self);
    bool     (*is_file)      (const path * self);
    list   * (*parse_path)   (const char * self);
    path   * (*destroy)      (      path * self);
    path   * (*join_paths)   (      path * self, ...);
    path   * (*new_path)     (const char * self);
    string * (*extract)      (const path * self);
}corgasm_pathlib_functions;


char     corgasm_pathlib_separator    ();
bool     corgasm_pathlib_exists       (const path * self);
bool     corgasm_pathlib_is_directory (const path * self);
bool     corgasm_pathlib_is_file      (const path * self);
path *   corgasm_pathlib_destroy      (      path * self);
path *   corgasm_pathlib_join_paths   (      path * self, ...);
path *   corgasm_pathlib_new_path     (const char * self);
list *   corgasm_pathlib_parse_path   (const char * self);
string * corgasm_pathlib_extract      (const path * self);


static const corgasm_pathlib_functions pathlib = {
    .separator    = corgasm_pathlib_separator,
    .exists       = corgasm_pathlib_exists,
    .is_directory = corgasm_pathlib_is_directory,
    .is_file      = corgasm_pathlib_is_file,
    .parse_path   = corgasm_pathlib_parse_path,
    .destroy      = corgasm_pathlib_destroy,
    .join_paths   = corgasm_pathlib_join_paths,
    .new_path     = corgasm_pathlib_new_path,
    .extract      = corgasm_pathlib_extract,

};

#endif /* __CORGASM_PATHLIB_H__ */
