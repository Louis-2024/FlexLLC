
#include "xyz/XYZStatsObject.hh"

#include <iostream>

namespace gem5::ruby
{

XYZStatsObject::XYZStatsObject(const XYZStatsObjectParams &params) :
    ClockedObject(params)
{
    std::cout << "XYZStatsObject !" << std::endl;
    wcl_bound = params.worst_case_latency_bound;
}

void XYZStatsObject::regStats() {
    Base::regStats();
    latencies.init(0, 100000, 50000);
    latencies.name(name() + "latency").desc("Mem access latency");

    l0_hits.name(name() + ".FlexLLC_L0_Hits").desc("");
    l1_hits.name(name() + ".FlexLLC_L1_Hits").desc("");
    llc_hits.name(name() + ".FlexLLC_LLC_Hits").desc("");
    mem_hits.name(name() + ".FlexLLC_Cache_Misses").desc("");
}

} // namespace gem5
