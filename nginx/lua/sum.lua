ngx.sleep(0.1)
local args = ngx.req.get_uri_args()
ngx.say(tonumber(args.a) + tonumber(args.b))
