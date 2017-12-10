
build:
	$(MAKE) -f objs/Makefile

install:
	$(MAKE) -f objs/Makefile install

clean:
	rm -rf Makefile objs

upgrade:
	/home/samuel/local/workspace/nginx/nginx-0.1.0/sbin/nginx -t
	kill -USR2 `cat /home/samuel/local/workspace/nginx/nginx-0.1.0/logs/nginx.pid`
	sleep 1
	test -f /home/samuel/local/workspace/nginx/nginx-0.1.0/logs/nginx.pid.newbin
	kill -WINCH `cat /home/samuel/local/workspace/nginx/nginx-0.1.0/logs/nginx.pid`

