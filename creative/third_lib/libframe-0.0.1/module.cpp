#include "module.h"

typedef std::map<uint32_t, uint64_t> DispatchMap;
static DispatchMap* get_dispath_map() {
    static DispatchMap g_dispath;
    return &g_dispath;
}

static DispatchMap* get_maker_map() {
    static DispatchMap g_maker;
    return &g_maker;
}

static DispatchMap* get_destruct_map() {
    static DispatchMap g_destruct;
    return &g_destruct;
}

int register_module(module_enter* enter) {
    module_enter* et = enter;
    DispatchMap* dispatch = get_dispath_map();
    DispatchMap* maker    = get_maker_map();
    DispatchMap* destruct = get_destruct_map();

    while (et) {
        if (et->msg_id == 0)
            break;

        if (et->maker && et->func) {
            if (dispatch->find(et->msg_id) != dispatch->end()) {
                ERROR("net msg id dispatch already register, msg id:%u", et->msg_id);
                assert(false);
                return -1;
            }

            if (maker->find(et->msg_id) != maker->end()) {
                ERROR("net msg id maker already register, msg id:%u", et->msg_id);
                assert(false);
                return -1;
            }

            maker->insert(std::make_pair(et->msg_id, (uint64_t)et->maker));
            dispatch->insert(std::make_pair(et->msg_id, (uint64_t)et->func));
        }

        if (et->destruct) {
            if (destruct->find(et->msg_id) != destruct->end()) {
                ERROR("net msg id destruct already register, msg id:%u", et->msg_id);
                assert(false);
                return -1;
            }

            destruct->insert(std::make_pair(et->msg_id, (uint64_t)et->destruct));
        }

        ++et;
    }

    return 0;
}

int unregister_module() {
    DispatchMap* destruct = get_destruct_map();
    for (DispatchMap::iterator it = destruct->begin(); it != destruct->end(); ++it) {
        void* __call = (void*)it->second;
        if (!__call)
            continue;

        function_convert<void*, int(*)()> conv(__call);
        (conv())();
    }

    destruct->clear();
    return 0;
}

int get_register_dispach(uint32_t msg_id, void** pObj, void** func) {
    DispatchMap* dispatch = get_dispath_map();
    DispatchMap* maker = get_maker_map();

    DispatchMap::iterator it= dispatch->find(msg_id);
    if (it == dispatch->end())
        return -1;

    if (func)
        *func = (void*)it->second;

    it = maker->find(msg_id);
    if (it!= maker->end() && it->second) {
        function_convert<void*, int(*)(void**)> conv((void*)it->second);
        return conv()(pObj);
    }

    return 0;
}


