-- From: https://moonbingbing.gitbooks.io/openresty-best-practices/content/openresty/log_response.html
-- 日志输出

local num = 55
local str = "string"
local obj

ngx.log(ngx.ERR, "num:", num)
ngx.log(ngx.INFO, " string:", str)
print([[i am print]])
ngx.log(ngx.ERR, " object:", obj)
