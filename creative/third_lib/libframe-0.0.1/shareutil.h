#ifndef __SHARE_UTIL_H__
#define __SHARE_UTIL_H__

#include <stdint.h>
#include <string>
#include <vector>
#include <sstream>

#ifndef container_of
#define container_of(ptr, type, member) ({					\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})
#endif

#ifndef likely
#define likely(x)	__builtin_expect((x),1)
#define unlikely(x)	__builtin_expect((x),0)
#endif

#ifdef FloatMin
#undef FloatMin
#endif
#define FloatMin(a,b) ((a-b > 0.000001) ? (b) : (a))

#ifdef FloatMax
#undef FloatMax
#endif
#define FloatMax(a,b) ((a-b > 0.000001) ? (a) : (b))

#define ArrayCount(a) (sizeof(a) / (sizeof(a[0])))

namespace shareutil {
inline uint64_t get_rand_val() {
    uint32_t _edx = 0;
    uint32_t _eax = 0;
#if defined(__x86_64__)
    __asm__ __volatile__(
        ".byte 0x0f, 0x31"
        :"=a"(_eax),"=d"(_edx)
    );
#elif defined(__i386__)
    __asm__ __volatile__(
        ".byte 0x0f, 0x31"
        :"=a"(_eax),"=d"(_edx)
    );
#endif
    return (((uint64_t)_edx)<< 32 ) | _eax;
}

template<typename T>
std::string number2string(T val) {
    std::ostringstream oss;
    oss << val;
    return oss.str();
}

int32_t  STOI(const char* s);
uint32_t STOU(const char* s);
int64_t  STOI64(const char* s);
uint64_t STOU64(const char* s);
float    STOF(const char* s);
double   STOD(const char* s);

std::string formatString(const char* format, ...);
void splitString(const std::string& str, std::vector<std::string>& out, const std::string& sSep = ",");
void parseItemIdList(const char* sItem, std::vector<uint32_t>& outIdList);
uint32_t calc_crc32(uint8_t* p, uint32_t len);
std::string toHexString(const char* data, uint32_t size);
int base64_encode(const unsigned char *value, int vlen, char *result);
int base64_decode(const char *value, int vlen, char *result);
}

#endif//shareutil_h_
