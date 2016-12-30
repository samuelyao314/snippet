-- OpenResty 系列课程
-- http://www.stuq.org/courseware/1015/1360

local delay = 5

local handler
handler = function (premeture, param)
    if premeture then
        return
    end
    ngx.log(ngx.ERR, "param is " .. param)

    ngx.timer.at(delay, handler, "hello again")
end


local ok, err = ngx.timer.at(delay, handler, "hello hippo master")
