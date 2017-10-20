LUA_LIB="/usr/local/lib/liblua.a"

all: window

window: window.c
	gcc window.c ../ch27/util.c -I../ch27/ $(LUA_LIB) -ldl -lm  -o window



clean:
	rm window
