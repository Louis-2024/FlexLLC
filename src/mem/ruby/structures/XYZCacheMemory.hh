#ifndef __MEM_RUBY_STRUCTURES_XYZCACHEMEMORY_HH__
#define __MEM_RUBY_STRUCTURES_XYZCACHEMEMORY_HH__

#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "mem/ruby/structures/CacheMemory.hh"
#include "params/XYZCache.hh"

namespace gem5 {
namespace ruby {

class XYZCacheMemory : public CacheMemory {
    struct MetadataPerLine {
        std::unordered_set<int> sharers;
        int owner = -1;
        bool dirty = false;
    };
    struct MetadataPerSet {
        std::unordered_map<Addr, MetadataPerLine> metadata_per_set;
    };
public:
    typedef XYZCacheParams XYZParams;
    XYZCacheMemory(const XYZParams& p);
    ~XYZCacheMemory();

    virtual void init();

    // metadata operations

    void addSharer(Addr address, int core_id, bool inclusive) {
        int64_t set_index = addressToCacheSet(address);
        bool is_present_in_LLC_directory = LLC_directory[set_index].metadata_per_set.find(address) != LLC_directory[set_index].metadata_per_set.end();
        bool is_present_in_NI_directory = NI_directory[set_index].metadata_per_set.find(address) != NI_directory[set_index].metadata_per_set.end();

        if (inclusive) {
            assert(!is_present_in_NI_directory);
            if (is_present_in_LLC_directory) {
                LLC_directory[set_index].metadata_per_set[address].sharers.insert(core_id);
            } else {
                LLC_directory[set_index].metadata_per_set[address] = { .sharers = {core_id} };
            }
        } else {
            assert(!is_present_in_LLC_directory);
            if (is_present_in_NI_directory) {
                NI_directory[set_index].metadata_per_set[address].sharers.insert(core_id);
            } else {
                NI_directory[set_index].metadata_per_set[address] = { .sharers = {core_id} };
            }
        }
    }

    void removeSharer(Addr address, int core_id, bool inclusive) {
        int64_t set_index = addressToCacheSet(address);
        bool is_present_in_LLC_directory = LLC_directory[set_index].metadata_per_set.find(address) != LLC_directory[set_index].metadata_per_set.end();
        bool is_present_in_NI_directory = NI_directory[set_index].metadata_per_set.find(address) != NI_directory[set_index].metadata_per_set.end();

        if (inclusive) {
            assert((!is_present_in_NI_directory) && is_present_in_LLC_directory);
            assert(LLC_directory[set_index].metadata_per_set[address].sharers.find(core_id) != LLC_directory[set_index].metadata_per_set[address].sharers.end());
            LLC_directory[set_index].metadata_per_set[address].sharers.erase(core_id);
        } else {
            assert((!is_present_in_LLC_directory) && is_present_in_NI_directory);
            assert(NI_directory[set_index].metadata_per_set[address].sharers.find(core_id) != NI_directory[set_index].metadata_per_set[address].sharers.end());
            NI_directory[set_index].metadata_per_set[address].sharers.erase(core_id);
        }
    }

    void clearSharers(Addr address, bool inclusive) {
        int64_t set_index = addressToCacheSet(address);
        bool is_present_in_LLC_directory = LLC_directory[set_index].metadata_per_set.find(address) != LLC_directory[set_index].metadata_per_set.end();
        bool is_present_in_NI_directory = NI_directory[set_index].metadata_per_set.find(address) != NI_directory[set_index].metadata_per_set.end();

        if (inclusive) {
            assert((!is_present_in_NI_directory) && is_present_in_LLC_directory);
            LLC_directory[set_index].metadata_per_set[address].sharers.clear();
        } else {
            assert((!is_present_in_LLC_directory) && is_present_in_NI_directory);
            NI_directory[set_index].metadata_per_set[address].sharers.clear();
        }
    }

