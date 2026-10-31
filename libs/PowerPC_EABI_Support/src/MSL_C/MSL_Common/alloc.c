#include "PowerPC_EABI_Support/MSL_C/MSL_Common/alloc.h"
#include <stddef.h>

void* memset(void*, int, size_t);
void* memcpy(void*, const void*, size_t);
void* __sys_alloc(unsigned long);
void __sys_free(void*);

typedef struct Block {
    struct Block* prev;
    struct Block* next;
    unsigned long max_size;
    unsigned long size;
} Block;

typedef struct SubBlock {
    unsigned long size;
    Block* block;
    struct SubBlock* prev;
    struct SubBlock* next;
} SubBlock;

struct FixSubBlock;

typedef struct FixBlock {
    struct FixBlock* prev_;
    struct FixBlock* next_;
    unsigned long client_size_;
    struct FixSubBlock* start_;
    unsigned long n_allocated_;
} FixBlock;

typedef struct FixSubBlock {
    FixBlock* block_;
    struct FixSubBlock* next_;
} FixSubBlock;

typedef struct FixStart {
    FixBlock* tail_;
    FixBlock* head_;
} FixStart;

typedef struct __mem_pool_obj {
    Block* start_;
    FixStart fix_start[6];
} __mem_pool_obj;

typedef struct __mem_pool {
    void* reserved[14];
} __mem_pool;

static SubBlock* SubBlock_merge_prev(SubBlock*, SubBlock**);
static void SubBlock_merge_next(SubBlock* , SubBlock** );

static const unsigned long fix_pool_sizes[] = {4, 12, 20, 36, 52, 68};

#define SubBlock_size(ths) ((ths)->size & 0xFFFFFFF8)
#define SubBlock_block(ths) ((Block*)((unsigned long)((ths)->block) & ~0x1))
#define Block_size(ths) ((ths)->size & 0xFFFFFFF8)
#define Block_start(ths) (*(SubBlock**)((char*)(ths) + Block_size((ths)) - sizeof(unsigned long)))

#define SubBlock_set_free(ths)                                     \
    unsigned long this_size = SubBlock_size((ths));                \
    (ths)->size &= ~0x2;                                           \
    *(unsigned long*)((char*)(ths) + this_size) &= ~0x4;           \
    *(unsigned long*)((char*)(ths) + this_size - sizeof(unsigned long)) = this_size


#define SubBlock_is_free(ths) !((ths)->size & 2)
#define SubBlock_set_size(ths, sz)                                 \
    (ths)->size &= ~0xFFFFFFF8;                                    \
    (ths)->size |= (sz) & 0xFFFFFFF8;                              \
    if (SubBlock_is_free((ths)))                                   \
        *(unsigned long*)((char*)(ths) + (sz) - sizeof(unsigned long)) = (sz)

#define SubBlock_from_pointer(ptr) ((SubBlock*)((char*)(ptr) - 8))
#define FixSubBlock_from_pointer(ptr) ((FixSubBlock*)((char*)(ptr) - 4))

#define FixBlock_client_size(ths) ((ths)->client_size_)
#define FixSubBlock_size(ths) (FixBlock_client_size((ths)->block_))

#define classify(ptr) (*(unsigned long*)((char*)(ptr) - sizeof(unsigned long)) & 1)
#define __msize_inline(ptr) (!classify(ptr) ? FixSubBlock_size(FixSubBlock_from_pointer(ptr)) : SubBlock_size(SubBlock_from_pointer(ptr)) - 8)

#define Block_empty(ths)                                                      \
    (_sb = (SubBlock*)((char*)(ths) + 16)),                    \
    SubBlock_is_free(_sb) && SubBlock_size(_sb) == Block_size((ths)) - 24

#define SubBlock_is_prev_allocated(ths) ((ths)->size & 0x04)

#define SubBlock_set_not_free(ths)                                 \
    unsigned long this_size = SubBlock_size((ths));                \
    (ths)->size |= 0x02;                                           \
    *(unsigned long*)((char*)(ths) + this_size) |= 0x04

static void SubBlock_construct(SubBlock* ths, unsigned long size, Block* bp, int prev_alloc, int this_alloc) {
    ths->block = (Block*)((unsigned long)bp | 0x1);
    ths->size = size;
    if (prev_alloc) {
        ths->size |= 0x04;
    }
    if (this_alloc) {
        ths->size |= 0x02;
        *(unsigned long*)((char*)ths + size) |= 0x04;
    } else {
        *(unsigned long*)((char*)ths + size - sizeof(unsigned long)) = size;
    }
}

