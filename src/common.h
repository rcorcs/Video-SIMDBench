
#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <inttypes.h>

#include <sys/time.h>


#include <assert.h>

typedef uint16_t pixel_10b;
typedef uint64_t pixel4_10b;
typedef int32_t  dctcoef_10b;
typedef uint32_t udctcoef_10b;

typedef uint8_t  pixel;
typedef uint32_t pixel4;
typedef int16_t  dctcoef;
typedef uint16_t udctcoef;


// GCC doesn't align stack variables on ARM, so use .bss
#if ARCH_ARM
#undef ALIGNED_16
#define ALIGNED_16( var ) DECLARE_ALIGNED( static var, 16 )
#endif

#define FIX8(f) ((int)(f*(1<<8)+.5))
#define ARRAY_ELEMS(a) ((sizeof(a))/(sizeof(a[0])))


#define X264_BFRAME_MAX 16
#define X264_REF_MAX 16
#define X264_THREAD_MAX 128
#define X264_LOOKAHEAD_THREAD_MAX 16
#define X264_PCM_COST (FRAME_SIZE(256*8)+16)
#define X264_PCM_COST_10B (FRAME_SIZE(256*10)+16)
#define X264_LOOKAHEAD_MAX 250
#define QP_BD_OFFSET (6*(8-8))
#define QP_BD_OFFSET_10B (6*(10-8))
#define QP_MAX_SPEC (51+QP_BD_OFFSET)
#define QP_MAX (QP_MAX_SPEC+18)
#define QP_MAX_MAX (51+2*6+18)


#   define MB_INTERLACED 0
#   define SLICE_MBAFF 0
#   define PARAM_INTERLACED 0


#define LOWRES_COST_MASK ((1<<14)-1)
#define LOWRES_COST_SHIFT 14

// arbitrary, but low because SATD scores are 1/4 normal
#define X264_LOOKAHEAD_QP (12+QP_BD_OFFSET)
#define SPEC_QP(x) MIN((x), QP_MAX_SPEC)


#define MAX(a,b) ( (a)<(b) ? (b) : (a) )

#define MIN(a,b) ( (a)<(b) ? (a) : (b) )
#define MIN3(a,b,c) MIN((a),MIN((b),(c)))
#define MIN4(a,b,c,d) MIN((a),MIN3((b),(c),(d)))

#define MC_CLIP_ADD(s,x) (s) = MIN((s)+(x),(1<<15)-1)


#define CHROMA_SIZE(s) ((s)>>(CHROMA_H_SHIFT+CHROMA_V_SHIFT))
#define FRAME_SIZE(s) ((s)+2*CHROMA_SIZE(s))

#define CHROMA_FORMAT CHROMA_420
#define CHROMA444 (CHROMA_FORMAT == CHROMA_444)

/* Unions for type-punning.
 *  * Mn: load or store n bits, aligned, native-endian
 *   * CPn: copy n bits, aligned, native-endian
 *    * we don't use memcpy for CPn because memcpy's args aren't assumed to be aligned */
typedef union { uint16_t i; uint8_t  c[2]; }  vbench_union16_t;
typedef union { uint32_t i; uint16_t b[2]; uint8_t  c[4]; } vbench_union32_t;
typedef union { uint64_t i; uint32_t a[2]; uint16_t b[4]; uint8_t c[8]; } vbench_union64_t;
typedef struct { uint64_t i[2]; } vbench_uint128_t;
typedef union { vbench_uint128_t i; uint64_t a[2]; uint32_t b[4]; uint16_t c[8]; uint8_t d[16]; } vbench_union128_t;
#define M16(src) (((vbench_union16_t*)(src))->i)
#define M32(src) (((vbench_union32_t*)(src))->i)
#define M64(src) (((vbench_union64_t*)(src))->i)
#define M128(src) (((vbench_union128_t*)(src))->i)
#define M128_ZERO ((vbench_uint128_t){{0,0}})
#define CP16(dst,src) M16(dst) = M16(src)
#define CP32(dst,src) M32(dst) = M32(src)
#define CP64(dst,src) M64(dst) = M64(src)
#define CP128(dst,src) M128(dst) = M128(src)



#   define PIXEL_SPLAT_X4_10B(x) ((x)*0x0001000100010001ULL)
#   define MPIXEL_X4_10B(src) M64(src)
#   define PIXEL_SPLAT_X4(x) ((x)*0x01010101U)
#   define MPIXEL_X4(src) M32(src)


