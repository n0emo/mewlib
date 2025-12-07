#include "mew/ini.h"

// TODO: nesting
bool parse_ini(StringView text, Ini *ini) {
    MewHashMapOptions map_options = {
        .alloc = ini->alloc,
        .user_data = ini,
        .hashfunc = mew_hashmap_sv_hash,
        .equals = mew_hashmap_sv_equals,
        .key_size = sizeof(StringView),
        .value_size = sizeof(IniSection),
    };
    IniSection *current = NULL;

    while (text.count > 0) {
        StringView line = sv_chop_by(&text, '\n');
        line = sv_chop_by(&line, ';');
        line = sv_trim_space(line);

        if (line.count == 0) {
            continue;
        } else if (line.items[0] == '[') {
            if (line.count < 3)
                return false;
            if (line.items[line.count - 1] != ']')
                return false;
            StringView section_name = sv_slice(line, 1, line.count - 2);
            section_name = sv_trim_space(section_name);

            IniSection section;
            MewHashMapOptions section_map_options = {
                .alloc = ini->alloc,
                .user_data = ini,
                .hashfunc = mew_hashmap_sv_hash,
                .equals = mew_hashmap_sv_equals,
                .key_size = sizeof(StringView),
                .value_size = sizeof(StringView),
            };
            mew_hashmap_init(&section.items, section_map_options);

            mew_hashmap_insert(&ini->sections, &section_name, &section);

            current = mew_hashmap_get(&ini->sections, &section_name);
        } else if (current == NULL) {
            return false;
        } else {
            StringView key = sv_trim_space(sv_chop_by(&line, '='));
            StringView value = sv_trim_space(line);

            if (value.count == 0) {
                return false;
            }

            mew_hashmap_insert(&current->items, &key, &value);
        }
    }

    return true;
}

IniSection *ini_get_section(Ini ini, StringView name) {
    return mew_hashmap_get(&ini.sections, &name);
}

StringView *ini_get_item(IniSection section, StringView name) {
    return mew_hashmap_get(&section.items, &name);
}
