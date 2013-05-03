#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "websocket_frame.h"

websocket_frame *websocket_frame_new( void ){
  return (struct _websocket_frame *) malloc( sizeof( struct _websocket_frame )) ;
}

void websocket_frame_set_length( websocket_frame *self, int length ){
  self->_length = length ;
}

void websocket_frame_set_opcode( websocket_frame *self, int opcode ){
  self->_opcode = opcode ;
}

void websocket_frame_set_mask( websocket_frame *self, int mask ){
  self->_mask = mask ;
}

void websocket_frame_set_data( websocket_frame *self, char *message ){
  self->_data = message ;
}

void websocket_frame_set_fin( websocket_frame *self, int fin ){
  self->_fin = fin ;
}

char *websocket_frame_as_string( websocket_frame *self ){
  int b1, b2 ;
  int data_len ;
  unsigned short b3 ;
  unsigned long long b4 ;
  char *result ;
  int len_len ;

  b1 = 0x80 | ( self->_opcode & 0x0f ) ;
  
  printf("First: 0x%x\n", b1) ;

  data_len = strlen( self->_data ) ;

  if ( data_len <= 125 ) {
    fprintf(stderr, "len less than 125\n") ;
    b2 = data_len ;
    len_len = sizeof( int ) ;
  }
  else 
    if ( data_len > 125 && data_len < 65536 ){
      fprintf(stderr, "len less than 65536\n") ;
      b2 = 126 ;
      b3 = data_len ;
      len_len = sizeof( int ) + sizeof( unsigned short ) ;
    }
    else
      if ( data_len >= 65536 ){
        b2 = 127 ;
        b4 = data_len ;
        len_len = sizeof( int ) + sizeof( unsigned long long ) ;
      }

  result = (char *) malloc(sizeof(int) + sizeof(int) + len_len) + strlen( self->_data ) ;
  result[0] = b1 ;

  if ( data_len <= 125 )
    result[1] = b2 ;
  else 
    if ( data_len > 125 && data_len < 65536 ){
      result[1] = b2 ;
      // @@TODO how to pack this in "network order"
    }
    else{
      result[1] = b2 ;
      // @@TODO
    }
  
  strcpy( &result[2], self->_data ) ;
  int i ;

  for( i = 0 ; i < strlen( result ) ; i++ ){
    printf("0x%x ", result[i]) ;
  }

  return result ;
}

#ifdef UNIT

int main(int argc, char *argv[ ])
{
  websocket_frame *wf = websocket_frame_new() ;
  wf->_opcode = 0x01 ;
  wf->_data = "Hello" ;

  char *res = websocket_frame_as_string( wf ) ;

  return 0 ;
}

#endif //UNIT