#define CPPIXEL_X4(dst,src) MPIXEL_X4(dst) = MPIXEL_X4(src)

#define PIXEL_MAX ((1 << 8)-1)
#define PIXEL_MAX_10B ((1 << 10)-1)

#define XCHG(type,a,b) do{ type t = a; a = b; b = t; } while(0)
#define MIN(a,b) ( (a)<(b) ? (a) : (b) )

#define FENC_STRIDE 16
#define FDEC_STRIDE 32





#define SCAN8_LUMA_SIZE (5*8)
#define SCAN8_SIZE (SCAN8_LUMA_SIZE*3)
#define SCAN8_0 (4+1*8)
/* Scan8 organization:
 *  0 1 2 3 4 5 6 7
 *  0  DY    y y y y y
 *  1        y Y Y Y Y
 *  2        y Y Y Y Y
 *  3        y Y Y Y Y
 *  4        y Y Y Y Y
 *  5  DU    u u u u u
 *  6        u U U U U
 *  7        u U U U U
 *  8        u U U U U
 *  9        u U U U U
 *  10 DV    v v v v v
 *  11       v V V V V
 *  12       v V V V V
 *  13       v V V V V
 *  14       v V V V V
 *  DY/DU/DV are for luma/chroma DC.
 **/

#define LUMA_DC   48
#define CHROMA_DC 49

static const uint8_t x264_scan8[16*3 + 3] =
{
    4+ 1*8, 5+ 1*8, 4+ 2*8, 5+ 2*8,
    6+ 1*8, 7+ 1*8, 6+ 2*8, 7+ 2*8,
    4+ 3*8, 5+ 3*8, 4+ 4*8, 5+ 4*8,
    6+ 3*8, 7+ 3*8, 6+ 4*8, 7+ 4*8,
    4+ 6*8, 5+ 6*8, 4+ 7*8, 5+ 7*8,
    6+ 6*8, 7+ 6*8, 6+ 7*8, 7+ 7*8,
    4+ 8*8, 5+ 8*8, 4+ 9*8, 5+ 9*8,
    6+ 8*8, 7+ 8*8, 6+ 9*8, 7+ 9*8,
    4+11*8, 5+11*8, 4+12*8, 5+12*8,
    6+11*8, 7+11*8, 6+12*8, 7+12*8,
    4+13*8, 5+13*8, 4+14*8, 5+14*8,
    6+13*8, 7+13*8, 6+14*8, 7+14*8,
    0+ 0*8, 0+ 5*8, 0+10*8
};

enum slice_type_e
{
    SLICE_TYPE_P  = 0,
    SLICE_TYPE_B  = 1,
    SLICE_TYPE_I  = 2,
};



/****************************************************************************
 * ANALYZE FLAGS
 ****************************************************************************/
/* Analyse flags */
#define ANALYSE_I4x4       0x0001  /* Analyse i4x4 */
#define ANALYSE_I8x8       0x0002  /* Analyse i8x8 (requires 8x8 transform) */
#define ANALYSE_PSUB16x16  0x0010  /* Analyse p16x8, p8x16 and p8x8 */
#define ANALYSE_PSUB8x8    0x0020  /* Analyse p8x4, p4x8, p4x4 */
#define ANALYSE_BSUB16x16  0x0100  /* Analyse b16x8, b8x16 and b8x8 */
#define DIRECT_PRED_NONE        0
#define DIRECT_PRED_SPATIAL     1
#define DIRECT_PRED_TEMPORAL    2
#define DIRECT_PRED_AUTO        3
#define ME_DIA                  0
#define ME_HEX                  1
#define ME_UMH                  2
#define ME_ESA                  3
#define ME_TESA                 4
#define CQM_FLAT                0
#define CQM_JVT                 1
#define CQM_CUSTOM              2
#define RC_CQP                  0
#define RC_CRF                  1
#define RC_ABR                  2
#define QP_AUTO                 0
#define AQ_NONE                 0
#define AQ_VARIANCE             1
#define AQ_AUTOVARIANCE         2
#define AQ_AUTOVARIANCE_BIASED  3
#define B_ADAPT_NONE            0
#define B_ADAPT_FAST            1
#define B_ADAPT_TRELLIS         2
#define WEIGHTP_NONE            0
#define WEIGHTP_SIMPLE          1
#define WEIGHTP_SMART           2
#define B_PYRAMID_NONE          0
#define B_PYRAMID_STRICT        1
#define B_PYRAMID_NORMAL        2
#define KEYINT_MIN_AUTO         0
#define KEYINT_MAX_INFINITE     (1<<30)












