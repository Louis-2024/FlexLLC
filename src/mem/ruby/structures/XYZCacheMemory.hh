#ifndef __MEM_RUBY_STRUCTURES_XYZCACHEMEMORY_HH__
#define __MEM_RUBY_STRUCTURES_XYZCACHEMEMORY_HH__

#include <unordered_set>
#include <unordered_map>
#include <set>
#include <vector>

#include "debug/FlexLLC.hh"
#include "debug/FlexLLC_Stats.hh"
#include "mem/ruby/structures/CacheMemory.hh"
#include "mem/ruby/common/MachineID.hh"
#include "mem/ruby/common/NetDest.hh"
#include "params/XYZCache.hh"

namespace gem5 {
namespace ruby {

class XYZCacheMemory : public CacheMemory {
    struct MetadataPerLine {
        NetDest sharers;
        NetDest owner;
        bool isDirty = false;

        int NI_transient_state = 0;
    };
    struct MetadataPerSet {
        std::unordered_map<Addr, MetadataPerLine> metadata_per_set;
    };
public:
    typedef XYZCacheParams XYZParams;
    XYZCacheMemory(const XYZParams& p);
    ~XYZCacheMemory();

    virtual void init();
    virtual AbstractCacheEntry* allocate(Addr address, AbstractCacheEntry* new_entry);
    virtual void deallocate(Addr address);

    void printMetadata(Addr address) {
        if (containLLCLine(address) || containNILine(address)) {
            DPRINTF(FlexLLC, "{Address = %#x, #Sharers = %d, #Owner = %d, isDirty = %d} \n", address, getSharers(address).count(), getOwner(address).count(), getIsDirty(address));
        } else {
            DPRINTF(FlexLLC, "{Address = %#x} \n", address);
        }
    }

    // NetDest operations

    NetDest getOwner(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        NetDest dest;
        if (containLLCLine(address)) {
            dest = LLC_directory[set_index].metadata_per_set[address].owner;
        } else if (containNILine(address)) {
            dest = NI_directory[set_index].metadata_per_set[address].owner;
        }
        return dest;
    }

    NetDest getASharer(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        NetDest dest;
        if (containLLCLine(address)) {
            dest.add(LLC_directory[set_index].metadata_per_set[address].sharers.smallestElement());
        } else if (containNILine(address)) {
            dest.add(NI_directory[set_index].metadata_per_set[address].sharers.smallestElement());
        }
        return dest;
    }

    NetDest getSharers(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        NetDest dest;
        if (containLLCLine(address)) {
            dest = LLC_directory[set_index].metadata_per_set[address].sharers;
        } else if (containNILine(address)) {
            dest = NI_directory[set_index].metadata_per_set[address].sharers;
        }
        return dest;
    }

    void addSharer(Addr address, MachineID core, bool inclusive) {
        int64_t set_index = addressToCacheSet(address);
        bool is_present_in_LLC_directory = containLLCLine(address);
        bool is_present_in_NI_directory = containNILine(address);

        if (inclusive) {
            assert(!is_present_in_NI_directory);
            if (is_present_in_LLC_directory) {
                LLC_directory[set_index].metadata_per_set[address].sharers.add(core);
            } else {
                NetDest dest;
                dest.add(core);
                LLC_directory[set_index].metadata_per_set[address] = { .sharers = dest };
            }
        } else {
            assert(!is_present_in_LLC_directory);
            if (is_present_in_NI_directory) {
                NI_directory[set_index].metadata_per_set[address].sharers.add(core);
            } else {
                NetDest dest;
                dest.add(core);
                NI_directory[set_index].metadata_per_set[address] = { .sharers = dest };
            }
        }
    }

    void removeSharer(Addr address, MachineID core) {
        int64_t set_index = addressToCacheSet(address);
        if (containLLCLine(address)) {
            LLC_directory[set_index].metadata_per_set[address].sharers.remove(core);
        } else if (containNILine(address)) {
            NI_directory[set_index].metadata_per_set[address].sharers.remove(core);
        }
    }

