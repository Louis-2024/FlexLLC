#include "xyz/XYZCacheMemory.hh"

namespace gem5 {
namespace ruby {

XYZCacheMemory::XYZCacheMemory(const XYZCacheParams &p): CacheMemory(p) {
    //
}

void XYZCacheMemory::init() {
    CacheMemory::init();
}

XYZCacheMemory::~XYZCacheMemory() {}

}; // namespace ruby
}; // namespace gem5