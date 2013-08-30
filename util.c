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

#include "tig.h"
#include "util.h"

static const char *status_messages[] = {
#define STATUS_CODE_MESSAGE(name, msg) msg
	STATUS_CODE_INFO(STATUS_CODE_MESSAGE)
};

const char *
get_status_message(enum status_code code)
{
	if (code == SUCCESS)
		return "";
	return status_messages[code];
}

int
warn(const char *msg, ...)
{
	va_list args;

	va_start(args, msg);
	fputs("tig warning: ", stderr);
	vfprintf(stderr, msg, args);
	fputs("\n", stderr);
	va_end(args);
	return ERR;
}

die_fn die_callback = NULL;
void TIG_NORETURN
die(const char *err, ...)
{
	va_list args;

	if (die_callback)
		die_callback();

	va_start(args, err);
	fputs("tig: ", stderr);
	vfprintf(stderr, err, args);
	fputs("\n", stderr);
	va_end(args);

	exit(1);
}


#define ENUM_MAP_MACRO(prefix, name)	ENUM_MAP(#name, ENUM_SYM_MACRO(prefix, name))

#define DEFINE_ENUM_MAP(name, info) \
	const struct enum_map name##_map__[] = { info(ENUM_MAP_MACRO) }; \
	const struct enum_map_info name##_map[] = { { name##_map__, ARRAY_SIZE(name##_map__) } }

DEFINE_ENUM_MAP(author, AUTHOR_ENUM);
DEFINE_ENUM_MAP(commit_order, COMMIT_ORDER_ENUM);
DEFINE_ENUM_MAP(date, DATE_ENUM);
DEFINE_ENUM_MAP(file_size, FILE_SIZE_ENUM);
DEFINE_ENUM_MAP(filename, FILENAME_ENUM);
DEFINE_ENUM_MAP(graphic, GRAPHIC_ENUM);
DEFINE_ENUM_MAP(ignore_space, IGNORE_SPACE_ENUM);

int
string_enum_compare(const char *str1, const char *str2, int len)
{
	size_t i;

#define string_enum_sep(x) ((x) == '-' || (x) == '_' || (x) == '.')

	/* Diff-Header == DIFF_HEADER */
	for (i = 0; i < len; i++) {
		if (ascii_toupper(str1[i]) == ascii_toupper(str2[i]))
			continue;

		if (string_enum_sep(str1[i]) &&
		    string_enum_sep(str2[i]))
			continue;

		return str1[i] - str2[i];
	}

	return 0;
}

char *
enum_map_name(const char *name, size_t namelen)
{
	static char buf[SIZEOF_STR];
	int bufpos;

	for (bufpos = 0; bufpos <= namelen; bufpos++) {
		buf[bufpos] = ascii_tolower(name[bufpos]);
		if (buf[bufpos] == '_')
			buf[bufpos] = '-';
	}

	buf[bufpos] = 0;
	return buf;
}

bool
map_enum_do(const struct enum_map *map, size_t map_size, int *value, const char *name)
{
	size_t namelen = strlen(name);
	int i;

	for (i = 0; i < map_size; i++)
		if (enum_equals(map[i], name, namelen)) {
			*value = map[i].value;
			return TRUE;
		}

	return FALSE;
}


int
timecmp(const struct time *t1, const struct time *t2)
{
	return t1->sec - t2->sec;
}

const char *
mkdate(const struct time *time, enum date date)
{
	static char buf[DATE_WIDTH + 1];
	static const struct enum_map reldate[] = {
		{ "second", 1,			60 * 2 },
		{ "minute", 60,			60 * 60 * 2 },
		{ "hour",   60 * 60,		60 * 60 * 24 * 2 },
		{ "day",    60 * 60 * 24,	60 * 60 * 24 * 7 * 2 },
		{ "week",   60 * 60 * 24 * 7,	60 * 60 * 24 * 7 * 5 },
		{ "month",  60 * 60 * 24 * 30,	60 * 60 * 24 * 365 },
		{ "year",   60 * 60 * 24 * 365, 0 },
	};
	struct tm tm;

	if (!date || !time || !time->sec)
		return "";

	if (date == DATE_RELATIVE) {
		struct timeval now;
		time_t date = time->sec + time->tz;
		time_t seconds;
		int i;

		gettimeofday(&now, NULL);
		seconds = now.tv_sec < date ? date - now.tv_sec : now.tv_sec - date;
		for (i = 0; i < ARRAY_SIZE(reldate); i++) {
			if (seconds >= reldate[i].value && reldate[i].value)
				continue;

			seconds /= reldate[i].namelen;
			if (!string_format(buf, "%ld %s%s %s",
					   seconds, reldate[i].name,
					   seconds > 1 ? "s" : "",
					   now.tv_sec >= date ? "ago" : "ahead"))
				break;
			return buf;
		}
	}

	if (date == DATE_LOCAL) {
		time_t date = time->sec + time->tz;
		localtime_r(&date, &tm);
	}
	else {
		gmtime_r(&time->sec, &tm);
	}
	return strftime(buf, sizeof(buf), DATE_FORMAT, &tm) ? buf : NULL;
}

const char *
mkfilesize(unsigned long size, enum file_size format)
{
	static char buf[64 + 1];
	static const char relsize[] = {
		'B', 'K', 'M', 'G', 'T', 'P'
	};

	if (!format)
		return "";

	if (format == FILE_SIZE_UNITS) {
		const char *fmt = "%.0f%c";
		double rsize = size;
		int i;

		for (i = 0; i < ARRAY_SIZE(relsize); i++) {
			if (rsize > 1024.0 && i + 1 < ARRAY_SIZE(relsize)) {
				rsize /= 1024;
				continue;
			}

			size = rsize * 10;
			if (size % 10 > 0)
				fmt = "%.1f%c";

			return string_format(buf, fmt, rsize, relsize[i])
				? buf : NULL;
		}
	}

	return string_format(buf, "%ld", size) ? buf : NULL;
}

