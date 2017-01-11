-- From: https://moonbingbing.gitbooks.io/openresty-best-practices/content/openresty/work_with_location.html

ngx.sleep(0.1)
local args = ngx.req.get_uri_args()
ngx.print(tonumber(args.a) - tonumber(args.b))
ngx.say("status:", res.status, " response: ", res.body)
