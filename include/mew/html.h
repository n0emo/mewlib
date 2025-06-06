#ifndef MEW_INCLUDE_MEW_HTML_H_
#define MEW_INCLUDE_MEW_HTML_H_

#include "mew/utils.h"
#include "mew/str.h"

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
    Attributes attributes;
    Classes classes;
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

#define HTML_TAG_LIST \
    X(a) \
    X(abbr) \
    X(acronym) \
    X(address) \
    X(applet) \
    X(area) \
    X(article) \
    X(aside) \
    X(audio) \
    X(b) \
    X(base) \
    X(basefont) \
    X(bdi) \
    X(bdo) \
    X(big) \
    X(blockquote) \
    X(body) \
    X(br) \
    X(button) \
    X(canvas) \
    X(caption) \
    X(center) \
    X(cite) \
    X(code) \
    X(col) \
    X(colgroup) \
    X(data) \
    X(datalist) \
    X(dd) \
    X(del) \
    X(details) \
    X(dfn) \
    X(dialog) \
    X(dir) \
    X(div) \
    X(dl) \
    X(dt) \
    X(em) \
    X(embed) \
    X(fieldset) \
    X(figcaption) \
    X(figure) \
    X(font) \
    X(footer) \
    X(form) \
    X(frame) \
    X(frameset) \
    X(h1) \
    X(h2) \
    X(h3) \
    X(h4) \
    X(h5) \
    X(h6) \
    X(head) \
    X(header) \
    X(hgroup) \
    X(hr) \
    X(i) \
    X(iframe) \
    X(img) \
    X(input) \
    X(ins) \
    X(kbd) \
    X(label) \
    X(legend) \
    X(li) \
    X(link) \
    X(main) \
    X(map) \
    X(mark) \
    X(menu) \
    X(meta) \
    X(meter) \
    X(nav) \
    X(noframes) \
    X(noscript) \
    X(object) \
    X(ol) \
    X(optgroup) \
    X(option) \
    X(output) \
    X(p) \
    X(param) \
    X(picture) \
    X(pre) \
    X(progress) \
    X(q) \
    X(rp) \
    X(rt) \
    X(ruby) \
    X(s) \
    X(samp) \
    X(script) \
    X(search) \
    X(section) \
    X(select) \
    X(small) \
    X(source) \
    X(span) \
    X(strike) \
    X(strong) \
    X(style) \
    X(sub) \
    X(summary) \
    X(sup) \
    X(svg) \
    X(table) \
    X(tbody) \
    X(td) \
    X(template) \
    X(textarea) \
    X(tfoot) \
    X(th) \
    X(thead) \
    X(time) \
    X(title) \
    X(tr) \
    X(track) \
    X(tt) \
    X(u) \
    X(ul) \
    X(var) \
    X(video) \
    X(wbr)

#define html_element(html, element, ...) \
    do { \
        html_ ## element ## _begin(html); \
        __VA_ARGS__ \
        html_ ## element ## _end(html); \
    } while(0)

#define X(tag) \
    void html_ ## tag ## _begin(Html *html); \
    void html_ ## tag ## _end(Html *html); \
    void html_ ## tag ## _short(Html *html);
HTML_TAG_LIST
#undef X

#define MEW_HTML_DEREF_OR_ADDR(html) _Generic((html), \
    Html*       : html, \
    const Html* : html, \
    Html        : &html \
)

