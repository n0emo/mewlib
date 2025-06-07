#include <stdio.h>

#define MEW_HTML_SHORT_TAGS

#include <mew/alloc.h>
#include <mew/str.h>
#include <mew/html.h>

// TODO: This example segfaults if compiled with GCC
int main() {
    StringBuilder sb = {0};
    sb.alloc = new_malloc_allocator();

    Html h = html_begin();

    h_head(h, {
        html_title_cstr(&h, "My page");
    });

    h_body(h, {
        h_h1(h, {
             html_text_cstr(&h, "Hello, World!");
        });
    });

    html_end(&h);
    html_render_to_sb_and_free(&h, &sb);

    printf(SV_FMT, SV_ARG(sb));
}
