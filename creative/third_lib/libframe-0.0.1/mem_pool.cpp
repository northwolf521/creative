#include "log.h"
#include "mem_pool.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

static void* alloc_shared_mem(int resume, unsigned long key, int size) {
    DBG("resume[%d], key[%lx], size[%d]", resume, key, size);

    int32_t shmid;
    if (resume)
        shmid = shmget(key, size, 0666);
    else
        shmid = shmget(key, size, IPC_CREAT|IPC_EXCL|0666);

    if (shmid < 0) {
        ERROR("shmget[%d] fail[%d]", size, errno);
        return (NULL);
    }

    void* mem = shmat(shmid, NULL, 0);
    if (!mem) {
        ERROR("shmat fail[%d]", errno);
        return (NULL);
    }

    return (mem);
}

inline int bsf_asm(uint64_t w) {
    int x1, x2;
    __asm__ __volatile__(
        "bsf %0,%0\n"
        "jnz 1f\n"
        "bsf %1,%0\n"
        "jz 1f\n"
        "addl $32,%0\n"
        "1:"
        : "=&q" (x1), "=&q" (x2)
        :"1" ((int) (w >> 32)), "0" ((int) w)
    );

    return x1;
}

inline int bsf_folded(uint64_t bb) {
    static const int lsb_64_table[64] = {
        63, 30,  3, 32, 59, 14, 11, 33,
        60, 24, 50,  9, 55, 19, 21, 34,
        61, 29,  2, 53, 51, 23, 41, 18,
        56, 28,  1, 43, 46, 27,  0, 35,
        62, 31, 58,  4,  5, 49, 54,  6,
        15, 52, 12, 40,  7, 42, 45, 16,
        25, 57, 48, 13, 10, 39,  8, 44,
        20, 47, 38, 22, 17, 37, 36, 26
    };

    unsigned int folded;
    bb ^= bb - 1;
    folded = (int)bb ^ (bb >> 32);
    return lsb_64_table[folded * 0x78291ACF >> 26];
}

inline int bsr_double(uint64_t bb) {
    union {
        double d;
        struct {
            unsigned int mantissal : 32;
            unsigned int mantissah : 20;
            unsigned int exponent : 11;
            unsigned int sign : 1;
        };
    } ud;

    ud.d = (double)(bb & ~(bb >> 32));

    return ud.exponent - 1023;
}

struct pool_entry_head {
    uint8_t index1;  //在map1中的位置,0-63
    uint8_t index2;  //在map2中的位置,0-63
    uint8_t index3;  //在map3中的位置,0-63
    uint8_t inuse;
    char data[0];
};

struct pool_entry_head* get_pool_entry(struct comm_pool *pool, int index) {
    if (index >= pool->num)
        return NULL;

    return (struct pool_entry_head*)&pool->data[pool->size * index];
}

void* get_next_inuse_comm_pool_entry(struct comm_pool *pool, int *index) {
    if (!pool || !index)
        return NULL;

    struct pool_entry_head* head = NULL;
    for (; *index < pool->num; ++(*index)) {
        head = get_pool_entry(pool, *index);
        assert(head);
        if (head->inuse) {
            ++(*index);
            return head->data;
        }
    }

    return NULL;
}

void* comm_pool_alloc(struct comm_pool *pool) {
    struct pool_entry_head* entry = NULL;
    if (pool->map1 == 0)
        return NULL;

    uint8_t index1 = bsf_folded(pool->map1);
    assert(pool->map2[index1] != 0);

    uint8_t index2 = bsf_folded(pool->map2[index1]);
    assert(pool->map3[index1][index2] != 0);

    uint8_t index3 = bsf_folded(pool->map3[index1][index2]);
    entry = get_pool_entry(pool, index3 + index2 * 64 + index1 * 64 * 64);

    assert((pool->map1 & 1LL << index1) != 0);
    assert((pool->map2[index1] & 1LL << index2) != 0);
    assert((pool->map3[index1][index2] & 1LL << index3) != 0);

    pool->map3[index1][index2] &= ~(1LL << index3);
    if (pool->map3[index1][index2] == 0LL) {
        pool->map2[index1] &= ~(1LL << index2);
        if (pool->map2[index1] == 0LL) {
            pool->map1 &= ~(1LL << index1);
        }
    }

    assert(entry->inuse == 0);
    entry->inuse = 1;
    return entry->data;
}

