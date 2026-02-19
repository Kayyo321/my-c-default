# my-c-default
My default project structure for all of my C projects

## Arena allocator API

`src/allocator.c` implements a virtual-disk backed arena system.

- Call `init_vdisk(bytes)` once to create the backing store.
- Create partitions with `partition_vdisk(block_size)`, which returns an `allocator_t*`.
- Allocate with `allocate(allocator, bytes)`.
- Reallocate with `reallocate(allocator, old, old_size, new_size)`.
	- Growth in place is supported only when `old` is the most recent allocation in that partition.
	- Otherwise, a new block is allocated in the same partition and data is copied.
- No per-allocation free is supported (arena behavior).
- Call `unmount_partition(allocator)` to remove one partition and compact later partitions left in vdisk.
- Call `free_vdisk()` to release all storage.
