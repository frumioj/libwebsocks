Simple client library for HTML Websockets

Sample usage:

    int main(int argc, char *argv[ ])
    {
      web_socket *ws = websocket_new() ;
      websocket_init( ws, "ws://localhost:8080" ) ;
      ws->on_open = &on_open ;
      return 0 ;
    }