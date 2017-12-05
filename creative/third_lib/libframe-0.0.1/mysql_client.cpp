#include "mysql_client.h"

static uint32_t find_field_pos(MYSQL_RES* res, const char* name) {
    if (!res || !name)
        return -1;

    uint32_t num_fields = ::mysql_num_fields(res);
    MYSQL_FIELD* fields = ::mysql_fetch_fields(res);
    for(uint32_t i = 0; i < num_fields; i++) {
        if (strcasecmp(name, fields[i].name) == 0)
            return i;
    }

    return -2;
}

CMysqlClient::CMysqlClient() : m_mysql(NULL), m_res(NULL), m_errno(0) {

}

CMysqlClient::~CMysqlClient() {
    close();
}

int32_t CMysqlClient::init(const char* host, int32_t port, const char* dbname, const char* user, const char* pass) {
    if (m_mysql != NULL)
        return -1;

    if (!host || !dbname)
        return -1;

    m_mysql = ::mysql_init(NULL);
    if (!m_mysql)
        return -1;

    char value = 1;
    ::mysql_options(m_mysql, MYSQL_OPT_RECONNECT, (char*)&value);
    if (m_mysql != ::mysql_real_connect(m_mysql, host, user, pass, dbname, port, NULL, 0)) {
        ::mysql_close(m_mysql);

        m_mysql = NULL;
        return -1;
    }

    if (::mysql_set_character_set(m_mysql, "utf8") != 0) {
        m_errno = ::mysql_errno(m_mysql);
        m_errmsg = ::mysql_error(m_mysql);

        ERROR("set character utf8 failed, errno:%d, errmsg:%s", m_errno, m_errmsg.c_str());

        ::mysql_close(m_mysql);
        m_mysql = NULL;
        return -1;
    }

    return (0);
}

int32_t CMysqlClient::close() {
    reset_query();

    if (m_mysql) {
        ::mysql_close(m_mysql);
        m_mysql = NULL;
    }

    return 0;
}

int32_t CMysqlClient::query(const char* sql) {
    reset_query();

    if (!m_mysql || !sql) {
        ERROR("invalid data: m_mysql[%p], sql[%p]", m_mysql, sql);
        return -1;
    }

    if (::mysql_real_query(m_mysql, sql, strlen(sql)) != 0) {
        m_errno = ::mysql_errno(m_mysql);
        m_errmsg = ::mysql_error(m_mysql);
        ERROR("mysql_real_query failed, errno:%d, errmsg:%s", m_errno, m_errmsg.c_str());
        return -1;
    }

    m_res = ::mysql_store_result(m_mysql);
    return 0;
}

int32_t CMysqlClient::exec(const char* sql) {
    reset_query();

    if (!m_mysql || !sql)
        return -1;

    if (::mysql_real_query(m_mysql, sql, strlen(sql)) != 0) {
        m_errno = ::mysql_errno(m_mysql);
        m_errmsg = ::mysql_error(m_mysql);
        ERROR("mysql_real_query failed, errno:%d, errmsg:%s", m_errno, m_errmsg.c_str());
        return -1;
    }

    return 0;
}

void CMysqlClient::reset_query() {
    m_errno = 0;
    m_errmsg.clear();

    if (m_res) {
        ::mysql_free_result(m_res);
        while (::mysql_next_result(m_mysql) == 0);
        m_res = NULL;
    }
}

MYSQL_ROW CMysqlClient::fetch_row() {
    return m_res ? ::mysql_fetch_row(m_res) : NULL;
}

uint64_t* CMysqlClient::fetch_lengths() {
    return m_res ? ::mysql_fetch_lengths(m_res) : NULL;
}

uint64_t CMysqlClient::escape_string(char* to, const char* from, uint64_t length) {
    return ::mysql_real_escape_string(m_mysql, to, from, length);
}

const char* CMysqlClient::fetch_field(MYSQL_ROW row, MYSQL_RES* res, const char* name, int32_t* len) {
    if (!res || !name || !len)
        return NULL;

    int32_t pos = find_field_pos(res, name);
    if (pos < 0)
        return NULL;

    uint64_t* lengths = ::mysql_fetch_lengths(res);
    if (!lengths)
        return NULL;

    *len = lengths[pos];
    return row[pos];
}

int32_t CMysqlClient::get_errno() {
    return m_errno;
}

const char* CMysqlClient::get_errmsg() {
    return m_errmsg.c_str();
}
