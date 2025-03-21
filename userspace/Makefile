all: udp_server tcp_server

udp_server: udp_server.c
	$(CC) -o udp_server udp_server.c

tcp_server: tcp_server.c
	$(CC) -o tcp_server tcp_server.c

clean:
	rm udp_server tcp_server *.o
