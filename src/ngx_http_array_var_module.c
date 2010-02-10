#define DDEBUG 0
#include "ddebug.h"

#include <ndk.h>


static char * ngx_http_array_split(ngx_conf_t *cf, ngx_command_t *cmd,
        void *conf);

static char * ngx_http_array_map(ngx_conf_t *cf, ngx_command_t *cmd,
        void *conf);

static char * ngx_http_array_map_op(ngx_conf_t *cf, ngx_command_t *cmd,
        void *conf);

static char * ngx_http_array_join(ngx_conf_t *cf, ngx_command_t *cmd,
        void *conf);

static ngx_int_t ngx_http_array_var_split(ngx_http_request_t *r,
        ngx_str_t *res, ngx_http_variable_value_t *v, void *data);

static ngx_int_t ngx_http_array_var_map(ngx_http_request_t *r,
        ngx_str_t *res, ngx_http_variable_value_t *v, void *data);

static ngx_int_t ngx_http_array_var_map_op(ngx_http_request_t *r,
        ngx_str_t *res, ngx_http_variable_value_t *v, void *data);

static ngx_int_t ngx_http_array_var_map_join(ngx_http_request_t *r,
        ngx_str_t *res, ngx_http_variable_value_t *v, void *data);


static ngx_command_t  ngx_http_array_var_commands[] = {
    {
        ngx_string ("array_split"),
        NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_SIF_CONF
            |NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_2MORE,
        ngx_http_array_split,
        0,
        0,
        NULL
    },
    {
        ngx_string ("array_map"),
        NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_SIF_CONF
            |NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_2MORE,
        ngx_http_array_map,
        0,
        0,
        NULL
    },
    {
        ngx_string ("array_map_op"),
        NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_SIF_CONF
            |NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_2MORE,
        ngx_http_array_map_op,
        0,
        0,
        NULL
    },
    {
        ngx_string("array_join"),
        NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_SIF_CONF
            |NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_2MORE,
        ngx_http_array_join,
        0,
        0,
        NULL
    },

    ngx_null_command
};


static ngx_http_module_t  ngx_http_array_var_module_ctx = {
    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL,                                  /* merge location configuration */
};


ngx_module_t  ngx_http_array_var_module = {
    NGX_MODULE_V1,
    &ngx_http_array_var_module_ctx,          // module context
    ngx_http_array_var_commands,             // module directives
    NGX_HTTP_MODULE,                         // module type
    NULL,                                    // init master
    NULL,                                    // init module
    NULL,                                    // init process
    NULL,                                    // init thread
    NULL,                                    // exit thread
    NULL,                                    // exit process
    NULL,                                    // exit master
    NGX_MODULE_V1_PADDING
};


static char *
ngx_http_array_split(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    /* TODO */
    return NGX_CONF_OK;
}


static char *
ngx_http_array_map(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    /* TODO */
    return NGX_CONF_OK;
}


static char *
ngx_http_array_map_op(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    /* TODO */
    return NGX_CONF_OK;
}


static char *
ngx_http_array_join(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    /* TODO */
    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_array_var_split(ngx_http_request_t *r,
        ngx_str_t *res, ngx_http_variable_value_t *v, void *data)
{
    /* TODO */
    return NGX_OK;
}


static ngx_int_t
ngx_http_array_var_map(ngx_http_request_t *r,
        ngx_str_t *res, ngx_http_variable_value_t *v, void *data)
{
    /* TODO */
    return NGX_OK;
}


static ngx_int_t
ngx_http_array_var_map_op(ngx_http_request_t *r,
        ngx_str_t *res, ngx_http_variable_value_t *v, void *data)
{
    /* TODO */
    return NGX_OK;
}


static ngx_int_t
ngx_http_array_var_map_join(ngx_http_request_t *r,
        ngx_str_t *res, ngx_http_variable_value_t *v, void *data)
{
    /* TODO */
    return NGX_OK;
}

