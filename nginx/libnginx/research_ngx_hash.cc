
#include "allinc.h"
#include "dump_nginx_struct.h"


static ngx_str_t names[] = {
    ngx_string("zieckey"),
    ngx_string("codeg"),
    ngx_string("jane") };

static const char* descs[] = { "zieckey's id is 0", "codeg's id is 1", "jane's id is 2" };

// hash table��һЩ��������
TEST_UNIT_P(ngx_hash)
{
    ngx_uint_t          k;
    ngx_hash_init_t     hash_init;
    ngx_array_t*        elements;
    ngx_hash_key_t*     arr_node;
    char*               find;
    size_t              i;

    ngx_cacheline_size = NGX_CPU_CACHE_LINE;

    hash_init.hash = NULL;                      // ��ΪNULL����ngx_hash_init����ʼ��
    hash_init.key = &ngx_hash_key_lc;          // hash�㷨����
    hash_init.max_size = 1024;                   // max_size
    hash_init.bucket_size = 64; // ngx_align(64, ngx_cacheline_size);
    hash_init.name = (char*)"codeg_hash";          // ��log����õ�
    hash_init.pool = pool;                 // �ڴ��
    hash_init.temp_pool = NULL;

    // ��������
    elements = ngx_array_create(pool, H_ARRAYSIZE(names), sizeof(ngx_hash_key_t));
    for (i = 0; i < H_ARRAYSIZE(names); i++) {
        arr_node = (ngx_hash_key_t*)ngx_array_push(elements);
        arr_node->key = (names[i]);
        arr_node->key_hash = ngx_hash_key_lc(arr_node->key.data, arr_node->key.len);
        arr_node->value = (void*)descs[i];
        printf("key: %s , key_hash: %u\n", arr_node->key.data, (unsigned int)arr_node->key_hash);
    }

    H_TEST_ASSERT(ngx_hash_init(&hash_init, (ngx_hash_key_t*)elements->elts, elements->nelts) == NGX_OK);

    // ����
    k = ngx_hash_key_lc(names[0].data, names[0].len);
    printf("%s key is %u\n", names[0].data, (unsigned int)k);
    find = (char*)ngx_hash_find(hash_init.hash, k, (u_char*)names[0].data, names[0].len);
    H_TEST_ASSERT(find != NULL);
    if (find) {
        printf("get desc of %s : %s\n", (char*)names[0].data, (char*)find);
    }

    ngx_array_destroy(elements);
}






// test code is copied from http://blog.csdn.net/livelylittlefish/article/details/6636229

#define Max_Size 1024
#define Bucket_Size 64  //256, 64, 32

#define NGX_HASH_ELT_SIZE(name)               \
    (sizeof(void *)+ngx_align((name)->key.len + 2, sizeof(void *)))

/* for hash test */
static ngx_str_t urls[] = {
    ngx_string("www.baidu.com"),  //220.181.111.147
    ngx_string("www.sina.com.cn"),  //58.63.236.35
    ngx_string("www.google.com"),  //74.125.71.105
    ngx_string("www.qq.com"),  //60.28.14.190
    ngx_string("www.163.com"),  //123.103.14.237
    ngx_string("www.sohu.com"),  //219.234.82.50
    ngx_string("www.abo321.org")  //117.40.196.26
};

static const char* values[] = {
    "220.181.111.147",
    "58.63.236.35",
    "74.125.71.105",
    "60.28.14.190",
    "123.103.14.237",
    "219.234.82.50",
    "117.40.196.26"
};


#define Max_Num H_ARRAYSIZE(values)

#define Max_Url_Len 15
#define Max_Ip_Len 15

/* for finding test */
static ngx_str_t urls2[] = {
    ngx_string("www.china.com"),  //60.217.58.79
    ngx_string("www.csdn.net")  //117.79.157.242
};

#define Max_Num2 H_ARRAYSIZE(urls2)

ngx_hash_t* init_hash(ngx_pool_t *pool, ngx_array_t *array);
void dump_pool(ngx_pool_t* pool);
void dump_hash_array(ngx_array_t* a);
void dump_hash(ngx_hash_t *hash, ngx_array_t *array);
ngx_array_t* add_urls_to_array(ngx_pool_t *pool);
void find_test(ngx_hash_t *hash, ngx_str_t addr[], int num, bool expect_found);

