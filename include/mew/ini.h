#ifndef MEW_INCLUDE_MEW_INI_H_
#define MEW_INCLUDE_MEW_INI_H_

#include <mew/core/alloc.h>
#include <mew/core/strings/sv.h>
#include <mew/core/utils.h>
#include <mew/containers/map/hashmap.h>

typedef struct {
    MewHashMap items;
} IniSection;

typedef struct {
    IniSection *items;
    size_t count;
    size_t capacity;
} IniSections;

typedef struct {
    Allocator alloc;
    MewHashMap sections;
} Ini;

bool parse_ini(StringView text, Ini *ini);
IniSection *ini_get_section(Ini ini, StringView name);
StringView *ini_get_item(IniSection section, StringView name);

#endif // MEW_INCLUDE_MEW_INI_H_
