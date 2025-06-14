#include <stdio.h>

#define MEW_HTML_SHORT_TAGS
#include <mew/html.h>
#include <mew/core.h>

int main(void) {
    Html h = html_begin();

    h_head(h, ht_title(h, "My page"));

    h_body(h, {
        h_nav(h, {
            // ht_attr inserts an attribute to the next element
            ht_attr(h, "href", "/");
            h_a(h, ht_text(h, "Main"));

            ht_attr(h, "href", "/about");
            h_a(h, ht_text(h, "About"));
        });

        // ht_class inserts a class to the next element
        ht_class(h, "main-content");
        h_div(h, {
            ht_class(h, "header-title");
            h_h1(h, ht_text(h, "Welcome to my page!"));
        });
    });

    html_end(&h);

    StringBuilder sb = {0};
    sb.alloc = new_malloc_allocator();
    html_render_to_sb_and_free(&h, &sb);
    printf(SV_FMT, SV_ARG(sb));

    return 0;
}
