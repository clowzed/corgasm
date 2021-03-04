#ifndef __CORGASM_MENULIB_H__
#define __CORGASM_MENULIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../listlib/listlib.h"
#include "../stringlib/stringlib.h"


typedef struct corgasm_menulib_choice
{
	size_t   key;
	string * message;
}choice;

typedef struct corgasm_menulib_menu
{
	string * header;
	string * format_string;
	list   * choices;
}menu;

/**
 * @brief Allocates memory for a new choice and adds it to menu
 * @param[in] menu * self pointer to menu
 * @param[in] size_t key for a new choice
 * @param[in] const char *
 *
 */
bool     corgasm_menulib_add_choice       (menu * self, size_t key, const char * message);
bool     corgasm_menulib_has_choice       (menu * self, size_t key);
choice * corgasm_menulib_ask_for_choice   (menu * self);
menu *   corgasm_menulib_new_menu         ();

bool     corgasm_menulib_set_format_string(menu * self, const char * format_string);
bool     corgasm_menulib_set_header       (menu * self, const char * header);
void     corgasm_menulib_clear            (menu * self);
void     corgasm_menulib_show             (menu * self);
menu *   corgasm_menulib_destroy          (menu * self);
size_t   corgasm_menulib_ask_for_key      (menu * self);


// * Utilities
choice * corgasm_menulib_new_choice(size_t key, const char * message);
choice * corgasm_menulib_destroy_choice(choice * ch);
void corgasm_menulib_destroy_choice_wrapped(void * ch);



typedef struct corgasm_menulib_functions
{
	menu *   (*new_menu)                    ();
	bool     (*add_choice)                  (menu * self, size_t key, const char * message);
	bool     (*has_choice)                  (menu * self, size_t key);
	void     (*clear)                       (menu * self);
	void     (*show)                        (menu * self);
	size_t   (*ask_for_key)                 (menu * self);
	choice * (*ask_for_choice)              (menu * self);
	bool     (*set_format_string)           (menu * self, const char * format_string);
	bool     (*set_header)                  (menu * self, const char * header);
	menu *   (*destroy)                     (menu * self);
} corgasm_menulib_functions;


static const corgasm_menulib_functions menulib = {
	.new_menu                     = corgasm_menulib_new_menu,
	.add_choice                   = corgasm_menulib_add_choice,
	.has_choice                   = corgasm_menulib_has_choice,
	.clear                        = corgasm_menulib_clear,
	.show                         = corgasm_menulib_show,
	.ask_for_key                  = corgasm_menulib_ask_for_key,
	.ask_for_choice               = corgasm_menulib_ask_for_choice,
	.set_format_string            = corgasm_menulib_set_format_string,
	.set_header                   = corgasm_menulib_set_header,
	.destroy                      = corgasm_menulib_destroy,
};


#endif /* __CORGASM_MENULIB_H__ */