static void SubBlock_split(SubBlock* ths, unsigned long sz) {
    unsigned long origsize = SubBlock_size(ths);
    int isfree = SubBlock_is_free(ths);
    int isprevalloc = SubBlock_is_prev_allocated(ths);
    SubBlock* np = (SubBlock*)((char*)ths + sz);
    Block* bp = SubBlock_block(ths);

    SubBlock_construct(ths, sz, bp, isprevalloc, !isfree);
    SubBlock_construct(np, origsize - sz, bp, !isfree, !isfree);

    if (isfree) {
        np->next = ths->next;
        np->next->prev = np;
        np->prev = ths;
        ths->next = np;
    }
}

static SubBlock* Block_subBlock(Block* ths, unsigned long requested_size, unsigned long* size_received) {
    SubBlock* st;
    SubBlock* sb;
    unsigned long sb_size;
    unsigned long max_found;

    st = Block_start(ths);
    if (st == 0) {
        ths->max_size = 0;
        return 0;
    }

    sb = st;
    sb_size = SubBlock_size(sb);
    max_found = sb_size;

    while (sb_size < requested_size) {
        sb = sb->next;
        sb_size = SubBlock_size(sb);
        if (max_found < sb_size) {
            max_found = sb_size;
        }
        if (sb == st) {
            ths->max_size = max_found;
            if (size_received) {
                *size_received = max_found - 8;
            }
            return 0;
        }
    }

    if (sb_size - requested_size >= 80) {
        SubBlock_split(sb, requested_size);
    }

    Block_start(ths) = sb->next;
    {
        SubBlock_set_not_free(sb);
    }
    {
        SubBlock** start = &Block_start(ths);
        if (*start == sb) {
            *start = sb->next;
        }
        if (*start == sb) {
            *start = 0;
            ths->max_size = 0;
        } else {
            sb->next->prev = sb->prev;
            sb->prev->next = sb->next;
        }
    }

    if (size_received) {
        *size_received = SubBlock_size(sb) - 8;
    }
    return sb;
}

void Block_link(Block* ths, SubBlock* sb) {
    SubBlock** st;
    SubBlock_set_free(sb);
    st = &Block_start(ths);

    if (*st != 0)
    {
        sb->prev = (*st)->prev;
        sb->prev->next = sb;
        sb->next = *st;
        (*st)->prev = sb;
        *st = sb;
        *st = SubBlock_merge_prev(*st, st);
        SubBlock_merge_next(*st, st);
    }
    else
    {
        *st = sb;
        sb->prev = sb;
        sb->next = sb;
    }
    if (ths->max_size < SubBlock_size(*st))
        ths->max_size = SubBlock_size(*st);
}

static SubBlock* SubBlock_merge_prev(SubBlock *ths, SubBlock **start) {
    unsigned long prevsz;
    SubBlock* p;

    if (!(ths->size & 0x04))
    {
        prevsz = *(unsigned long*)((char*)ths - sizeof(unsigned long));
        if (prevsz & 0x2)
            return ths;
        p = (SubBlock*)((char*)ths - prevsz);
        SubBlock_set_size(p, prevsz + SubBlock_size(ths));

        if (*start == ths)
            *start = (*start)->next;
        ths->next->prev = ths->prev;
        ths->next->prev->next = ths->next;
        return p;
    }
    return ths;
}


static void SubBlock_merge_next(SubBlock *pBlock, SubBlock **pStart) {
    SubBlock* next_sub_block;
    unsigned long this_cur_size;

    next_sub_block = (SubBlock*)((char*)pBlock + (pBlock->size & 0xFFFFFFF8));

    if (!(next_sub_block->size & 2)) {
        this_cur_size = (pBlock->size & 0xFFFFFFF8) + (next_sub_block->size & 0xFFFFFFF8);

        pBlock->size &= ~0xFFFFFFF8;
        pBlock->size |= this_cur_size & 0xFFFFFFF8;

        if (!(pBlock->size & 2)) {
            *(unsigned long*)((char*)(pBlock) + (this_cur_size) - 4) = (this_cur_size);
        }

        if (!(pBlock->size & 2)) {
            *(unsigned long *)((char*)pBlock + this_cur_size) &= ~4;
        }
        else {
            *(unsigned long *)((char*)pBlock + this_cur_size) |= 4;
        }

        if (*pStart == next_sub_block) {
            *pStart = (*pStart)->next;
        }

        if (*pStart == next_sub_block) {
            *pStart = 0;
        }

        next_sub_block->next->prev = next_sub_block->prev;
        next_sub_block->prev->next = next_sub_block->next;
    }
}

