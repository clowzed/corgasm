#include "argparselib.h"

/**
 * @brief Allocates memory for a new argument
 * @param[in] short type set type of argumnet (FLAG or ARGUMENT)
 * @param[in] bool  is_required sets if argument is required
 * @param[in] const char * short_name Set short name of argument [startswith '-']
 * @param[in] const char * long_name Set long name of argument   [startswith '--']
 * @param[in] const char * description Set description of argument
 * @note Description and long_name can be NULL
 * @warning Function can return NULL
 * @return argument * Pointer to an new allocated argument
 */
argument * corgasm_argparselib_new_argument(short   type,
                                            bool    is_required,
                                            const char  * short_name,
                                            const char  * long_name,
                                            const char  * description)
{
    argument * new_argument = NULL;
    // ? long name and description can be NULL
    // TODO: Description will be generated automatically
    if (is_correct_type(type)             &&
        is_correct_short_name(short_name) &&
        is_correct_long_name(long_name)   &&
        is_correct_description(description))
    {
        new_argument = malloc(sizeof(argument));
        if (new_argument)
        {
            new_argument->is_flag     = is_flag(type);
            new_argument->is_argument = is_argument(type);
            new_argument->is_enabled  = false;
            new_argument->is_required = is_required;
            new_argument->short_name  = stringlib.new_string_from(short_name);
            new_argument->long_name   = long_name   ? stringlib.new_string_from(long_name)   : NULL;
            new_argument->description = description ? stringlib.new_string_from(description) : NULL;
            new_argument->values      = listlib.new_list(stringlib.destroy_wrapped);

            // * Handling memory errors in initialization
            if  ((description && !new_argument->description) || !new_argument->short_name               ||
                (long_name    && !new_argument->long_name)   || !new_argument->values)
                new_argument = argparselib.destroy_argument(new_argument);
        }
    }
    return new_argument;
}


/**
 * @brief Deallocates memory of an argument
 * @param[in] argument * self pointer to an argument
 * @return argument *
 * @note Always return NULL
 */
argument * corgasm_argparselib_destroy_argument(argument * self)
{
    if (self)
    {
        stringlib.destroy(self->short_name);
        stringlib.destroy(self->long_name);
        stringlib.destroy(self->description);
        listlib.destroy(self->values);
        free(self);
    }
    return NULL;
}


/**
 * @brief Wrapped destructor for argument
 * @param[in] void * self pointer to an argument
 * @return void
 */
void corgasm_argparselib_destroy_argument_wrapped(void * self)
{
    if (self)
        argparselib.destroy_argument((argument*)self);
}


/**
 * @brief Deallocates memory of many arguments
 * @param[in] argument * first pointer to a first argument
 * @return void
 * @note Arguments must end with a NULL pointer
 */
void corgasm_argparselib_destroy(argument * first, ...)
{
    if (first)
    {
        va_list arguments;
        va_start(arguments, first);

        // * Iterating while argument is not NULL
        while (true)
        {
            argument * current_argument = va_arg(arguments, argument *);
            if (current_argument)
                argparselib.destroy_argument(current_argument);
            else
                break;
        }

        va_end(arguments);
    }
}


/**
 * @brief Parses arguments from argv
 * @param[in] int argc number of arguments
 * @param[in] char ** argv pointer to arguments
 * @param[in] ... any number of argument *
 * @return bool true if arguments were parsed correctly
 */
bool corgasm_argparselib_parse(int argc, char ** argv, ...)
{
    bool is_parsed_correctly = false;

    if (argv && argc > 0)
    {

        // * Just store all args in list
        list * arguments = listlib.new_list(NULL);
        if (arguments)
        {
            is_parsed_correctly = true;
            va_list func_args;
            va_start(func_args, argv);

            while (true && is_parsed_correctly)
            {
                argument * arg = va_arg(func_args, argument *);

                if (arg) is_parsed_correctly = listlib.append(arguments, (void*)arg);
                else     break;
            }
            va_end(func_args);
        }
        else is_parsed_correctly = false;


        // * Store cmd args in list
        list * cmd_args = NULL;
        if (is_parsed_correctly)
        {
            cmd_args = listlib.new_list(NULL);
            if (cmd_args)
            {
                for (int i = 1; i < argc && is_parsed_correctly; i++)
                {
                    string * new_arg = stringlib.new_string_from(argv[i]);
                    if (new_arg)
                        is_parsed_correctly = listlib.append(cmd_args, new_arg);
                    else
                        is_parsed_correctly = false;
                }
            }
            else is_parsed_correctly = false;
        }

        // * If no memory errors occured
        // * We parse cmd args
        if (is_parsed_correctly)
            is_parsed_correctly = corgasm_argparselib_parse_list_arguments(cmd_args, arguments);

        if (is_parsed_correctly)
            is_parsed_correctly = corgasm_argparselib_check_if_args_are_correct(arguments);

        listlib.destroy(arguments);
        listlib.destroy(cmd_args);
    }
    return is_parsed_correctly;
}


/**
 * @brief Get program name from argv
 * @param[in] char ** argv pointer to arguments
 * @return string *
 * @warning Can return NULL
 */