int comm_pool_free(struct comm_pool *pool, void *data) {
    struct pool_entry_head *entry;
    assert(pool);
    assert(data);
    entry = (struct pool_entry_head *)((uint64_t)data - sizeof(struct pool_entry_head));

    assert((pool->map3[entry->index1][entry->index2] & 1LL << entry->index3) == 0);

    pool->map1 |= 1LL << entry->index1;
    pool->map2[entry->index1] |= 1LL << entry->index2;
    pool->map3[entry->index1][entry->index2] |= 1LL << entry->index3;

    assert(entry->inuse == 1);
    entry->inuse = 0;
    return (0);
}

int comm_pool_free__(struct comm_pool* pool, void* data) {
    struct pool_entry_head* entry = (struct pool_entry_head*)((uint64_t)data - sizeof(struct pool_entry_head));
    pool->map1 |= 1LL << entry->index1;
    pool->map2[entry->index1] |= 1LL << entry->index2;
    pool->map3[entry->index1][entry->index2] |= 1LL << entry->index3;

    entry->inuse = 0;
    return (0);
}

//int count_init_comm_pool_size(int data_size, int data_num)
//{
//  data_size += sizeof(struct pool_entry_head);
//  return (data_size * data_num);
//}

//************************************
// Method:    init_comm_pool
// FullName:  init_comm_pool
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: int resume        是否是恢复模式
// Parameter: int data_size     每个结构的大小
// Parameter: int data_num      结构个数
// Parameter: unsigned long key  key的至
// Parameter: struct comm_pool * ret    返回内存指针
//************************************
int init_comm_pool(int resume, int data_size, int data_num, unsigned long key, struct comm_pool *ret) {
    int i;
    uint8_t index1, index2, index3;
    struct pool_entry_head *entry;
    assert(ret);
    if (data_num > (64 * 64 * 64))
        return (-1);

    data_size += sizeof(struct pool_entry_head);

    memset(ret, 0, sizeof(struct comm_pool));
//  ret->data = (char *)malloc(data_size * data_num);
    ret->data = (char *)alloc_shared_mem(resume, key, data_size * data_num);
    if (!ret->data)
        return (-2);

    ret->size = data_size;
    ret->num = data_num;

//  if (resume)
//      return (0);

    for (i = 0; i < data_num; ++i) {
        index1 = i / 64 / 64 % 64;
        index2 = i / 64 % 64;
        index3 = i % 64;
        entry = get_pool_entry(ret, i);
        entry->index1 = index1;
        entry->index2 = index2;
        entry->index3 = index3;
//      ((struct pool_entry_head *)(&ret->data[i * data_size]))->index3 = index3;
//      ((struct pool_entry_head *)(&ret->data[i * data_size]))->index2 = index2;
//      ((struct pool_entry_head *)(&ret->data[i * data_size]))->index1 = index1;

//      ret->map1 |= 1LL << index1;
//      ret->map2[index1] |= 1LL << index2;
//      ret->map3[index1][index2] |= 1LL << index3;
        if (!resume || !entry->inuse)
            comm_pool_free__(ret, entry->data);
    }

    return (0);
}

struct mass_pool_entry_head {
    uint8_t index1;  //在map1中的位置,0-63
    uint8_t index2;  //在map2中的位置,0-63
    uint8_t index3;  //在map3中的位置,0-63
    uint8_t index4;  //在map3中的位置,0-63
//  uint8_t inuse;
    char data[0];
};

struct mass_pool_entry_head *get_mass_pool_entry(struct mass_pool *pool, int index) {
    if (index >= pool->num)
        return NULL;
    return (struct mass_pool_entry_head *)&pool->data[pool->size * index];
}

