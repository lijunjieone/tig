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

#ifndef TIG_UTIL_H
#define TIG_UTIL_H

#include "tig.h"

/*
 * Error handling.
 */

#define STATUS_CODE_INFO(_) \
	_(INTEGER_VALUE_OUT_OF_BOUND, "Integer value out of bound"), \
	_(INVALID_STEP_VALUE, "Invalid step value"), \
	_(NO_OPTION_VALUE, "No option value"), \
	_(NO_VALUE_ASSIGNED, "No value assigned"), \
	_(OBSOLETE_REQUEST_NAME, "Obsolete request name"), \
	_(OUT_OF_MEMORY, "Out of memory"), \
	_(TOO_MANY_OPTION_ARGUMENTS, "Too many option arguments"), \
	_(FILE_DOES_NOT_EXIST, "File does not exist"), \
	_(UNKNOWN_ATTRIBUTE, "Unknown attribute"), \
	_(UNKNOWN_COLOR, "Unknown color"), \
	_(UNKNOWN_COLOR_NAME, "Unknown color name"), \
	_(UNKNOWN_KEY, "Unknown key"), \
	_(UNKNOWN_KEY_MAP, "Unknown key map"), \
	_(UNKNOWN_OPTION_COMMAND, "Unknown option command"), \
	_(UNKNOWN_REQUEST_NAME, "Unknown request name"), \
	_(UNKNOWN_VARIABLE_NAME, "Unknown variable name"), \
	_(UNMATCHED_QUOTATION, "Unmatched quotation"), \
	_(WRONG_NUMBER_OF_ARGUMENTS, "Wrong number of arguments"), \
	_(HOME_UNRESOLVABLE, "HOME environment variable could not be resolved"),

enum status_code {
	SUCCESS,
#define STATUS_CODE_ENUM(name, msg) ERROR_ ## name
	STATUS_CODE_INFO(STATUS_CODE_ENUM)
};

const char *get_status_message(enum status_code code);

typedef void (*die_fn)(void);
extern die_fn die_callback;
void TIG_NORETURN die(const char *err, ...) PRINTF_LIKE(1, 2);
int warn(const char *msg, ...) PRINTF_LIKE(1, 2);

/*
 * Enumerations
 */

struct enum_map {
	const char *name;
	int namelen;
	int value;
};

struct enum_map_info {
	const struct enum_map *map;
	const int size;
};

#define ENUM_MAP(name, value) { name, STRING_SIZE(name), value }

#define ENUM_SYM_MACRO(prefix, name)	prefix##_##name

#define DEFINE_ENUM(name, info) \
	enum name { info(ENUM_SYM_MACRO) }; \
	extern const struct enum_map_info name##_map[]

int string_enum_compare(const char *str1, const char *str2, int len);

#define enum_equals(entry, str, len) \
	((entry).namelen == (len) && !string_enum_compare((entry).name, str, len))

char *enum_map_name(const char *name, size_t namelen);

#define enum_name(entry) enum_map_name((entry).name, (entry).namelen)

bool map_enum_do(const struct enum_map *map, size_t map_size, int *value, const char *name);

#define map_enum(attr, map, name) \
	map_enum_do(map, ARRAY_SIZE(map), attr, name)

#define AUTHOR_ENUM(_) \
	_(AUTHOR, NO), \
	_(AUTHOR, FULL), \
	_(AUTHOR, ABBREVIATED), \
	_(AUTHOR, EMAIL), \
	_(AUTHOR, EMAIL_USER)

DEFINE_ENUM(author, AUTHOR_ENUM);

#define COMMIT_ORDER_ENUM(_) \
	_(COMMIT_ORDER, DEFAULT), \
	_(COMMIT_ORDER, TOPO), \
	_(COMMIT_ORDER, DATE), \
	_(COMMIT_ORDER, REVERSE)

DEFINE_ENUM(commit_order, COMMIT_ORDER_ENUM);

#define DATE_ENUM(_) \
	_(DATE, NO), \
	_(DATE, DEFAULT), \
	_(DATE, LOCAL), \
	_(DATE, RELATIVE), \
	_(DATE, SHORT)

DEFINE_ENUM(date, DATE_ENUM);

#define FILE_SIZE_ENUM(_) \
	_(FILE_SIZE, NO), \
	_(FILE_SIZE, DEFAULT), \
	_(FILE_SIZE, UNITS)

DEFINE_ENUM(file_size, FILE_SIZE_ENUM);

#define FILENAME_ENUM(_) \
	_(FILENAME, NO), \
	_(FILENAME, ALWAYS), \
	_(FILENAME, AUTO)

DEFINE_ENUM(filename, FILENAME_ENUM);

#define GRAPHIC_ENUM(_) \
	_(GRAPHIC, ASCII), \
	_(GRAPHIC, DEFAULT), \
	_(GRAPHIC, UTF_8)

DEFINE_ENUM(graphic, GRAPHIC_ENUM);

#define IGNORE_SPACE_ENUM(_) \
	_(IGNORE_SPACE, NO), \
	_(IGNORE_SPACE, ALL), \
	_(IGNORE_SPACE, SOME), \
	_(IGNORE_SPACE, AT_EOL)

DEFINE_ENUM(ignore_space, IGNORE_SPACE_ENUM);

/*
 * Git data formatters and parsers.
 */

struct time {
	time_t sec;
	int tz;
};

struct ident {
	const char *name;
	const char *email;
};

/* Small author cache to reduce memory consumption. It uses binary
 * search to lookup or find place to position new entries. No entries
 * are ever freed. */
struct ident *get_author(const char *name, const char *email);

extern const struct ident unknown_ident;
#define author_trim(cols) (cols == 0 || cols > 10)

const char *mkdate(const struct time *time, enum date date);
const char *mkfilesize(unsigned long size, enum file_size format);
const char *mkauthor(const struct ident *ident, int cols, enum author author);
const char *mkmode(mode_t mode);

int timecmp(const struct time *t1, const struct time *t2);
int ident_compare(const struct ident *i1, const struct ident *i2);

void parse_timesec(struct time *time, const char *sec);
void parse_timezone(struct time *time, const char *zone);
void parse_author_line(char *ident, const struct ident **author, struct time *time);

#endif
/* vim: set ts=8 sw=8 noexpandtab: */
