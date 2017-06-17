-- openresty最佳实践：获取URI 参数
-- input: curl 'http://127.0.0.1:8080/api/print_param?a=1&b=2%26' -d 'c=3&d=4%25'

local arg = ngx.req.get_uri_args()
for k, v in pairs(arg) do
    ngx.say("[GET] key:", k, " v:", v)
end

ngx.req.read_body()
local arg = ngx.req.get_post_args()
for k,v in pairs(arg) do
    ngx.say("[POST] key: ", k, " v:", v)
end