void* mass_pool_alloc(struct mass_pool*pool) {
    if (!pool || pool->map1 == 0)
        return NULL;

    uint8_t index1 = bsf_folded(pool->map1);

    assert(pool->map2[index1] != 0);
    uint8_t index2 = bsf_folded(pool->map2[index1]);

    assert(pool->map3[index1][index2] != 0);
    uint8_t index3 = bsf_folded(pool->map3[index1][index2]);

    assert(pool->map4[index1][index2][index3] != 0);
    uint8_t index4 = bsf_folded(pool->map4[index1][index2][index3]);

    struct mass_pool_entry_head* entry = get_mass_pool_entry(pool, index4 + index3 * 64 + index2 * 64 * 64 + index1 * 64 * 64 * 64);

    assert((pool->map1 & 1LL << index1) != 0);
    assert((pool->map2[index1] & 1LL << index2) != 0);
    assert((pool->map3[index1][index2] & 1LL << index3) != 0);
    assert((pool->map4[index1][index2][index3] & 1LL << index4) != 0);

    pool->map4[index1][index2][index3] &= ~(1LL << index4);
    if (pool->map4[index1][index2][index3] == 0LL) {
        pool->map3[index1][index2] &= ~(1LL << index3);

        if (pool->map3[index1][index2] == 0LL) {
            pool->map2[index1] &= ~(1LL << index2);

            if (pool->map2[index1] == 0LL) {
                pool->map1 &= ~(1LL << index1);
            }
        }
    }

    return entry->data;
}

int mass_pool_free(struct mass_pool *pool, void *data) {
    struct mass_pool_entry_head *entry;
    assert(pool);
    assert(data);
    entry = (struct mass_pool_entry_head *)((uint64_t)data - sizeof(struct mass_pool_entry_head));

    assert((pool->map4[entry->index1][entry->index2][entry->index3] & 1LL << entry->index4) == 0);

    pool->map1 |= 1LL << entry->index1;
    pool->map2[entry->index1] |= 1LL << entry->index2;
    pool->map3[entry->index1][entry->index2] |= 1LL << entry->index3;
    pool->map4[entry->index1][entry->index2][entry->index3] |= 1LL << entry->index4;
    return (0);
}

int mass_pool_free__(struct mass_pool *pool, void *data) {
    struct mass_pool_entry_head *entry;
    assert(pool);
    assert(data);
    entry = (struct mass_pool_entry_head *)((uint64_t)data - sizeof(struct mass_pool_entry_head));

    pool->map1 |= 1LL << entry->index1;
    pool->map2[entry->index1] |= 1LL << entry->index2;
    pool->map3[entry->index1][entry->index2] |= 1LL << entry->index3;
    pool->map4[entry->index1][entry->index2][entry->index3] |= 1LL << entry->index4;
    return (0);
}

int count_init_mass_pool_size(int data_size, int data_num) {
    data_size += sizeof(struct mass_pool_entry_head);
    return (data_size * data_num);
}

int init_mass_pool(int data_size, int data_num, struct mass_pool *ret) {
    int i;
    uint8_t index1, index2, index3, index4;
    assert(ret);
    if (data_num > (64 * 64 * 64 * 64))
        return (-1);

    data_size += sizeof(struct mass_pool_entry_head);

    memset(ret, 0, sizeof(struct mass_pool));
    ret->data = (char *)malloc(data_size * data_num);
    if (!ret->data)
        return (-2);

    ret->size = data_size;
    ret->num  = data_num;

    for (i = 0; i < data_num; ++i) {
        index1 = i / 64 / 64 / 64 % 64;
        index2 = i / 64 / 64 % 64;
        index3 = i / 64 % 64;
        index4 = i % 64;
        struct mass_pool_entry_head* entry = get_mass_pool_entry(ret, i);
        entry->index1 = index1;
        entry->index2 = index2;
        entry->index3 = index3;
        entry->index4 = index4;
        mass_pool_free__(ret, entry->data);
    }

    return (0);
}
