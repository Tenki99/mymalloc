#ifndef _MY_MALLOC_H
#define _MY_MALLOC_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>

typedef struct _block{
    size_t size;
    int isFree; // 1 if free, 0 if occupied
    struct _block * prev;
    struct _block * next;
} block;

typedef struct _blocklist{
    block * head;
    block * tail;
} blocklist;

extern blocklist mlist;

void pushback_block(void * cur, size_t size);
void split_block(block * cur, size_t size);
void merge_block(block * cur);

void * ff_find_block(size_t size);
void * bf_find_block(size_t size);
void * my_malloc(size_t size, int method);
void * ff_malloc(size_t size);
void * bf_malloc(size_t size);

void my_free(void * ptr);
void ff_free(void * ptr);
void bf_free(void * ptr);

unsigned long get_largest_free_data_segment_size();//in bytes
unsigned long get_total_free_size();//in bytes

#endif