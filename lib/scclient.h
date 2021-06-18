#ifndef __SCCLIENT_H__
#define __SCCLIENT_H__

enum parseresult
{
    ISAUTHENTICATED,
    PUBLISH,
    REMOVETOKEN,
    SETTOKEN,
    EVENT,
    ACKRECEIVE
};

struct recv_message
{
    int rid;
    int cid;
    json_object *data;
    char *event;
    json_object *error;
};

struct ackdata
{
    char *name;
    int id;
    /* data */
};
struct ackobject
{
    /* data */
    char *event;
    void (*listener)(char *event, json_object *error, json_object *data);
};

struct socket
{
    char *id;
    char *address;
    int port;
    char *token;
    char *path;
    char *proxy_address;
    int proxy_port;
    int (*connect)();
    void (*disconnect)();

    //Declaring function pointers to set of callbacks
    void (*allowselfsigned)();
    void (*connect_callback)(struct socket *);
    void (*disconnect_callback)(struct socket *);
    void (*connect_error_callback)(struct socket *);
    void (*onauth_callback)(struct socket *, bool);
    void (*onauthtoken_callback)(struct socket *, char *token);

    //Declaring functions for sending and receiving events
    void (*emitint)(char *, int);
    void (*emitstring)(char *, char *);
    void (*emitobject)(char *, json_object *);

    //Declaring functions for sending and receiving events along with ack
    void (*emitintack)(char *, int, void (*f)(char *, json_object *, json_object *));
    void (*emitstringack)(char *, char *, void (*f)(char *, json_object *, json_object *));
    void (*emitobjectack)(char *, json_object *, void (*f)(char *, json_object *, json_object *));

    //Declaring functions for subscribe
    void (*subscribe)(char *);
    void (*subscribe_ack)(char *, void (*f)(char *, json_object *, json_object *));

    //Declaring functions for unsubscribe

    void (*unsubscribe)(char *);
    void (*unsubscribe_ack)(char *, void (*f)(char *, json_object *, json_object *));

    //Declaring functions for publish
    void (*publishint)(char *, int);
    void (*publishstring)(char *, char *);
    void (*publishobject)(char *, json_object *);

    //Declaring functions for publish events along with ack
    void (*publishintack)(char *, int, void (*f)(char *, json_object *, json_object *));
    void (*publishstringack)(char *, char *, void (*f)(char *, json_object *, json_object *));
    void (*publishobjectack)(char *, json_object *, void (*f)(char *, json_object *, json_object *));

    //Declaring listeners for receiving
    void (*on)(char *, void (*f)(char *, json_object *));
    void (*onack)(char *, void (*f)(struct ackdata *, json_object *data, void (*f)(struct ackdata *, json_object *error, json_object *data)));
    void (*onpublish)(char *, void (*f)(char *, json_object *));
    /* data */
} *s;

struct socket * init_socket(char *protocol, char *address, int port, char *path, char *proxy_address, int proxy_port);
void delete_socket(struct socket * s);


// callback format used everywhere
typedef void (*callback)(struct ackdata *, json_object *, json_object *);

int socket_connect();
void socket_disconnect();

extern int destroy_flag;
extern int connection_flag;
extern int writeable_flag;

// Maps of each callback type
map_t acks;
map_t singlecallbacks;
map_t singleackcallbacks;
map_t publishcallbacks;

/* emitters */
void _emit_int(char *event, int data);
void _emit_string(char *event, char *data);
void _emit_object(char *event, json_object *data);

void _emit_int_ack(char *event, int data, void (*f)(char *event, json_object *error, json_object *data));
void _emit_string_ack(char *event, char *data, void (*f)(char *event, json_object *error, json_object *data));
void _emit_object_ack(char *event, json_object *data, void (*f)(char *event, json_object *error, json_object *data));

/* subscribers */
void _subscribe(char *);
void _subscribe_ack(char *, void (*f)(char *event, json_object *error, json_object *data));

void _unsubscribe(char *);
void _unsubscribe_ack(char *, void (*f)(char *event, json_object *error, json_object *data));

/* publishers */
void _publishint(char *, int);
void _publishstring(char *, char *);
void _publishobject(char *, json_object *);

void _publishint_ack(char *, int, void (*f)(char *event, json_object *error, json_object *data));
void _publishstring_ack(char *, char *, void (*f)(char *event, json_object *error, json_object *data));
void _publishobject_ack(char *, json_object *, void (*f)(char *event, json_object *error, json_object *data));

/* listeners */
void _on(char *event, void (*f)(char *event, json_object *data));
void _onack(char *event, void (*f)(struct ackdata *name, json_object *data, void (*f)(struct ackdata *name, json_object *error, json_object *data)));
void _onpublish(char *event, void (*f)(char *event, json_object *data));


void handle_emit(char *event, json_object *object);
void handle_emit_ack(struct ackdata *ack, json_object *object, void (*f)(struct ackdata *, json_object *error, json_object *data));
void handle_publish(char *event, json_object *object);

void _Ack(struct ackdata *ack, json_object *error, json_object *data);


// LWS Socket Settings

// Setting everything for connect method
struct lws_context *context;
struct lws_context_creation_info info;
struct lws *wsi;
struct lws_client_connect_info i;
struct lws_protocols protocol;
struct sigaction act;

const struct lws_extension exts[] = {
    {"permessage-deflate", lws_extension_callback_pm_deflate, "permessage-deflate; client_max_window_bits"},
    {"deflate-frame", lws_extension_callback_pm_deflate, "deflate_frame"},
    {NULL, NULL, NULL /* terminator */}};

#endif