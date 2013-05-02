typedef struct _websocket_frame websocket_frame ;

struct websocket_frame {

  int _length ;
  int _opcode ;
  int _mask ;
  char *data ;
  int _fin = WS_FIN ;

} ;

websocket_frame *websocket_frame_new( void ) ;

void websocket_frame_set_length( websocket_frame *self, int length ) ;
void websocket_frame_set_opcode( websocket_frame *self, WS_TEXT ) ;
void websocket_frame_set_mask( websocket_frame *self, WS_NOMASK ) ;
void websocket_frame_set_data( websocket_frame *self, char *message ) ;
void websocket_frame_set_fin( websocket_frame *self, WS_FIN ) ;

