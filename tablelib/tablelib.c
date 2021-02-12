#include "tablelib.h"


table * corgasm_tablelib_new_table(size_t headers_amount, ...)
{
    table * new_table = malloc(sizeof(table));
    if (new_table)
    {
        new_table->headers                 = listlib.new_list(stringlib.destroy_wrapped);
        new_table->lines                   = listlib.new_list(listlib.destroy_wrapped);
        new_table->columns_lenghts         = listlib.new_list(free);
        new_table->upper_left_angle        = '+';
        new_table->upper_right_angle       = '+';
        new_table->lower_left_angle        = '+';
        new_table->lower_right_angle       = '+';
        new_table->rows_delimetr           = '-';
        new_table->columns_delimetr        = '|';
        new_table->delimetrs_intersections = '+';
        if (!new_table->headers         ||\
            !new_table->lines           ||\
            !new_table->columns_lenghts)
            new_table = tablelib.destroy(new_table);
        else
        {
            va_list args_pointer;
            va_start(args_pointer, headers_amount);

            while (headers_amount)
            {
                char   * next        = va_arg(args_pointer, char *);
                string * next_header = stringlib.new_string_from(next);

                if (next_header)
                {
                    size_t * column_length = malloc(sizeof(size_t));
                    *column_length = next_header->length;

                    listlib.append(new_table->columns_lenghts, column_length);
                    listlib.append(new_table->headers, next_header);
                }
                else
                {
                    new_table = tablelib.destroy(new_table);
                    break;
                }
                headers_amount--;
            }
        }
    }
    return new_table;
}

table * corgasm_tablelib_destroy(table * t)
{
    if (t)
    {
        listlib.destroy(t->headers);
        listlib.destroy(t->lines);
        listlib.destroy(t->columns_lenghts);
        free(t);
    }
    return NULL;
}

bool corgasm_tablelib_add_line(table * t, ...)
{
    bool was_added = t != NULL;
    if (was_added)
    {
        size_t arguments_left = t->headers->length;

        va_list args_pointer;
        va_start(args_pointer, t);

        list * new_line = listlib.new_list(stringlib.destroy_wrapped);
        if (new_line)
        {
            while (arguments_left)
            {
                const char * next = va_arg(args_pointer, const char *);
                string * data = stringlib.new_string_from(next);
                if (data)
                {
                    listlib.append(new_line, (void *)data);
                    arguments_left--;
                }
                else
                {
                    listlib.destroy(new_line);
                    was_added = false;
                    break;
                }
            }
            if (was_added)
                listlib.append(t->lines, new_line);
        }
        else
            was_added = false;
    }
    if (was_added)
        corgasm_tablelib_align_columns(t);
    return was_added;
}

void corgasm_tablelib_align_columns(table * t)
{
    if (t)
        for (size_t column_index = 0; column_index < t->headers->length; column_index++)
            corgasm_tablelib_align_column(t, column_index);
}

void corgasm_tablelib_align_column(table * t, size_t column_index)
{
    if (t)
    {
        size_t max_column_length = corgasm_tablelib_max_column_length(t, column_index);
        node * data = listlib.get(t->columns_lenghts, column_index);
        if (data)
            *((size_t *)(data->data)) = max_column_length;
    }
}

size_t corgasm_tablelib_max_column_length(table * t, size_t column_index)
{
    string * column_header = (string*)(listlib.get(t->columns_lenghts, column_index)->data);
    size_t max_column_length = column_header->length;
    for (node * current_node  = listlib.begin(t->lines);
                current_node != listlib.end(t->lines);
                current_node  = listlib.next(current_node))
    {
        list * current_line = (list *)(current_node->data);
        if (current_line->length)
        {
            string * column_data = (string *)(listlib.get(current_line, column_index)->data);
            if (column_data->length)
                max_column_length = column_data->length > max_column_length ? column_data->length : max_column_length;
        }
    }
    return max_column_length;
}

void corgasm_tablelib_show(table * t)
{
    corgasm_tablelib_print_upper_delimiter(t);
    corgasm_tablelib_print_headers(t);
    corgasm_tablelib_print_delimeters(t);
    for (node * current_node  = listlib.begin(t->lines);
                current_node != listlib.end(t->lines);
                current_node  = listlib.next(current_node))
    {
        corgasm_tablelib_print_line(t, (list*)(current_node->data));
        if (current_node->next)
            corgasm_tablelib_print_delimeters(t);
    }
    corgasm_tablelib_print_bottom_delimeters(t);
}

void corgasm_tablelib_print_upper_delimiter(table * t)
{
    printf("%c", t->upper_left_angle);
    for (node * current_node  = listlib.begin(t->columns_lenghts);
                current_node != listlib.end(t->columns_lenghts);
                current_node  = listlib.next(current_node))
    {
        size_t column_length = *((size_t*)(current_node->data));

        for (size_t _ = 0; _ < column_length + 2; ++_)
            printf("%c", t->rows_delimetr);

        if (current_node->next) printf("%c", t->delimetrs_intersections);
        else                    printf("%c", t->upper_right_angle);
    }
    printf("\n");
}

void corgasm_tablelib_print_headers(table * t)
{
    size_t current_header_index = 0;
    for (node * current_node  = listlib.begin(t->headers);
                current_node != listlib.end(t->headers);
                current_node  = listlib.next(current_node))
    {
        string * header = (string*)(current_node->data);
        size_t   column_max_len = *((size_t*)(listlib.get(t->columns_lenghts, current_header_index++)->data));
        printf("%c %s", t->columns_delimetr, stringlib.extract(header));

        for (size_t i = 0; i < column_max_len + 1 - header->length; i++)
            printf("%c", ' ');
    }
    printf("%c", t->columns_delimetr);
    printf("\n");
}

void corgasm_tablelib_print_delimeters(table*t)
{
    printf("%c", t->columns_delimetr);
    for (node * current_node  = listlib.begin(t->columns_lenghts);
                current_node != listlib.end(t->columns_lenghts);
                current_node  = listlib.next(current_node))
    {
        size_t column_length = *((size_t*)(current_node->data));

        for (size_t _ = 0; _ < column_length + 2; ++_)
            printf("%c", t->rows_delimetr);

        if (current_node->next) printf("%c", t->delimetrs_intersections);
        else                    printf("%c", t->columns_delimetr);
    }
    printf("\n");
}

void corgasm_tablelib_print_line(table * t, list * line)
{
    size_t current_header_index = 0;
    for (node * current_node  = listlib.begin(line);
                current_node != listlib.end(line);
                current_node  = listlib.next(current_node))
    {
        string * data = (string*)(current_node->data);
        size_t   column_max_len = *((size_t*)(listlib.get(t->columns_lenghts, current_header_index++)->data));
        printf("%c %s", t->columns_delimetr, stringlib.extract(data));

        for (size_t i = 0; i < column_max_len + 1 - data->length; i++)
            printf("%c", ' ');
    }
    printf("%c", t->columns_delimetr);
    printf("\n");
}

void corgasm_tablelib_print_bottom_delimeters(table * t)
{
    printf("%c", t->lower_left_angle);
    for (node * current_node  = listlib.begin(t->columns_lenghts);
                current_node != listlib.end(t->columns_lenghts);
                current_node  = listlib.next(current_node))
    {
        size_t column_length = *((size_t*)(current_node->data));

        for (size_t _ = 0; _ < column_length + 2; ++_)
            printf("%c", t->rows_delimetr);

        if (current_node->next) printf("%c", t->delimetrs_intersections);
        else                    printf("%c", t->lower_right_angle);
    }
    printf("\n");
}
