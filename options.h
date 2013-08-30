/* Copyright (c) 2006-2013 Jonas Fonseca <fonseca@diku.dk>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include "tig.h"

#define OPTION_INFO \
	OPT_ENUM(GRAPHIC_ENUM, graphic, line_graphics, GRAPHIC_DEFAULT) \
	OPT_ENUM(DATE_ENUM, date, show_date, DATE_DEFAULT) \
	OPT_ENUM(AUTHOR_ENUM, author, show_author, AUTHOR_FULL) \
	OPT_ENUM(FILENAME_ENUM, filename, show_filename, FILENAME_AUTO) \
	OPT_ENUM(FILE_SIZE_ENUM, file_size, show_file_size, FILE_SIZE_DEFAULT) \
	OPT_BOOL(show_rev_graph, TRUE) \
	OPT_BOOL(show_line_numbers, FALSE) \
	OPT_BOOL(show_refs, TRUE) \
	OPT_BOOL(show_changes, TRUE) \
	OPT_BOOL(status_untracked_dirs, TRUE) \
	OPT_BOOL(read_git_colors, TRUE) \
	OPT_BOOL(wrap_lines, FALSE) \
	OPT_BOOL(ignore_case, FALSE) \
	OPT_BOOL(focus_child, TRUE) \
	OPT_INT(diff_context, 3) \
	OPT_ENUM(IGNORE_SPACE_ENUM, ignore_space, ignore_space, IGNORE_SPACE_NO) \
	OPT_ENUM(COMMIT_ORDER_ENUM, commit_order, commit_order, COMMIT_ORDER_DEFAULT) \
	OPT_BOOL(show_notes, TRUE) \
	OPT_STR(notes_arg, SIZEOF_STR, "--show-notes") \
	OPT_INT(line_number_interval, 5) \
	OPT_DOUBLE(horizontal_scroll, 0.50) \
	OPT_DOUBLE(split_view_height, 2.0 / 3.0) \
	OPT_DOUBLE(scale_vsplit_view, 0.5) \
	OPT_BOOL(vertical_split, FALSE) \
	OPT_INT(tab_size, 8) \
	OPT_INT(author_width, AUTHOR_WIDTH) \
	OPT_INT(filename_width, FILENAME_WIDTH) \
	OPT_STR(path, SIZEOF_STR, "") \
	OPT_STR(file, SIZEOF_STR, "") \
	OPT_STR(ref, SIZEOF_REF, "") \
	OPT_(unsigned long, goto_line, 0) \
	OPT_STR(search, SIZEOF_STR, "") \
	OPT_STR(editor, SIZEOF_STR, "") \
	OPT_BOOL(editor_line_number, TRUE) \
	OPT_ARGV(diff_options, NULL) \
	OPT_ARGV(rev_argv, NULL) \
	OPT_ARGV(file_argv, NULL) \
	OPT_ARGV(blame_options, NULL) \
	OPT_INT(lineno, 0) \
	OPT_BOOL(show_id, FALSE) \
	OPT_INT(id_width, ID_WIDTH) \
	OPT_BOOL(file_filter, TRUE) \
	OPT_BOOL(show_title_overflow, FALSE) \
	OPT_INT(title_overflow, 50) \

struct options {
#define OPT_ENUM(type, tname, name, value)	enum tname name;
#define OPT_BOOL(name, value)		bool name;
#define OPT_INT(name, value)		int name;
#define OPT_DOUBLE(name, value)		double name;
#define OPT_STR(name, size, value)	char name[size];
#define OPT_ARGV(name, value)		const char **name;
#define OPT_(type, name, value)		type name;
	OPTION_INFO
#undef OPT_ENUM
#undef OPT_BOOL
#undef OPT_INT
#undef OPT_DOUBLE
#undef OPT_STR
#undef OPT_ARGV
#undef OPT_
};

extern struct options opt;

int load_options(void);
int load_git_config(void);

enum status_code set_option(const char *opt, char *value);
enum status_code parse_int(int *opt, const char *arg, int min, int max);

#endif
/* vim: set ts=8 sw=8 noexpandtab: */