    void setOwner(Addr address, int core_id, bool inclusive) {
        int64_t set_index = addressToCacheSet(address);
        bool is_present_in_LLC_directory = LLC_directory[set_index].metadata_per_set.find(address) != LLC_directory[set_index].metadata_per_set.end();
        bool is_present_in_NI_directory = NI_directory[set_index].metadata_per_set.find(address) != NI_directory[set_index].metadata_per_set.end();

        if (inclusive) {
            assert(!is_present_in_NI_directory);
            if (is_present_in_LLC_directory) {
                LLC_directory[set_index].metadata_per_set[address].owner = core_id;
            } else {
                LLC_directory[set_index].metadata_per_set[address] = { .owner = core_id };
            }
        } else {
            assert(!is_present_in_LLC_directory);
            if (is_present_in_NI_directory) {
                NI_directory[set_index].metadata_per_set[address].owner = core_id;
            } else {
                NI_directory[set_index].metadata_per_set[address] = { .owner = core_id };
            }
        }
    }

    void unsetOwner(Addr address, bool inclusive) {
        int64_t set_index = addressToCacheSet(address);
        bool is_present_in_LLC_directory = LLC_directory[set_index].metadata_per_set.find(address) != LLC_directory[set_index].metadata_per_set.end();
        bool is_present_in_NI_directory = NI_directory[set_index].metadata_per_set.find(address) != NI_directory[set_index].metadata_per_set.end();

        if (inclusive) {
            assert((!is_present_in_NI_directory) && is_present_in_LLC_directory);
            LLC_directory[set_index].metadata_per_set[address].owner = -1;
        } else {
            assert((!is_present_in_LLC_directory) && is_present_in_NI_directory);
            NI_directory[set_index].metadata_per_set[address].owner = -1;
        }
    }

    void convertOwnerToSharer(Addr address, bool inclusive) {
        int64_t set_index = addressToCacheSet(address);
        if (inclusive) {
            assert((!is_present_in_NI_directory) && is_present_in_LLC_directory);
            LLC_directory[set_index].metadata_per_set[address].sharers.insert(LLC_directory[set_index].metadata_per_set[address].owner);
            LLC_directory[set_index].metadata_per_set[address].owner = -1;
        } else {
            assert((!is_present_in_LLC_directory) && is_present_in_NI_directory);
            NI_directory[set_index].metadata_per_set[address].sharers.insert(NI_directory[set_index].metadata_per_set[address].owner);
            NI_directory[set_index].metadata_per_set[address].owner = -1;
        }
    }

    void setDirty(Addr address, bool inclusive) {
        int64_t set_index = addressToCacheSet(address);
        bool is_present_in_LLC_directory = LLC_directory[set_index].metadata_per_set.find(address) != LLC_directory[set_index].metadata_per_set.end();
        bool is_present_in_NI_directory = NI_directory[set_index].metadata_per_set.find(address) != NI_directory[set_index].metadata_per_set.end();

        if (inclusive) {
            assert((!is_present_in_NI_directory) && is_present_in_LLC_directory);
            LLC_directory[set_index].metadata_per_set[address].dirty = 1;
        } else {
            assert((!is_present_in_LLC_directory) && is_present_in_NI_directory);
            NI_directory[set_index].metadata_per_set[address].dirty = 1;
        }
    }

    void convertNILineToIN(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        assert((LLC_directory[set_index].metadata_per_set.find(address) == LLC_directory[set_index].metadata_per_set.end()) && (NI_directory[set_index].metadata_per_set.find(address) != NI_directory[set_index].metadata_per_set.end()));
        
        MetadataPerLine target_line = NI_directory[set_index].metadata_per_set[address];
        LLC_directory[set_index].metadata_per_set[address] = target_line;
        NI_directory[set_index].metadata_per_set.erase(address);
    }

    void convertINLineToNI(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        assert((NI_directory[set_index].metadata_per_set.find(address) == NI_directory[set_index].metadata_per_set.end()) && (LLC_directory[set_index].metadata_per_set.find(address) != LLC_directory[set_index].metadata_per_set.end()));
        
        MetadataPerLine target_line = LLC_directory[set_index].metadata_per_set[address];
        NI_directory[set_index].metadata_per_set[address] = target_line;
        LLC_directory[set_index].metadata_per_set.erase(address);
    }

