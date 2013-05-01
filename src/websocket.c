#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <openssl/sha.h>

#include "utils.h"
#include "cencode.h"
#include "http_request.h"
#include "websocket.h"

#define SCHEME_DELIM "://"
#define PORT_DELIM ":"
#define BUFSIZE 1024
#define NONCE_SIZE 16
#define WS_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

typedef enum { false, true } bool;

web_socket *websocket_new( void ){
  return (struct _web_socket *) malloc( sizeof( struct _web_socket )) ;
}

void websocket_init( web_socket *self, char *ws_uri ){

  char *aport = strdup( strstr( ws_uri, SCHEME_DELIM ) + strlen( SCHEME_DELIM ) ) ;
  char *host = strdup( aport ) ;
  char *address = strtok( aport, PORT_DELIM ) ; 
  char buf[BUFSIZE] ;
  char *b64nonce = base64_encode( nonce( NONCE_SIZE ) ) ;

  puts( address ) ;
  puts( b64nonce ) ;

  char *port = strdup(strrchr( ws_uri, ':' ) + 1) ; 
  self->_url = ws_uri ;
  puts( port ) ;

  int sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ;
  
  if (sockfd < 0){
    fprintf(stderr,"ERROR opening socket to host") ;
    exit(0) ;
  }

  struct hostent *server = gethostbyname( address ) ;

  if ( server == NULL ){
    fprintf(stderr, "ERROR: no such host as %s!\n", address ) ;
    exit(0);
  }

  struct sockaddr_in serveraddr ;

  /* build the server's Internet address */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
        (char *)&serveraddr.sin_addr.s_addr, server->h_length);
  serveraddr.sin_port = htons(atoi(port));
  
  /* connect: create a connection with the server */
  if (connect(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
    fprintf(stderr, "ERROR connecting\n");
    exit(0) ;
  }

  /* char *hrequest = */
  /*    "GET / HTTP/1.1\r\n" */
  /*    "Upgrade: WebSocket\r\n" */
  /*    "Connection: Upgrade\r\n" */
  /*    "Host: localhost:8080\r\n" */
  /*    "Sec-WebSocket-Key: 42VhyIDNJjphjqRMidI1Tw==\r\n"  */
  /*    "Sec-WebSocket-Version: 13\r\n" */
  /*    "\r\n" ; */

  http_request *request = http_request_new() ;
  http_request_init( request, GET, ws_uri ) ;
  http_request_set_header( request, "Upgrade", "WebSocket" ) ;
  http_request_set_header( request, "Connection", "Upgrade" ) ;
  http_request_set_header( request, "Host", host ) ;
  http_request_set_header( request, "Sec-WebSocket-Version", "13" ) ;
  http_request_set_header( request, "Sec-WebSocket-Key", b64nonce ) ;

  char *http_request = http_request_as_string( request ) ;

  int n = write(sockfd, http_request, strlen(http_request));

  if (n < 0) 
    fprintf(stderr,"ERROR writing to socket\n");

  /* print the server's reply */
  bzero(buf, BUFSIZE);
  n = read(sockfd, buf, BUFSIZE);
  
  if (n < 0) 
    fprintf(stderr, "ERROR reading from socket\n");

  char delims[] = "\n" ;
  char *res = NULL ;
  char *result = NULL ;
  bool is_ws = false ;
  bool upgrade = false ;
  bool upswitch = false ;
  bool nonce_agreed = false ;

  res = strtok( buf, delims ) ;
  result = (char *) malloc ((strlen(res) + 1) * sizeof( char )) ;
  strcpy( result, res ) ;

  char * end ;

  end = result + strlen( result) - 1 ;
  end = 0 ;

  while( result != NULL ) {

    if ( strcasestr( result, "HTTP" ) != NULL &&
         strcasestr( result, "101" ) != NULL )
      upswitch = true ;
    else 
      if ( strcasestr( result, "connection" ) != NULL &&
         strcasestr( result, "upgrade" ) != NULL )
        upgrade = true ;
      else 
        if ( strcasestr( result, "upgrade" ) != NULL &&
             strcasestr( result, "websocket" ) != NULL )
          is_ws = true ;
        else
          if ( strcasestr( result, "sec-websocket-accept" ) != NULL){
            char *server_nonce = strdup( strstr( result, ":" ) + 1) ;

            while (*server_nonce == ' ' || 
                   *server_nonce == '\t' || 
                   *server_nonce == '\n' || 
                   *server_nonce == '\v' ||
                   *server_nonce == '\f' ||
                   *server_nonce == '\r') server_nonce++;

            char *end ;
            end = server_nonce + strlen(server_nonce) - 1;
            while(end > server_nonce && (*end == ' ' ||
                                    *end == '\n' ||
                                    *end == '\r')) end--;
            
            // Write new null terminator
            *(end+1) = 0;

            //printf("NONCE received: |%s|\n", server_nonce ) ;
            char compare[80] ;
            unsigned char hash[20] ;

            strcpy( compare, b64nonce ) ;
            strcat( compare, WS_KEY ) ;
            //printf("comp: %s\n", compare ) ;
            SHA1(compare, strlen(compare), hash);
            //printf("comparison: \n{%s}\n", base64_encode(hash)) ;
            //printf("comparison: \n{%s}\n", server_nonce) ;
            nonce_agreed = strcmp(base64_encode( hash ), server_nonce) == 0 ;
            printf("Agrees?: %d\n", strcmp(base64_encode( hash ), server_nonce) ) ;
          }
               
    printf( "result is %s\n", result );    
    result = strtok( NULL, delims );
  }

  printf("Proceed: %s\n", upgrade && is_ws && upswitch && nonce_agreed ? "True" : "False") ;

  if ( upgrade && is_ws && upswitch && nonce_agreed ){

    if ( self->on_open ){
      self->on_open(self) ;
      self->_socket = sockfd ;
    }
  }
  else{
    close(sockfd);
  }

  /* 
     @@TODO Implement this according to the spec.
   */
}

void websocket_close( web_socket *self ){
  if ( self->_socket )
    close( self->_socket ) ;
}

void websocket_destroy( web_socket *self ){
  free (self->_url) ;
}

void websocket_binary_type( web_socket *self, char *type ){
  self->_binary_type = (char *) malloc(sizeof(char) * strlen( type )) ;
  strcpy( type, self->_binary_type ) ;
}

void websocket_send( web_socket *self, char *message ){

  websocket_frame_start( self->_socket ) ;
  websocket_frame_text( message ) ;
  /*

    @@TODO Implement me!

   */
}


