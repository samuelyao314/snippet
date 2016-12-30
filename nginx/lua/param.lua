-- https://moonbingbing.gitbooks.io/openresty-best-practices/content/openresty/simple_api.html
--
local _M = {}

function _M.is_number(...)
    local arg = {...}

    local num
    for _, v in ipairs(arg) do
        num = tonumber(v)
        if nil == num then
            return false
        end
    end
    return true
end

return _M
