local delay = 5
local handler
 handler = function (premature)
     -- do some routine job in Lua just like a cron job
     if premature then
         return
     end

     ngx.log(ngx.ERR, "init worker running..., pid:", ngx.worker.pid())
     local ok, err = ngx.timer.at(delay, handler)
     if not ok then
         ngx.log(ngx.ERR, "failed to create the timer: ", err)
         return
     end
 end

 local ok, err = ngx.timer.at(delay, handler)
 if not ok then
     ngx.log(ngx.ERR, "failed to create the timer: ", err)
     return
 end