static void Block_construct(Block* ths, unsigned long size) {
    SubBlock* sb;

    ths->size = size | 0x3;
    *(unsigned long*)((char*)ths + size - 8) = ths->size;
    sb = (SubBlock*)((char*)ths + 16);
    SubBlock_construct(sb, size - 24, ths, 0, 0);
    ths->max_size = size - 24;
    Block_start(ths) = 0;
    Block_link(ths, sb);
}

static Block* __link(__mem_pool_obj* pool_obj, Block* bp) {
    if (pool_obj->start_ != 0) {
        bp->prev = pool_obj->start_->prev;
        bp->prev->next = bp;
        bp->next = pool_obj->start_;
        pool_obj->start_->prev = bp;
        pool_obj->start_ = bp;
    } else {
        pool_obj->start_ = bp;
        bp->prev = bp;
        bp->next = bp;
    }
    return bp;
}

static Block* __unlink(__mem_pool_obj* pool_obj, Block* bp) {
    Block* result = bp->next;
    if (result == bp) {
        result = 0;
    }

    if (pool_obj->start_ == bp) {
        pool_obj->start_ = result;
    }

    if (result != 0) {
        result->prev = bp->prev;
        result->prev->next = result;
    }

    bp->next = 0;
    bp->prev = 0;
    return result;
}

static Block* link_new_block(__mem_pool_obj* pool_obj, unsigned long size) {
    Block* bp;

    size = (size + 24 + 7) & ~7;
    if (size < 0x10000) {
        size = 0x10000;
    }

    bp = (Block*)__sys_alloc(size);
    if (bp == 0) {
        return 0;
    }

    Block_construct(bp, size);
    return __link(pool_obj, bp);
}

static void* allocate_from_var_pools(__mem_pool_obj* pool_obj, unsigned long size, unsigned long* size_received) {
    Block* bp;
    SubBlock* ptr;

    if (size_received) {
        *size_received = 0;
    }

    size = (size + 8 + 7) & ~7;
    if (size < 80) {
        size = 80;
    }

    bp = pool_obj->start_ != 0 ? pool_obj->start_ : link_new_block(pool_obj, size);
    if (bp == 0) {
        return 0;
    }

    do {
        if (size <= bp->max_size) {
            ptr = Block_subBlock(bp, size, size_received);
            if (ptr != 0) {
                pool_obj->start_ = bp;
                goto done;
            }
        }
        bp = bp->next;
    } while (bp != pool_obj->start_);

    bp = link_new_block(pool_obj, size);
    if (bp == 0) {
        return 0;
    }
    ptr = Block_subBlock(bp, size, size_received);

done:
    return (char*)ptr + 8;
}

static void* soft_allocate_from_var_pools(__mem_pool_obj* pool_obj, unsigned long size, unsigned long* max_size) {
    Block* bp;
    SubBlock* ptr;

    size = (size + 8 + 7) & ~7;
    if (size < 80) {
        size = 80;
    }

    *max_size = 0;
    bp = pool_obj->start_;
    if (bp == 0) {
        return 0;
    }

    do {
        if (size <= bp->max_size) {
            ptr = Block_subBlock(bp, size, 0);
            if (ptr != 0) {
                pool_obj->start_ = bp;
                goto done;
            }
        }
        if (bp->max_size > 8 && *max_size < bp->max_size - 8) {
            *max_size = bp->max_size - 8;
        }
        bp = bp->next;
    } while (bp != pool_obj->start_);

    return 0;

done:
    return (char*)ptr + 8;
}

static void deallocate_from_var_pools(__mem_pool_obj* pool_obj, void *ptr) {
    SubBlock* sb = SubBlock_from_pointer(ptr);
    SubBlock* _sb;

    Block* bp = SubBlock_block(sb);
    Block_link(bp, sb);

    if (Block_empty(bp)) {
        __unlink(pool_obj, bp);
        __sys_free(bp);
    }
}

