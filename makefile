all: client.c server.c
	g++ -o server server.c
	g++ -o client client.c

.PHONY: clean

clean:
	rm client
	rm server
