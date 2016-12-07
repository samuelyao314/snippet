require "lstring"

local s = "abc"
local tmp = string.hexlify(s)
print("hexlify", tmp)
local s2 = string.unhexlify(tmp)
assert(s2 == s, "invalid, unhexlify")

local s = "aaa|bbb|ccc"
local array = string.split(s, "|")
print("split: ", #array)

local s = "  a line\n"
s = string.strip(s)
assert(s == "a line", "error, strip")
