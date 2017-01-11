-- From: http://www.stuq.org/course/1015/courseware/1458
-- 比较ipair, pair 在LuaJIT下的性能差异
-- http://wiki.luajit.org/NYI
--

local t = {}
for i=1, 1000*50 do
    t[i] = i
end

local function do_loop(func_pair)
    local sum = 0
    for i=1, 1000 do
        for k, v in func_pair(t) do
            sum = sum + v
        end
    end
end


local s = os.clock()
do_loop(ipairs)
print("ipairs, ", os.clock() - s)

local s = os.clock()
do_loop(pairs)
print("pairs, ", os.clock() - s)
