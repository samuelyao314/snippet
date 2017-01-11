-- From https://moonbingbing.gitbooks.io/openresty-best-practices/content/openresty/get_req_body.html
-- 请求体已经被存入临时文件
local function getfile(file_name)
    local f = assert(io.open(file_name, 'r'))
    local string = f:read("*all")
    f:close()
    return string
end

ngx.req.read_body()
local data = ngx.req.get_body_data()
if nil == data then
    local file_name = ngx.req.get_body_file()
    ngx.say(">> temp file: ", file_name)
    if file_name then
        data = getfile(file_name)
    end
end
ngx.say("hello ", data)
