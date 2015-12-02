/*
 * virtual_addr.h
 * Author: Aven Bross
 * Date: 11/14/2015
 * Windows/OSX/Linux compatible virtual memory management
*/

#if defined(_WIN32) && ! defined(__CYGWIN__)

#include <windows.h>

void* AllocateAddressSpace(size_t size)
{
    return VirtualAlloc(NULL, size, MEM_RESERVE , PAGE_NOACCESS);
}
 
void* CommitMemory(void* addr, size_t size)
{
    return VirtualAlloc(addr, size, MEM_COMMIT, PAGE_READWRITE);
}
 
void DecommitMemory(void* addr, size_t size)
{
    VirtualFree((void*)addr, size, MEM_DECOMMIT);
}
 
void FreeAddressSpace(void* addr, size_t size)
{
    VirtualFree((void*)addr, 0, MEM_RELEASE)
}

#else

#include <sys/mman.h>

void* AllocateAddressSpace(size_t size)
{
    void * ptr = mmap((void*)0, size, PROT_NONE, MAP_PRIVATE|MAP_ANON, -1, 0);
    msync(ptr, size, MS_SYNC|MS_INVALIDATE);
    return ptr;
}
 
void* CommitMemory(void* addr, size_t size)
{
    void * ptr = mmap(addr, size, PROT_READ|PROT_WRITE, MAP_FIXED|MAP_SHARED|MAP_ANON, -1, 0);
    msync(addr, size, MS_SYNC|MS_INVALIDATE);
    return ptr;
}
 
void DecommitMemory(void* addr, size_t size)
{
    // instead of unmapping the address, we're just gonna trick 
    // the TLB to mark this as a new mapped area which, due to 
    // demand paging, will not be committed until used.
 
    mmap(addr, size, PROT_NONE, MAP_FIXED|MAP_PRIVATE|MAP_ANON, -1, 0);
    msync(addr, size, MS_SYNC|MS_INVALIDATE);
}
 
void FreeAddressSpace(void* addr, size_t size)
{
    msync(addr, size, MS_SYNC);
    munmap(addr, size);
}

#endif

