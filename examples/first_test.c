#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <websocket.h>
#include <cencode.h>

static void on_open( web_socket *ws ){
  char *message = "Hello" ;
  printf("My on open called!\n") ;
  websocket_send( ws, message ) ;
  websocket_close( ws ) ;
}

int main(int argc, char *argv[ ])
{
  web_socket *ws = websocket_new() ;
  char *message = "Play:Client=abcde123,Name=univac.webm,ChunkId=1" ;
  //char *message = "Hello" ;
  ws->on_open = on_open ;
  websocket_init( ws, "ws://localhost:8080" ) ;
  websocket_send( ws, message ) ;
  websocket_close( ws ) ;
  return 0 ;
}
