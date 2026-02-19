#include "allocator.h"

#include <stdlib.h>
#include <string.h>

enum {
	MaxAllocators = 64,
};

void *vdisk = 0;
usize vdisk_size = 0;
usize allocator_count = 0;

struct {
	allocator_t allocator;
	usize used;
	boolean not_empty;
} allocator_refs[MaxAllocators] = {0};

static usize active_partitions_bytes(void) {
	usize total = 0;

	for (usize i = 0; i < allocator_count; i++) {
		total += allocator_refs[i].allocator.block_size;
	}

	return total;
}

static usize allocator_index(const allocator_t *allocator) {
	for (usize i = 0; i < allocator_count; i++) {
		if (&allocator_refs[i].allocator == allocator && allocator_refs[i].not_empty == True) {
			return i;
		}
	}

	return (usize)-1; // fck someones day up
}

void init_vdisk(usize bytes) {
	if (vdisk != null()) {
		free_vdisk();
	}

	vdisk_size = bytes;
	vdisk = malloc(vdisk_size);

	if (vdisk == null()) {
		vdisk_size = 0;
	}
}

void free_vdisk(void) {
	free(vdisk);

	vdisk = null();
	vdisk_size = 0;
	allocator_count = 0;
	memset(allocator_refs, 0, sizeof(allocator_refs));
}

allocator_t *partition_vdisk(usize block_size) {
	if (vdisk == null() || block_size == 0) {
		return null();
	}

	if (allocator_count >= MaxAllocators) {
		return null();
	}

	usize next_offset = active_partitions_bytes();
	if (next_offset + block_size > vdisk_size) {
		return null();
	}

	allocator_t *allocator = &allocator_refs[allocator_count].allocator;
	allocator_refs[allocator_count].used = 0;
	allocator_refs[allocator_count].not_empty = True;

	allocator->block = ((char *)vdisk) + next_offset;
	allocator->block_size = block_size;
	allocator_count++;

	return allocator;
}

void *allocate(allocator_t *allocator, usize bytes) {
	if (allocator == null() || bytes == 0) {
		return null();
	}

	usize index = allocator_index(allocator);
	if (index == (usize)-1) {
		return null();
	}

	usize used = allocator_refs[index].used;
	if (used + bytes > allocator_refs[index].allocator.block_size) {
		return null();
	}

	void *ptr = ((char *)allocator_refs[index].allocator.block) + used;
	allocator_refs[index].used += bytes;

	return ptr;
}

void *reallocate(allocator_t *allocator, void *old, usize old_size, usize new_size) {
	if (allocator == null()) {
		return null();
	}

	if (old == null()) {
		return allocate(allocator, new_size);
	}

	if (new_size == 0) {
		return null();
	}

	usize index = allocator_index(allocator);
	if (index == (usize)-1) {
		return null();
	}

	char *block_start = (char *)allocator_refs[index].allocator.block;
	usize block_size = allocator_refs[index].allocator.block_size;
	usize used = allocator_refs[index].used;
	char *old_ptr = (char *)old;

	if (old_ptr < block_start || old_ptr >= block_start + block_size) {
		return null();
	}

	usize offset = (usize)(old_ptr - block_start);
	if (offset + old_size > used) {
		return null();
	}

	if (new_size <= old_size) {
		return old;
	}

	if (offset + old_size == used) {
		usize growth = new_size - old_size;
		if (used + growth <= block_size) {
			allocator_refs[index].used += growth;
			return old;
		}
	}

	void *new_ptr = allocate(allocator, new_size);
	if (new_ptr == null()) {
		return null();
	}

	memcpy(new_ptr, old, old_size);
	return new_ptr;
}

void unmount_partition(allocator_t *allocator) {
	if (allocator == null() || vdisk == null()) {
		return;
	}

	usize index = allocator_index(allocator);
	if (index == (usize)-1) {
		return;
	}

	char *base = (char *)vdisk;
	usize total_bytes = active_partitions_bytes();
	usize start_offset = (usize)(((char *)allocator_refs[index].allocator.block) - base);
	usize removed_size = allocator_refs[index].allocator.block_size;
	usize tail_start = start_offset + removed_size;
	usize tail_size = total_bytes - tail_start;

	if (tail_size > 0) {
		memmove(base + start_offset, base + tail_start, tail_size);
	}

	if (removed_size > 0) {
		memset(base + total_bytes - removed_size, 0, removed_size);
	}

	for (usize i = index + 1; i < allocator_count; i++) {
		allocator_refs[i].allocator.block = ((char *)allocator_refs[i].allocator.block) - removed_size;
	}

	for (usize i = index; i + 1 < allocator_count; i++) {
		allocator_refs[i] = allocator_refs[i + 1];
	}

	allocator_count--;
	memset(&allocator_refs[allocator_count], 0, sizeof(allocator_refs[allocator_count]));
}
