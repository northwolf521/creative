#include "log.h"
#include "inifile.h"

IniFile::IniFile() {

}

IniFile::~IniFile() {
    if (_file.is_open()) {
        _file.close();
    }
}

int32_t IniFile::load_file(const char* filename) {
    _file.open(filename);
    if (!_file.good()) {
        return -1;
    }

    int32_t ret = 0;
    char line[1024] = {'\0'};

    while (_file.getline(line, sizeof(line))) {
        int32_t line_len = strlen(line);
        if (line_len == 0)
            continue;

        char* k_start = line;
        char* k_end = k_start + line_len;
        while (((*k_start == ' ') || (*k_start == '\t')) && (k_start < k_end))
            ++k_start;

        if (*k_start == '#')
            continue;

        k_end = strchr(line, '=');
        if (k_end == NULL)
            continue;

        char* tmp = k_end;

        --k_end;
        while (((*k_end == ' ') || (*k_end == '\t')) && (k_end > k_start)) {
            k_end --;
        }

        ++k_end;
        if (k_end <= k_start)
            continue;

        std::string key;
        key.assign(k_start, k_end);

        k_start = tmp + 1;
        k_end = line + line_len;

        while ((*k_start == ' ' || *k_start == '\t') && k_start < k_end)
            ++k_start;

        std::string val;
        val.assign(k_start);
        _map[key] = val;
    }

    return ret;
}

int32_t IniFile::load_file_list(const char* filename) {
    _file.open(filename);
    if (!_file.good()) {
        return -1;
    }

    int32_t ret = 0;
    char line[1024] = {'\0'};

    while (_file.getline(line, sizeof(line))) {
        int32_t line_len = strlen(line);
        if (line_len == 0)
            continue;

        std::string val = line;
        trim(val);
        if (val.find("#") != std::string::npos)
            continue;

        _list.push_back(val);
    }

    return ret;
}

void IniFile::get_list(std::vector<std::string> &list) {
    for (ValList::const_iterator it = _list.begin(); it != _list.end(); ++it) {
        list.push_back(*it);
    }
}

int32_t IniFile::get_int(const char* key, int32_t default_val) const {
    int32_t result = default_val;
    if (key) {
        std::string szKey(key);
        KeyValMap::const_iterator it = _map.find(szKey);
        if (it != _map.end()) {
            result = strtol(it->second.c_str(), NULL, 0);
        }
    }

    return result;
}

uint32_t IniFile::get_uint(const char* key, uint32_t default_val) const {
    uint32_t result = default_val;
    if (key) {
        std::string szKey(key);
        KeyValMap::const_iterator it = _map.find(szKey);
        if (it != _map.end()) {
            result = strtoul(it->second.c_str(), NULL, 0);
        }
    }

    return result;
}

int64_t IniFile::get_int64(const char* key, int64_t default_val) const {
    int64_t result = default_val;
    if (key) {
        std::string szKey(key);
        KeyValMap::const_iterator it = _map.find(szKey);
        if (it != _map.end()) {
            result = strtoll(it->second.c_str(), NULL, 0);
        }
    }

    return result;
}

uint64_t IniFile::get_uint64(const char* key, uint64_t default_val) const {
    uint64_t result = default_val;
    if (key) {
        std::string szKey(key);
        KeyValMap::const_iterator it = _map.find(szKey);
        if (it != _map.end()) {
            result = strtoull(it->second.c_str(), NULL, 0);
        }
    }

    return result;
}

std::string IniFile::get_str(const char * key, const char* default_val) const {
    std::string result = default_val;
    if (key) {
        std::string szKey(key);
        KeyValMap::const_iterator it = _map.find(szKey);
        if (it != _map.end()) {
            result = it->second.c_str();
        }
    }

    return result;
}

int32_t IniFile::dict_by_prefix(const char* prefix, std::map<int32_t, std::string>& dict) {
    if (!prefix) {
        ERROR("invalid prefix[%p]", prefix);
        return -1;
    }

    dict.clear();
    for (KeyValMap::iterator iter = _map.begin(); iter != _map.end(); iter++) {
        std::string key = (*iter).first;
        std::string val = (*iter).second;
        int32_t pos = key.find(prefix);
        int32_t len = key.length();
        if (0 != pos || len == 0) {
            //仅匹配前缀字符串(如prefix*,*为数字)
            continue;
        }

        int32_t prefix_len = strlen(prefix);
        if (prefix_len == len) {
            //不完整匹配字符串规则
            continue;
        }

        int32_t id = atoi(key.substr(pos + prefix_len).c_str());
        dict[id] = val;

        DBG("id[%d], val[%s]", id, val.c_str());
    }

    return 0;
}