static void FixBlock_construct(FixBlock* ths, FixBlock* prev, FixBlock* next, int index, FixSubBlock* chunk, unsigned long chunk_size) {
    unsigned long fixSubBlock_size;
    unsigned long n;
    char* p;
    unsigned long i;

    ths->prev_ = prev;
    ths->next_ = next;
    prev->next_ = ths;
    next->prev_ = ths;
    ths->client_size_ = fix_pool_sizes[index];
    fixSubBlock_size = fix_pool_sizes[index] + 4;
    n = chunk_size / fixSubBlock_size;
    p = (char*)chunk;
    for (i = 0; i < n - 1; ++i) {
        char* next = p + fixSubBlock_size;
        ((FixSubBlock*)p)->block_ = ths;
        ((FixSubBlock*)p)->next_ = (FixSubBlock*)next;
        p = next;
    }
    ((FixSubBlock*)p)->block_ = ths;
    ((FixSubBlock*)p)->next_ = 0;
    ths->start_ = chunk;
    ths->n_allocated_ = 0;
}

static void __init_pool_obj(__mem_pool* pool_obj) {
    memset(pool_obj, 0, sizeof(__mem_pool_obj));
}

static __mem_pool* get_malloc_pool(void) {
    static __mem_pool protopool;
    static unsigned char init = 0;
    if (!init) {
        __init_pool_obj(&protopool);
        init = 1;
    }

    return &protopool;
}

static void* allocate_from_fixed_pools(__mem_pool_obj* pool_obj, unsigned long size, unsigned long* size_received) {
    unsigned long i = 0;
    FixSubBlock* p;
    FixStart* fs;

    while (size > fix_pool_sizes[i]) {
        ++i;
    }

    fs = &pool_obj->fix_start[i];
    if (fs->head_ == 0 || fs->head_->start_ == 0) {
        unsigned long size_requested;
        unsigned long size_has;
        unsigned long n;
        unsigned long n_try;
        unsigned long received;
        unsigned long received2;
        char* newblock;

        n = 4076 / (fix_pool_sizes[i] + 4);
        if (n > 256) {
            n = 256;
        }

        n_try = n;
        while (n >= 10) {
            size_requested = n * (fix_pool_sizes[i] + 4) + sizeof(FixBlock);
            newblock = (char*)soft_allocate_from_var_pools(pool_obj, size_requested, &received);
            if (newblock != 0) {
                break;
            }
            if (received > sizeof(FixBlock)) {
                n = (received - sizeof(FixBlock)) / (fix_pool_sizes[i] + 4);
            } else {
                n = 0;
            }
        }

        if (newblock == 0) {
            do {
                size_requested = n_try * (fix_pool_sizes[i] + 4) + sizeof(FixBlock);
                newblock = (char*)allocate_from_var_pools(pool_obj, size_requested, &received2);
                if (newblock != 0) {
                    goto have_block;
                }
                n_try = received2 / (fix_pool_sizes[i] + 4 + sizeof(FixBlock));
            } while (n_try >= 10);

            if (size_received) {
                *size_received = 0;
            }
            return 0;
        }

have_block:
        size_has = __msize_inline(newblock);
        if (fs->head_ == 0) {
            fs->head_ = (FixBlock*)newblock;
            fs->tail_ = (FixBlock*)newblock;
        }
        FixBlock_construct((FixBlock*)newblock, fs->tail_, fs->head_, i, (FixSubBlock*)(newblock + sizeof(FixBlock)), size_has - sizeof(FixBlock));
        fs->head_ = (FixBlock*)newblock;
    }

    p = fs->head_->start_;
    fs->head_->start_ = p->next_;
    ++fs->head_->n_allocated_;

    if (fs->head_->start_ == 0) {
        fs->head_ = fs->head_->next_;
        fs->tail_ = fs->tail_->next_;
    }

    if (size_received) {
        *size_received = fix_pool_sizes[i];
    }
    return (char*)p + 4;
}

void deallocate_from_fixed_pools(__mem_pool_obj* pool_obj, void* ptr, unsigned long size) {
    unsigned long i = 0;
    FixSubBlock* p;
    FixBlock* b;
    FixStart* fs;

    while (size > fix_pool_sizes[i]) {
        ++i;
    }

    fs = &pool_obj->fix_start[i];
    p = FixSubBlock_from_pointer(ptr);
    b = p->block_;

    if (b->start_ == 0 && fs->head_ != b) {
        if (fs->tail_ == b) {
            fs->head_ = fs->head_->prev_;
            fs->tail_ = fs->tail_->prev_;
        }
        else {
            b->prev_->next_ = b->next_;
            b->next_->prev_ = b->prev_;
            b->next_ = fs->head_;
            b->prev_ = b->next_->prev_;
            b->prev_->next_ = b;
            b->next_->prev_ = b;
            fs->head_ = b;
        }
    }

    p->next_ = b->start_;
    b->start_ = p;

    if (--b->n_allocated_ == 0) {
        if (fs->head_ == b) {
            fs->head_ = b->next_;
        }

        if (fs->tail_ == b) {
            fs->tail_ = b->prev_;
        }

        b->prev_->next_ = b->next_;
        b->next_->prev_ = b->prev_;

        if (fs->head_ == b) {
            fs->head_ = 0;
        }

        if (fs->tail_ == b) {
            fs->tail_ = 0;
        }

        deallocate_from_var_pools(pool_obj, b);
    }
}

