ngx.sleep(0.1)
local args = ngx.req.get_uri_args()
ngx.print(tonumber(args.a) - tonumber(args.b))
igx.say("status:", res.status, " response: ", res.body)
