#ifndef MEW_INCLUDE_MEW_HTML_H_
#define MEW_INCLUDE_MEW_HTML_H_

#include <mew/core/strings.h>
#include <mew/core/utils.h>
#include <mew/core/allocators/arena.h>
#include <mew/containers/vector.h>

typedef struct {
    StringView name;
    StringView value;
} Attribute;

typedef struct {
    Attribute *items;
    size_t count;
    size_t capacity;
} Attributes;

typedef struct {
    StringView *items;
    size_t count;
    size_t capacity;
} Classes;

typedef struct {
    Arena arena;
    Allocator alloc;
    StringBuilder sb;
    MewVector attributes;
    MewVector classes;
    size_t indentation;
} Html;

void html_tag_begin(Html *html, const char *tag);
void html_tag_end(Html *html, const char *tag);
void html_tag_short(Html *html, const char *tag);
void html_push_attribute(Html *html, Attribute attribute);
void html_push_attribute_cstrs(Html *html, const char *name, const char *value);
void html_push_class(Html *html, StringView cls);
void html_push_class_cstr(Html *html, const char *cls);
void html_text_cstr(Html *html, const char *text);
void html_text(Html *html, StringView sv);
void html_render_to_sb_and_free(Html *html, StringBuilder *sb);

Html html_begin(void);
void html_end(Html *html);

void html_image(Html *html, StringView src);
void html_hyperlink(Html *html, StringView text, StringView href);
void html_hyperlink_cstr(Html *html, const char *text, const char *href);
void html_title(Html *html, StringView title);
void html_title_cstr(Html *html, const char *title);

