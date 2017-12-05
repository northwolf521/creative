#ifndef __MEM_POOL_H__
#define __MEM_POOL_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct comm_pool {
    uint64_t map1;
    uint64_t map2[64];
    uint64_t map3[64][64];
    int   size;
    int   num;
    char* data;
};

void* comm_pool_alloc(struct comm_pool *pool);
void* get_next_inuse_comm_pool_entry(struct comm_pool *pool, int *index);
int comm_pool_free(struct comm_pool *pool, void *data);
int init_comm_pool(int resume, int data_size, int data_num, unsigned long key, struct comm_pool *ret);
//int count_init_comm_pool_size(int data_size, int data_num);

struct mass_pool {
    uint64_t map1;
    uint64_t map2[64];
    uint64_t map3[64][64];
    uint64_t map4[64][64][64];
    int   size;
    int   num;
    char* data;
};

void* mass_pool_alloc(struct mass_pool *pool);
int mass_pool_free(struct mass_pool *pool, void *data);
int init_mass_pool(int data_size, int data_num, struct mass_pool *ret);
int count_init_mass_pool_size(int data_size, int data_num);

#endif
