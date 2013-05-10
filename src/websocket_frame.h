#define WS_FIN 0x01
#define WS_TEXT 0x01
#define WS_NOMASK 0x00

typedef struct _websocket_frame websocket_frame ;

struct _websocket_frame {

  int _length ;
  int _opcode ;
  unsigned char _mask[2] ;
  char *_data ;
  int _fin ;

} ;

websocket_frame *websocket_frame_new( void ) ;

void websocket_frame_init( websocket_frame *self, int opcode, unsigned char mask[4]) ;
void websocket_frame_set_length( websocket_frame *self, int length ) ;
void websocket_frame_set_opcode( websocket_frame *self, int opcode ) ;
void websocket_frame_set_mask( websocket_frame *self, unsigned char mask[4] ) ;
void websocket_frame_set_data( websocket_frame *self, char *message ) ;
void websocket_frame_set_fin( websocket_frame *self, int fin ) ;
unsigned char *websocket_frame_as_string( websocket_frame *self, int *frame_size ) ;
