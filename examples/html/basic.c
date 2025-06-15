#include <stdio.h>

#define MEW_HTML_SHORT_TAGS
#include <mew/html.h>
#include <mew/core.h>

int main(void) {
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
    StringBuilder sb = sb_new_default();
    html_render_to_sb_and_free(&h, &sb);
    printf(SB_FMT, SB_ARG(sb));

    return 0;
}
