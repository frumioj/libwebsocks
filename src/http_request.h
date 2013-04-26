#define GET "GET"

typedef struct _http_request http_request ;

struct _http_request {

  char *method ;
  char **header_names ;
  char **header_values ;
  int header_row ;
  char *body ;
  char *uri ;
  char *path ;

} ;

http_request *http_request_new() ;

void http_request_init( http_request *req, char *method, char *uri ) ;
void http_request_set_header( http_request *req, char *name, char *value ) ;
char *http_request_as_string( http_request *req ) ;
