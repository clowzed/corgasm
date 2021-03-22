#ifndef __CORGASM_PROGRESSLIB_H__
#define __CORGASM_PROGRESSLIB_H__

#include <time.h>

#include "../stringlib/stringlib.h"

#define max_terminal_length 80
#define lable_max_length    25

#define go_back '\r'

typedef struct corgasm_progresslib_format
{
    char begin;
    char end;
    char fill;
    char empty;
}progressbar_format;


static const progressbar_format progressbar_default_format = {
    .begin = '|',
    .end   = '|',
    .fill  = '=',
    .empty = ' ',
};

typedef struct corgasm_progresslib_progressbar
{
    FILE   *           stream ;
    string *           lable  ;
    size_t             maximum;
    size_t             current;
    time_t             startup;
    progressbar_format format ;
    size_t             last_drawn_length;

}progressbar;


typedef struct corgasm_progresslib_functions
{
    progressbar * (*new_progressbar)(const char  * lable, size_t maximum);
    progressbar * (*destroy)        (progressbar * self);
    bool          (*increment)      (progressbar * self);
    bool          (*decrement)      (progressbar * self);
    bool          (*draw)           (progressbar * self);

}corgasm_progresslib_functions;


/**
 * @brief Allocates memory for a new progress bar.
 *          User is responsible for calling progresslib.finish
 *
 * @param[in] const char * lable The name of the progress bar
 * @param[in] size_t maximum     Size of the progress bar
 * @return    progressbar *      Pointer to a new progress bar
 *
 * @note This function is NULL safe
 * @warning This function can return NULL
 */
progressbar * corgasm_progresslib_new_progressbar(const char * lable, size_t maximum);


/**
 * @brief Deallocates memory of a progress bar
 *
 * @param[in] progressbar * self  pointer to a progress bar to destroy
 * @return    progressbar *
 *
 * @note This function is NULL safe
 * @warning Allways returns NULL
 */
progressbar * corgasm_progresslib_destroy(progressbar * self);


/**
 * @brief Increments the progress bar and draw it
 *
 * @param[in] progressbar * self pointer to a progressbar
 *
 * @return bool if operation was successful
 * @note This function is NULL safe
 */
bool corgasm_progresslib_increment(progressbar * self);


/**
 * @brief Decrements the progress bar and draw it
 *
 * @param[in] progressbar * self pointer to a progressbar
 *
 * @return bool if operation was successful
 * @note This function is NULL safe
 */
bool corgasm_progresslib_decrement(progressbar * self);


/**
 * @brief Draws progress bar
 *
 * @param[in] progressbar * self pointer to a progressbar
 *
 * @return bool if operation was successful
 * @note This function is NULL safe
 */
bool corgasm_progresslib_draw(progressbar * self);


static const corgasm_progresslib_functions progresslib = {
    .new_progressbar = corgasm_progresslib_new_progressbar,
    .destroy         = corgasm_progresslib_destroy,
    .increment       = corgasm_progresslib_increment,
    .decrement       = corgasm_progresslib_decrement,
    .draw            = corgasm_progresslib_draw,
};

#endif /* __CORGASM_PROGRESSLIB_H__ */
