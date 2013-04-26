#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http_request.h"

#define SCHEME_DELIM "://"
#define PORT_DELIM ":"
#define PATH_DELIM "/"

http_request *http_request_new( void ){
  return (struct _http_request *) malloc( sizeof( struct _http_request )) ;
}

void http_request_init( http_request *req, char *method, char *uri ){
  char *aport = strdup( strstr( uri, SCHEME_DELIM ) + strlen( SCHEME_DELIM ) ) ;
  char *path = strstr( aport, PATH_DELIM ) ;

  if ( path != NULL ){
    req->path = strdup( path ) ;
  }
  else{
    req->path = "/" ;
  }

req->method = strdup( method ) ;
req->uri = strdup( uri ) ;
req->header_names = NULL ;
req->header_values = NULL ;
req->header_row = 0 ;
}

void http_request_set_header( http_request *req, char *name, char *value ){
  int len = strlen( name ) ;
  int v_len = strlen( value ) ;

  req->header_names = (char **) realloc( req->header_names, sizeof(char**) * (req->header_row + 1)) ;
  req->header_values = (char **) realloc( req->header_values, sizeof(char**) * (req->header_row + 1)) ;
  req->header_names[req->header_row] = (char *) malloc( len + 1 ) ;
  req->header_values[req->header_row] = (char *) malloc( v_len + 1 ) ;
  strncpy( req->header_names[req->header_row], name, len ) ;
  strncpy( req->header_values[req->header_row], value, v_len ) ;
  req->header_row++ ;
}

char *http_request_as_string( http_request *req ){
  int body_len = 0 ;

  if ( req->body && req->body != NULL )
    body_len = strlen(req->body) ;

  char *buffer = calloc(((req->header_row * 255) + (body_len + 255)), sizeof(char)) ;

strncat( buffer, req->method, strlen(req->method) ) ;
strncat( buffer, " ", 1 ) ;
strncat( buffer, req->path, strlen(req->path) ) ;
strncat( buffer, " HTTP/1.1\r\n", 11 ) ;

  int i ;

  for ( i = 0 ; i < req->header_row; i++ ){
    strncat( buffer, req->header_names[ i ], strlen( req->header_names[ i ] )) ;
    strncat( buffer, ": ", 2 ) ;
    strncat( buffer, req->header_values[ i ], strlen( req->header_values[ i ] )) ;
    strncat( buffer, "\r\n", 2 ) ;
  }

//strncat( buffer, "\r\n", 2 ) ;

  if ( req->body && req->body != NULL ){
    strncat( buffer, req->body, strlen( req->body )) ;
  }

  fprintf( stderr, "REQ: %sEND", buffer ) ;
  return buffer ;
}
