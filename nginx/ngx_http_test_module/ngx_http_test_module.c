// 《深入理解Ngnix》, 第3章例子
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

// "Hello" 例子
static char *test(ngx_conf_t *, ngx_command_t *, void *);
static ngx_int_t handler(ngx_http_request_t *);

// 将磁盘文件作为包体发送
static char *download(ngx_conf_t *, ngx_command_t *, void *);
static ngx_int_t file_handler(ngx_http_request_t *);

static ngx_command_t test_commands[] = {
    {
        ngx_string("test"),
        NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
        test,
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL
    },
    {
        ngx_string("download"),
        NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
        download,
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL
    },
    ngx_null_command
};

static ngx_http_module_t test_ctx = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};


ngx_module_t ngx_http_test_module = {
    NGX_MODULE_V1,
    &test_ctx,
    test_commands,
    NGX_HTTP_MODULE,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NGX_MODULE_V1_PADDING
};

static char *test(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_core_loc_conf_t *corecf;
    corecf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    corecf->handler = handler;
    return NGX_CONF_OK;
};

static ngx_int_t handler(ngx_http_request_t *r) {
    if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD))) {
        return NGX_HTTP_NOT_ALLOWED;
    }

    ngx_int_t rc = ngx_http_discard_request_body(r);
    if (rc != NGX_OK) {
        return rc;
    }

    ngx_str_t type = ngx_string("text/plain");
    ngx_str_t response = ngx_string("Hello World!");
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = response.len;
    r->headers_out.content_type = type;

    rc = ngx_http_send_header(r);
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }

    ngx_buf_t *b;
    b = ngx_create_temp_buf(r->pool, response.len);
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    ngx_memcpy(b->pos, response.data, response.len);
    b->last = b->pos + response.len;
    b->last_buf = 1;

    ngx_chain_t out;
    out.buf = b;
    out.next = NULL;

    return ngx_http_output_filter(r, &out);
}

static char *download(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_core_loc_conf_t *corecf;
    corecf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    corecf->handler = file_handler;
    return NGX_CONF_OK;
};

static ngx_int_t file_handler(ngx_http_request_t *r) {
    if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD))) {
        return NGX_HTTP_NOT_ALLOWED;
    }

    ngx_int_t rc = ngx_http_discard_request_body(r);
    if (rc != NGX_OK) {
        return rc;
    }

    ngx_buf_t *b;
    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    u_char *filename = (u_char*) "/tmp/test.txt";
    b->in_file = 1;
    b->file = ngx_pcalloc(r->pool, sizeof(ngx_file_t));
    b->file->fd = ngx_open_file(filename, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK, NGX_FILE_OPEN, 0);
    b->file->log = r->connection->log;
    b->file->name.data = filename;
    b->file->name.len = strlen(filename);
    if (b->file->fd <= 0) {
        return NGX_HTTP_NOT_FOUND;
    }
    if (ngx_file_info(filename, &b->file->info) == NGX_FILE_ERROR) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    b->file_pos = 0;
    b->file_last = b->file->info.st_size;

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = b->file->info.st_size;
    ngx_str_t type = ngx_string("text/plain");
    r->headers_out.content_type = type;

    rc = ngx_http_send_header(r);
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }

    ngx_chain_t out;
    out.buf = b;
    out.next = NULL;

    return ngx_http_output_filter(r, &out);
}
