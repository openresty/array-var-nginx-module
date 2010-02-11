#ifndef NGX_HTTP_ARRAY_VAR_UTIL_C
#define NGX_HTTP_ARRAY_VAR_UTIL_C

#ifndef ngx_str3cmp

#  define ngx_str3cmp(m, c0, c1, c2)                                       \
    m[0] == c0 && m[1] == c1 && m[2] == c2

#endif /* ngx_str3cmp */

#endif /* NGX_HTTP_ARRAY_VAR_UTIL_C */

