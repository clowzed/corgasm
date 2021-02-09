#ifndef __CORGASM_LOGLIB_H__
#define __CORGASM_LOGLIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// * Levels of errors
#define level_critical 50
#define level_error    40
#define level_warning  30
#define level_info     20
#define level_debug    10

#define is_correct_loglevel(loglevel) ((loglevel == level_critical)  || \
                                       (loglevel == level_error   )  || \
                                       (loglevel == level_info    )  || \
                                       (loglevel == level_warning )  || \
                                       (loglevel == level_debug   ))

#define logging_args_are_correct(logger, message, file, func, line) (logger  && \
                                                                     message && \
                                                                     file    && \
                                                                     func    && \
                                                                     line > 0)


#define additional_information __FILE__, __func__, __LINE__
#define format_string "[file: %-10s][level: %-10s][func: %-10s][line: %-7d](%s:%d) : %s\n"

#define critical(logger, message) critical_message(logger, message, additional_information)
#define error(logger, message)    error_message   (logger, message, additional_information)
#define warning(logger, message)  warning_message (logger, message, additional_information)
#define info(logger, message)     info_message    (logger, message, additional_information)
#define debug(logger, message)    debug_message   (logger, message, additional_information)

#define __critical_message__ "critical"
#define __error_message__    "error"
#define __warning_message__  "warning"
#define __info_message__     "info"
#define __debug_message__    "debug"


typedef struct corgasm_loglib_logger
{
    FILE * logfile;
    int    loglevel;
}logger;

typedef struct corgasm_loglib_functions
{
    logger * (*new_logger)      ();
    logger * (*destroy)         (logger * self);
    bool     (*set_loglevel)    (logger * self, int loglevel);
    bool     (*set_logfile)     (logger * self, FILE * logfile);
    bool     (*critical_message)(logger * self, const char * message, const char * file, const char * func, int line);
    bool     (*error_message)   (logger * self, const char * message, const char * file, const char * func, int line);
    bool     (*warning_message) (logger * self, const char * message, const char * file, const char * func, int line);
    bool     (*info_message)    (logger * self, const char * message, const char * file, const char * func, int line);
    bool     (*debug_message)   (logger * self, const char * message, const char * file, const char * func, int line);
}corgasm_loglib_functions;


/**
 * @brief Logs debug message
 * @param[in] logger * self pointer to the logger
 * @param[in] const char * message to be logged
 * @param[in] const char * file where loging was performed
 * @param[in] const char * func where logging was performed
 * @param[in] int   line        where logging was performed
 * @return bool if operation was successful
 */
bool corgasm_loglib_debug_message(logger * self, const char * message, const char * file, const char * func, int line);


/**
 * @brief Logs info message
 * @param[in] logger * self pointer to the logger
 * @param[in] const char * message to be logged
 * @param[in] const char * file where loging was performed
 * @param[in] const char * func where logging was performed
 * @param[in] int   line        where logging was performed
 * @return bool if operation was successful
 */
bool corgasm_loglib_info_message(logger * self, const char * message, const char * file, const char * func, int line);


/**
 * @brief Logs warning message
 * @param[in] logger * self pointer to the logger
 * @param[in] const char * message to be logged
 * @param[in] const char * file where loging was performed
 * @param[in] const char * func where logging was performed
 * @param[in] int   line        where logging was performed
 * @return bool if operation was successful
 */
bool corgasm_loglib_warning_message(logger * self, const char * message, const char * file, const char * func, int line);


/**
 * @brief Logs error message
 * @param[in] logger * self pointer to the logger
 * @param[in] const char * message to be logged
 * @param[in] const char * file where loging was performed
 * @param[in] const char * func where logging was performed
 * @param[in] int   line        where logging was performed
 * @return bool if operation was successful
 */
bool corgasm_loglib_error_message(logger * self, const char * message, const char * file, const char * func, int line);


/**
 * @brief Logs critical message
 * @param[in] logger * self pointer to the logger
 * @param[in] const char * message to be logged
 * @param[in] const char * file where loging was performed
 * @param[in] const char * func where logging was performed
 * @param[in] int   line        where logging was performed
 * @return bool if operation was successful
 */
bool corgasm_loglib_critical_message(logger * self, const char * message, const char * file, const char * func, int line);


/**
 * @brief Logs message with any level
 * @param[in] logger * self pointer to a logger
 * @param[in] int loglevel level of message
 * @param[in] const char * loglevel_repr string of loglevel
 * @param[in] const char * message to log
 * @param[in] const char * file where loging was performed
 * @param[in] const char * func where logging was performed
 * @param[in] int   line        where logging was performed
 * @return void
 */
void corgasm_loglib_log_message(logger * self, int loglevel, const char * loglevel_repr, const char * message, const char * file, const char * func, int line);


/**
 * @brief Sets logfile for th logger
 * @param[in] logger * self pointer to a logger
 * @param[in] FILE * logfile pointer to a new logfile
 * @return bool if operation was successful
 * @note default is stdout
 */
bool corgasm_loglib_set_logfile(logger * self, FILE * logfile);


/**
 * @brief Allocates memory for a new logger
 * @return logger * pointer to a new allocated logger
 * @warning Can return NULL
 * @note Default logging level is debug, Logfile: stdout
 */
logger * corgasm_loglib_new_logger();


/**
 * @brief Deallocates memory of a logger
 * @param[in] logger * self pointer to a logger
 * @return logger * pointer to a old logger
 * @warning Allways return NULL
 */
logger * corgasm_loglib_destroy(logger * self);


/**
 * @brief Sets new loglevel for logger
 * @param[in] logger * self pointer to a logger
 * @param[in] int loglevel new loglevel
 * @return bool if operation was successful
 * @note if loglevel is incorrect false is returned
 */
bool corgasm_loglib_set_loglevel(logger * self, int loglevel);


static const corgasm_loglib_functions loglib =
{
    .new_logger       = corgasm_loglib_new_logger,
    .destroy          = corgasm_loglib_destroy,
    .set_loglevel     = corgasm_loglib_set_loglevel,
    .set_logfile      = corgasm_loglib_set_logfile,
    .critical_message = corgasm_loglib_critical_message,
    .error_message    = corgasm_loglib_error_message,
    .warning_message  = corgasm_loglib_warning_message,
    .info_message     = corgasm_loglib_info_message,
    .debug_message    = corgasm_loglib_debug_message,
};

#endif
