LUA_LIB="/usr/local/lib/liblua.a"

mylib.so: mylib.c
	gcc -fpic -c  mylib.c
	gcc -fpic -c  util.c
	gcc -shared -o mylib.so mylib.o util.o


clean:
	rm mylib.so mylib.o
