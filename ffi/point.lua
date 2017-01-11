-- From: http://luajit.org/ext_ffi_tutorial.html
-- 使用 C 数据结构
--
local ffi = require("ffi")

ffi.cdef[[
typedef struct {double x, y;} point_t;
]]

local point
local mt = {
    __add = function (a, b) return point(a.x+b.x, a.y+b.y) end,
    __len = function(a) return math.sqrt(a.x*a.x + a.y*a.y) end,
    __index = {
        area = function(a) return a.x*a.x + a.y*a.y end
    },
}

point = ffi.metatype("point_t", mt)
--print(point)

local a = point(3, 4)
--print(a)
print(a.x, a.y)

print(#a)
print(a:area())
local b = a + point(0.5, 8)
print(#b)