    void clearSharers(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        if (containLLCLine(address)) {
            LLC_directory[set_index].metadata_per_set[address].sharers.clear();
        } else if (containNILine(address)) {
            NI_directory[set_index].metadata_per_set[address].sharers.clear();
        }
    }

    void addOwner(Addr address, MachineID core, bool inclusive) {
        int64_t set_index = addressToCacheSet(address);
        bool is_present_in_LLC_directory = containLLCLine(address);
        bool is_present_in_NI_directory = containNILine(address);

        if (inclusive) {
            assert(!is_present_in_NI_directory);
            if (is_present_in_LLC_directory) {
                LLC_directory[set_index].metadata_per_set[address].owner.add(core);
            } else {
                NetDest dest;
                dest.add(core);
                LLC_directory[set_index].metadata_per_set[address] = { .owner = dest };
            }
            assert(LLC_directory[set_index].metadata_per_set[address].owner.count() == 1);
        } else {
            assert(!is_present_in_LLC_directory);
            if (is_present_in_NI_directory) {
                NI_directory[set_index].metadata_per_set[address].owner.add(core);
            } else {
                NetDest dest;
                dest.add(core);
                NI_directory[set_index].metadata_per_set[address] = { .owner = dest };
            }
            assert(NI_directory[set_index].metadata_per_set[address].owner.count() == 1);
        }
    }

    void clearOwner(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        if (containLLCLine(address)) {
            LLC_directory[set_index].metadata_per_set[address].owner.clear();
        } else if (containNILine(address)) {
            NI_directory[set_index].metadata_per_set[address].owner.clear();
        }
    }

    void convertOwnerToSharer(Addr address, bool inclusive) {
        int64_t set_index = addressToCacheSet(address);
        bool is_present_in_LLC_directory = containLLCLine(address);
        bool is_present_in_NI_directory = containNILine(address);

        if (inclusive) {
            assert((!is_present_in_NI_directory) && is_present_in_LLC_directory);
            LLC_directory[set_index].metadata_per_set[address].sharers.addNetDest(LLC_directory[set_index].metadata_per_set[address].owner);
            LLC_directory[set_index].metadata_per_set[address].owner.clear();
        } else {
            assert((!is_present_in_LLC_directory) && is_present_in_NI_directory);
            NI_directory[set_index].metadata_per_set[address].sharers.addNetDest(NI_directory[set_index].metadata_per_set[address].owner);
            NI_directory[set_index].metadata_per_set[address].owner.clear();
        }
    }

    // metadata operations

    bool containLLCLine(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        return (LLC_directory[set_index].metadata_per_set.find(address) != LLC_directory[set_index].metadata_per_set.end());
    }

    bool containNILine(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        return (NI_directory[set_index].metadata_per_set.find(address) != NI_directory[set_index].metadata_per_set.end());
    }

    bool getIsDirty(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        if (containLLCLine(address)) {
            return LLC_directory[set_index].metadata_per_set[address].isDirty;
        } else if (containNILine(address)) {
            return NI_directory[set_index].metadata_per_set[address].isDirty;
        } else {
            assert(false);
            return false;
        }
    }
    
    void setIsDirty(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        if (containLLCLine(address)) {
            LLC_directory[set_index].metadata_per_set[address].isDirty = 1;
        } else if (containNILine(address)) {
            NI_directory[set_index].metadata_per_set[address].isDirty = 1;
        } else {
            assert(false);
        }
    }

    void unSetIsDirty(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        if (containLLCLine(address)) {
            LLC_directory[set_index].metadata_per_set[address].isDirty = 0;
        } else if (containNILine(address)) {
            NI_directory[set_index].metadata_per_set[address].isDirty = 0;
        } else {
            assert(false);
        }
    }

    void setNITransientState(Addr address, int state) {
        assert(containNILine(address));
        int64_t set_index = addressToCacheSet(address);
        NI_directory[set_index].metadata_per_set[address].NI_transient_state = state;
    }

