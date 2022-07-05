## Background

Shared memory (or specifically interprocess shared memory) refers to memory that is simultaneously mapped into the virtual address space of multiple processes. This means that the writes to the shared memory region carried out by one process are visible to other processes.

Shared memory is especially useful for data that is intended to be used by multiple processes at once, avoiding the redundant copies needed by other modes of interprocess communication, such as pipes and sockets. Shared memory is generally very fast, because the shared memory region may be read from and written to much like normal virtual memory.

Reads and writes to the respective virtual addresses mapped to process 1 and process 2 will modify the underlying shared memory region in physical memory.
There are two main concerns for processes that use shared memory:

1. The virtual address of the shared memory region in process 1, in general, will be different from that of process 2. As such, pointers to the shared memory region cannot be directly shared between processes, and the content of the shared memory region usually should not contain pointers.
2. Reads and writes occur on different threads, and as such, programs must put in place some synchronization mechanism when accessing and modifying the data in shared memory.

## API

```c
shmheap_memory_handle shmheap_create(const char *name, size_t len);
```

This function creates a new shared heap with the given name and size, and maps the shared heap into process memory. `name` should be passed as-is to `shm_open`. It is guaranteed that there is no existing shared memory of the same name, and that `len` is an integer multiple of the system page size. This function will be called just once per shared heap, before any other functions that operate on the same heap. It returns a handle to the shared heap.

```c
shmheap_memory_handle shmheap_connect(const char *name);
```

This function opens an existing shared heap with the given name, and maps the shared heap into process memory. `name` must be a valid shared heap that was previously created with `shmheap_create`. This function will be called once for each process that wants to connect to the shared heap.

```c
void shmheap_disconnect(shmheap_memory_handle mem);
```

This function is the logical opposite of `shmheap_connect`. It unmaps the shared heap. The mem handle must refer to a valid and connected shared memory.

```c
void shmheap_destroy(const char *name, shmheap_memory_handle mem);
```

This function is the logical opposite of `shmheap_create`. It unmaps the shared heap, and unlinks (i.e. deletes) the shared memory with the given name. It is guaranteed that the handle refers to a valid and connected shared heap, that no other process is connected to the heap. After this call, the shared memory should not remain in the system.

```c
void *shmheap_alloc(shmheap_memory_handle mem, size_t sz);
```

This function allocates an object of the given size on the given shared heapshmheap_create, similar to `malloc` but in shared memory. It returns a pointer to the object that was allocated. It is guaranteed that `sz` is no larger than `(len – 80)`, where `len` is the argument of `shmheap_create`. `mem` must be a valid and connected shared heap. Allocation follows a **first-fit** algorithm.

```c
shmheap_object_handle shmheap_ptr_to_handle(shmheap_memory_handle mem, void *ptr);
```

This function converts the pointer `ptr` (that points to an object in the shared heap given by `mem`) into a handle that may be transferred to other processes.

```c
void *shmheap_handle_to_ptr(shmheap_memory_handle mem, shmheap_object_handle hdl);
```

This function converts the handle `hdl` (that was received from another process) into a pointer to an object in the shared heap given by `mem`.
