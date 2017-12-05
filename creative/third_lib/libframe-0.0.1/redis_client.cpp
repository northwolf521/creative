#include "redis_client.h"

CAutoRedisReply::CAutoRedisReply(redisReply* r) : _reply(r) {

}

CAutoRedisReply::~CAutoRedisReply() {
    if (_reply) {
        freeReplyObject(_reply);
        _reply = NULL;
    }
}

void CAutoRedisReply::set(redisReply* r) {
    if (_reply) {
        freeReplyObject(_reply);
        _reply = NULL;
    }

    _reply = r;
}

redisReply* CAutoRedisReply::get() {
    return _reply;
}

CRedisClient::CRedisClient() : m_isConnect(false), m_redisCtx(NULL) {

}

CRedisClient::~CRedisClient() {
    clear();
}

void CRedisClient::clear() {
    if (m_redisCtx) {
        redisFree(m_redisCtx);
        m_redisCtx = NULL;
    }
}

int CRedisClient::connect(const char* host, int port, int timeout, const char* passwd) {
    if (!host || port <= 0)
        return -1;

    m_szHost = std::string(host);
    m_nPort = port;
    m_timeout = timeout;
    m_szPasswd = passwd ? passwd : "";
    return connect();
}

int CRedisClient::check_connect() {
    if (m_isConnect) {
        redisReply* r = (redisReply*)redisCommand(m_redisCtx, "ping");
        if (!r) {
            DBG("ping redis error, will reconnect");
            m_isConnect = false;
        }
        else {
            if (r->type == REDIS_REPLY_ERROR) {
                ERROR("ping redis failed, will reconnect, err: %s", (r && r->str ? r->str : "unknown"));
                m_isConnect = false;
            }

            freeReplyObject(r);
        }
    }

    return m_isConnect ? 0 : connect();
}

int CRedisClient::count(const char* table) {
    if (!table)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed\n");
        return -1;
    }

    std::ostringstream oss;
    oss << "HLEN " << table;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_INTEGER) {
        return -3;
    }

    return r->integer;
}

int CRedisClient::exist(const char* table, const char* key) {
    if (!table || !key)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "HEXISTS  " << table << " " << key;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_INTEGER) {
        return -3;
    }

    return r->integer;
}

int CRedisClient::set(const char* table, const char* key, const char* val) {
    if (!table || !key || !val)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed\n");
        return -1;
    }

    std::ostringstream oss;
    oss << "HSET " << table << " " << key << " " << val;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_INTEGER) {
        return -3;
    }

    return 0;
}

int CRedisClient::inc(const char* table, const char* key, uint32_t val) {
    if (!table || !key)
        return -1;

    if (0 == val)
        return 0;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "HINCRBY " << table << " " << key << " " << val;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_INTEGER) {
        return -3;
    }

    return 0;
}

int CRedisClient::rename(const char* table, const char* newtable) {
    if (!table || !newtable)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "RENAME " << table << " " << newtable;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_STATUS || !r->str || 0 != strcasecmp(r->str, "OK")) {
        return -3;
    }

    return 0;
}

int CRedisClient::get(const char* table, const std::vector<uint64_t>& keys, std::vector<std::string>& out) {
    if (!table)
        return -1;

    if (keys.empty()) {
        out.clear();
        return 0;
    }

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "HMGET " << table;
    for (size_t i = 0; i < keys.size(); ++i) {
        oss << " " << keys[i];
    }

    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_ARRAY) {
        return -3;
    }

    out.resize(keys.size());

    for (size_t i = 0; i < out.size() && i < r->elements; ++i) {
        redisReply* childReply = r->element[i];
        std::string val;
        if (childReply->type == REDIS_REPLY_STRING && childReply->len > 0) {
            val.assign(childReply->str, childReply->len);
        }

        out[i] = val;
    }

    return 0;
}

