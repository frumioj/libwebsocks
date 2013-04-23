#include "i_websocket.h"

/*
  void websocket_on_open( web_socket *ws ) ;
  void websocket_on_message( web_socket *self, ws_event *event ) ;
  void websocket_on_close( web_socket *ws ) ;
  void websocket_on_error( web_socket *ws ) ;
*/

web_socket *web_socket_new( void ) ;

void websocket_init( web_socket *self, char *ws_uri ) ;
void websocket_binary_type( web_socket *ws, char *type ) ;
void websocket_send( web_socket *self, char *message ) ;
void websocket_destroy( web_socket *self ) ;
