all: select epoll epolloneshot 

clean:
	rm -rf select epoll epolloneshot

epolloneshot:
	gcc -o epolloneshot epoll_oneshot.c -lpthread -Wall -g

epoll:
	gcc  -o epoll epoll.c -Wall -g


select:
	gcc -o select select.c -Wall -g

