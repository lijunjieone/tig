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

#ifndef LINE_H
#define LINE_H

#include "tig.h"
#include "refs.h"

/*
 * Line-oriented content detection.
 */

#define LINE_INFO(_) \
_(DIFF_HEADER,		"diff--",		COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(DIFF_CHUNK,		"@@",			COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
_(DIFF_ADD,		"+",			COLOR_GREEN,	COLOR_DEFAULT,	0), \
_(DIFF_ADD2,		"+",			COLOR_GREEN,	COLOR_DEFAULT,	0), \
_(DIFF_DEL,		"-",			COLOR_RED,	COLOR_DEFAULT,	0), \
_(DIFF_DEL2,		"-",			COLOR_RED,	COLOR_DEFAULT,	0), \
_(DIFF_INDEX,		"index",		COLOR_BLUE,	COLOR_DEFAULT,	0), \
_(DIFF_OLDMODE,		"oldfile mode ",	COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(DIFF_NEWMODE,		"newfile mode ",	COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(DIFF_DELETED_FILE_MODE, \
			"deleted file mode ",	COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(DIFF_COPY_FROM,	"copyfrom ",		COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(DIFF_COPY_TO,		"copyto ",		COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(DIFF_RENAME_FROM,	"renamefrom ",		COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(DIFF_RENAME_TO,	"renameto ",		COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(DIFF_SIMILARITY,	"similarity ",		COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(DIFF_DISSIMILARITY,	"dissimilarity",	COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(DIFF_TREE,		"diff-tree",		COLOR_BLUE,	COLOR_DEFAULT,	0), \
_(PP_AUTHOR,		"Author: ",		COLOR_CYAN,	COLOR_DEFAULT,	0), \
_(PP_COMMIT,		"Commit: ",		COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
_(PP_MERGE,		"Merge: ",		COLOR_BLUE,	COLOR_DEFAULT,	0), \
_(PP_DATE,		"Date:   ",		COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(PP_ADATE,		"AuthorDate: ",		COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(PP_CDATE,		"CommitDate: ",		COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(PP_REFS,		"Refs: ",		COLOR_RED,	COLOR_DEFAULT,	0), \
_(PP_REFLOG,		"Reflog: ",		COLOR_RED,	COLOR_DEFAULT,	0), \
_(PP_REFLOGMSG,		"Reflog message: ",	COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(STASH,		"stash@{",		COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
_(COMMIT,		"commit ",		COLOR_GREEN,	COLOR_DEFAULT,	0), \
_(PARENT,		"parent ",		COLOR_BLUE,	COLOR_DEFAULT,	0), \
_(TREE,			"tree ",		COLOR_BLUE,	COLOR_DEFAULT,	0), \
_(AUTHOR,		"author ",		COLOR_GREEN,	COLOR_DEFAULT,	0), \
_(COMMITTER,		"committer ",		COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
_(SIGNOFF,		"    Signed-off-by",	COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(ACKED,		"    Acked-by",		COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(TESTED,		"    Tested-by",	COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(REVIEWED,		"    Reviewed-by",	COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(DEFAULT,		"",			COLOR_DEFAULT,	COLOR_DEFAULT,	A_NORMAL), \
_(CURSOR,		"",			COLOR_WHITE,	COLOR_GREEN,	A_BOLD), \
_(STATUS,		"",			COLOR_GREEN,	COLOR_DEFAULT,	0), \
_(DELIMITER,		"",			COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
_(DATE,			"",			COLOR_BLUE,	COLOR_DEFAULT,	0), \
_(MODE,			"",			COLOR_CYAN,	COLOR_DEFAULT,	0), \
_(ID,			"",			COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
_(OVERFLOW,		"",			COLOR_RED,	COLOR_DEFAULT,	0), \
_(FILENAME,		"",			COLOR_DEFAULT,	COLOR_DEFAULT,	0), \
_(FILE_SIZE,		"",			COLOR_DEFAULT,	COLOR_DEFAULT,	0), \
_(LINE_NUMBER,		"",			COLOR_CYAN,	COLOR_DEFAULT,	0), \
_(TITLE_BLUR,		"",			COLOR_WHITE,	COLOR_BLUE,	0), \
_(TITLE_FOCUS,		"",			COLOR_WHITE,	COLOR_BLUE,	A_BOLD), \
_(MAIN_COMMIT,		"",			COLOR_DEFAULT,	COLOR_DEFAULT,	0), \
_(MAIN_TAG,		"",			COLOR_MAGENTA,	COLOR_DEFAULT,	A_BOLD), \
_(MAIN_LOCAL_TAG,	"",			COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
_(MAIN_REMOTE,		"",			COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(MAIN_REPLACE,		"",			COLOR_CYAN,	COLOR_DEFAULT,	0), \
_(MAIN_TRACKED,		"",			COLOR_YELLOW,	COLOR_DEFAULT,	A_BOLD), \
_(MAIN_REF,		"",			COLOR_CYAN,	COLOR_DEFAULT,	0), \
_(MAIN_HEAD,		"",			COLOR_CYAN,	COLOR_DEFAULT,	A_BOLD), \
_(MAIN_REVGRAPH,	"",			COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
_(TREE_HEAD,		"",			COLOR_DEFAULT,	COLOR_DEFAULT,	A_BOLD), \
_(TREE_DIR,		"",			COLOR_YELLOW,	COLOR_DEFAULT,	A_NORMAL), \
_(TREE_FILE,		"",			COLOR_DEFAULT,	COLOR_DEFAULT,	A_NORMAL), \
_(STAT_HEAD,		"",			COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(STAT_SECTION,		"",			COLOR_CYAN,	COLOR_DEFAULT,	0), \
_(STAT_NONE,		"",			COLOR_DEFAULT,	COLOR_DEFAULT,	0), \
_(STAT_STAGED,		"",			COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
_(STAT_UNSTAGED,	"",			COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
_(STAT_UNTRACKED,	"",			COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
_(HELP_KEYMAP,		"",			COLOR_CYAN,	COLOR_DEFAULT,	0), \
_(HELP_GROUP,		"",			COLOR_BLUE,	COLOR_DEFAULT,	0), \
_(DIFF_STAT,		"",			COLOR_BLUE,	COLOR_DEFAULT,	0), \
_(PALETTE_0,		"",			COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
_(PALETTE_1,		"",			COLOR_YELLOW,	COLOR_DEFAULT,	0), \
_(PALETTE_2,		"",			COLOR_CYAN,	COLOR_DEFAULT,	0), \
_(PALETTE_3,		"",			COLOR_GREEN,	COLOR_DEFAULT,	0), \
_(PALETTE_4,		"",			COLOR_DEFAULT,	COLOR_DEFAULT,	0), \
_(PALETTE_5,		"",			COLOR_WHITE,	COLOR_DEFAULT,	0), \
_(PALETTE_6,		"",			COLOR_RED,	COLOR_DEFAULT,	0), \
_(GRAPH_COMMIT,		"",			COLOR_BLUE,	COLOR_DEFAULT,	0)

enum line_type {
#define DEFINE_LINE_ENUM(type, line, fg, bg, attr) LINE_##type
	LINE_INFO(DEFINE_LINE_ENUM),
	LINE_NONE
};

struct line_info {
	const char *name;	/* Option name. */
	int namelen;		/* Size of option name. */
	const char *line;	/* The start of line to match. */
	int linelen;		/* Size of string to match. */
	int fg, bg, attr;	/* Color and text attributes for the lines. */
	int color_pair;
};

struct line_info *get_line_info(enum line_type type);
struct line_info *get_line_info_from_name(const char *name);
enum line_type get_line_type(const char *line);
enum line_type get_line_type_from_ref(const struct ref *ref);
struct line_info *add_custom_color(const char *quoted_line);
int init_colors(void);

#define COLOR_ID(line_type)		((line_type) + 1)

static inline int
get_line_attr(enum line_type type)
{
	struct line_info *info = get_line_info(type);

	return COLOR_PAIR(COLOR_ID(info->color_pair)) | info->attr;
}

static inline int
get_line_color(enum line_type type)
{
	return COLOR_ID(get_line_info(type)->color_pair);
}

#endif
/* vim: set ts=8 sw=8 noexpandtab: */
