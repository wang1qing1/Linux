.PHONY:all
all:tcp_server tcp_client

tcp_server:tcp_server.cc
	g++ -o $@ $^ -std=c++11 -lpthread
tcp_client:tcp_client.cc
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -rf tcp_server tcp_client


