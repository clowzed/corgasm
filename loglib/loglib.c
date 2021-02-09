#include "loglib.h"


/**
 * @brief Logs debug message
 * @param[in] logger * self pointer to the logger
 * @param[in] const char * message to be logged
 * @param[in] const char * file where loging was performed
 * @param[in] const char * func where logging was performed
 * @param[in] int   line        where logging was performed
 * @return bool if operation was successful
 */
bool corgasm_loglib_debug_message(logger * self, const char * message, const char * file, const char * func, int line)
{
    bool was_logged_correctly = logging_args_are_correct(self, message, file, func, line);
    if (was_logged_correctly)
        corgasm_loglib_log_message(self, level_debug, __debug_message__, message, file, func, line);
    return was_logged_correctly;}


/**
 * @brief Logs info message
 * @param[in] logger * self pointer to the logger
 * @param[in] const char * message to be logged
 * @param[in] const char * file where loging was performed
 * @param[in] const char * func where logging was performed
 * @param[in] int   line        where logging was performed
 * @return bool if operation was successful
 */
bool corgasm_loglib_info_message(logger * self, const char * message, const char * file, const char * func, int line)
{
    bool was_logged_correctly = logging_args_are_correct(self, message, file, func, line);
    if (was_logged_correctly)
        corgasm_loglib_log_message(self, level_info, __info_message__, message, file, func, line);
    return was_logged_correctly;}


/**
 * @brief Logs warning message
 * @param[in] logger * self pointer to the logger
 * @param[in] const char * message to be logged
 * @param[in] const char * file where loging was performed
 * @param[in] const char * func where logging was performed
 * @param[in] int   line        where logging was performed
 * @return bool if operation was successful
 */
bool corgasm_loglib_warning_message(logger * self, const char * message, const char * file, const char * func, int line)
{
    bool was_logged_correctly = logging_args_are_correct(self, message, file, func, line);
    if (was_logged_correctly)
        corgasm_loglib_log_message(self, level_warning, __warning_message__, message, file, func, line);
    return was_logged_correctly;
}


/**
 * @brief Logs error message
 * @param[in] logger * self pointer to the logger
 * @param[in] const char * message to be logged
 * @param[in] const char * file where loging was performed
 * @param[in] const char * func where logging was performed
 * @param[in] int   line        where logging was performed
 * @return bool if operation was successful
 */
bool corgasm_loglib_error_message(logger * self, const char * message, const char * file, const char * func, int line)
{
    bool was_logged_correctly = logging_args_are_correct(self, message, file, func, line);
    if (was_logged_correctly)
        corgasm_loglib_log_message(self, level_error, __error_message__, message, file, func, line);
    return was_logged_correctly;
}


/**
 * @brief Logs critical message
 * @param[in] logger * self pointer to the logger
 * @param[in] const char * message to be logged
 * @param[in] const char * file where loging was performed
 * @param[in] const char * func where logging was performed
 * @param[in] int   line        where logging was performed
 * @return bool if operation was successful
 */
bool corgasm_loglib_critical_message(logger * self, const char * message, const char * file, const char * func, int line)
{
    bool was_logged_correctly = logging_args_are_correct(self, message, file, func, line);
    if (was_logged_correctly)
        corgasm_loglib_log_message(self, level_critical, __critical_message__, message, file, func, line);
    return was_logged_correctly;
}


/**
 * @brief Logs message with any level
 * @param[in] logger * self pointer to a logger
 * @param[in] const char * loglevel string of loglevel
 * @param[in] const char * message to log
 * @param[in] const char * file where loging was performed
 * @param[in] const char * func where logging was performed
 * @param[in] int   line        where logging was performed
 * @return void
 */
void corgasm_loglib_log_message(logger * self, int loglevel, const char * loglevel_repr, const char * message, const char * file, const char * func, int line)
{
    if (loglevel >= self->loglevel)
        fprintf(self->logfile,
                format_string,
                file, loglevel_repr, func, line, file, line, message);
}


/**
 * @brief Sets logfile for th logger
 * @param[in] logger * self pointer to a logger
 * @param[in] FILE * logfile pointer to a new logfile
 * @return bool if operation was successful
 * @note default is stdout
 */
bool corgasm_loglib_set_logfile(logger * self, FILE * logfile)
{
    bool was_setted = false;
    if (self && logfile)
    {
        self->logfile = logfile;
        was_setted = true;
    }
    return was_setted;
}


/**
 * @brief Allocates memory for a new logger
 * @return logger * pointer to a new allocated logger
 * @warning Can return NULL
 * @note Default logging level is debug
 */
logger * corgasm_loglib_new_logger()
{
    logger * new_logger = malloc(sizeof(logger));
    if (new_logger)
    {
        new_logger->loglevel = level_debug;
        new_logger->logfile  = stdout;
    }
    return new_logger;
}


/**
 * @brief Deallocates memory of a logger
 * @param[in] logger * self pointer to a logger
 * @return logger * pointer to a old logger
 * @warning Allways return NULL
 */
logger * corgasm_loglib_destroy(logger * self)
{
    if (self)
    {
        if (self->logfile && self->logfile != stdin)
            fclose(self->logfile);
        free(self);
    }
    return NULL;
}


/**
 * @brief Sets new loglevel for logger
 * @param[in] logger * self pointer to a logger
 * @param[in] int loglevel new loglevel
 * @return bool if operation was successful
 * @note if loglevel is incorrect false is returned
 */
bool corgasm_loglib_set_loglevel(logger * self, int loglevel)
{
    bool was_setted_correctly = false;
    if (self && is_correct_loglevel(loglevel))
    {
        self->loglevel       = loglevel;
        was_setted_correctly = true;
    }
    return was_setted_correctly;
}