int CRedisClient::get(const char* table, const std::vector<std::string>& keys, std::vector<std::string>& out) {
    if (!table)
        return -1;

    if (keys.empty()) {
        out.clear();
        return 0;
    }

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "HMGET " << table;
    for (size_t i = 0; i < keys.size(); ++i) {
        oss << " " << keys[i];
    }

    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_ARRAY) {
        return -3;
    }

    out.resize(keys.size());
    for (size_t i = 0; i < out.size() && i < r->elements; ++i) {
        std::string val;
        redisReply* childReply = r->element[i];
        if (childReply->type == REDIS_REPLY_STRING && childReply->len > 0) {
            val.assign(childReply->str, childReply->len);
        }

        out[i] = val;
    }

    return 0;
}

std::string CRedisClient::get(const char* table, const char* key) {
    if (!table || !key)
        return "";

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return "";
    }

    std::ostringstream os;
    os << "HGET " << table << " " << key;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, os.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return "";
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_STRING || r->len <= 0) {
        return "";
    }

    return std::string(r->str);
}

int CRedisClient::get(const char* table, const char* key, std::string& out) {
    if (!table || !key)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream os;
    os << "HGET " << table << " " << key;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, os.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type == REDIS_REPLY_STRING && r->str && r->len > 0) {
        out = r->str;
    }

    return 0;
}

int CRedisClient::del(const char* table, const std::vector<uint64_t>& keys) {
    if (!table)
        return -1;

    if (keys.empty())
        return 0;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "HDEL " << table;
    for (size_t i = 0; i < keys.size(); ++i) {
        oss << " " << keys[i];
    }

    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type == REDIS_REPLY_INTEGER) {
        return r->integer == 1 ? 0 : -1;
    }

    return -1;
}

int CRedisClient::del(const char* table, const std::vector<std::string>& keys) {
    if (!table)
        return -1;

    if (keys.empty())
        return 0;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "HDEL " << table;
    for (size_t i = 0; i < keys.size(); ++i) {
        oss << " " << keys[i];
    }

    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type == REDIS_REPLY_INTEGER) {
        return r->integer == 1 ? 0 : -1;
    }

    return -1;
}

int CRedisClient::del(const char* table, const char* key) {
    if (!table || !key)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "HDEL " << table << " " << key;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type == REDIS_REPLY_INTEGER) {
        return r->integer == 1 ? 0 : -1;
    }

    return -1;
}

int CRedisClient::zset(const char* key, uint64_t member, uint64_t score) {
    if (!key)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "ZADD " << key << " " << score << " " << member;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_INTEGER) {
        return -3;
    }

    return 0;
}

int CRedisClient::zgetHL64(const char* key, int startNo, int endNo, std::vector<std::pair<uint64_t, uint64_t> >& out) {
    return zget64(true, key, startNo, endNo, out);
}

int CRedisClient::zgetHL(const char* key, int startNo, int endNo, std::vector<std::pair<uint64_t, uint32_t> >& out) {
    return zget32(true, key, startNo, endNo, out);
}

int CRedisClient::zgetLH64(const char* key, int startNo, int endNo, std::vector<std::pair<uint64_t, uint64_t> >& out) {
    return zget64(false, key, startNo, endNo, out);
}

int CRedisClient::zgetLH(const char* key, int startNo, int endNo, std::vector<std::pair<uint64_t, uint32_t> >& out) {
    return zget32(false, key, startNo, endNo, out);
}

int CRedisClient::zget_rankHL(const char* key, uint64_t member, uint32_t& out) {
    return zget_rank(true, key, member, out);
}

int CRedisClient::zget_rankLH(const char* key, uint64_t member, uint32_t& out) {
    return zget_rank(false, key, member, out);
}

int CRedisClient::zcount(const char* key, int min, int max, uint32_t& out) {
    if (!key) {
        ERROR("null key");
        return -1;
    }

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "ZCOUNT " << key << " " << min << " " << max;
    redisReply* reply = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!reply) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(reply);
    if (reply->type != REDIS_REPLY_INTEGER) {
        return -3;
    }

    out = (uint32_t)(reply->integer);
    return 0;
}

