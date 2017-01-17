-- https://moonbingbing.gitbooks.io/openresty-best-practices/content/web/check_data_valid.html
-- 检查HEX字符串
-- 
-- 纯 lua 版本，优点是兼容性好，可以使用任何 lua 语言环境
function check_hex_lua( str, correct_len )
    if "string" ~= type(str) then
        return false
    end

    for i=1, #str do
        local c = str:sub(i, i)
        if (c >= 'A' and c <= 'F') or
         (c >= 'a' and c <= 'f') or
         (c >= '0' and c <= '9')
         then
            -- print(c)
        else
            return false
        end
    end

    if correct_len and correct_len ~= #str then
      return false
    end

    return true
end

-- 使用 ngx.re.* 完成，没有使用任何调优参数
function check_hex_default( str )
    if "string" ~= type(str) then
        return false
    end

    return ngx.re.find(str, "([^0-9^a-f^A-F])")
end

-- 使用 ngx.re.* 完成，使用调优参数 "jo"
function check_hex_jo( str )
    if "string" ~= type(str) then
        return false
    end

    return ngx.re.find(str, "([^0-9^a-f^A-F])", "jo")
end


-- 下面就是测试用例部分代码
function do_test( name, fun )
    ngx.update_time()
    local start = ngx.now()

    local t = "012345678901234567890123456789abcdefABCDEF"
    for i=1,10000*300 do
        fun(t)
    end

    ngx.update_time()
    print(name, "\ttimes:", ngx.now() - start)
end

do_test("check_hex_lua", check_hex_lua)
do_test("check_hex_default", check_hex_default)
do_test("check_hex_jo", check_hex_jo)
