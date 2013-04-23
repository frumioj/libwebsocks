#define CONNECTING = 0 ;
#define OPEN = 1 ;
#define CLOSING = 2 ;
#define CLOSED = 3 ;

typedef struct _ws_event {

  char *data ;

} ws_event ;

typedef struct _web_socket {

  /*
    Websocket API, from http://www.w3.org/TR/websockets/
   */

  /*
    Following functions to be implemented by concrete implementations
    other than the "base"
   */

  void (*on_open)( struct _web_socket *self ) ;
  void (*on_message)( struct _web_socket *self, struct _ws_event *event ) ;
  void (*on_close)( struct _web_socket *self ) ;
  void (*on_error)( struct _web_socket *self ) ;


  /* To be implemented in base functions */

  void (*binary_type)( struct _web_socket *self, char *type ) ;
  void (*init)( struct _web_socket *self, char *ws_uri ) ;
  
  /* 
     @@TODO: Add other methods (and a struct for message) for sending
     binary data
   */

  void (*send)( struct _web_socket *self, char *message ) ;

  void (*close)( struct _web_socket *self ) ;

  /* Private */

  char *_url ;
  char *_binary_type ;
  short _ready_state ;
  long _buffered_amount ;
  char * _protocol ;
  
} web_socket;
