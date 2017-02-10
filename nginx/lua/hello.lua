--  OpenResty 系列课程
local args = ngx.req.get_uri_args()
ngx.say("hello " .. args.name)