int CRedisClient::zcard(const char* key, uint32_t& out) {
    if (!key)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "ZCARD " << key;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_INTEGER) {
        return -3;
    }

    out = (uint32_t)(r->integer);
    return 0;
}

int CRedisClient::zget_score64(const char* key, uint64_t member, uint64_t& out) {
    if (!key)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed\n");
        return -1;
    }

    std::ostringstream oss;
    oss << "ZSCORE " << key << " " << member;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_STRING) {
        return -3;
    }

    out = r->len > 0 ? strtoull(r->str, NULL, 0) : 0;
    return 0;
}

int CRedisClient::zget_score(const char* key, uint64_t member, uint32_t& out) {
    uint64_t tmp = 0;
    int ret = zget_score64(key, member, tmp);
    if (ret == 0) {
        out = (uint32_t)tmp;
    }

    return ret;
}

int CRedisClient::zdel(const char* key, const std::vector<uint64_t>& dellist) {
    if (!key)
        return -1;

    if (dellist.empty())
        return 0;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "ZREM " << key;
    for (size_t i = 0; i < dellist.size(); ++i) {
        oss << " " << dellist[i];
    }

    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    return 0;
}

int CRedisClient::zinc(const char* table, const char* key, uint32_t val, uint64_t* newval) {
    if (!table || !key)
        return -1;

    if (val == 0)
        return 0;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "ZINCRBY " << table << " " << val << " " << key;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_STRING) {
        return -3;
    }

    if (newval) {
        *newval = r->len > 0 ? strtoull(r->str, NULL, 0) : 0;
    }

    return 0;
}

int CRedisClient::del_table(const char* key) {
    if (!key)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "DEL " << key;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    return 0;
}

int CRedisClient::list_insert(const char* key, const char* value) {
    if (!key || !value)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "LPUSH " << key << " " << value;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type == REDIS_REPLY_INTEGER) {
        return 0;
    }

    return -1;
}

int CRedisClient::list_set(const char* key, uint32_t idx,  const char* value) {
    if (!key || !value)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "LSET " << key << " " << idx << " " << value;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type == REDIS_REPLY_INTEGER) {
        return 0;
    }

    return -1;
}

int CRedisClient::list_get(const char* key, int startNo, int endNo, std::vector<std::string>& out) {
    if (!key)
        return -1;

    if (startNo > endNo) {
        out.clear();
        return 0;
    }

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "LRANGE " << key << " " << startNo << " " << endNo;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_ARRAY) {
        return -3;
    }

    out.clear();

    for (size_t i = 0; i < r->elements; ++i) {
        std::string tmp;
        redisReply* childReply = r->element[i];
        if (childReply->type == REDIS_REPLY_STRING && childReply->len > 0) {
            tmp.assign(childReply->str, childReply->len);
        }

        out.push_back(tmp);
    }

    return 0;
}

int CRedisClient::list_header_pop(const char* key) {
    if (!key)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "RPOP " << key;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    return 0;
}

int CRedisClient::list_size(const char* key) {
    if (!key)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "LLEN " << key;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_INTEGER) {
        return -3;
    }

    return r->integer;
}

int CRedisClient::lock(const char* key, const char* val) {
    if (!key || !val)
        return 0;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "SETNX " << key << " " << val;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return 0;
    }

    CAutoRedisReply autoR(r);
    if (r->type == REDIS_REPLY_INTEGER) {
        return r->integer;
    }

    return 0;
}

int CRedisClient::unlock(const char* key) {
    if (!key)
        return 0;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "DEL " << key;

    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return 0;
    }

    CAutoRedisReply autoR(r);
    if (r->type == REDIS_REPLY_INTEGER) {
        return r->integer;
    }

    return 0;
}

