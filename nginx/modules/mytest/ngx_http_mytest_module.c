// from http://blog.csdn.net/fzy0201/article/details/17802901
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


static char *ngx_http_mytest(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r);

static ngx_http_module_t ngx_http_mytest_module_ctx = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

static ngx_command_t ngx_http_mytest_commands[] = {
    {
        ngx_string("mytest"),
        NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS|NGX_CONF_TAKE1,
        ngx_http_mytest,
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL},

    ngx_null_command
};


ngx_module_t ngx_http_mytest_module = {
    NGX_MODULE_V1,
    &ngx_http_mytest_module_ctx,
    ngx_http_mytest_commands,
    NGX_HTTP_MODULE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING
};



static char *
ngx_http_mytest(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t* clcf;

    /* 找到 mytest 配置项所在的配置块 */
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);

    /* 请求处理函数入口  */
    clcf->handler = ngx_http_mytest_handler;

    return NGX_CONF_OK;
}



static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r)
{
	// 必须是GET或者HEAD方法，否则返回405 Not Allowed
	if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD))) {
		return NGX_HTTP_NOT_ALLOWED;
	}

	// 丢弃请求中的包体
	ngx_int_t rc = ngx_http_discard_request_body(r);
	if (rc != NGX_OK) {
		return rc;
	}

	// 设置返回的Content-Type
	ngx_str_t type = ngx_string("text/plain");
	// 返回包体内容
	ngx_str_t response = ngx_string("Hello Nginx!\n");
	// 设置返回状态码
	r->headers_out.status = NGX_HTTP_OK;
	// 响应包内容长度
	r->headers_out.content_length_n = response.len;
	// 设置Content-Type
	r->headers_out.content_type = type;

	// 发送HTTP头部
	rc = ngx_http_send_header(r);
	if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
		return rc;
	}

	// 构造ngx_buf_t结构体发送包体
	ngx_buf_t *b;
	b = ngx_create_temp_buf(r->pool, response.len);
	if (b == NULL) {
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	// 将Hello Nginx复制到buf中
	ngx_memcpy(b->pos, response.data, response.len);
	// 设置last指针
	b->last = b->pos + response.len;
	// 声明最后一块缓冲区
	b->last_buf = 1;

	// 构造发送时的ngx_chain_t结构体
	ngx_chain_t out;
	out.buf = b;
	out.next = NULL;

	// 发送包体，最后调用ngx_http_finalize_request方法结束请求
	return ngx_http_output_filter(r, &out);
}
