local _M = {}

local function f()
    for i=1, 100 do
        coroutine.yield(i)
    end
end


function _M.create()
    local co = coroutine.create(f)
    _M.room_co = co
    return "create succ"
end

function _M.resume()
    return coroutine.resume(_M.room_co)
end


return _M