    int getNIState(Addr address) {
        assert(containNILine(address));
        int64_t set_index = addressToCacheSet(address);
        if (NI_directory[set_index].metadata_per_set[address].NI_transient_state == 0) {
            if ((NI_directory[set_index].metadata_per_set[address].sharers.count() > 0) && (NI_directory[set_index].metadata_per_set[address].owner.count() == 0)) {
                return 1; // S_NI
            } else if ((NI_directory[set_index].metadata_per_set[address].sharers.count() == 0) && (NI_directory[set_index].metadata_per_set[address].owner.count() > 0)) {
                return 2; // M_NI
            }
        } else {
            return NI_directory[set_index].metadata_per_set[address].NI_transient_state;
        }
        return 0;
    }

    void convertNILineToIN(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        assert((!containLLCLine(address)) && (containNILine(address)));
        
        MetadataPerLine target_line = NI_directory[set_index].metadata_per_set[address];
        LLC_directory[set_index].metadata_per_set[address] = target_line;
        NI_directory[set_index].metadata_per_set.erase(address);
        assert((containLLCLine(address)) && (!containNILine(address)));
    }

    void convertINLineToNI(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        assert((containLLCLine(address)) && (!containNILine(address)));
        
        MetadataPerLine target_line = LLC_directory[set_index].metadata_per_set[address];
        NI_directory[set_index].metadata_per_set[address] = target_line;
        LLC_directory[set_index].metadata_per_set.erase(address);
        assert((!containLLCLine(address)) && (containNILine(address)));
    }

    void removeLineFromLLCDirectory(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        assert((containLLCLine(address)) && (!containNILine(address)));
        LLC_directory[set_index].metadata_per_set.erase(address);
    }

    void removeLineFromNIDirectory(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        assert((!containLLCLine(address)) && (containNILine(address)));
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
            if ((!line->second.isDirty) && (line->second.owner.count() == 0) && (line->second.sharers.count() == 0)) {
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
            if ((line->second.isDirty) && (line->second.owner.count() == 0) && (line->second.sharers.count() == 0)) {
                llc_only_dirty_lines.insert(line->first);
            }
        }
        return llc_only_dirty_lines;
    }

    Addr getLRULine(std::unordered_set<Addr> lines) {
        assert(lines.size() > 0);
        Tick LRU_time = MaxTick;
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
        assert(LRU_line > 0);
        return LRU_line;
    }

    // victim selection operations

    bool existVacancyPerSet(Addr address) {
        int64_t set_index = addressToCacheSet(address);
        return (LLC_directory[set_index].metadata_per_set.size() < m_cache_assoc);
    }

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

    // stats functions

    void checkNewEpoch() {
        if ((int) (curTick() / tickPerEpoch) > epochCount) {
            printEpochStats();
        }
    }

    void incrementINFetchCount() {
        INFetches++;
    }

    void incrementNIFetchCount() {
        NIFetches++;
    }

    void incrementINConversionCount() {
        INConvertions++;
    }

    void printEpochStats() {
        DPRINTF(FlexLLC_Stats, "==========[Epoch: %d]========== \n", epochCount);
        DPRINTF(FlexLLC_Stats, "Number of IN lines: %d \n", getCacheLineCount(true));
        DPRINTF(FlexLLC_Stats, "Number of NI lines: %d \n", getCacheLineCount(false));
        DPRINTF(FlexLLC_Stats, "Number of IN fetches: %d \n", INFetches);
        DPRINTF(FlexLLC_Stats, "Number of NI fetches: %d \n", NIFetches);
        DPRINTF(FlexLLC_Stats, "Number of IN conversions: %d \n", INConvertions);

        epochCount++;
        INFetches = 0;
        NIFetches = 0;
        INConvertions = 0;
    }

protected:
    std::vector<MetadataPerSet> LLC_directory;
    std::vector<MetadataPerSet> NI_directory;

    int epochCount = 0;
    Tick tickPerEpoch = 10000000000;

    int INFetches = 0;
    int NIFetches = 0;
    int INConvertions = 0;
};

}  // namespace ruby
}  // namespace gem5

#endif  // __MEM_RUBY_STRUCTURES_XYZCACHEMEMORY_HH__