const struct ident unknown_ident = { "Unknown", "unknown@localhost" };

int
ident_compare(const struct ident *i1, const struct ident *i2)
{
	if (!i1 || !i2)
		return (!!i1) - (!!i2);
	if (!i1->name || !i2->name)
		return (!!i1->name) - (!!i2->name);
	return strcmp(i1->name, i2->name);
}

/* Parse author lines where the name may be empty:
 *	author  <email@address.tld> 1138474660 +0100
 */
void
parse_author_line(char *ident, const struct ident **author, struct time *time)
{
	char *nameend = strchr(ident, '<');
	char *emailend = strchr(ident, '>');
	const char *name, *email = "";

	if (nameend && emailend)
		*nameend = *emailend = 0;
	name = chomp_string(ident);
	if (nameend)
		email = chomp_string(nameend + 1);
	if (!*name)
		name = *email ? email : unknown_ident.name;
	if (!*email)
		email = *name ? name : unknown_ident.email;

	*author = get_author(name, email);

	/* Parse epoch and timezone */
	if (time && emailend && emailend[1] == ' ') {
		char *secs = emailend + 2;
		char *zone = strchr(secs, ' ');

		parse_timesec(time, secs);

		if (zone && strlen(zone) == STRING_SIZE(" +0700"))
			parse_timezone(time, zone + 1);
	}
}

static const char *
get_author_initials(const char *author)
{
	static char initials[AUTHOR_WIDTH * 6 + 1];
	size_t pos = 0;
	const char *end = strchr(author, '\0');

#define is_initial_sep(c) (isspace(c) || ispunct(c) || (c) == '@' || (c) == '-')

	memset(initials, 0, sizeof(initials));
	while (author < end) {
		unsigned char bytes;
		size_t i;

		while (author < end && is_initial_sep(*author))
			author++;

		bytes = utf8_char_length(author, end);
		if (bytes >= sizeof(initials) - 1 - pos)
			break;
		while (bytes--) {
			initials[pos++] = *author++;
		}

		i = pos;
		while (author < end && !is_initial_sep(*author)) {
			bytes = utf8_char_length(author, end);
			if (bytes >= sizeof(initials) - 1 - i) {
				while (author < end && !is_initial_sep(*author))
					author++;
				break;
			}
			while (bytes--) {
				initials[i++] = *author++;
			}
		}

		initials[i++] = 0;
	}

	return initials;
}

static const char *
get_email_user(const char *email)
{
	static char user[AUTHOR_WIDTH * 6 + 1];
	const char *end = strchr(email, '@');
	int length = end ? end - email : strlen(email);

	string_format(user, "%.*s%c", length, email, 0);
	return user;
}

const char *
mkauthor(const struct ident *ident, int cols, enum author author)
{
	bool trim = author_trim(cols);
	bool abbreviate = author == AUTHOR_ABBREVIATED || !trim;

	if (author == AUTHOR_NO || !ident)
		return "";
	if (author == AUTHOR_EMAIL && ident->email)
		return ident->email;
	if (author == AUTHOR_EMAIL_USER && ident->email)
		return get_email_user(ident->email);
	if (abbreviate && ident->name)
		return get_author_initials(ident->name);
	return ident->name;
}

const char *
mkmode(mode_t mode)
{
	if (S_ISDIR(mode))
		return "drwxr-xr-x";
	else if (S_ISLNK(mode))
		return "lrwxrwxrwx";
	else if (S_ISGITLINK(mode))
		return "m---------";
	else if (S_ISREG(mode) && mode & S_IXUSR)
		return "-rwxr-xr-x";
	else if (S_ISREG(mode))
		return "-rw-r--r--";
	else
		return "----------";
}

DEFINE_ALLOCATOR(realloc_authors, struct ident *, 256)

struct ident *
get_author(const char *name, const char *email)
{
	static struct ident **authors;
	static size_t authors_size;
	int from = 0, to = authors_size - 1;
	struct ident *ident;

	while (from <= to) {
		size_t pos = (to + from) / 2;
		int cmp = strcmp(name, authors[pos]->name);

		if (!cmp)
			return authors[pos];

		if (cmp < 0)
			to = pos - 1;
		else
			from = pos + 1;
	}

	if (!realloc_authors(&authors, authors_size, 1))
		return NULL;
	ident = calloc(1, sizeof(*ident));
	if (!ident)
		return NULL;
	ident->name = strdup(name);
	ident->email = strdup(email);
	if (!ident->name || !ident->email) {
		free((void *) ident->name);
		free(ident);
		return NULL;
	}

	memmove(authors + from + 1, authors + from, (authors_size - from) * sizeof(*authors));
	authors[from] = ident;
	authors_size++;

	return ident;
}

void
parse_timesec(struct time *time, const char *sec)
{
	time->sec = (time_t) atol(sec);
}

void
parse_timezone(struct time *time, const char *zone)
{
	long tz;

	tz  = ('0' - zone[1]) * 60 * 60 * 10;
	tz += ('0' - zone[2]) * 60 * 60;
	tz += ('0' - zone[3]) * 60 * 10;
	tz += ('0' - zone[4]) * 60;

	if (zone[0] == '-')
		tz = -tz;

	time->tz = tz;
	time->sec -= tz;
}

/* vim: set ts=8 sw=8 noexpandtab: */
