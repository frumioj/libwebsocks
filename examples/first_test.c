#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <websocket.h>
#include <cencode.h>

static void on_open( web_socket *ws ){
  printf("on open called!\n") ;
}

int main(int argc, char *argv[ ])
{
  web_socket *ws = websocket_new() ;

  ws->on_open = on_open ;
  websocket_init( ws, "ws://localhost:8080" ) ;
  return 0 ;
}
