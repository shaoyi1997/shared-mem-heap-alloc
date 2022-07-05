#include <stddef.h>
#include <pthread.h>

typedef struct
{
    size_t len;
    pthread_mutex_t mutex;
    char data[];
} shmheap_block;

typedef struct
{
    size_t sz; // upper 29 bits for len (include header), last 1 bit for used flag
    char data[];
} shmheap_node;

typedef shmheap_block *shmheap_memory_handle;
typedef size_t shmheap_object_handle;

shmheap_memory_handle shmheap_create(const char *name, size_t len);
shmheap_memory_handle shmheap_connect(const char *name);
void shmheap_disconnect(shmheap_memory_handle mem);
void *shmheap_underlying(shmheap_memory_handle mem);
void shmheap_destroy(const char *name, shmheap_memory_handle mem);
void *shmheap_alloc(shmheap_memory_handle mem, size_t sz);
void shmheap_free(shmheap_memory_handle mem, void *ptr);
shmheap_object_handle shmheap_ptr_to_handle(shmheap_memory_handle mem, void *ptr);
void *shmheap_handle_to_ptr(shmheap_memory_handle mem, shmheap_object_handle hdl);
