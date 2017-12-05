#include "conn_server.h"

static struct event cb_event;
static void cb_handler(evutil_socket_t fd, short ev, void* arg) {
    auto p = (conn_server*)arg;
    if (p) {
        p->tick_proc();
    }
}

static void cb_signal_handler(evutil_socket_t sig, short events, void *arg) {
    auto p = (conn_server*)arg;
    if (!p) {
        ERROR("p = NULL");
        return;
    }

    switch(sig) {
    case SIGUSR1:
        p->signal_usr1();
        break;
    case SIGUSR2:
        p->signal_usr2();
        break;
    case SIGINT:
        p->signal_int();
        break;
    default:
        ERROR("no proccess function, sig[%d]", sig);
        break;
    }
}

conn_server::conn_server() : _client_listen(NULL), _server_listen(NULL) {

}

conn_server::~conn_server() {
    if (_client_listen) {
        delete _client_listen;
        _client_listen = NULL;
    }


    if (_server_listen) {
        delete _server_listen;
        _server_listen = NULL;
    }
}

int32_t conn_server::init(int32_t argc, char** argv) {
    if (g_vgame_log.Init(argv[1], "demo") != 0) {
        printf("init tlog failed, errno[%d]\n", errno);
        return -1;
    }

    if (-1 == server_node_base::init(argc, argv)) {
        ERROR("server_node_base::init()");
        return -1;
    }

    if (-1 == add_signal_handler(&cb_signal_handler, SIGUSR1, this)) {
        ERROR("add_signal_handler() SIGUSR1 failed");
        return -1;
    }

    if (-1 == add_signal_handler(&cb_signal_handler, SIGUSR2, this)) {
        ERROR("add_signal_handler() SIGUSR2 failed");
        return -1;
    }

    if (-1 == add_signal_handler(&cb_signal_handler, SIGINT, this)) {
        ERROR("add_signal_handler() SIGINT failed");
        return -1;
    }

    if (-1 == add_period_cb(&cb_event, 5000, &cb_handler, (void*)this)) {
        ERROR("add_period_cb() failed");
        return -1;
    }

    _client_listen = new client_conn_listen(_evbase);
    if (!_client_listen) {
        ERROR("_client_listen is NULL");
        return -1;
    }

    _client_listen->set_port(10001);
    _client_listen->listen();

    _server_listen = new server_conn_listen(_evbase);
    if (!_server_listen) {
        ERROR("_server_listen is NULL");
        return -1;
    }

    _server_listen->set_port(10002);
    _server_listen->listen();

    INFO("conn_server::init() succ");
    return 0;
}

int32_t conn_server::final() {
    server_node_base::final();
    INFO("final() succ");
    return 0;
}

void conn_server::signal_usr1() {
    INFO("signal_usr1() succ");
}

void conn_server::signal_usr2() {
    INFO("signal_usr2() succ");
}

void conn_server::signal_int() {
    INFO("signal_int() succ");
    stop();
}

client_conn_listen* conn_server::client_listen_ptr() {
    return _client_listen;
}

server_conn_listen* conn_server::server_listen_ptr() {
    return _server_listen;
}

void conn_server::tick_proc() {

}

void conn_server::reload() {
    INFO("reload resource ...");
}

int32_t main(int32_t argc, char** argv) {
    if (daemon(1, 0) < 0) {
        printf("daemon: errno[%d]\n", errno);
        return -1;
    }

    auto svr = new conn_server();
    int32_t ret = svr->init(argc, argv);
    if (-1 == ret) {
        return -1;
    }

    INFO("server start.");
    svr->loop();
    INFO("server stoped.");

    svr->final();

    if (svr) {
        delete svr;
        svr = NULL;
    }
    return 0;
}
