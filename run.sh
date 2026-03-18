mkdir -p stats

#kernels

PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt --debug-flags=FlexLLC_Stats --debug-file=flexllc.log --debug-start=0 \
    configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
    --program /gem5/Splash-3/codes/kernels/cholesky/CHOLESKY --cwd /gem5/Splash-3/codes/kernels/cholesky --args "-p4 -B32 -C65536" \
    --input-file /gem5/Splash-3/codes/kernels/cholesky/inputs/tk16.O \
    2>&1 | tee stats/cholesky_terminal.txt
sleep 1
mv m5out/stats.txt stats/cholesky_stats.txt
[ -f m5out/flexllc.log ] && mv m5out/flexllc.log stats/cholesky_flexllc.log

PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt --debug-flags=FlexLLC_Stats --debug-file=flexllc.log --debug-start=0 \
    configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
    --program /gem5/Splash-3/codes/kernels/radix/RADIX --cwd /gem5/Splash-3/codes/kernels/radix --args "-p4 -n524288 -r256" \
    2>&1 | tee stats/radix_terminal.txt
sleep 1
mv m5out/stats.txt stats/radix_stats.txt
[ -f m5out/flexllc.log ] && mv m5out/flexllc.log stats/radix_flexllc.log

PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt --debug-flags=FlexLLC_Stats --debug-file=flexllc.log --debug-start=0 \
    configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
    --program /gem5/Splash-3/codes/kernels/fft/FFT --cwd /gem5/Splash-3/codes/kernels/fft --args "-p4 -m20 -n20 -l4" \
    2>&1 | tee stats/fft_terminal.txt
sleep 1
mv m5out/stats.txt stats/fft_stats.txt
[ -f m5out/flexllc.log ] && mv m5out/flexllc.log stats/fft_flexllc.log

PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt --debug-flags=FlexLLC_Stats --debug-file=flexllc.log --debug-start=0 \
    configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
    --program /gem5/Splash-3/codes/kernels/lu/contiguous_blocks/LU --cwd /gem5/Splash-3/codes/kernels/lu/contiguous_blocks --args "-p4 -n512 -b16" \
    2>&1 | tee stats/lu_contiguous_terminal.txt
sleep 1
mv m5out/stats.txt stats/lu_contiguous_stats.txt
[ -f m5out/flexllc.log ] && mv m5out/flexllc.log stats/lu_contiguous_flexllc.log

# apps

PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt --debug-flags=FlexLLC_Stats --debug-file=flexllc.log --debug-start=0 \
    configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
    --program /gem5/Splash-3/codes/apps/ocean/contiguous_partitions/OCEAN --cwd /gem5/Splash-3/codes/apps/ocean/contiguous_partitions \
    --args "-p4 -n130" \
    2>&1 | tee stats/ocean_contiguous_terminal.txt
sleep 1
mv m5out/stats.txt stats/ocean_contiguous_stats.txt
[ -f m5out/flexllc.log ] && mv m5out/flexllc.log stats/ocean_contiguous_flexllc.log

PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt --debug-flags=FlexLLC_Stats --debug-file=flexllc.log --debug-start=0 \
    configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
    --program /gem5/Splash-3/codes/apps/ocean/non_contiguous_partitions/OCEAN --cwd /gem5/Splash-3/codes/apps/ocean/non_contiguous_partitions \
    --args "-p4 -n130" \
    2>&1 | tee stats/ocean_noncontig_terminal.txt
sleep 1
mv m5out/stats.txt stats/ocean_noncontig_stats.txt
[ -f m5out/flexllc.log ] && mv m5out/flexllc.log stats/ocean_noncontig_flexllc.log

PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt --debug-flags=FlexLLC_Stats --debug-file=flexllc.log --debug-start=0 \
    configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
    --program /gem5/Splash-3/codes/apps/raytrace/RAYTRACE \
    --cwd /gem5/Splash-3/codes/apps/raytrace \
    --args "-p4 /gem5/Splash-3/codes/apps/raytrace/inputs/teapot.env" \
    2>&1 | tee stats/raytrace_terminal.txt
sleep 1
mv m5out/stats.txt stats/raytrace_stats.txt
[ -f m5out/flexllc.log ] && mv m5out/flexllc.log stats/raytrace_flexllc.log

PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt --debug-flags=FlexLLC_Stats --debug-file=flexllc.log --debug-start=0 \
    configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
    --program /gem5/Splash-3/codes/apps/barnes/BARNES --cwd /gem5/Splash-3/codes/apps/barnes \
    --input-file /gem5/Splash-3/codes/apps/barnes/inputs/n8k-p4 \
    2>&1 | tee stats/barnes_small_terminal.txt
sleep 1
mv m5out/stats.txt stats/barnes_small_stats.txt
[ -f m5out/flexllc.log ] && mv m5out/flexllc.log stats/barnes_small_flexllc.log

PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt --debug-flags=FlexLLC_Stats --debug-file=flexllc.log --debug-start=0 \
    configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
    --program /gem5/Splash-3/codes/apps/barnes/BARNES --cwd /gem5/Splash-3/codes/apps/barnes \
    --input-file /gem5/Splash-3/codes/apps/barnes/inputs/n16384-p4 \
    2>&1 | tee stats/barnes_large_terminal.txt
sleep 1
mv m5out/stats.txt stats/barnes_large_stats.txt
[ -f m5out/flexllc.log ] && mv m5out/flexllc.log stats/barnes_large_flexllc.log

PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt --debug-flags=FlexLLC_Stats --debug-file=flexllc.log --debug-start=0 \
    configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
    --program /gem5/Splash-3/codes/apps/water-nsquared/WATER-NSQUARED --cwd /gem5/Splash-3/codes/apps/water-nsquared --args "-p4 -n256" \
    2>&1 | tee stats/water_nsquared_terminal.txt
sleep 1
mv m5out/stats.txt stats/water_nsquared_stats.txt
[ -f m5out/flexllc.log ] && mv m5out/flexllc.log stats/water_nsquared_flexllc.log

PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt --debug-flags=FlexLLC_Stats --debug-file=flexllc.log --debug-start=0 \
    configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
    --program /gem5/Splash-3/codes/apps/water-spatial/WATER-SPATIAL --cwd /gem5/Splash-3/codes/apps/water-spatial --args "-p4 -n343" \
    2>&1 | tee stats/water_spatial_terminal.txt
sleep 1
mv m5out/stats.txt stats/water_spatial_stats.txt
[ -f m5out/flexllc.log ] && mv m5out/flexllc.log stats/water_spatial_flexllc.log

PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt --debug-flags=FlexLLC_Stats --debug-file=flexllc.log --debug-start=0 \
    configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
    --program /gem5/Splash-3/codes/apps/fmm/FMM --cwd /gem5/Splash-3/codes/apps/fmm \
    --input-file /gem5/Splash-3/codes/apps/fmm/inputs/input.4.16384 \
    2>&1 | tee stats/fmm_terminal.txt
sleep 1
mv m5out/stats.txt stats/fmm_stats.txt
[ -f m5out/flexllc.log ] && mv m5out/flexllc.log stats/fmm_flexllc.log