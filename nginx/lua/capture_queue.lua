-- From: https://moonbingbing.gitbooks.io/openresty-best-practices/content/openresty/work_with_location.html
local start_time = ngx.now()
local res1 = ngx.location.capture("/api/sum", {args={a=3, b=8}})
local res2 = ngx.location.capture("/api/subduction", {args={a=3, b=8}})
ngx.say("status:", res1.status, " response:", res1.body)
ngx.say("status:", res2.status, " response:", res2.body)
ngx.say("time used:", ngx.now() - start_time)
