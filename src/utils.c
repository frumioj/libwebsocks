#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "cencode.h"
#include "utils.h"

char *nonce( size_t num_bytes ){

  srand ((unsigned int) time (NULL)) ;
  
  char *stream = malloc ( num_bytes + 1) ;
  size_t i ;

  for ( i = 0 ; i < num_bytes; i++ ){
    stream[i] = rand() ;
  }

  stream[i] = 0 ;

  return stream ;
}

char *base64_encode( char *to_encode ){
  base64_encodestate state ;
  base64_init_encodestate( &state ) ;

  long enc_len = ((strlen(to_encode)/3) + 0.5 ) * 5 ;

  char *encoded = calloc(enc_len, sizeof(char)) ;

  puts("about to encde") ;
  int r1 = base64_encode_block( to_encode, strlen( to_encode ), encoded, &state ) ;
  puts("so far encoded") ;
  base64_encode_blockend( encoded + r1, &state ) ;
  puts("block ended") ;

  char *end ;
  end = encoded + strlen(encoded) - 1;
  while(end > encoded && (*end == ' ' ||
                          *end == '\n' ||
                          *end == '\r')) end--;
  
  // Write new null terminator
  *(end+1) = 0;
  puts(encoded) ;

  return encoded ;
}
