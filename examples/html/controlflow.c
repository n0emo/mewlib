#include <stdio.h>

#define MEW_HTML_SHORT_TAGS
#include <mew/html.h>
#include <mew/core.h>

int main(void) {
    Html h = html_begin();

    int age = 20;
    const char *partners[] = {"MaxOil", "TurboEngine", "HABUDI"};

    h_head(h, ht_title(h, "Autoschool"));

    h_body(h, {
        h_h1(h, ht_text(h, "Autoschool"));

        // Conditions
        if (age < 16) {
            h_p(h, ht_text(h, "You are not allowed to drive a car"));
        } else {
            h_p(h, ht_text(h, "Welcome to our autoschool!"));
        }

        // Loops
        h_h2(h, ht_text(h, "Our partners:"));
        h_ul(h, {
            size_t len = (sizeof partners / sizeof partners[0]);
            for (size_t i = 0; i < len; i++) {
                h_li(h, ht_text(h, partners[i]));
            }
        });
    });

    StringBuilder sb = {0};
    sb_default(&sb);
    html_render_to_sb_and_free(&h, &sb);
    printf(SB_FMT, SB_ARG(sb));

    return 0;
}