#define HTML_TAG_LIST                                                                                                  \
    X(a)                                                                                                               \
    X(abbr)                                                                                                            \
    X(acronym)                                                                                                         \
    X(address)                                                                                                         \
    X(applet)                                                                                                          \
    X(area)                                                                                                            \
    X(article)                                                                                                         \
    X(aside)                                                                                                           \
    X(audio)                                                                                                           \
    X(b)                                                                                                               \
    X(base)                                                                                                            \
    X(basefont)                                                                                                        \
    X(bdi)                                                                                                             \
    X(bdo)                                                                                                             \
    X(big)                                                                                                             \
    X(blockquote)                                                                                                      \
    X(body)                                                                                                            \
    X(br)                                                                                                              \
    X(button)                                                                                                          \
    X(canvas)                                                                                                          \
    X(caption)                                                                                                         \
    X(center)                                                                                                          \
    X(cite)                                                                                                            \
    X(code)                                                                                                            \
    X(col)                                                                                                             \
    X(colgroup)                                                                                                        \
    X(data)                                                                                                            \
    X(datalist)                                                                                                        \
    X(dd)                                                                                                              \
    X(del)                                                                                                             \
    X(details)                                                                                                         \
    X(dfn)                                                                                                             \
    X(dialog)                                                                                                          \
    X(dir)                                                                                                             \
    X(div)                                                                                                             \
    X(dl)                                                                                                              \
    X(dt)                                                                                                              \
    X(em)                                                                                                              \
    X(embed)                                                                                                           \
    X(fieldset)                                                                                                        \
    X(figcaption)                                                                                                      \
    X(figure)                                                                                                          \
    X(font)                                                                                                            \
    X(footer)                                                                                                          \
    X(form)                                                                                                            \
    X(frame)                                                                                                           \
    X(frameset)                                                                                                        \
    X(h1)                                                                                                              \
    X(h2)                                                                                                              \
    X(h3)                                                                                                              \
    X(h4)                                                                                                              \
    X(h5)                                                                                                              \
    X(h6)                                                                                                              \
    X(head)                                                                                                            \
    X(header)                                                                                                          \
    X(hgroup)                                                                                                          \
    X(hr)                                                                                                              \
    X(i)                                                                                                               \
    X(iframe)                                                                                                          \
    X(img)                                                                                                             \
    X(input)                                                                                                           \
    X(ins)                                                                                                             \
    X(kbd)                                                                                                             \
    X(label)                                                                                                           \
    X(legend)                                                                                                          \
    X(li)                                                                                                              \
    X(link)                                                                                                            \
    X(main)                                                                                                            \
    X(map)                                                                                                             \
    X(mark)                                                                                                            \
    X(menu)                                                                                                            \
    X(meta)                                                                                                            \
    X(meter)                                                                                                           \
    X(nav)                                                                                                             \
    X(noframes)                                                                                                        \
    X(noscript)                                                                                                        \
    X(object)                                                                                                          \
    X(ol)                                                                                                              \
    X(optgroup)                                                                                                        \
    X(option)                                                                                                          \
    X(output)                                                                                                          \
    X(p)                                                                                                               \
    X(param)                                                                                                           \
    X(picture)                                                                                                         \
    X(pre)                                                                                                             \
    X(progress)                                                                                                        \
    X(q)                                                                                                               \
    X(rp)                                                                                                              \
    X(rt)                                                                                                              \
    X(ruby)                                                                                                            \
    X(s)                                                                                                               \
    X(samp)                                                                                                            \
    X(script)                                                                                                          \
    X(search)                                                                                                          \
    X(section)                                                                                                         \
    X(select)                                                                                                          \
    X(small)                                                                                                           \
    X(source)                                                                                                          \
    X(span)                                                                                                            \
    X(strike)                                                                                                          \
    X(strong)                                                                                                          \
    X(style)                                                                                                           \
    X(sub)                                                                                                             \
    X(summary)                                                                                                         \
    X(sup)                                                                                                             \
    X(svg)                                                                                                             \
    X(table)                                                                                                           \
    X(tbody)                                                                                                           \
    X(td)                                                                                                              \
    X(template)                                                                                                        \
    X(textarea)                                                                                                        \
    X(tfoot)                                                                                                           \
    X(th)                                                                                                              \
    X(thead)                                                                                                           \
    X(time)                                                                                                            \
    X(title)                                                                                                           \
    X(tr)                                                                                                              \
    X(track)                                                                                                           \
    X(tt)                                                                                                              \
    X(u)                                                                                                               \
    X(ul)                                                                                                              \
    X(var)                                                                                                             \
    X(video)                                                                                                           \
    X(wbr)

#define html_element(html, element, ...)                                                                               \
    do {                                                                                                               \
        html_##element##_begin(html);                                                                                  \
        __VA_ARGS__;                                                                                                   \
        html_##element##_end(html);                                                                                    \
    } while (0)

#define X(tag)                                                                                                         \
    void html_##tag##_begin(Html *html);                                                                               \
    void html_##tag##_end(Html *html);                                                                                 \
    void html_##tag##_short(Html *html);
HTML_TAG_LIST
#undef X

#define MEW_HTML_AUTOPTR(html) _Generic((html), Html *: html, const Html *: html, Html: &html)

#define MEW_HTML_TEXT_AUTOSV(text)                                                                                     \
    _Generic((text), const char *: cstr_to_sv, char *: cstr_to_sv, StringBuilder: sb_to_sv, default: sv_identity)(text)

