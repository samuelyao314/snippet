-- OpenResty 系列课程
-- http://www.stuq.org/courseware/1015/1361

local shared = ngx.shared["ngx_cache"]

local succ, err, forc

for index = 1, 10000, 1 do
    succ, err, forc = shared:set(tostring(index), string.rep('a', 50))
end

local keys = shared:get_keys(0)

ngx.say(#keys)
