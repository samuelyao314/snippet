-- From: https://moonbingbing.gitbooks.io/openresty-best-practices/content/openresty/get_url_param.html
--
-- Input:
-- curl 'http://127.0.0.1:8080/api/deliver_param'
--
local res = ngx.location.capture(
        '/api/print_param',
        {
            method = ngx.HTTP_POST,
            args = ngx.encode_args({a=1, b='2&'}),
            body = ngx.encode_args({c=3, b='4&'}),
        }
    )

ngx.say(res.body)
