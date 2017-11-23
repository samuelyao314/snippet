#include <ngx_config.h>
#include <ngx_core.h>


static void *ngx_test_module_create_conf(ngx_cycle_t *cycle);
char * ngx_test(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_command_t ngx_test_commands[] = {
     {
        ngx_string("commands"),
        /*
        第一参数，模块中没有初始化函数；
        第二参数，nginx.conf配置文件的指令配置类型，就像worker_processes；
        第三个参数，该指令后面没有参数
        */
        NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_NOARGS,
        ngx_test,
        0,
        0,
        NULL
      },
      ngx_null_command /*命令集结束*/
};

static ngx_core_module_t ngx_test_module_ctx = {
    ngx_string("test"),
    ngx_test_module_create_conf,
    NULL
};

ngx_module_t ngx_test_module = {
    NGX_MODULE_V1,
    &ngx_test_module_ctx,
    ngx_test_commands,
    NGX_CORE_MODULE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING
};

char * ngx_test(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    FILE *fp = fopen("test", "w");
    fprintf(fp,"call ngx_test\n");
    fclose(fp);
    return NGX_CONF_OK;

}

static void *ngx_test_module_create_conf(ngx_cycle_t *cycle) {
    printf("call ngx_test_module_create_conf\n");
}
