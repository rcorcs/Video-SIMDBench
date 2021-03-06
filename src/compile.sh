
# 
# Flags:
#      - SYS_WINDOWS
#      - ARCH_ARM
#      - ARCH_X86
#      - ARCH_X86_64
#      - ARCH_PPC
#      - ARCH_AARCH64 
#      - ARCH_MIPS
#      - HAVE_X86_INLINE_ASM
#      - HAVE_MMX
#

# set the echo on
set -x 

#yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8 ./asm/x86/sad16-a.asm -o sad16-a.o



yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 ./asm/x86/cpu-a.asm -o cpu-detect.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 ./asm/x86/checkasm-a.asm -o checkasm-a.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DBIT_DEPTH=8  ./asm/x86/const-a.asm -o const-a.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8 ./asm/x86/predict-a.asm -o predict-a.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8 ./asm/x86/pixel-a.asm -o pixel-a.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8 ./asm/x86/sad-a.asm -o sad-a.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8 ./asm/x86/dct-a.asm -o dct-a.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8 ./asm/x86/dct-64.asm -o dct-64.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8 ./asm/x86/quant-a.asm -o quant-a.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8 ./asm/x86/trellis-64.asm -o trellis-64.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8 ./asm/x86/cabac-a.asm -o cabac-a.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8 ./asm/x86/mc-a.asm -o mc-a.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8 ./asm/x86/mc-a2.asm -o mc-a2.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8 ./asm/x86/deblock-a.asm -o deblock-a.o
yasm -I.  -DARCH_X86_64=1 -f elf64 -Worphan-labels -DSTACK_ALIGNMENT=32 -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8 ./asm/x86/bitstream-a.asm -o bitstream-a.o



#fake.sh icc  -O3 -vec-report=5 -std=c99 -DARCH_X86_64=1 -DHAVE_MMX \

#clang-3.8 -O3 -std=c99 -DARCH_X86_64=1 -DHAVE_MMX \

#gcc-5  -O3 -mtune=core-avx2 -march=core-avx2 -ftree-vectorize  -ftree-vectorizer-verbose=5 -funroll-all-loops -fopt-info-vec-missed  --std=gnu99 -DARCH_X86_64=1 -DHAVE_MMX 



#gcc-5  -O3 -fno-tree-vectorize  --std=gnu99 -DARCH_X86_64=1 -DHAVE_MMX \



gcc-5  -O3 -mtune=core-avx2 -march=core-avx2 -ftree-vectorize  -ftree-vectorizer-verbose=5 -funroll-all-loops -fopt-info-vec-all  --std=gnu99 -DARCH_X86_64=1 -DHAVE_MMX  \
    cpu-detect.o checkasm-a.o predict-a.o const-a.o pixel-a.o  sad-a.o dct-a.o dct-64.o quant-a.o trellis-64.o  cabac-a.o mc-a.o mc-a2.o deblock-a.o  bitstream-a.o  \
    ./asm/x86/predict-c.c ./asm/x86/mc-c.c \
    ./c_kernels/pixel.c ./c_kernels/predict.c ./c_kernels/quant.c  ./c_kernels/dct.c  ./c_kernels/mc.c ./c_kernels/deblock.c ./c_kernels/bitstream.c ./c_kernels/cabac.c \
    main.c bench_pixel.c  bench_dct.c bench_cabac.c  bench_mc.c bench_intra.c bench_deblock.c bench_quant.c bench_bitstream.c \
    bench.c cpu.c   -I./ -lm

#gcc -g -Wall -O3 -ftree-vectorizer-verbose=2 -march=corei7 --std=gnu99 -DARCH_X86_64=1 -DHAVE_MMX \