string * corgasm_argparselib_get_program_name(char ** argv)
{
    return argv? stringlib.new_string_from(*argv) : NULL;
}


/**
 * @brief Parses arguments from argv
 * @param[in] list * cmd_args List with strings
 * @param[in] list * arguments list of arguments
 * @return bool
 * @warning No check for incorrect params. That is why it is static!
 * @todo Now always retruns True! On mistakes make false
 */
static bool corgasm_argparselib_parse_list_arguments(list * cmd_args, __attribute__((unused)) list * arguments)
{
    bool is_parsed_correctly = true;

    argument * last_arg = NULL;
    foreach(string *, arg, cmd_args)
    {
        if (string_is_argument(arg))
        {
            last_arg = corgasm_argparselib_find_argument(arguments, arg);
            if (last_arg)
                last_arg->is_enabled = true;
        }
        else
            if (last_arg && last_arg->is_argument)
                listlib.append(last_arg->values, arg);
    }
    return is_parsed_correctly;
}


/**
 * @brief Finds given argument in list
 * @param[in] list * arguments pointer to arguments list
 * @param[in] string * arg a to search for
 * @return argument *
 * @warning Can return NULL
 */
static argument * corgasm_argparselib_find_argument(list * arguments, string * arg)
{
    argument * found = NULL;
    foreach_condition(argument *, current_argument, arguments, !found)
    {
        if (stringlib.are_same(current_argument->short_name, arg) ||
            stringlib.are_same(current_argument->long_name,  arg))
            found = current_argument;
    }
    return found;
}


/**
 * @brief checks if all arguments are correct
 * @param[in] list * arguments pointer to arguments list
 * @return true if all arguments are correct
 */
static bool corgasm_argparselib_check_if_args_are_correct(list * arguments)
{
    bool is_parsed_correctly = true;
    foreach_condition(argument *, current_argument, arguments, is_parsed_correctly)
    {
        is_parsed_correctly = is_parsed_correctly && ((current_argument->is_required && current_argument->is_enabled) || !current_argument->is_required);
        is_parsed_correctly = is_parsed_correctly && (current_argument->is_flag || (current_argument->is_argument && ((current_argument->is_required && current_argument->values->length) || !current_argument->is_required)));
    }
    return is_parsed_correctly;
}

/**
 * @brief Checks if argumentwas setted by user
 * @param[in] argument * pointer to argument
 * @return true if argument was setted
 */
bool corgasm_argparselib_setted(argument * self)
{
    return self ? self->is_enabled : false;
}

/**
 * @brief Get list of values given by the user
 * @param[in] argument * pointer to argument
 * @return list * pointer to list of values
 * @warning Can return NULL if argument is FLAG or NULL
 */
list * corgasm_argparselib_get_values(argument * self)
{
    list * values = NULL;
    if (self && self->is_argument) values = self->values;
    return values;
}

/**
 * @brief Shows all arguments of the program
 * @param[in] argument * pointer to the first argument
 * @return void
 */
void corgasm_argparselib_show(argument * first, ...)
{
    va_list func_args;
    va_start(func_args, first);

    printf("\nUsage:\n");
    while (true)
    {
        argument * arg = va_arg(func_args, argument *);
        if (arg)
            corgasm_argparselib_show_argument(arg);
        else
            break;
    }
    va_end(func_args);
}

/**
 * @brief Shows argument in stdin
 * @param[in] argument * pointer to the argument
 * @return void
 */
void corgasm_argparselib_show_argument(argument * self)
{
    if (self)
    {
        char format_string[] = "\n%-8s [%s, %s]\n\t required: %s\n\t Description: %s\n\n";
        fprintf(stdout,
                format_string,
                self->is_argument? "argument" : "flag",
                stringlib.extract(self->short_name),
                self->long_name  ? stringlib.extract(self->long_name) : "no long name",
                self->is_required? "required" : "not required",
                self->description? stringlib.extract(self->description) : "no description"
                );
    }
}

/*
int main(int argc, char **argv)
{
    argument * help_flag       = argparselib.new_argument(FLAG,     NOT_REQUIRED, "-h", "--help",     "Display help message");
    argument * name_argument   = argparselib.new_argument(ARGUMENT, REQUIRED,     "-n", NO_LONG_NAME, "Sets names to show");
    argument * unused_argument = argparselib.new_argument(ARGUMENT, NOT_REQUIRED, "-u", NO_LONG_NAME,  NO_DESCRIPTION);

    bool args_parsed_correctly = argparselib.parse_arguments(argc, argv, name_argument, help_flag, unused_argument);



    if (args_parsed_correctly)
    {
        list * names = argparselib.get_values(name_argument);
        foreach(string *, name, names)
            printf("Hello, %s!\n", stringlib.extract(name));

        if (argparselib.setted(help_flag))
            argparselib.show_arguments( name_argument,
                                        unused_argument,
                                        help_flag);
    }
    else
        argparselib.show_arguments( name_argument,
                                    unused_argument,
                                    help_flag);

    return 0;
}
*/