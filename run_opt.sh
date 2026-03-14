rm -rf m5out

# quick tests

PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt \
    configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
    --program /gem5/Splash-3/codes/kernels/cholesky/CHOLESKY --cwd /gem5/Splash-3/codes/kernels/cholesky --args "-p4 -B32 -C65536" \
    --input-file /gem5/Splash-3/codes/kernels/cholesky/inputs/tk16.O \
    2>&1 | tee cholesky_4_8_8_8_4_16_1024.txt

# PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt \
#     configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
#     --program /gem5/Splash-3/codes/kernels/radix/RADIX --cwd /gem5/Splash-3/codes/kernels/radix --args "-p4 -n262144 -r256" \
#     2>&1 | tee radix_4_8_8_8_4_16_1024.txt

# PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt \
#     configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
#     --program /gem5/Splash-3/codes/apps/ocean/contiguous_partitions/OCEAN --cwd /gem5/Splash-3/codes/apps/ocean/contiguous_partitions \
#     --args "-p4 -n130" \
#     2>&1 | tee ocean_4_8_8_8_4_16_1024.txt

# # moderate tests

# PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt \
#     configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
#     --program /gem5/Splash-3/codes/apps/raytrace/RAYTRACE \
#     --cwd /gem5/Splash-3/codes/apps/raytrace \
#     --args "-p4 /gem5/Splash-3/codes/apps/raytrace/inputs/teapot.env" \
#     2>&1 | tee raytrace_4_8_8_8_4_16_1024.txt

# PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt \
#     configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
#     --program /gem5/Splash-3/codes/apps/barnes/BARNES --cwd /gem5/Splash-3/codes/apps/barnes \
#     --input-file /gem5/Splash-3/codes/apps/barnes/inputs/n8k-p4 \
#     2>&1 | tee barnes_small_4_8_8_8_4_16_1024.txt

# # slow tests

# PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt \
#     configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --wc --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
#     --program /gem5/Splash-3/codes/apps/fmm/FMM --cwd /gem5/Splash-3/codes/apps/fmm \
#     --input-file /gem5/Splash-3/codes/apps/fmm/inputs/input.4.16384 \
#     2>&1 | tee fmm_4_8_8_8_4_16_1024.txt

# PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt \
#     configs/xyz/simple_ruby.py --wc --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
#     --program /gem5/Splash-3/codes/apps/barnes/BARNES --cwd /gem5/Splash-3/codes/apps/barnes \
#     --input-file /gem5/Splash-3/codes/apps/barnes/inputs/n16384-p4 \
#     2>&1 | tee barnes_large_4_8_8_8_4_16_1024.txt

# synthetic test
 
# PYTHONPATH="$PWD/configs:$PWD/build/X86_LC_MSI/python" ./build/X86_LC_MSI/gem5.opt \
#     configs/xyz/simple_ruby.py --wc --ruby-test --subslot-opt --ncore 4 --l1-assoc 8 --l2-assoc 8 --l3-assoc 8 --l1-size 4kB --l2-size 16kB --l3-size 1MB \
#     --nreq 20000