#define WS_FIN 0x01

typedef struct _websocket_frame websocket_frame ;

struct _websocket_frame {

  int _length ;
  int _opcode ;
  int _mask ;
  char *_data ;
  int _fin ;

} ;

websocket_frame *websocket_frame_new( void ) ;

void websocket_frame_set_length( websocket_frame *self, int length ) ;
void websocket_frame_set_opcode( websocket_frame *self, int opcode ) ;
void websocket_frame_set_mask( websocket_frame *self, int mask ) ;
void websocket_frame_set_data( websocket_frame *self, char *message ) ;
void websocket_frame_set_fin( websocket_frame *self, int fin ) ;

