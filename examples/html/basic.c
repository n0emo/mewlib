#include <stdio.h>

#define MEW_HTML_SHORT_TAGS

#include <mew/alloc.h>
#include <mew/str.h>
#include <mew/html.h>

// TODO: This example segfaults if compiled with GCC
int main() {
    StringBuilder sb = {0};
    sb.alloc = new_malloc_allocator();

    Html html = html_begin();

    h_head({
        html_title_cstr(&html, "My page");
    });

    h_body({
        h_h1({
             html_text_cstr(&html, "Hello, World!");
        });
    });

    html_end(&html);
    html_render_to_sb_and_free(&html, &sb);

    printf(SV_FMT, SV_ARG(sb));
}
