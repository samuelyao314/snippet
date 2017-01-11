-- From: https://moonbingbing.gitbooks.io/openresty-best-practices/content/lua/FFI.html
local ffi = require "ffi"

local int_array_t = ffi.typeof("int[?]")
print(int_array_t)
local bucket_sz = 32
local bucket_v = ffi.new(int_array_t, 32)
print(bucket_v)

ffi.fill(bucket_v, ffi.sizeof(int_array_t, bucket_sz), 0)


-- 对象的垃圾回收
ffi.cdef[[
typedef struct { int *a; } foo_t;
]]

local s = ffi.new("foo_t", ffi.new("int[10]"))   -- WRONG!
print(s)

local a = ffi.new("int[10]")  -- OK
local s = ffi.new("foo_t", a)
