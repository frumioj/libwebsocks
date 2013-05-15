gcc -shared -Wall -fpic -o libwebsocks.dylib -I./src -lcrypto src/websocket.c src/cencode.c  src/utils.c src/http_request.c src/websocket_frame.c
gcc -o first_test examples/first_test.c -I./src -L. -lwebsocks
