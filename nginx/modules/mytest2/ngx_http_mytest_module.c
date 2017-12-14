//  《深入理解 NGNIX 》第4章, 配置, error日志和请求上下文
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

typedef struct {
    ngx_str_t my_str;
    ngx_int_t my_num;
    ngx_flag_t my_flag;
    size_t my_size;
    ngx_array_t *my_str_array;
    ngx_array_t *my_keyval;
    off_t my_off;
    ngx_msec_t my_msec;
    time_t my_sec;
    ngx_bufs_t my_bufs;
    ngx_uint_t my_enum_seq;
    ngx_uint_t my_bitmask;
    ngx_uint_t my_access;
    ngx_path_t* my_path;

    // 自定义
    ngx_str_t my_config_str;
    ngx_int_t my_config_num;
} ngx_http_mytest_conf_t;


static ngx_conf_enum_t test_enums[] = {
    {ngx_string("apple"), 1},
    {ngx_string("banana"), 2},
    {ngx_string("orange"), 3},
    {ngx_null_string, 0}
};

static ngx_conf_bitmask_t test_bitmasks[] = {
    {ngx_string("good"), 0x002},
    {ngx_string("better"), 0x004},
    {ngx_string("best"), 0x008},
    {ngx_null_string, 0},
};

static ngx_int_t ngx_http_mytest_init(ngx_conf_t *cf);
static void *ngx_http_mytest_create_loc_conf(ngx_conf_t *cf);
static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r);
static char* ngx_conf_set_myconfig(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_http_module_t ngx_http_mytest_module_ctx = {
    NULL,
    ngx_http_mytest_init,
    NULL,
    NULL,
    NULL,
    NULL,
    ngx_http_mytest_create_loc_conf,
    NULL,
};

static ngx_command_t ngx_http_mytest_commands[] = {
    {
        ngx_string("test_flag"),
        NGX_HTTP_LOC_CONF | NGX_CONF_FLAG,
        ngx_conf_set_flag_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_flag),
        NULL},
    {
        ngx_string("test_str"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF| NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_str),
        NULL},
    {
        ngx_string("test_str_array"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_str_array_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_str_array),
        NULL},

    {
        ngx_string("test_keyval"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE2,
        ngx_conf_set_keyval_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_keyval),
        NULL},

    {
        ngx_string("test_num"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_num),
        NULL},

    {
        ngx_string("test_size"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_size_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_size),
        NULL},
    {
        ngx_string("test_off"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_off_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_off),
        NULL},
    {
        ngx_string("test_msec"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_msec_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_msec),
        NULL},
    {
        ngx_string("test_sec"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_sec_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_sec),
        NULL},
    {
        ngx_string("test_bufs"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE2,
        ngx_conf_set_bufs_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_bufs),
        NULL},
    {
        // 这个指令暂时会报错
        ngx_string("test_enum"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE2,
        ngx_conf_set_enum_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_enum_seq),
        test_enums},
    {
        ngx_string("test_bitmask"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_bitmask_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_bitmask),
        test_bitmasks},
    {
        // 这个指令暂时会报错
        ngx_string("test_access"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE123,
        ngx_conf_set_access_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_access),
        NULL},
    {
        ngx_string("test_path"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1234,
        ngx_conf_set_path_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_path),
        NULL},

    {
        ngx_string("test_myconfig"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE12,
        ngx_conf_set_myconfig,
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




static void *
ngx_http_mytest_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_mytest_conf_t *mycf;

    mycf = (ngx_http_mytest_conf_t *)ngx_palloc(cf->pool, sizeof(ngx_http_mytest_conf_t));
    if (mycf == NULL) {
        return NULL;
    }

    mycf->my_flag = NGX_CONF_UNSET;
    mycf->my_num = NGX_CONF_UNSET;
    mycf->my_str_array = NGX_CONF_UNSET_PTR;
    mycf->my_keyval = NULL;
    mycf->my_off = NGX_CONF_UNSET;
    mycf->my_msec = NGX_CONF_UNSET_MSEC;
    mycf->my_sec = NGX_CONF_UNSET;
    mycf->my_size = NGX_CONF_UNSET_SIZE;

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "create_loc_conf, %p", mycf);

    return mycf;
}

static ngx_int_t
ngx_http_mytest_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt *h;
    ngx_http_core_main_conf_t *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);
    h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }
    *h = ngx_http_mytest_handler;

    return NGX_OK;
}

static ngx_int_t
ngx_http_mytest_handler(ngx_http_request_t *r)
{
    ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "handler start!");

	// 必须是GET或者HEAD方法，否则返回405 Not Allowed
	if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD))) {
		return NGX_HTTP_NOT_ALLOWED;
	}

	// 丢弃请求中的包体
	ngx_int_t rc = ngx_http_discard_request_body(r);
	if (rc != NGX_OK) {
		return rc;
	}

    // 获取配置信息
    ngx_http_mytest_conf_t* mycf;
    mycf = ngx_http_get_module_loc_conf(r, ngx_http_mytest_module);
    ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0,
        "mycf=%p, my_flag=%d, my_str: %V, my_num: %d, my_size: %z, my_off: %O, my_msec: %ui, my_sec: %ui, my_enum_seq: %ui, my_bitmask: %ui",
        mycf, mycf->my_flag, &mycf->my_str,
        mycf->my_num, mycf->my_size, mycf->my_off,
        mycf->my_msec, mycf->my_sec, mycf->my_enum_seq, mycf->my_bitmask);

    if (mycf->my_str_array) {
        ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "my_str_array.nelts: %d",
            mycf->my_str_array->nelts);
        for (ngx_uint_t i = 0; i < mycf->my_str_array->nelts; i++) {
            ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "my_str_array: %V",
                    mycf->my_str_array->elts + sizeof(ngx_str_t)*i);
        }
    }

    if (mycf->my_keyval) {
        ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "my_keyval.nelts: %d",
            mycf->my_keyval->nelts);
        ngx_keyval_t* pkv = mycf->my_keyval->elts;
        for (ngx_uint_t i = 0; i < mycf->my_keyval->nelts; i++) {
            ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "my_keyval, key: %V , value: %V",
                    &(pkv[i].key), &(pkv[i].value));
        }
    }

    ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0,
        "my_bufs, num: %ui, size: %z", mycf->my_bufs.num, mycf->my_bufs.size);

    ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "my_path, name:%V", &(mycf->my_path->name));

    ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "my_config_str: %V, my_config_num: %d",
            &(mycf->my_config_str), mycf->my_config_num);



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

static char*
ngx_conf_set_myconfig(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_mytest_conf_t *mycf = conf;

    ngx_str_t* value = cf->args->elts;
    if (cf->args->nelts > 1) {
        mycf->my_config_str = value[1];
    }
    if (cf->args->nelts > 2) {
        mycf->my_config_num = ngx_atoi(value[2].data, value[2].len);
        if (mycf->my_config_num == NGX_ERROR) {
            return "invalid number";
        }
    }

    return NGX_CONF_OK;
}
