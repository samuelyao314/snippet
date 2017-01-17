-- https://moonbingbing.gitbooks.io/openresty-best-practices/content/ngx_lua/log.html
local logger = require "resty.logger.socket"
if not logger.initted() then
    local ok, err = logger.init{
        host = '127.0.0.1',
        port = 514,
        flush_limit = 1,   --日志长度大于flush_limit的时候会将msg信息推送一次
        drop_limit = 99999,
    }
    if not ok then
        ngx.log(ngx.ERR, "failed to initialize the logger: ",err)
        return
    end
end

local msg = "hello openresty"
local bytes, err = logger.log(msg)
if err then
    ngx.log(ngx.ERR, "failed to log message: ", err)
    return
end

local bytes, err = logger.flush()
ngx.say(bytes)
