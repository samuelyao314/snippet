ngx.req.read_body()
local data = ngx.req.get_body_data()
ngx.print(ngx.md5(data))
