#ifndef __INI_FILE_H__
#define __INI_FILE_H__

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <map>

inline void trim(std::string &str) {
    int32_t s = str.find_first_not_of(" \t");
    int32_t e = str.find_last_not_of(" \t");
    str = str.substr(s, e - s + 1);
}

inline int32_t parse_addr_and_port(const std::string &host, std::string &addr, int32_t &port) {
    char chost[128] = {'\0'};
    int32_t len = sizeof(chost);
    strncpy(chost, host.c_str(), len - 1);
    chost[len - 1] = '\0';

    std::vector<std::string> v;
    char* p = strtok (chost, ":");
    while (p) {
        v.push_back(p);
        p = strtok(NULL, ":");
    }

    if (2 != v.size()) {
        ERROR("Parse error, [%s]", host.c_str());
        return -1;
    }

    addr = v[0];
    port = atoi(v[1].c_str());
    return 0;
}

class IniFile {
    typedef std::map<std::string, std::string> KeyValMap;
    typedef std::vector<std::string> ValList;

public:
    IniFile();
    ~IniFile();

public:
    int32_t load_file(const char* filename);
    int32_t load_file_list(const char* filename);
    int32_t get_int(const char* key, int32_t default_val = 0) const;
    uint32_t get_uint(const char* key, uint32_t default_val = 0) const;
    int64_t get_int64(const char* key, int64_t default_val = 0) const;
    uint64_t get_uint64(const char* key, uint64_t default_val = 0) const;
    std::string get_str(const char* key, const char* default_val = "") const;
    void get_list(std::vector<std::string> &list);
    int32_t dict_by_prefix(const char* prefix, std::map<int32_t, std::string>& dict);

private:
    std::ifstream _file;
    KeyValMap _map;
    ValList _list;
};

#endif
