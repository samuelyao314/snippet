-- From: http://wuzhiwei.net/lua_performance/
-- 测试：local 和 global 性能差距
a = os.clock()
for i = 1,10000000 do
  local x = math.sin(i)
end
b = os.clock()
print("global verson", b-a)


a = os.clock()
local sin = math.sin
for i = 1,10000000 do
    local x = sin(i)
end
b = os.clock()
print("local verson", b-a)
