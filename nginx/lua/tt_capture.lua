--  OpenResty 系列课程
-- http://www.stuq.org/courseware/1015/1359

ngx.req.read_body()
local data = ngx.req.get_body_data()

local res = ngx.location.capture('/api/sub_capture', {body = data})
ngx.say(res.body)
--ngx.say(data)
