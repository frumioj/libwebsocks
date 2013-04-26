#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "utils.h"
#include "cencode.h"
#include "http_request.h"
#include "websocket.h"

#define SCHEME_DELIM "://"
#define PORT_DELIM ":"
#define BUFSIZE 1024
#define NONCE_SIZE 16

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

  char *hrequest =
     "GET / HTTP/1.1\r\n"
     "Upgrade: WebSocket\r\n"
     "Connection: Upgrade\r\n"
     "Host: localhost:8080\r\n"
     "Sec-WebSocket-Key: 42VhyIDNJjphjqRMidI1Tw==\r\n" 
     "Sec-WebSocket-Version: 13\r\n"
     "\r\n" ;

  http_request *request = http_request_new() ;
  http_request_init( request, GET, ws_uri ) ;
  http_request_set_header( request, "Upgrade", "WebSocket" ) ;
  http_request_set_header( request, "Connection", "Upgrade" ) ;
  http_request_set_header( request, "Host", host ) ;
  http_request_set_header( request, "Sec-WebSocket-Version", "13" ) ;
  http_request_set_header( request, "Sec-WebSocket-Key", b64nonce ) ;

  char *http_request = http_request_as_string( request ) ;

  int n = write(sockfd, hrequest, strlen(hrequest));

  if (n < 0) 
    fprintf(stderr,"ERROR writing to socket\n");

  /* print the server's reply */
  bzero(buf, BUFSIZE);
  n = read(sockfd, buf, BUFSIZE);
  
  if (n < 0) 
    fprintf(stderr, "ERROR reading from socket\n");

  if ( self->on_open ){
    self->on_open(self) ;
  }

  printf("Echo from server: %s", buf);
  close(sockfd);


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

