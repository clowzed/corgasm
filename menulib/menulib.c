#include "menulib.h"

bool corgasm_menulib_has_choice(menu * self, size_t key)
{
	bool was_found = false;
	if (self)
	{
		foreach(choice *, current_choice, self->choices)
			was_found = current_choice->key == key;
	}
	return was_found;
}




/*
@brief This reads menu from file
@param[in] menu * menu_ pointer to structure of menu
@param[in] const char * filename name of the file to load load_from
@return menu_error_code code of error
*/

bool corgasm_menulib_load_from(menu * self, const char * filename)
{
	bool    was_loaded_correctly = false;
	string * message_buffer     = stringlib.new_string();
	size_t key_buffer           = 0;

	FILE * menu_file = fopen(filename, "r");

	if (menu_file && message_buffer)
	{
		while (true && was_loaded_correctly)
		{
			stringlib.clear(message_buffer);
			key_buffer = 0;

			int scanf_result = fscanf(menu_file, "%zu", &key_buffer);
			if (scanf_result == 1)
			{
				char c = '\0';
				while (c = fgetc(menu_file) != EOF)
				{
					if (c == '`')
						break;
					else
						stringlib.add_char(message_buffer, c);
				}
				was_loaded_correctly = menulib.add_choice(self, key_buffer, stringlib.extract(message_buffer));
			}
			else
				was_loaded_correctly = false;
		}
	}
	stringlib.destroy(message_buffer);
	fclose(menu_file);

	return was_loaded_correctly;
}

choice * corgasm_menulib_new_choice(size_t key, const char * message)
{
	choice * new_choice = NULL;
	if (message)
	{
		new_choice = malloc(sizeof(choice));
		if (new_choice)
		{
			new_choice->message = stringlib.new_string_from(message);
			new_choice->key = key;
			if (!new_choice->message)
				new_choice = corgasm_menulib_destroy_choice(new_choice);
		}
	}
	return new_choice;
}

bool corgasm_menulib_add_choice(menu * self, size_t key, const char * message)
{
	bool was_added = false;
	if (self && message)
	{
		if (!menulib.has_choice(self, key))
		{
			choice * new_choice = corgasm_menulib_new_choice(key, message);

			if (new_choice)
				was_added = listlib.append(self->choices, (void*)new_choice);

			if (!was_added)
				new_choice = corgasm_menulib_destroy_choice(new_choice);
		}
	}
	return was_added;
}

void corgasm_menulib_remove_choice(menu * self, size_t key)
{
	if (self && menulib.has_choice(self, key))
	{
		choice * choice_to_remove = NULL;
		choice * current_choice   = NULL;

		foreach_condition(choice *, current_choice, self->choices, !choice_to_remove)
			if (current_choice && current_choice->key == key) choice_to_remove  = current_choice;

		// ! MAKE REMOVE IN LISTLIB
		//if (choice_to_remove)
		//listlib.remove((void*)choice_to_remove);
	}
}

void corgasm_menulib_clear(menu * self)
{
	if (self)
		listlib.clear(self->choices);
}

void corgasm_menulib_show_choice(menu * self, choice * choice_to_show)
{
	if (choice_to_show)
	{
		fprintf(stdout,
			stringlib.extract(self->format_string),
			                  choice_to_show->key,
			stringlib.extract(choice_to_show->message));
	}
}

void corgasm_menulib_show(menu * self)
{
	if (self && self->choices)
	{
		fprintf(stdout, "%s\n", stringlib.extract(self->header));

		foreach(choice *, current_choice, self->choices)
			corgasm_menulib_show_choice(self, current_choice);
	}
}

choice * corgasm_menulib_ask_for_choice(menu * self)
{
	menulib.show(self);

	char     user_input_buffer[10] = "\0";
	size_t   key_buffer             = 0;
	choice * found_choice           = NULL;

	while (true)
	{
		fprintf(stdout, "Enter your choice > ");

		if (fgets(user_input_buffer, 10, stdin))
		{
			int sscanf_res = sscanf(user_input_buffer, "%zu", &key_buffer);

			// ! Can cause UB
			fflush(stdin);

			if (sscanf_res)
			{
				choice * current_choice = NULL;

				foreach_condition(choice *, current_choice, self->choices, !found_choice)
					if (current_choice->key == key_buffer) found_choice = current_choice;

				if (!found_choice)
					fprintf(stdout, "Incorrect key was given! Try again!\n");
				else
					break;
			}
			else
				fprintf(stdout, "Incorrect input! Try again!\n");
		}
	}
	return found_choice;
}

size_t corgasm_menulib_ask_for_key(menu * self)
{
	choice * given_choice = menulib.ask_for_choice(self);
	return given_choice->key;
}

bool corgasm_menulib_set_header(menu * self, const char * header)
{
	bool was_setted = false;
	if (self && header)
	{
		stringlib.destroy(self->header);
		self->header = stringlib.new_string_from(header);
		was_setted = self->header != NULL;
	}
	return was_setted;
}

bool corgasm_menulib_set_format_string(menu * self, const char * format_string)
{
	bool was_setted = false;
	if (self && format_string)
	{
		stringlib.destroy(self->format_string);
		self->format_string = stringlib.new_string_from(format_string);
		was_setted = self->format_string != NULL;
	}
	return was_setted;
}

menu * corgasm_menulib_destroy(menu * self)
{
	if (self)
	{
		menulib.clear(self);
		stringlib.destroy(self->format_string);
		stringlib.destroy(self->header);
		free(self);
	}
	return NULL;
}

menu * corgasm_menulib_new_menu()
{
	menu * new_menu = malloc(sizeof(menu));
	if (new_menu)
	{
		new_menu->choices       = listlib.new_list(corgasm_menulib_destroy_choice_wrapped);
		new_menu->header        = stringlib.new_string();
		new_menu->format_string = stringlib.new_string();

		if (!new_menu->choices       ||\
			!new_menu->header        ||\
			!new_menu->format_string)
			new_menu = menulib.destroy(new_menu);
		else
		{
			bool correctly_setted = (menulib.set_format_string(new_menu, "[%d] - %s\n") ||\
									 menulib.set_header(new_menu, "Welcome to menu!\nPlease, type number of your choice.\n"));
			if (!correctly_setted)
				new_menu = menulib.destroy(new_menu);
		}
	}
	return new_menu;
}

choice * corgasm_menulib_destroy_choice(choice * ch)
{
	if (ch)
	{
		stringlib.destroy(ch->message);
		free(ch);
	}
	return NULL;
}

void corgasm_menulib_destroy_choice_wrapped(void * ch)
{
	if (ch)
		corgasm_menulib_destroy_choice((choice *) ch);
}