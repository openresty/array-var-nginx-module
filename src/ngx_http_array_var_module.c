#define DDEBUG 0
#include "ddebug.h"

#include "ngx_http_array_var_util.h"

#include <ndk.h>


typedef struct {
    ngx_uint_t          nargs;

} ngx_http_array_split_data_t;

typedef struct {
    ngx_flag_t                       in_place;
    ngx_http_complex_value_t        *template;
    ngx_int_t                        array_it_index;

} ngx_http_array_map_data_t;


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
    ndk_set_var_t                    filter;
    ngx_str_t                        target;
    ngx_str_t                       *value;
    ngx_str_t                       *bad_arg;
    ngx_http_array_split_data_t     *data = NULL;

    data = ngx_palloc(cf->pool, sizeof(ngx_http_array_split_data_t));
    if (data == NULL) {
        return NGX_CONF_ERROR;
    }

    filter.type = NDK_SET_VAR_MULTI_VALUE_DATA;
    filter.func = ngx_http_array_var_split;
    filter.data = data;

    value = cf->args->elts;

    if (cf->args->nelts == 2 + 1) {
        /* array_split $sep $var */
        data->nargs = filter.size = 2;
        target = value[2];
        return ndk_set_var_multi_value_core(cf, &target, &value[1], &filter);
    }

    /* cf->args->nelts >= 3 + 1 */

    if (value[3].len >= sizeof("to=") - 1
            && ngx_str3cmp(value[3].data, 't', 'o', '='))
    {
        /* array_split $sep $str to=$array */
        data->nargs = filter.size = 2;

        target.data = value[3].data + sizeof("to=") - 1;
        target.len = value[3].len - (sizeof("to=") - 1);

        if (cf->args->nelts > 3 + 1) {
            bad_arg = &value[4];
            goto unexpected_arg;
        }

        return ndk_set_var_multi_value_core(cf, &target, &value[1], &filter);
    }

    /* the 3rd argument is max_items */

    if (cf->args->nelts > 4 + 1) {
        bad_arg = &value[5];
        goto unexpected_arg;
    }

    if (cf->args->nelts == 4 + 1) {
        /* array_split $sep $str $max to=$array */

        if (value[4].len < sizeof("to=") - 1
                || ! (ngx_str3cmp(value[4].data, 't', 'o', '=')))
        {
            ngx_conf_log_error(NGX_LOG_ERR, cf, 0,
                    "%V: expecting the \"to\" option at the "
                    "4th argument: \"%V\"",
                    &cmd->name, &value[4]);

            return NGX_CONF_ERROR;
        }

        data->nargs = filter.size = 3;

        target.data = value[4].data + sizeof("to=") - 1;
        target.len = value[4].len - (sizeof("to=") - 1);

        return ndk_set_var_multi_value_core(cf, &target, &value[1], &filter);
    }

    /* cf->args->nelts == 3 + 1 */

    /* array_split $sep $var $max */

    target = value[2];
    data->nargs = filter.size = 3;

    return ndk_set_var_multi_value_core(cf, &target, &value[1], &filter);

unexpected_arg:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
            "%V: unexpected argument \"%V\"",
            &cmd->name, bad_arg);

    return NGX_CONF_ERROR;
}


static char *
ngx_http_array_map(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    /* TODO */
    return NGX_CONF_ERROR;
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
    ngx_http_array_map_data_t       *conf = data;
    ngx_http_variable_value_t       *array_it;
    ngx_uint_t                       i;
    ngx_str_t                       *value, *new_value;
    ngx_array_t                     *array, *new_array;

    if (conf->template == NULL) {
        return NGX_OK;
    }

    if (v[0].len != sizeof(ngx_array_t *)) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                "array_join: invalid array variable value in the 2nd "
                "argument: \"%.*s\"", &v[0]);

        return NGX_ERROR;
    }

    array = (ngx_array_t *) v[0].data;

    value = array->elts;

    array_it = ngx_http_get_indexed_variable(r, conf->array_it_index);

    if ( conf->in_place) {
        new_array = array;
    } else {
        new_array = ngx_array_create(r->pool, array->nelts,
                sizeof(ngx_str_t));
        if (new_array == NULL) {
            return NGX_ERROR;
        }
    }

    for (i = 0; i < array->nelts; i++) {
        array_it->data = value[i].data;
        array_it->len = value[i].len;

        if (conf->in_place) {
            new_value = &value[i];

        } else {
            new_value = ngx_array_push(new_array);
            if (new_value == NULL) {
                return NGX_ERROR;
            }
        }

        if (ngx_http_complex_value(r, conf->template, new_value) != NGX_OK) {
            return NGX_ERROR;
        }
    }

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