TEST_UNIT_P(ngx_hash_by_abo)
{
    //ngx_pool_t *pool = NULL;
    ngx_array_t *array = NULL; // store elements of type ngx_hash_key_t
    ngx_hash_t *hash;

    printf("--------------------------------\n");
    printf("create a new pool:\n");
    printf("--------------------------------\n");

    dump_pool(pool);

    printf("--------------------------------\n");
    printf("create and add urls to it:\n");
    printf("--------------------------------\n");
    array = add_urls_to_array(pool);  //in fact, here should validate array
    dump_hash_array(array);

    printf("--------------------------------\n");
    printf("the pool:\n");
    printf("--------------------------------\n");
    dump_pool(pool);

    hash = init_hash(pool, array);
    if (hash == NULL)
    {
        printf("Failed to initialize hash!\n");
        goto ExitHandler;
    }

    printf("--------------------------------\n");
    printf("the hash:\n");
    printf("--------------------------------\n");
    dump_hash(hash, array);
    printf("\n");

    printf("--------------------------------\n");
    printf("the pool:\n");
    printf("--------------------------------\n");
    dump_pool(pool);

    //find test
    printf("--------------------------------\n");
    printf("find test:\n");
    printf("--------------------------------\n");
    find_test(hash, urls, Max_Num, true);
    printf("\n");

    find_test(hash, urls2, Max_Num2, false);

ExitHandler:
    //release
    ngx_array_destroy(array);
}

ngx_hash_t* init_hash(ngx_pool_t *pool, ngx_array_t *array)
{
    ngx_int_t result;
    /* ngx_hash_init_t�ṹ�����ֶκ��壺
    hash:	���ֶ����ΪNULL����ô�������ʼ�������󣬸��ֶ�ָ���´���������hash��������ֶβ�ΪNULL����ô�ڳ�ʼ��ʱ�����е����ݱ�����������ֶ���ָ��hash���С�
    key:	ָ����ַ�������hashֵ��hash������nginx��Դ�������ṩ��Ĭ�ϵ�ʵ�ֺ���ngx_hash_key_lc��
    max_size:	hash���е�Ͱ�ĸ��������ֶ�Խ��Ԫ�ش洢ʱ��ͻ�Ŀ�����ԽС��ÿ��Ͱ�д洢��Ԫ�ػ���٣����ѯ�������ٶȸ��졣��Ȼ�����ֵԽ��Խ����ڴ���˷�ҲԽ��(ʵ����Ҳ�˷Ѳ��˶���)��
    bucket_size:	ÿ��Ͱ��������ƴ�С����λ���ֽڡ�����ڳ�ʼ��һ��hash���ʱ�򣬷���ĳ��Ͱ�����޷�������������ڸ�Ͱ��Ԫ�أ���hash���ʼ��ʧ�ܡ�
    name:	��hash������֡�
    pool:	��hash������ڴ�ʹ�õ�pool��
    temp_pool:	��hash��ʹ�õ���ʱpool���ڳ�ʼ������Ժ󣬸�pool���Ա��ͷź����ٵ���
    */
    ngx_hash_init_t hinit;

    ngx_cacheline_size = NGX_CPU_CACHE_LINE;

    hinit.hash = NULL;  //if hinit.hash is NULL, it will alloc memory for it in ngx_hash_init
    hinit.key = &ngx_hash_key_lc;  //hash function
    hinit.max_size = Max_Size;
    hinit.bucket_size = Bucket_Size;
    hinit.name = (char*)"my_hash_sample";
    hinit.pool = pool;  //the hash table exists in the memory pool
    hinit.temp_pool = NULL;

    result = ngx_hash_init(&hinit, (ngx_hash_key_t*)array->elts, array->nelts);
    if (result != NGX_OK)
        return NULL;

    return hinit.hash;
}


