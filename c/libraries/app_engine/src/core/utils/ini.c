/*
 * =====================================================================================
 *
 *       Filename:  ini.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 14, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "app.h"
#include "ini.h"
#include "debug.h"

#define MAX_LINE        1024
#define MAX_SECTION     1024
#define MAX_NAME        1024

static char* rstrip(char* s)
{
    char* p = s + (strlen(s) - 1);
    while (p > s && ((*p == '\"') || (*p == '\n') || isspace(*p))) {
        *p = '\0';
        --p;
    }

    return s;
}

static char* lskip(const char* s)
{
    while (*s && (isspace(*s) || (*s == '\"'))) {
        s++;
    }
    return (char*)s;
}

static char* find_char_or_comment(const char* s, char c)
{
    int was_whitespace = 0;
    while (*s && *s != c && !(was_whitespace && *s == ';')) {
        was_whitespace = isspace(*s);
        s++;
    }
    return (char*)s;
}

static char* strncpy0(char* dest, const char* src, size_t size)
{
    strncpy(dest, src, size);
    dest[size - 1] = '\0';
    return dest;
}

int ini_parse_file(FILE* file,
        int (*handler)(void*, const char*, 
            const char*, const char*),
            void* user)
{
    char line[MAX_LINE];
    char section[MAX_SECTION] = "";
    char prev_name[MAX_NAME] = "";

    char* start;
    char* end;
    char* name;
    char* value;
    int lineno = 0;
    int error = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        lineno++;
        start = lskip(rstrip(line));

        if (*start == ';' || *start == '#') {
        } else if (*start == '[') {
            /* [section] line */
            end = find_char_or_comment(start + 1, ']');
            if (*end == ']') {
                *end = '\0';
                strncpy0(section, start + 1, sizeof(section));
                *prev_name = '\0';
            } else if (!error) {
                /* No ']' found */
                error = lineno;
            }
        } else if (*start && *start != ';') {
            end = find_char_or_comment(start, '=');
            if (*end != '=') {
                end = find_char_or_comment(start, ':');
            }
            if (*end == '=' || *end == ':') {
                *end = '\0';
                name = rstrip(start);
                value = lskip(end + 1);
                end = find_char_or_comment(value, '\0');
                if (*end == ';') {
                    *end = '\0';
                }
                rstrip(value);

                /* call handler */
                strncpy0(prev_name, name, sizeof(prev_name));
                if ((handler(user, section, name, value) < 0) && !error) {
                    error = lineno;
                }
            } else if (!error) {
                /* No '=' or ':' found */
                error = lineno;
            }
        }
    }

    return error;
}

int ini_parse(const char* filename,
              int (*handler)(void*, const char*, const char*, const char*),
              void* user)
{
    FILE* file;
    int error;

    file = fopen(filename, "r");
    if (!file) {
        DEBUG("ini_parse: fopen error\n");
        return -1;
    }
    error = ini_parse_file(file, handler, user);
    fclose(file);
    return error;
}

int ini_parse_file2(void *simple_ini,
        int (*handler)(void*, const char*, 
            const char*, const char*),
            void* user)
{
    int error = 0;

    CSimpleIniA *ini = static_cast<CSimpleIniA*>(simple_ini);

    CSimpleIniA::TNamesDepend sections;
    ini->GetAllSections(sections);

    CSimpleIniA::TNamesDepend::const_iterator s;
    for (s = sections.begin(); s != sections.end(); ++s) { 
        CSimpleIniA::TNamesDepend keys;
        ini->GetAllKeys(s->pItem, keys);

        CSimpleIniA::TNamesDepend::const_iterator k;
        for (k = keys.begin(); k != keys.end(); ++k) { 
            
            const char *pszValue = ini->GetValue(s->pItem,
                                                 k->pItem, 
                                                 NULL);

            DEBUG("section=%s, key=%s, value=%s\n",
                    s->pItem, k->pItem, pszValue);

            if (handler != NULL) {
                handler(user, s->pItem, k->pItem, pszValue);
            } else {
                error = -1;
            }
        }
    }

    return error;
}

int ini_parse2(const char* filename,
              int (*handler)(void*, const char*, const char*, const char*),
              void* user)
{
    int error = 0;

    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(filename);    

    error = ini_parse_file2(static_cast<void*>(&ini), handler, user);

    return error;
}
