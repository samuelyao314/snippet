--  OpenResty 系列课程
-- From: http://www.stuq.org/courseware/1015/1357

local function print_table(t)
    local function parse_array(key, tab)
        local str = ''
        for _, v in pairs(tab) do
            str = str .. key .. ' ' .. v .. '\r\n'
        end
        return str
    end

    local str = ''
    for k, v in pairs(t) do
        if type(v) == "table" then
            str = str .. parse_array(k, v)
        else
            str = str .. k .. ' ' .. (v) .. '\r\n'
        end
    end
    return str
end

--[[
local header = ngx.req.get_headers(0, true)
ngx.say(print_table(header))
--]]


ngx.req.set_header('Foo', 'Bar1')
ngx.req.set_header('user-agent', nil)
local res = ngx.location.capture('/api/sub_request_header')

if res.status  == ngx.HTTP_OK then
    ngx.say(res.body)
else
    ngx.say(res.status)
end



-- test
-- curl -H 'Foo:bar'  http://127.0.0.1:8080/request_header.json

