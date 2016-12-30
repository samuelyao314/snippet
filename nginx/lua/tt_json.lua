--  OpenResty 系列课程
-- From: http://www.stuq.org/courseware/1015/1358
--

local json = require "cjson"

local function format_table(t)
    local str = ''
    for k, v in pairs(t) do
        str = str .. k .. '--' .. v .. '\r\n'
    end
    return str
end


-- json 字符串转成lua table
--[[
local str_json = '{"key": "this is key", "value": "this is value", "num": 1}'
local t = json.decode(str_json)
ngx.say(format_table(t))
--]]

-- lua table 转成json 字符串
--[[
local t = {key ="table key", value = "table value", num=1}
local str_json = json.encode(t)
ngx.say(str_json)
--]]

-- lua table 转成json数组
--[[
local t = {key={"list", "list2", "list..."}, num=1}
local str_json = json.encode(t)
ngx.say(str_json)
--]]

-- 空 table
--[[
local t = {}
json.encode_empty_table_as_object(false)
local str_json = json.encode(t)
ngx.say(str_json)
--]]

-- sparse array
--[[
local t = {1, 2}
t[1000] = 99
json.encode_sparse_array(true)
local str_json = json.encode(t)
ngx.say(str_json)
--]]

local function json_decode(str)
    local json_value = nil
    pcall(function (str) json_value = json.decode(str) end, str)
    return json_value
end

-- error capture
local str_error_json = [["key": "this is key", "value": "this is value"}]]
--local t = json.decode(str_error_json)
local t = json_decode(str_error_json)
ngx.say(t)