#ifdef MEW_HTML_SHORT_TAGS
    #define h_a(html, ...) html_element(MEW_HTML_AUTOPTR(html), a, __VA_ARGS__)
    #define h_abbr(html, ...) html_element(MEW_HTML_AUTOPTR(html), abbr, __VA_ARGS__)
    #define h_acronym(html, ...) html_element(MEW_HTML_AUTOPTR(html), acronym, __VA_ARGS__)
    #define h_address(html, ...) html_element(MEW_HTML_AUTOPTR(html), address, __VA_ARGS__)
    #define h_applet(html, ...) html_element(MEW_HTML_AUTOPTR(html), applet, __VA_ARGS__)
    #define h_area(html, ...) html_element(MEW_HTML_AUTOPTR(html), area, __VA_ARGS__)
    #define h_article(html, ...) html_element(MEW_HTML_AUTOPTR(html), article, __VA_ARGS__)
    #define h_aside(html, ...) html_element(MEW_HTML_AUTOPTR(html), aside, __VA_ARGS__)
    #define h_audio(html, ...) html_element(MEW_HTML_AUTOPTR(html), audio, __VA_ARGS__)
    #define h_b(html, ...) html_element(MEW_HTML_AUTOPTR(html), b, __VA_ARGS__)
    #define h_base(html, ...) html_element(MEW_HTML_AUTOPTR(html), base, __VA_ARGS__)
    #define h_basefont(html, ...) html_element(MEW_HTML_AUTOPTR(html), basefont, __VA_ARGS__)
    #define h_bdi(html, ...) html_element(MEW_HTML_AUTOPTR(html), bdi, __VA_ARGS__)
    #define h_bdo(html, ...) html_element(MEW_HTML_AUTOPTR(html), bdo, __VA_ARGS__)
    #define h_big(html, ...) html_element(MEW_HTML_AUTOPTR(html), big, __VA_ARGS__)
    #define h_blockquote(html, ...) html_element(MEW_HTML_AUTOPTR(html), blockquote, __VA_ARGS__)
    #define h_body(html, ...) html_element(MEW_HTML_AUTOPTR(html), body, __VA_ARGS__)
    #define h_br(html, ...) html_element(MEW_HTML_AUTOPTR(html), br, __VA_ARGS__)
    #define h_button(html, ...) html_element(MEW_HTML_AUTOPTR(html), button, __VA_ARGS__)
    #define h_canvas(html, ...) html_element(MEW_HTML_AUTOPTR(html), canvas, __VA_ARGS__)
    #define h_caption(html, ...) html_element(MEW_HTML_AUTOPTR(html), caption, __VA_ARGS__)
    #define h_center(html, ...) html_element(MEW_HTML_AUTOPTR(html), center, __VA_ARGS__)
    #define h_cite(html, ...) html_element(MEW_HTML_AUTOPTR(html), cite, __VA_ARGS__)
    #define h_code(html, ...) html_element(MEW_HTML_AUTOPTR(html), code, __VA_ARGS__)
    #define h_col(html, ...) html_element(MEW_HTML_AUTOPTR(html), col, __VA_ARGS__)
    #define h_colgroup(html, ...) html_element(MEW_HTML_AUTOPTR(html), colgroup, __VA_ARGS__)
    #define h_data(html, ...) html_element(MEW_HTML_AUTOPTR(html), data, __VA_ARGS__)
    #define h_datalist(html, ...) html_element(MEW_HTML_AUTOPTR(html), datalist, __VA_ARGS__)
    #define h_dd(html, ...) html_element(MEW_HTML_AUTOPTR(html), dd, __VA_ARGS__)
    #define h_del(html, ...) html_element(MEW_HTML_AUTOPTR(html), del, __VA_ARGS__)
    #define h_details(html, ...) html_element(MEW_HTML_AUTOPTR(html), details, __VA_ARGS__)
    #define h_dfn(html, ...) html_element(MEW_HTML_AUTOPTR(html), dfn, __VA_ARGS__)
    #define h_dialog(html, ...) html_element(MEW_HTML_AUTOPTR(html), dialog, __VA_ARGS__)
    #define h_dir(html, ...) html_element(MEW_HTML_AUTOPTR(html), dir, __VA_ARGS__)
    #define h_div(html, ...) html_element(MEW_HTML_AUTOPTR(html), div, __VA_ARGS__)
    #define h_dl(html, ...) html_element(MEW_HTML_AUTOPTR(html), dl, __VA_ARGS__)
    #define h_dt(html, ...) html_element(MEW_HTML_AUTOPTR(html), dt, __VA_ARGS__)
    #define h_em(html, ...) html_element(MEW_HTML_AUTOPTR(html), em, __VA_ARGS__)
    #define h_embed(html, ...) html_element(MEW_HTML_AUTOPTR(html), embed, __VA_ARGS__)
    #define h_fieldset(html, ...) html_element(MEW_HTML_AUTOPTR(html), fieldset, __VA_ARGS__)
    #define h_figcaption(html, ...) html_element(MEW_HTML_AUTOPTR(html), figcaption, __VA_ARGS__)
    #define h_figure(html, ...) html_element(MEW_HTML_AUTOPTR(html), figure, __VA_ARGS__)
    #define h_font(html, ...) html_element(MEW_HTML_AUTOPTR(html), font, __VA_ARGS__)
    #define h_footer(html, ...) html_element(MEW_HTML_AUTOPTR(html), footer, __VA_ARGS__)
    #define h_form(html, ...) html_element(MEW_HTML_AUTOPTR(html), form, __VA_ARGS__)
    #define h_frame(html, ...) html_element(MEW_HTML_AUTOPTR(html), frame, __VA_ARGS__)
    #define h_frameset(html, ...) html_element(MEW_HTML_AUTOPTR(html), frameset, __VA_ARGS__)
    #define h_h1(html, ...) html_element(MEW_HTML_AUTOPTR(html), h1, __VA_ARGS__)
    #define h_h2(html, ...) html_element(MEW_HTML_AUTOPTR(html), h2, __VA_ARGS__)
    #define h_h3(html, ...) html_element(MEW_HTML_AUTOPTR(html), h3, __VA_ARGS__)
    #define h_h4(html, ...) html_element(MEW_HTML_AUTOPTR(html), h4, __VA_ARGS__)
    #define h_h5(html, ...) html_element(MEW_HTML_AUTOPTR(html), h5, __VA_ARGS__)
    #define h_h6(html, ...) html_element(MEW_HTML_AUTOPTR(html), h6, __VA_ARGS__)
    #define h_head(html, ...) html_element(MEW_HTML_AUTOPTR(html), head, __VA_ARGS__)
    #define h_header(html, ...) html_element(MEW_HTML_AUTOPTR(html), header, __VA_ARGS__)
    #define h_hgroup(html, ...) html_element(MEW_HTML_AUTOPTR(html), hgroup, __VA_ARGS__)
    #define h_hr(html, ...) html_element(MEW_HTML_AUTOPTR(html), hr, __VA_ARGS__)
    #define h_i(html, ...) html_element(MEW_HTML_AUTOPTR(html), i, __VA_ARGS__)
    #define h_iframe(html, ...) html_element(MEW_HTML_AUTOPTR(html), iframe, __VA_ARGS__)
    #define h_img(html, ...) html_element(MEW_HTML_AUTOPTR(html), img, __VA_ARGS__)
    #define h_input(html, ...) html_element(MEW_HTML_AUTOPTR(html), input, __VA_ARGS__)
    #define h_ins(html, ...) html_element(MEW_HTML_AUTOPTR(html), ins, __VA_ARGS__)
    #define h_kbd(html, ...) html_element(MEW_HTML_AUTOPTR(html), kbd, __VA_ARGS__)
    #define h_label(html, ...) html_element(MEW_HTML_AUTOPTR(html), label, __VA_ARGS__)
    #define h_legend(html, ...) html_element(MEW_HTML_AUTOPTR(html), legend, __VA_ARGS__)
    #define h_li(html, ...) html_element(MEW_HTML_AUTOPTR(html), li, __VA_ARGS__)
    #define h_link(html, ...) html_element(MEW_HTML_AUTOPTR(html), link, __VA_ARGS__)
    #define h_main(html, ...) html_element(MEW_HTML_AUTOPTR(html), main, __VA_ARGS__)
    #define h_map(html, ...) html_element(MEW_HTML_AUTOPTR(html), map, __VA_ARGS__)
    #define h_mark(html, ...) html_element(MEW_HTML_AUTOPTR(html), mark, __VA_ARGS__)
    #define h_menu(html, ...) html_element(MEW_HTML_AUTOPTR(html), menu, __VA_ARGS__)
    #define h_meta(html, ...) html_element(MEW_HTML_AUTOPTR(html), meta, __VA_ARGS__)
    #define h_meter(html, ...) html_element(MEW_HTML_AUTOPTR(html), meter, __VA_ARGS__)
    #define h_nav(html, ...) html_element(MEW_HTML_AUTOPTR(html), nav, __VA_ARGS__)
    #define h_noframes(html, ...) html_element(MEW_HTML_AUTOPTR(html), noframes, __VA_ARGS__)
    #define h_noscript(html, ...) html_element(MEW_HTML_AUTOPTR(html), noscript, __VA_ARGS__)
    #define h_object(html, ...) html_element(MEW_HTML_AUTOPTR(html), object, __VA_ARGS__)
    #define h_ol(html, ...) html_element(MEW_HTML_AUTOPTR(html), ol, __VA_ARGS__)
    #define h_optgroup(html, ...) html_element(MEW_HTML_AUTOPTR(html), optgroup, __VA_ARGS__)
    #define h_option(html, ...) html_element(MEW_HTML_AUTOPTR(html), option, __VA_ARGS__)
    #define h_output(html, ...) html_element(MEW_HTML_AUTOPTR(html), output, __VA_ARGS__)
    #define h_p(html, ...) html_element(MEW_HTML_AUTOPTR(html), p, __VA_ARGS__)
    #define h_param(html, ...) html_element(MEW_HTML_AUTOPTR(html), param, __VA_ARGS__)
    #define h_picture(html, ...) html_element(MEW_HTML_AUTOPTR(html), picture, __VA_ARGS__)
    #define h_pre(html, ...) html_element(MEW_HTML_AUTOPTR(html), pre, __VA_ARGS__)
    #define h_progress(html, ...) html_element(MEW_HTML_AUTOPTR(html), progress, __VA_ARGS__)
    #define h_q(html, ...) html_element(MEW_HTML_AUTOPTR(html), q, __VA_ARGS__)
    #define h_rp(html, ...) html_element(MEW_HTML_AUTOPTR(html), rp, __VA_ARGS__)
    #define h_rt(html, ...) html_element(MEW_HTML_AUTOPTR(html), rt, __VA_ARGS__)
    #define h_ruby(html, ...) html_element(MEW_HTML_AUTOPTR(html), ruby, __VA_ARGS__)
    #define h_s(html, ...) html_element(MEW_HTML_AUTOPTR(html), s, __VA_ARGS__)
    #define h_samp(html, ...) html_element(MEW_HTML_AUTOPTR(html), samp, __VA_ARGS__)
    #define h_script(html, ...) html_element(MEW_HTML_AUTOPTR(html), script, __VA_ARGS__)
    #define h_search(html, ...) html_element(MEW_HTML_AUTOPTR(html), search, __VA_ARGS__)
    #define h_section(html, ...) html_element(MEW_HTML_AUTOPTR(html), section, __VA_ARGS__)
    #define h_select(html, ...) html_element(MEW_HTML_AUTOPTR(html), select, __VA_ARGS__)
    #define h_small(html, ...) html_element(MEW_HTML_AUTOPTR(html), small, __VA_ARGS__)
    #define h_source(html, ...) html_element(MEW_HTML_AUTOPTR(html), source, __VA_ARGS__)
    #define h_span(html, ...) html_element(MEW_HTML_AUTOPTR(html), span, __VA_ARGS__)
    #define h_strike(html, ...) html_element(MEW_HTML_AUTOPTR(html), strike, __VA_ARGS__)
    #define h_strong(html, ...) html_element(MEW_HTML_AUTOPTR(html), strong, __VA_ARGS__)
    #define h_style(html, ...) html_element(MEW_HTML_AUTOPTR(html), style, __VA_ARGS__)
    #define h_sub(html, ...) html_element(MEW_HTML_AUTOPTR(html), sub, __VA_ARGS__)
    #define h_summary(html, ...) html_element(MEW_HTML_AUTOPTR(html), summary, __VA_ARGS__)
    #define h_sup(html, ...) html_element(MEW_HTML_AUTOPTR(html), sup, __VA_ARGS__)
    #define h_svg(html, ...) html_element(MEW_HTML_AUTOPTR(html), svg, __VA_ARGS__)
    #define h_table(html, ...) html_element(MEW_HTML_AUTOPTR(html), table, __VA_ARGS__)
    #define h_tbody(html, ...) html_element(MEW_HTML_AUTOPTR(html), tbody, __VA_ARGS__)
    #define h_td(html, ...) html_element(MEW_HTML_AUTOPTR(html), td, __VA_ARGS__)
    #define h_template(html, ...) html_element(MEW_HTML_AUTOPTR(html), template, __VA_ARGS__)
    #define h_textarea(html, ...) html_element(MEW_HTML_AUTOPTR(html), textarea, __VA_ARGS__)
    #define h_tfoot(html, ...) html_element(MEW_HTML_AUTOPTR(html), tfoot, __VA_ARGS__)
    #define h_th(html, ...) html_element(MEW_HTML_AUTOPTR(html), th, __VA_ARGS__)
    #define h_thead(html, ...) html_element(MEW_HTML_AUTOPTR(html), thead, __VA_ARGS__)
    #define h_time(html, ...) html_element(MEW_HTML_AUTOPTR(html), time, __VA_ARGS__)
    #define h_title(html, ...) html_element(MEW_HTML_AUTOPTR(html), title, __VA_ARGS__)
    #define h_tr(html, ...) html_element(MEW_HTML_AUTOPTR(html), tr, __VA_ARGS__)
    #define h_track(html, ...) html_element(MEW_HTML_AUTOPTR(html), track, __VA_ARGS__)
    #define h_tt(html, ...) html_element(MEW_HTML_AUTOPTR(html), tt, __VA_ARGS__)
    #define h_u(html, ...) html_element(MEW_HTML_AUTOPTR(html), u, __VA_ARGS__)
    #define h_ul(html, ...) html_element(MEW_HTML_AUTOPTR(html), ul, __VA_ARGS__)
    #define h_var(html, ...) html_element(MEW_HTML_AUTOPTR(html), var, __VA_ARGS__)
    #define h_video(html, ...) html_element(MEW_HTML_AUTOPTR(html), video, __VA_ARGS__)
    #define h_w(html, ...) html_element(MEW_HTML_AUTOPTR(html), wbr, __VA_ARGS__)

    #define ht_text(html, ...) html_text(MEW_HTML_AUTOPTR(html), MEW_HTML_TEXT_AUTOSV(__VA_ARGS__))
    #define ht_title(html, ...) h_title(html, ht_text(html, __VA_ARGS__))
    #define ht_attr(html, name, ...)                                                                                   \
        html_push_attribute(                                                                                           \
            MEW_HTML_AUTOPTR(html),                                                                                    \
            (Attribute) {MEW_HTML_TEXT_AUTOSV(name), MEW_HTML_TEXT_AUTOSV(__VA_ARGS__)}                                \
        )
    #define ht_class(html, ...) html_push_class(MEW_HTML_AUTOPTR(html), MEW_HTML_TEXT_AUTOSV(__VA_ARGS__))
#endif

void html_append_current_indentation(Html *html);
void html_render_class(Html *html);
void html_render_attributes(Html *html);

#endif // MEW_INCLUDE_MEW_HTML_H_
