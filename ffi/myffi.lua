local ffi = require "ffi"
local myffi = ffi.load('myffi', true)

ffi.cdef[[
int add(int x, int y);
]]

local res = ffi.C.add(1, 2)
print(res)
