-- From: https://moonbingbing.gitbooks.io/openresty-best-practices/content/openresty/work_with_location.html
--
local res = ngx.location.capture(
                "/api/sum",  {args={a=3, b=8}}
        )
ngx.say("status:", res.status, " response:", res.body)