static int __pool_allocate_resize(__mem_pool* pool, void* ptr, unsigned long new_size, unsigned long* size_received) {
    __mem_pool_obj* pool_obj = (__mem_pool_obj*)pool;
    unsigned long current_size = __msize_inline(ptr);

    if (new_size == current_size) {
        if (size_received) {
            *size_received = current_size;
        }
        return 1;
    }

    if (classify(ptr)) {
        unsigned long new_block_size;
        SubBlock* sb;
        SubBlock* next;
        unsigned long block_size;

        if (new_size > (unsigned long)-0x81) {
            new_size = (unsigned long)-0x81;
        }
        new_block_size = (new_size + 8 + 7) & ~7;
        if (new_block_size < 80) {
            new_block_size = 80;
        }

        sb = SubBlock_from_pointer(ptr);
        block_size = current_size + 8;
        next = (SubBlock*)((char*)sb + block_size);
        if (SubBlock_is_free(next) && new_block_size <= block_size + SubBlock_size(next)) {
            Block* bp = SubBlock_block(sb);
            SubBlock_merge_next(sb, &Block_start(bp));
            block_size = SubBlock_size(sb);
        }

        if (block_size >= new_block_size + 80) {
            SubBlock_split(sb, new_block_size);
            Block_link(SubBlock_block(sb), (SubBlock*)((char*)sb + new_block_size));
            block_size = SubBlock_size(sb);
        }

        if (size_received) {
            *size_received = block_size - 8;
        }

        if (new_size > current_size) {
            if (size_received && new_size > block_size - 8 && SubBlock_is_free(next)) {
                *size_received += SubBlock_size(next);
            }
            return new_size <= block_size - 8;
        }
        return block_size - 8 < current_size;
    }

    if (size_received) {
        *size_received = current_size;
    }
    return 0;
}

static void* __pool_alloc(__mem_pool* pool, unsigned long size) {
    __mem_pool_obj* pool_obj = (__mem_pool_obj*)pool;

    if (size > (unsigned long)-0x31) {
        return 0;
    }
    if (size <= 68) {
        return allocate_from_fixed_pools(pool_obj, size, 0);
    }
    return allocate_from_var_pools(pool_obj, size, 0);
}

static void __pool_free(__mem_pool *pool, void *ptr) {
    __mem_pool_obj* pool_obj;
    unsigned long size;

    if (ptr == 0) {
        return;
    }

    pool_obj = (__mem_pool_obj*)pool;
    size = __msize_inline(ptr);

    if (size <= 68) {
        deallocate_from_fixed_pools(pool_obj, ptr, size);
    }
    else {
        deallocate_from_var_pools(pool_obj, ptr);
    }
}

static void* __pool_realloc(__mem_pool* pool, void* ptr, unsigned long size) {
    void* newptr;
    unsigned long cursize;

    if (ptr == 0) {
        return __pool_alloc(pool, size);
    }
    if (size == 0) {
        __pool_free(pool, ptr);
        return 0;
    }

    cursize = __msize_inline(ptr);
    if (__pool_allocate_resize(pool, ptr, size, 0) || size <= cursize) {
        return ptr;
    }

    newptr = __pool_alloc(pool, size);
    if (newptr != 0) {
        memcpy(newptr, ptr, cursize);
        __pool_free(pool, ptr);
    }
    return newptr;
}

static void* __pool_alloc_clear(__mem_pool* pool, unsigned long size) {
    void* result = __pool_alloc(pool, size);
    if (result != 0) {
        memset(result, 0, size);
    }
    return result;
}

void* malloc(size_t size) {
    if (size != 0) {
        return __pool_alloc(get_malloc_pool(), size);
    }
    return 0;
}

void free(void *ptr) {
    __pool_free(get_malloc_pool(), ptr);
}

void* realloc(void* ptr, size_t size) {
    return __pool_realloc(get_malloc_pool(), ptr, size);
}

void* calloc(size_t nmemb, size_t size) {
    return __pool_alloc_clear(get_malloc_pool(), size * nmemb);
}
