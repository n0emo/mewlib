#include <stdio.h>

#define MEW_HTML_SHORT_TAGS
#include <mew/html.h>

// TODO: This example segfaults if compiled with GCC
int main() {
    Html h = html_begin();

    // Short tag macros with nesting support
    h_head(h, {
        // Insert a page title
        ht_title(h, "My page");
    });

    h_body(h, {
        // Insert text inside an element
        h_h1(h, ht_text(h, "Welcome to my page"));
    });

    html_end(&h);

    // Render HTML to string builder and print
    StringBuilder sb = {0};
    sb.alloc = new_malloc_allocator();
    html_render_to_sb_and_free(&h, &sb);
    printf(SV_FMT, SV_ARG(sb));

    return 0;
}