int CRedisClient::get_all_val(const char* table, std::vector<std::pair<std::string, std::string> >& out) {
    if (!table)
        return 0;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "HGETALL " << table;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -1;
    }

    CAutoRedisReply autoR(r);
    if (r->type == REDIS_REPLY_ARRAY) {
        for (size_t i = 0; i < r->elements / 2; ++i) {
            redisReply* key = r->element[i * 2];
            redisReply* val = r->element[i * 2 + 1];
            if (key->type == REDIS_REPLY_STRING && key->len > 0 && val->type == REDIS_REPLY_STRING && val->len > 0) {
                out.push_back(std::make_pair(std::string(key->str, key->len), std::string(val->str, val->len)));
            }
        }
    }

    return 0;
}

int CRedisClient::expire_key(const char* key, int32_t seconds) {
    if (!key)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << "EXPIREAT " << key << " " << seconds;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_INTEGER) {
        return -3;
    }

    return r->integer;
}

int CRedisClient::connect() {
    if (m_isConnect)
        return 0;

    clear();

    struct timeval tv;
    tv.tv_sec = m_timeout / 1000;
    tv.tv_usec = m_timeout % 1000;
    m_redisCtx = redisConnectWithTimeout(m_szHost.c_str(), m_nPort, tv);
    if (NULL == m_redisCtx || m_redisCtx->err) {
        ERROR("connect redis server failed, err: %d, errmsg: %s", m_redisCtx->err, m_redisCtx->errstr);
        clear();
        return -2;
    }

    if (!m_szPasswd.empty()) {
        std::ostringstream oss;
        oss << "AUTH " << m_szPasswd;
        redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
        if (!r) {
            ERROR("call redis auth failed");
            clear();
            return -3;
        }

        CAutoRedisReply autoR(r);
        if (r->type == REDIS_REPLY_ERROR) {
            ERROR("redis auth failed");
            clear();
            return -10;
        }
    }

    m_isConnect = true;
    return 0;
}

int CRedisClient::zget64(bool rev, const char* key, int startNo, int endNo, std::vector<std::pair<uint64_t, uint64_t> >& out) {
    if (!key)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << (rev ? "ZREVRANGE " : "ZRANGE ") << key << " " << startNo << " " << endNo << " WITHSCORES";
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type != REDIS_REPLY_ARRAY) {
        return -3;
    }

    out.clear();

    for (size_t i = 0; i < r->elements / 2; ++i) {
        uint64_t playerid = 0;
        uint64_t score = 0;
        redisReply* childReply = r->element[i * 2];
        if (childReply->type == REDIS_REPLY_STRING && childReply->len > 0)
            playerid = strtoull(childReply->str, NULL, 0);

        childReply = r->element[i * 2 + 1];
        if (childReply->type == REDIS_REPLY_STRING && childReply->len > 0)
            score = strtoull(childReply->str, NULL, 0);

        out.push_back(std::make_pair(playerid, score));
    }

    return 0;
}

int CRedisClient::zget32(bool rev, const char* key, int startNo, int endNo, std::vector<std::pair<uint64_t, uint32_t> >& out) {
    std::vector<std::pair<uint64_t, uint64_t> > tmp;
    int ret = zget64(rev, key, startNo, endNo, tmp);
    if (ret == 0) {
        for (size_t i = 0; i < tmp.size(); ++i) {
            out.push_back(std::make_pair(tmp[i].first, (uint32_t)tmp[i].second));
        }
    }

    return ret;
}

int CRedisClient::zget_rank(bool rev, const char* key, uint64_t member, uint32_t& out) {
    if (!key)
        return -1;

    int ret = connect();
    if (0 != ret) {
        ERROR("connect redis failed");
        return -1;
    }

    std::ostringstream oss;
    oss << (rev ? "ZREVRANK " : "ZRANK ") << key << " " << member;
    redisReply* r = (redisReply*)redisCommand(m_redisCtx, oss.str().c_str());
    if (!r) {
        ERROR("call redisCommand error, redis break connection");
        m_isConnect = false;
        return -2;
    }

    CAutoRedisReply autoR(r);
    if (r->type == REDIS_REPLY_NIL) {
        out = 0;
        return 0;
    }

    if (r->type != REDIS_REPLY_INTEGER) {
        return -3;
    }

    out = (uint32_t)(r->integer);
    return 0;
}
