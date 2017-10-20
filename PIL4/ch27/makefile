LUA_LIB="/usr/local/lib/liblua.a"

all: simple_lua example

simple_lua:
	gcc simple_lua.c $(LUA_LIB) -ldl -lm  -o simple_lua

example:
	gcc example.c util.c $(LUA_LIB) -ldl -lm -o example


clean:
	rm simple_lua example
