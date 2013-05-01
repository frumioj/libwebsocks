#include "i_websocket.h"

web_socket *websocket_new( void ) ;

void websocket_init( web_socket *self, char *ws_uri ) ;
void websocket_binary_type( web_socket *ws, char *type ) ;
void websocket_send( web_socket *self, char *message ) ;
void websocket_close( web_socket *self ) ;
void websocket_destroy( web_socket *self ) ;
