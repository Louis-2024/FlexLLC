#ifndef __MEM_RUBY_STRUCTURES_XYZCACHEMEMORY_HH__
#define __MEM_RUBY_STRUCTURES_XYZCACHEMEMORY_HH__

#include "mem/ruby/structures/CacheMemory.hh"
#include "params/XYZCache.hh"

namespace gem5 {
namespace ruby {

class XYZCacheMemory : public CacheMemory {

public:
    typedef XYZCacheParams XYZParams;
    XYZCacheMemory(const XYZParams& p);
    ~XYZCacheMemory();

    virtual void init();
};

}  // namespace ruby
}  // namespace gem5

#endif  // __MEM_RUBY_STRUCTURES_XYZCACHEMEMORY_HH__
