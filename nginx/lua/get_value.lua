local redis = require "resty.redis_iresty"
local red = redis:new()

function set_to_cache(key, value, exptime)
    if not exptime then
        exptime = 0
    end
    local cache_ngx = ngx.shared.cache_ngx
    local succ, err, forcible = cache_ngx:set(key, value, exptime)
    return succ
end

function get_from_cache(key)
    local cache_ngx = ngx.shared.cache_ngx
    local value = cache_ngx:get(key)
    if not value then
        value = get_from_redis(key)
        set_to_cache(key, value)
    end
    return value
end

function get_from_redis(key)
    local res, err = red:get("dog")
    if res then
        return "yes"
    else
        return "no"
    end
end

local res = get_from_cache('dog')
ngx.say(res)
