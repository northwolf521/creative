#include "game_server.h"
#include "abstract_thread.h"

static struct event cb_event;
static void cb_handler(evutil_socket_t fd, short ev, void* arg) {
    auto p = (game_server*)arg;
    if (p) {
        p->tick_proc();
    }
}

static void cb_signal_handler(evutil_socket_t sig, short events, void *arg) {
    auto p = (game_server*)arg;
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

game_server::game_server() : _listen(NULL), _redis(NULL) {

}

game_server::~game_server() {
    if (_listen) {
        delete _listen;
        _listen = NULL;
    }

    if (_redis) {
        delete _redis;
        _redis = NULL;
    }
}

int32_t game_server::init(int32_t argc, char** argv) {
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

    _listen = new conn_listen(_evbase);
    if (!_listen) {
        ERROR("_listen is NULL");
        return -1;
    }

    _listen->set_port(10003);
    _listen->listen();

    _redis = new redis_proxy(_evbase);
    _redis->set_addr("127.0.0.1");
    _redis->set_port(6379);
    _redis->set_partid(1);
    _redis->connect();

    INFO("game_server::init() succ");
    return 0;
}

int32_t game_server::final() {
    server_node_base::final();
    INFO("final() succ");
    return 0;
}

void game_server::signal_usr1() {
    INFO("signal_usr1() succ");
}

void game_server::signal_usr2() {
    INFO("signal_usr2() succ");
}

void game_server::signal_int() {
    INFO("signal_int() succ");
    stop();
}

conn_listen* game_server::listen_ptr() {
    return _listen;
}

void game_server::tick_proc() {
    if (!redis()) {
        ERROR("_redis is NULL");
        return;
    }

    if (redis()->is_connected()) {
        redis()->test();
        return;
    }

    ERROR("Invalid network connection");
    return;
}

redis_proxy* game_server::redis() {
    return _redis;
}

void game_server::reload() {
    INFO("reload resource ...");
}

int32_t main(int32_t argc, char** argv) {
    if (daemon(1, 0) < 0) {
        printf("daemon: errno[%d]\n", errno);
        return -1;
    }

    game_server* svr = new game_server();
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