#include "my_malloc.h"

#define BLOCK_SIZE sizeof(block)
blocklist mlist;

void pushback_block(void * cur, size_t size){

    block * ptr = (void *) cur - BLOCK_SIZE;
    if (!mlist.tail){
        mlist.head = ptr;
        mlist.tail = ptr;
    } else {
        mlist.tail->next = ptr;
        ptr->prev = mlist.tail;
        mlist.tail = ptr;
    }

    ptr->size = size;
    ptr->isFree = 0;

}

void split_block(block * cur, size_t size){

    cur->isFree = 0;
    if (size + BLOCK_SIZE >= cur->size) {return;}

    block * new_block = (void *)(cur + 1) + size;

    block * next = cur->next;
    cur->next = new_block;
    new_block->next = next;
    new_block->prev = cur;

    if (next) {next->prev = new_block;}
    else {mlist.tail = new_block;} 

    new_block->size = cur->size - size - BLOCK_SIZE;
    new_block->isFree = 1;
    cur->size = size;

}

void merge_block(block * cur){
    if (cur->next == mlist.tail) {mlist.tail = cur;}
    cur->size += (cur->next->size + BLOCK_SIZE);
    cur->next = cur->next->next;
    if (cur->next) {cur->next->prev = cur;}
    
}

void * ff_find_block(size_t size){
    block * ptr = mlist.head;
    if (!ptr) {return NULL;}
    while (ptr){
        if (ptr->isFree && ptr->size >= size){
            split_block(ptr, size);
            return ptr + 1;
        }
        ptr = ptr->next;
    }
    return NULL;
}


void * bf_find_block(size_t size){

    block * ptr = mlist.head;
    if (!ptr) {return NULL;}

    block * ret_ptr = NULL;
    size_t min_size = INT_MAX;
    while (ptr){
        if (ptr->isFree && ptr->size >= size && ptr->size < min_size){
            ret_ptr = ptr;
            min_size = ptr->size;
        }
        ptr = ptr->next;
    }

    if (ret_ptr) {
        split_block(ret_ptr, size);
        return ret_ptr + 1;
    }
    return NULL;
}

void * my_malloc(size_t size, int method){

    void * ret_addr;

    // find currently free block
    if (method){
        ret_addr = ff_find_block(size);
    } else {
        ret_addr = bf_find_block(size);
    }
    
    // if NULL, use sbrk to allocate new block
    // what if sbrk fails?
    if (!ret_addr){
        ret_addr = sbrk(size + BLOCK_SIZE) + BLOCK_SIZE;
        pushback_block(ret_addr, size);
    }
    
    return ret_addr;

}

void * ff_malloc(size_t size){
    return my_malloc(size, 1);
}

void * bf_malloc(size_t size){
    return my_malloc(size, 0);
}

void my_free(void * ptr){
    block * ptr_meta = ptr - BLOCK_SIZE;
    block * cur = mlist.head;
    while (cur && cur != ptr_meta) {cur = cur->next;}
    if (cur) {
        cur->isFree = 1;
        block * prev = cur->prev, * next = cur->next;
        if (next && next->isFree) {merge_block(cur);}
        if (prev && prev->isFree) {merge_block(prev);}
    }
}

void ff_free(void * ptr){
    my_free(ptr);
}

void bf_free(void * ptr){
    my_free(ptr);
}

unsigned long get_largest_free_data_segment_size(){
    block * ptr = mlist.head;

    unsigned long max_size = 0;
    while (ptr){
        if (ptr->isFree && ptr->size > max_size){
            max_size = ptr->size;
        }
        ptr = ptr->next;
    }
    return max_size;
}
unsigned long get_total_free_size(){
    block * ptr = mlist.head;

    unsigned long free_size = 0;
    while (ptr){
        if (ptr->isFree){
            free_size += ptr->size;
        }
        ptr = ptr->next;
    }
    return free_size;
}