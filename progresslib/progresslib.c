#include "progresslib.h"

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
progressbar * corgasm_progresslib_new_progressbar(const char * lable, size_t maximum)
{
    progressbar * self = NULL;

    if (lable && strlen(lable) <= lable_max_length)
    {
        self = (progressbar *)(malloc(sizeof(progressbar)));

        if (self)
        {
            self->current           = 0;
            self->format            = progressbar_default_format;
            self->lable             = stringlib.new_string_from(lable);
            self->maximum           = maximum;
            self->startup           = time(NULL);
            self->stream            = stdout;
            self->last_drawn_length = 0;
        }
        if (self && !self->lable)
            self = progresslib.destroy(self);
    }

    return self;
}

/**
 * @brief Increments the progress bar and draw it
 *
 * @param[in] progressbar * self pointer to a progressbar
 *
 * @return bool if operation was successful
 * @note This function is NULL safe
 */
bool corgasm_progresslib_increment(progressbar * self)
{
    bool was_incremented = false;

    if (self)
    {
        self->current++;
        was_incremented = progresslib.draw(self);
    }

    return was_incremented;
}

/**
 * @brief Decrements the progress bar and draw it
 *
 * @param[in] progressbar * self pointer to a progressbar
 *
 * @return bool if operation was successful
 * @note This function is NULL safe
 */
bool corgasm_progresslib_decrement(progressbar * self)
{
    bool was_decremented = false;

    if (self && self->current)
    {
        self->current--;
        was_decremented = progresslib.draw(self);
    }

    return was_decremented;
}

/**
 * @brief Deallocates memory of a progress bar
 *
 * @param[in] progressbar * self  pointer to a progress bar to destroy
 * @return    progressbar *
 *
 * @note    This function is NULL safe
 * @warning Allways returns NULL
 */
progressbar * corgasm_progresslib_destroy(progressbar * self)
{
    if (self)
    {
        stringlib.destroy(self->lable);
        free(self);
    }
    return NULL;
}

/**
 * @brief Draws progress bar
 *
 * @param[in] progressbar * self pointer to a progressbar
 *
 * @return bool if operation was successful
 * @note This function is NULL safe
 */
bool corgasm_progresslib_draw(progressbar * self)
{
    bool was_drawed = false;

    if (self)
    {
        char buffer[1024] = "\0";
        snprintf(buffer, sizeof(buffer), "[%u/%u] ", self->current, self->maximum);
        string * progress = stringlib.new_string_from(buffer);

        size_t   loading_bar_lenght = max_terminal_length - self->lable->length - 3 - strlen(progress->data);
        size_t   filled             = loading_bar_lenght * ((double)self->current / self->maximum);
        size_t   empty              = loading_bar_lenght - filled;

        string * line_buffer = stringlib.new_string();
                 line_buffer = stringlib.concat(line_buffer,   self->lable);
                 line_buffer = stringlib.add_char(line_buffer, ' ');
                 line_buffer = stringlib.concat(line_buffer, progress);
                 line_buffer = stringlib.add_char(line_buffer, self->format.begin);


        for (size_t _ = 0; _ < filled; _++)
            line_buffer = stringlib.add_char(line_buffer, self->format.fill);

        for (size_t _ = 0; _ < empty; _++)
            line_buffer = stringlib.add_char(line_buffer, self->format.empty);

        line_buffer = stringlib.add_char(line_buffer, self->format.end);

        fputc(go_back, self->stream);

        fprintf(self->stream, stringlib.extract(line_buffer));

        self->last_drawn_length = line_buffer->length;

        stringlib.destroy(line_buffer);
        stringlib.destroy(progress);
        was_drawed = true;
    }

    return was_drawed;
}


#ifdef LIB_BUILD_PROGRESSLIB

int main()
{
    int max_hellos = 100;
    progressbar * progress = progresslib.new_progressbar("Hello counter", max_hellos);

    for (int i = 0; i < max_hellos; i++)
        progresslib.increment(progress);

    progress = progresslib.destroy(progress);
    return 0;
}

#endif