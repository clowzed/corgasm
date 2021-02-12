/**
 * @file   tablelib.h
 * @author clowzed.exe
 * @date   15 Jan 2021
 * @brief  File containing functions signatures and structures typedefs for table module
 * @see    https://github.com/clowzed/corgasm
 */

#ifndef __CORGASM_TABLELIB_H__
#define __CORGASM_TABLELIB_H__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#include "../stringlib/stringlib.h"
#include "../listlib/listlib.h"


typedef struct corgasm_tablelib_table
{
    char   upper_left_angle;        /** < default = '+' */
    char   upper_right_angle;       /** < default = '+' */
    char   lower_left_angle;        /** < default = '+' */
    char   lower_right_angle;       /** < default = '+' */
    char   rows_delimetr;           /** < default = '-' */
    char   columns_delimetr;        /** < default = '|' */
    char   delimetrs_intersections; /** < default = '+' */

    list * headers;                 /** < list[string *]         that are columns names                             */
    list * lines;                   /** < list[list *[string *]] of data                                            */
    list * columns_lenghts;         /** < list[size_t *]         represents max lenght of the data stirng in column */
} table;

typedef struct corgasm_tablelib_functions
{
    table * (*new_table) (size_t , ...);
    bool    (*add_line)  (table *, ...);
    table * (*destroy)   (table *     );
    void    (*show)      (table *     );

} corgasm_tablelib_functions;


/**
 * @brief Allocates memory for a new table
 * @param[in] size_t        number of variadic argumnte
 * @param[in] const char *  variadic arguments of type const char * representing columns names
 * @return    table *       pointer to a new table
 * @warning   if any error occurs function will return NULL
 */
table * corgasm_tablelib_new_table(size_t , ...);


/**
 * @brief Adds line to tablelib
 * @param[in] table *      data table to add a line to
 * @param[in] const char * variadic arguments of colums data [amount = columns amount]
 * @return    bool         if line was sucesfully added
 */
bool corgasm_tablelib_add_line(table *, ...);


/**
 * @brief Deallocates all memory in table
 * @param[in] table * pointer to table
 * @return    table *
 * @warning   Always returns NULL to reduce lines in your code
 */
table * corgasm_tablelib_destroy(table *);


/**
 * @brief     Prints table to stdin
 * @param[in] table * pointer to table to show
 * @return    no return
 */
void corgasm_tablelib_show(table *);


// * utilites
void   corgasm_tablelib_print_bottom_delimeters(table *        );
void   corgasm_tablelib_print_upper_delimiter  (table *        );
size_t corgasm_tablelib_max_column_length      (table *, size_t);
void   corgasm_tablelib_align_columns          (table *        );
void   corgasm_tablelib_print_delimeters       (table *        );
void   corgasm_tablelib_align_column           (table *, size_t);
void   corgasm_tablelib_print_headers          (table *        );
void   corgasm_tablelib_print_line             (table *, list *);


static const corgasm_tablelib_functions tablelib = {
    .new_table = corgasm_tablelib_new_table,
    .add_line  = corgasm_tablelib_add_line,
    .destroy   = corgasm_tablelib_destroy,
    .show      = corgasm_tablelib_show,
};

#endif /* __CORGASM_TABLELIB_H__ */
