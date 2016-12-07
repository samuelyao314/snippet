-- From: http://wuzhiwei.net/lua_performance/
-- 比较2种字符串连接方式
a = os.clock()
local s = ''
for i = 1,300000 do
        s = s .. 'a'
    end
b = os.clock()
print("concat 1", b-a)


a = os.clock()
local s = ''
local t = {}
for i = 1,300000 do
    t[#t + 1] = 'a'
end
s = table.concat( t, '')
b = os.clock()
print("concat 2", b-a)

