#include "xyz/XYZCacheMemory.hh"

namespace gem5 {
namespace ruby {

XYZCacheMemory::XYZCacheMemory(const XYZCacheParams &p): CacheMemory(p) {
    //
}

void XYZCacheMemory::init() {
    CacheMemory::init();
    LLC_directory.resize(m_cache_num_sets);
    NI_directory.resize(m_cache_num_sets);
}

XYZCacheMemory::~XYZCacheMemory() {}

}; // namespace ruby
}; // namespace gem5