void dump_hash_array(ngx_array_t* a)
{
    char prefix[] = "          ";

    if (a == NULL)
        return;

    printf("array = 0x%p\n", a);
    printf("  .elts = 0x%p\n", a->elts);
    printf("  .nelts = %d\n", (int)a->nelts);
    printf("  .size = %d\n", (int)a->size);
    printf("  .nalloc = %d\n", (int)a->nalloc);
    printf("  .pool = 0x%p\n", a->pool);

    printf("  elements:\n");
    ngx_hash_key_t *ptr = (ngx_hash_key_t*)(a->elts);
    for (; ptr < (ngx_hash_key_t*)((char*)a->elts + a->nalloc * a->size); ptr++)
    {
        printf("    0x%p: {key = (\"%s\"%s, %d), key_hash = %-10d, value = \"%s\"%s}\n",
            ptr, 
            (char*)ptr->key.data, 
            prefix, 
            (int)ptr->key.len,
            (unsigned int)ptr->key_hash, (char*)ptr->value, prefix);
    }
    printf("\n");
}

/**
* pass array pointer to read elts[i].key_hash, then for getting the position - key
*/
void dump_hash(ngx_hash_t *hash, ngx_array_t *array)
{
    int loop;
    //char prefix[] = "          ";
    u_short test[512] = { 0 };
    ngx_uint_t key;
    ngx_hash_key_t* elts;
    int nelts;

    if (hash == NULL)
        return;

    printf("hash = 0x%p: **buckets = 0x%p, size = %d\n", hash, hash->buckets, (int)hash->size);

    for (loop = 0; loop < (int)hash->size; loop++)
    {
        ngx_hash_elt_t *elt = hash->buckets[loop];
        printf("  0x%p: buckets[%d] = 0x%p\n", &(hash->buckets[loop]), loop, elt);
    }
    printf("\n");

    // ��ȫ����hash����ڴ�ṹ������
    elts = (ngx_hash_key_t*)array->elts;
    nelts = array->nelts;
    for (loop = 0; loop < nelts; loop++)
    {
        char url[Max_Url_Len + 1] = { 0 };

        key = elts[loop].key_hash % hash->size;
        ngx_hash_elt_t *elt = (ngx_hash_elt_t *)((u_char *)hash->buckets[key] + test[key]);

        ngx_strlow((u_char*)url, elt->name, elt->len);
        printf("  buckets %d: 0x%p: {value = \"%s\", len = %u, name = \"%s\"}\n",
            (unsigned int)key, elt, (char*)elt->value,
            elt->len, url); //replace elt->name with url

        test[key] = (u_short)(test[key] + NGX_HASH_ELT_SIZE(&elts[loop]));
    }
}

ngx_array_t* add_urls_to_array(ngx_pool_t *pool)
{
    size_t loop;
    //char prefix[] = "          ";
    ngx_array_t *a = ngx_array_create(pool, Max_Num, sizeof(ngx_hash_key_t));

    for (loop = 0; loop < Max_Num; loop++)
    {
        ngx_hash_key_t *hashkey = (ngx_hash_key_t*)ngx_array_push(a);
        hashkey->key = urls[loop];
        hashkey->key_hash = ngx_hash_key_lc(urls[loop].data, urls[loop].len);
        hashkey->value = (void*)values[loop];
        /** for debug
        printf("{key = (\"%s\"%.*s, %d), key_hash = %-10ld, value = \"%s\"%.*s}, added to array\n",
        hashkey->key.data, Max_Url_Len - hashkey->key.len, prefix, hashkey->key.len,
        hashkey->key_hash, hashkey->value, Max_Ip_Len - strlen(hashkey->value), prefix);
        */
    }

    return a;
}

void find_test(ngx_hash_t *hash, ngx_str_t addr[], int num, bool expect_found)
{
    ngx_uint_t key;
    int loop;
    //char prefix[] = "          ";

    for (loop = 0; loop < num; loop++)
    {
        key = ngx_hash_key_lc(addr[loop].data, addr[loop].len);
        char *value = (char*)ngx_hash_find(hash, key, addr[loop].data, addr[loop].len);
        if (expect_found) {
            H_TEST_ASSERT(value != NULL);
            H_TEST_ASSERT(strncmp(value, values[loop], strlen(value)) == 0);
        }
        else {
            H_TEST_ASSERT(!value);
        }

        if (value)
        {
            printf("(url = \"%s\", key = %-10u) found, (ip = \"%s\")\n",
                (char*)addr[loop].data, (unsigned int)key, (char*)value);
        }
        else
        {
            printf("(url = \"%s\", key = %-10u) not found!\n",
                (char*)addr[loop].data, (unsigned int)key);
        }
    }
}

