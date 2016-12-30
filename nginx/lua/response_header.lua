--  OpenResty 系列课程
-- From: http://www.stuq.org/courseware/1015/1357

-- curl -I http://127.0.0.1:8080/api/hello 

ngx.header.Foo = 'Bar'

ngx.header['Foo_Bar'] = 'Bar2'
