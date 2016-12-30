local start_time = ngx.now()
local res1 = ngx.location.capture_multi( {
    {"/api/sum", {args={a=3, b=8}}}
})
local res2 = ngx.location.capture_multi( {
    {"/api/subduction", {args={a=3, b=8}}}
})
ngx.say("status:", res1.status, " response:", res1.body)
ngx.say("status:", res2.status, " response:", res2.body)
ngx.say("time used:", ngx.now() - start_time)