#define BENCH_RUNS 100  // tradeoff between accuracy and speed
#define BENCH_ALIGNS 32 // number of stack+heap data alignments (another accuracy vs speed tradeoff)
#define MAX_FUNCS 8192  // just has to be big enough to hold all the existing functions
#define MAX_CPUS 64     // number of different combinations of cpu flags



/****************************************************************************
 * CPU FLAGS
 ****************************************************************************/

/* x86 */
#define VSIMD_CPU_CMOV            0x0000001
#define VSIMD_CPU_MMX             0x0000002
#define VSIMD_CPU_MMX2            0x0000004  /* MMX2 aka MMXEXT aka ISSE */
#define VSIMD_CPU_MMXEXT          VSIMD_CPU_MMX2
#define VSIMD_CPU_SSE             0x0000008
#define VSIMD_CPU_SSE2            0x0000010
#define VSIMD_CPU_SSE3            0x0000020
#define VSIMD_CPU_SSSE3           0x0000040
#define VSIMD_CPU_SSE4            0x0000080  /* SSE4.1 */
#define VSIMD_CPU_SSE42           0x0000100  /* SSE4.2 */
#define VSIMD_CPU_LZCNT           0x0000200  /* Phenom support for "leading zero count" instruction. */
#define VSIMD_CPU_AVX             0x0000400  /* AVX support: requires OS support even if YMM registers aren't used. */
#define VSIMD_CPU_XOP             0x0000800  /* AMD XOP */
#define VSIMD_CPU_FMA4            0x0001000  /* AMD FMA4 */
#define VSIMD_CPU_FMA3            0x0002000  /* FMA3 */
#define VSIMD_CPU_AVX2            0x0004000  /* AVX2 */
#define VSIMD_CPU_BMI1            0x0008000  /* BMI1 */
#define VSIMD_CPU_BMI2            0x0010000  /* BMI2 */
/* x86 modifiers */
#define VSIMD_CPU_CACHELINE_32    0x0020000  /* avoid memory loads that span the border between two cachelines */
#define VSIMD_CPU_CACHELINE_64    0x0040000  /* 32/64 is the size of a cacheline in bytes */
#define VSIMD_CPU_SSE2_IS_SLOW    0x0080000  /* avoid most SSE2 functions on Athlon64 */
#define VSIMD_CPU_SSE2_IS_FAST    0x0100000  /* a few functions are only faster on Core2 and Phenom */
#define VSIMD_CPU_SLOW_SHUFFLE    0x0200000  /* The Conroe has a slow shuffle unit (relative to overall SSE performance) */
#define VSIMD_CPU_STACK_MOD4      0x0400000  /* if stack is only mod4 and not mod16 */
#define VSIMD_CPU_SLOW_CTZ        0x0800000  /* BSR/BSF x86 instructions are really slow on some CPUs */
#define VSIMD_CPU_SLOW_ATOM       0x1000000  /* The Atom is terrible: slow SSE unaligned loads, slow
                                             * SIMD multiplies, slow SIMD variable shifts, slow pshufb,
                                             * cacheline split penalties -- gather everything here that
                                             * isn't shared by other CPUs to avoid making half a dozen
                                             * new SLOW flags. */
#define VSIMD_CPU_SLOW_PSHUFB     0x2000000  /* such as on the Intel Atom */
#define VSIMD_CPU_SLOW_PALIGNR    0x4000000  /* such as on the AMD Bobcat */

/* PowerPC */
#define VSIMD_CPU_ALTIVEC         0x0000001

/* ARM and AArch64 */
#define VSIMD_CPU_ARMV6           0x0000001
#define VSIMD_CPU_NEON            0x0000002  /* ARM NEON */
#define VSIMD_CPU_FAST_NEON_MRC   0x0000004  /* Transfer from NEON to ARM register is fast (Cortex-A9) */
#define VSIMD_CPU_ARMV8           0x0000008

/* MIPS */
#define VSIMD_CPU_MSA             0x0000001  /* MIPS MSA */




/** CPU DETECT */

typedef struct
{
    const char name[16];
    uint32_t flags;
} cpu_name_t;

extern const cpu_name_t cpu_names[];

/* CPU flags */

