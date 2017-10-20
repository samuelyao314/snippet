LUA_LIB="/usr/local/lib/liblua.a"

array.so: array.c
	gcc -fpic -c  array.c
	gcc -shared -o array.so array.o


clean:
	rm array.so array.o
