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
        AbstractCacheEntry* victim_entry = lookup(victim_address);
        uint32_t target_set = victim_entry->getSet();
        uint32_t target_way = victim_entry->getWay();
        assert(target_set == addressToCacheSet(address));
        // 2. deallocate it 
        assert(m_cache[target_set][target_way] != nullptr);
        deallocate(victim_entry->m_Address);
        assert(m_cache[target_set][target_way] == nullptr);
        // 3. allocate new line
        new_entry = CacheMemory::allocate(address, entry);
        assert(m_cache[target_set][target_way] != nullptr);
    } else {
        assert(false);
    }
    return new_entry;
}

void XYZCacheMemory::deallocate(Addr address) {
    CacheMemory::deallocate(address);
    removeLineFromLLCDirectory(address);
}

}; // namespace ruby
}; // namespace gem5