    void removeLineFromLLCDirectory(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        assert((NI_directory[set_index].metadata_per_set.find(address) == NI_directory[set_index].metadata_per_set.end()) && (LLC_directory[set_index].metadata_per_set.find(address) != LLC_directory[set_index].metadata_per_set.end()));
        LLC_directory[set_index].metadata_per_set.erase(address);
    }

    void removeLineFromNIDirectory(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        assert((LLC_directory[set_index].metadata_per_set.find(address) == LLC_directory[set_index].metadata_per_set.end()) && (NI_directory[set_index].metadata_per_set.find(address) != NI_directory[set_index].metadata_per_set.end()));
        NI_directory[set_index].metadata_per_set.erase(address);
    }

    int getCacheLineCount(bool inclusive) {
        int count = 0;
        if (inclusive) {
            for (int i = 0; i < m_cache_num_sets; i++) {
                count += LLC_directory[i].metadata_per_set.size();
            }
        } else {
            for (int i = 0; i < m_cache_num_sets; i++) {
                count += NI_directory[i].metadata_per_set.size();
            }
        }
        return count;
    }

    // line searching operations

    std::unordered_set<Addr> getLinesPerSet(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        std::unordered_map<Addr, MetadataPerLine> target_set = LLC_directory[set_index].metadata_per_set;
        std::unordered_set<Addr> lines;

        for (auto line = target_set.begin(); line != target_set.end(); line++) {
            lines.insert(line->first);
        }
        return lines;
    }

    std::unordered_set<Addr> getLLCOnlyCleanLinesPerSet(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        std::unordered_map<Addr, MetadataPerLine> target_set = LLC_directory[set_index].metadata_per_set;
        std::unordered_set<Addr> llc_only_clean_lines;

        for (auto line = target_set.begin(); line != target_set.end(); line++) {
            if ((!line->second.dirty) && (line->second.owner == -1) && (line->second.sharers.size() == 0)) {
                llc_only_clean_lines.insert(line->first);
            }
        }
        return llc_only_clean_lines;
    }

    std::unordered_set<Addr> getLLCOnlyDirtyLinesPerSet(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        std::unordered_map<Addr, MetadataPerLine> target_set = LLC_directory[set_index].metadata_per_set;
        std::unordered_set<Addr> llc_only_dirty_lines;

        for (auto line = target_set.begin(); line != target_set.end(); line++) {
            if ((line->second.dirty) && (line->second.owner == -1) && (line->second.sharers.size() == 0)) {
                llc_only_dirty_lines.insert(line->first);
            }
        }
        return llc_only_dirty_lines;
    }

    Addr getLRULine(std::unordered_set<Addr> lines) {
        assert(lines.size() > 0);
        Tick LRU_time = curTick();
        Addr LRU_line = 0;
        
        for (const Addr& address : lines) {
            AbstractCacheEntry* entry = lookup(address);
            assert(entry != nullptr);
            Tick RU_time = entry->getLastAccess();
            if (RU_time < LRU_time) {
                LRU_time = RU_time;
                LRU_line = address;
            }
        }
        return LRU_line;
    }

    // victim selection operations

    bool existLLCOnlyCleanLinePerSet(Addr address) {
        return (getLLCOnlyCleanLinesPerSet(address).size() > 0);
    }

    Addr getLRULLCOnlyCleanLinePerSet(Addr address) {
        return getLRULine(getLLCOnlyCleanLinesPerSet(address));
    }

    bool existLLCOnlyDirtyLinePerSet(Addr address) {
        return (getLLCOnlyDirtyLinesPerSet(address).size() > 0);
    }

    Addr getLRULLCOnlyDirtyLinePerSet(Addr address) {
        return getLRULine(getLLCOnlyDirtyLinesPerSet(address));
    }


protected:
    std::vector<MetadataPerSet> LLC_directory;
    std::vector<MetadataPerSet> NI_directory;
};

}  // namespace ruby
}  // namespace gem5

#endif  // __MEM_RUBY_STRUCTURES_XYZCACHEMEMORY_HH__
