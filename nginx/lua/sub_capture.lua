--  OpenResty 系列课程
-- http://www.stuq.org/courseware/1015/1359

--ngx.say('hello')

ngx.req.read_body()
local body = ngx.req.get_body_data()
ngx.say(body)