#ifdef MEW_HTML_SHORT_TAGS
#define h_a(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), a, __VA_ARGS__)
#define h_abbr(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), abbr, __VA_ARGS__)
#define h_acronym(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), acronym, __VA_ARGS__)
#define h_address(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), address, __VA_ARGS__)
#define h_applet(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), applet, __VA_ARGS__)
#define h_area(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), area, __VA_ARGS__)
#define h_article(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), article, __VA_ARGS__)
#define h_aside(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), aside, __VA_ARGS__)
#define h_audio(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), audio, __VA_ARGS__)
#define h_b(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), b, __VA_ARGS__)
#define h_base(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), base, __VA_ARGS__)
#define h_basefont(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), basefont, __VA_ARGS__)
#define h_bdi(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), bdi, __VA_ARGS__)
#define h_bdo(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), bdo, __VA_ARGS__)
#define h_big(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), big, __VA_ARGS__)
#define h_blockquote(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), blockquote, __VA_ARGS__)
#define h_body(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), body, __VA_ARGS__)
#define h_br(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), br, __VA_ARGS__)
#define h_button(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), button, __VA_ARGS__)
#define h_canvas(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), canvas, __VA_ARGS__)
#define h_caption(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), caption, __VA_ARGS__)
#define h_center(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), center, __VA_ARGS__)
#define h_cite(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), cite, __VA_ARGS__)
#define h_code(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), code, __VA_ARGS__)
#define h_col(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), col, __VA_ARGS__)
#define h_colgroup(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), colgroup, __VA_ARGS__)
#define h_data(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), data, __VA_ARGS__)
#define h_datalist(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), datalist, __VA_ARGS__)
#define h_dd(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), dd, __VA_ARGS__)
#define h_del(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), del, __VA_ARGS__)
#define h_details(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), details, __VA_ARGS__)
#define h_dfn(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), dfn, __VA_ARGS__)
#define h_dialog(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), dialog, __VA_ARGS__)
#define h_dir(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), dir, __VA_ARGS__)
#define h_div(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), div, __VA_ARGS__)
#define h_dl(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), dl, __VA_ARGS__)
#define h_dt(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), dt, __VA_ARGS__)
#define h_em(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), em, __VA_ARGS__)
#define h_embed(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), embed, __VA_ARGS__)
#define h_fieldset(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), fieldset, __VA_ARGS__)
#define h_figcaption(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), figcaption, __VA_ARGS__)
#define h_figure(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), figure, __VA_ARGS__)
#define h_font(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), font, __VA_ARGS__)
#define h_footer(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), footer, __VA_ARGS__)
#define h_form(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), form, __VA_ARGS__)
#define h_frame(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), frame, __VA_ARGS__)
#define h_frameset(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), frameset, __VA_ARGS__)
#define h_h1(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), h1, __VA_ARGS__)
#define h_h2(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), h2, __VA_ARGS__)
#define h_h3(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), h3, __VA_ARGS__)
#define h_h4(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), h4, __VA_ARGS__)
#define h_h5(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), h5, __VA_ARGS__)
#define h_h6(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), h6, __VA_ARGS__)
#define h_head(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), head, __VA_ARGS__)
#define h_header(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), header, __VA_ARGS__)
#define h_hgroup(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), hgroup, __VA_ARGS__)
#define h_hr(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), hr, __VA_ARGS__)
#define h_i(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), i, __VA_ARGS__)
#define h_iframe(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), iframe, __VA_ARGS__)
#define h_img(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), img, __VA_ARGS__)
#define h_input(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), input, __VA_ARGS__)
#define h_ins(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), ins, __VA_ARGS__)
#define h_kbd(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), kbd, __VA_ARGS__)
#define h_label(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), label, __VA_ARGS__)
#define h_legend(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), legend, __VA_ARGS__)
#define h_li(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), li, __VA_ARGS__)
#define h_link(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), link, __VA_ARGS__)
#define h_main(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), main, __VA_ARGS__)
#define h_map(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), map, __VA_ARGS__)
#define h_mark(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), mark, __VA_ARGS__)
#define h_menu(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), menu, __VA_ARGS__)
#define h_meta(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), meta, __VA_ARGS__)
#define h_meter(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), meter, __VA_ARGS__)
#define h_nav(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), nav, __VA_ARGS__)
#define h_noframes(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), noframes, __VA_ARGS__)
#define h_noscript(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), noscript, __VA_ARGS__)
#define h_object(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), object, __VA_ARGS__)
#define h_ol(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), ol, __VA_ARGS__)
#define h_optgroup(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), optgroup, __VA_ARGS__)
#define h_option(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), option, __VA_ARGS__)
#define h_output(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), output, __VA_ARGS__)
#define h_p(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), p, __VA_ARGS__)
#define h_param(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), param, __VA_ARGS__)
#define h_picture(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), picture, __VA_ARGS__)
#define h_pre(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), pre, __VA_ARGS__)
#define h_progress(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), progress, __VA_ARGS__)
#define h_q(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), q, __VA_ARGS__)
#define h_rp(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), rp, __VA_ARGS__)
#define h_rt(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), rt, __VA_ARGS__)
#define h_ruby(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), ruby, __VA_ARGS__)
#define h_s(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), s, __VA_ARGS__)
#define h_samp(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), samp, __VA_ARGS__)
#define h_script(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), script, __VA_ARGS__)
#define h_search(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), search, __VA_ARGS__)
#define h_section(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), section, __VA_ARGS__)
#define h_select(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), select, __VA_ARGS__)
#define h_small(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), small, __VA_ARGS__)
#define h_source(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), source, __VA_ARGS__)
#define h_span(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), span, __VA_ARGS__)
#define h_strike(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), strike, __VA_ARGS__)
#define h_strong(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), strong, __VA_ARGS__)
#define h_style(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), style, __VA_ARGS__)
#define h_sub(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), sub, __VA_ARGS__)
#define h_summary(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), summary, __VA_ARGS__)
#define h_sup(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), sup, __VA_ARGS__)
#define h_svg(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), svg, __VA_ARGS__)
#define h_table(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), table, __VA_ARGS__)
#define h_tbody(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), tbody, __VA_ARGS__)
#define h_td(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), td, __VA_ARGS__)
#define h_template(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), template, __VA_ARGS__)
#define h_textarea(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), textarea, __VA_ARGS__)
#define h_tfoot(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), tfoot, __VA_ARGS__)
#define h_th(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), th, __VA_ARGS__)
#define h_thead(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), thead, __VA_ARGS__)
#define h_time(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), time, __VA_ARGS__)
#define h_title(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), title, __VA_ARGS__)
#define h_tr(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), tr, __VA_ARGS__)
#define h_track(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), track, __VA_ARGS__)
#define h_tt(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), tt, __VA_ARGS__)
#define h_u(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), u, __VA_ARGS__)
#define h_ul(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), ul, __VA_ARGS__)
#define h_var(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), var, __VA_ARGS__)
#define h_video(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), video, __VA_ARGS__)
#define h_w(...) html_element(MEW_HTML_DEREF_OR_ADDR(html), wbr, __VA_ARGS__)
#endif

void html_append_current_indentation(Html *html);
void html_render_class(Html *html);
void html_render_attributes(Html *html);

#endif // MEW_INCLUDE_MEW_HTML_H_