/* x86 */
#define CPU_CMOV            0x0000001
#define CPU_MMX             0x0000002
#define CPU_MMX2            0x0000004  /* MMX2 aka MMXEXT aka ISSE */
#define CPU_MMXEXT          CPU_MMX2
#define CPU_SSE             0x0000008
#define CPU_SSE2            0x0000010
#define CPU_SSE3            0x0000020
#define CPU_SSSE3           0x0000040
#define CPU_SSE4            0x0000080  /* SSE4.1 */
#define CPU_SSE42           0x0000100  /* SSE4.2 */
#define CPU_LZCNT           0x0000200  /* Phenom support for "leading zero count" instruction. */
#define CPU_AVX             0x0000400  /* AVX support: requires OS support even if YMM registers aren't used. */
#define CPU_XOP             0x0000800  /* AMD XOP */
#define CPU_FMA4            0x0001000  /* AMD FMA4 */
#define CPU_FMA3            0x0002000  /* FMA3 */
#define CPU_AVX2            0x0004000  /* AVX2 */
#define CPU_BMI1            0x0008000  /* BMI1 */
#define CPU_BMI2            0x0010000  /* BMI2 */
/* x86 modifiers */
#define CPU_CACHELINE_32    0x0020000  /* avoid memory loads that span the border between two cachelines */
#define CPU_CACHELINE_64    0x0040000  /* 32/64 is the size of a cacheline in bytes */
#define CPU_SSE2_IS_SLOW    0x0080000  /* avoid most SSE2 functions on Athlon64 */
#define CPU_SSE2_IS_FAST    0x0100000  /* a few functions are only faster on Core2 and Phenom */
#define CPU_SLOW_SHUFFLE    0x0200000  /* The Conroe has a slow shuffle unit (relative to overall SSE performance) */
#define CPU_STACK_MOD4      0x0400000  /* if stack is only mod4 and not mod16 */
#define CPU_SLOW_CTZ        0x0800000  /* BSR/BSF x86 instructions are really slow on some CPUs */
#define CPU_SLOW_ATOM       0x1000000  /* The Atom is terrible: slow SSE unaligned loads, slow
                                             * SIMD multiplies, slow SIMD variable shifts, slow pshufb,
                                             * cacheline split penalties -- gather everything here that
                                             * isn't shared by other CPUs to avoid making half a dozen
                                             * new SLOW flags. */

#define CPU_SLOW_PSHUFB     0x2000000  /* such as on the Intel Atom */
#define CPU_SLOW_PALIGNR    0x4000000  /* such as on the AMD Bobcat */

/* PowerPC */
#define CPU_ALTIVEC         0x0000001

/* ARM and AArch64 */
#define CPU_ARMV6           0x0000001
#define CPU_NEON            0x0000002  /* ARM NEON */
#define CPU_FAST_NEON_MRC   0x0000004  /* Transfer from NEON to ARM register is fast (Cortex-A9) */
#define CPU_ARMV8           0x0000008

/* MIPS */
#define CPU_MSA             0x0000001  /* MIPS MSA */



uint32_t cpu_detect( void );


#if ARCH_X86 || ARCH_X86_64
int stack_pagealign( int (*func)(), int align );

/* detect when callee-saved regs aren't saved
 * needs an explicit asm check because it only sometimes crashes in normal use. */
intptr_t benchmark_call( intptr_t (*func)(), int *ok, ... );
#else
#define stack_pagealign( func, align ) func()
#endif




#define CPPIXEL_X4(dst,src) MPIXEL_X4(dst) = MPIXEL_X4(src)


/* Calls for the benchmarks */
int check_pixel( int cpu_ref, int cpu_new );



static pixel vbench_clip_pixel( int x )
{
    return ( (x & ~PIXEL_MAX) ? (-x)>>31 & PIXEL_MAX : x );
}

static pixel_10b vbench_clip_pixel_10b( int x )
{
    return ( (x & ~PIXEL_MAX_10B) ? (-x)>>31 & PIXEL_MAX_10B : x );
}

static int vbench_clip3( int v, int i_min, int i_max )
{
    return ( (v < i_min) ? i_min : (v > i_max) ? i_max : v );
}

static double vbench_clip3f( double v, double f_min, double f_max )
{
    return ( (v < f_min) ? f_min : (v > f_max) ? f_max : v );
}

static int vbench_median( int a, int b, int c )
{
    int t = (a-b)&((a-b)>>31);
    a -= t;
    b += t;
    b -= (b-c)&((b-c)>>31);
    b += (a-b)&((a-b)>>31);
    return b;
}







#endif // COMMON_H


