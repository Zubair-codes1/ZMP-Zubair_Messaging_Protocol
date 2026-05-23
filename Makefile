# runs both server and client binaries
all: server client

# creates server binary including include files
server: src/server.c src/protocol.c
	gcc src/server.c src/protocol.c -o server -Iinclude
# where to find the include file ------------^

# creates client binary inluding the include files
client: src/client.c src/protocol.c
	gcc src/client.c src/protocol.c -o client -Iinclude

# deleted the compiled binaries
clean:
	rm -f server client
