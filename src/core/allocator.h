#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "common.h"

typedef struct {
	void *block;
	usize block_size;
} allocator_t;

void init_vdisk(usize bytes);
void free_vdisk(void);

allocator_t *partition_vdisk(usize block_size);

void *reallocate(allocator_t *allocator, void *old, usize old_size, usize new_size);
void *allocate(allocator_t *allocator, usize bytes);

void unmount_partition(allocator_t *allocator);

#endif
