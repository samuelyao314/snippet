-- From: http://luajit.org/ext_ffi_tutorial.html
--
local ffi = require("ffi")
ffi.cdef[[
int poll(struct pollfd *fds, unsigned long nfds, int timeout);
]]

function sleep(s)
    ffi.C.poll(nil, 0, s*1000)
end

for i=1, 160 do
    io.write("."); io.flush()
    sleep(0.01)
end
io.write("\n")
