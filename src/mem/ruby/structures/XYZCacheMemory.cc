#include "xyz/XYZCacheMemory.hh"

namespace gem5 {
namespace ruby {

XYZCacheMemory::XYZCacheMemory(const XYZCacheParams &p): CacheMemory(p) {}

void XYZCacheMemory::init() {
    CacheMemory::init();
    LLC_directory.resize(m_cache_num_sets);
    NI_directory.resize(m_cache_num_sets);
}

XYZCacheMemory::~XYZCacheMemory() {}

// functions

AbstractCacheEntry* XYZCacheMemory::allocate(Addr address, AbstractCacheEntry *entry) {
    AbstractCacheEntry* new_entry = nullptr;
    if (existVacancyPerSet(address)) {
        new_entry = CacheMemory::allocate(address, entry);
    } else if (existLLCOnlyCleanLinePerSet(address)) {
        // 1. locate LLC only clean line in the set
        Addr victim_address = getLRULLCOnlyCleanLinePerSet(address);
        // 2. deallocate it 
        deallocate(victim_address);
        // 3. allocate new line
        new_entry = CacheMemory::allocate(address, entry);
        assert(lookup(address) != nullptr);
    } else {
        assert(false);
    }
    return new_entry;
}

void XYZCacheMemory::deallocate(Addr address) {
    assert(containLLCLine(address));
    CacheMemory::deallocate(address);
    removeLineFromLLCDirectory(address);
}

}; // namespace ruby
}; // namespace gem5