#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "websocket.h"

web_socket *websocket_new( void ){
  return (web_socket *) malloc( sizeof( web_socket )) ;
}

void websocket_init( web_socket *self, char *ws_uri ){

  self->_url = (char *) malloc(sizeof(char) * strlen( ws_uri )) ;
  strcpy( ws_uri, self->_url ) ;

  /* 
     @@TODO Implement this according to the spec.
   */
}

/* 
   Destroy/free all resources used

 */

void websocket_destroy( web_socket *self ){
  free (self->_url) ;
}

void websocket_binary_type( web_socket *self, char *type ){
  self->_binary_type = (char *) malloc(sizeof(char) * strlen( type )) ;
  strcpy( type, self->_binary_type ) ;
}

void websocket_send( web_socket *self, char *message ){
  /*

    @@TODO Implement me!

   */
}
