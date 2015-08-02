#ifndef EE_FFT_MODULE_H
#define EE_FFT_MODULE_H

#define intpow(n) _pow_##n
#define _pow_2  4
#define _pow_3  8
#define _pow_4  16
#define _pow_5  32
#define _pow_6  64
#define _pow_7  128
#define _pow_8  256
#define _pow_9  512
#define _pow_10 1024
#define _pow_11 2048
#define _pow_12 4096
#define _pow_13 8192
#define _pow_14 16384
#define _pow_15 32768
#define _pow_16 65536
#define _pow_17 131072
#define _pow_18 262144
#define _pow_19 524288
#define _pow_20 1048576
#define _pow_21 2097152

//xmm7: NPNP = {1.0, -1.0, 1.0, -1.0}
#define module_radix2(re, im) \
    __asm__ __volatile__ \
    ( \
        "movaps %0, %%xmm0                 \n" \
        "pshufd $0b11110101, %%xmm0, %%xmm1\n" \
        "movaps %1, %%xmm2                 \n" \
        "pshufd $0b10100000, %%xmm0, %%xmm0\n" \
        "mulps  %2, %%xmm1                 \n" \
        "pshufd $0b11110101, %%xmm2, %%xmm3\n" \
        "mulps  %2, %%xmm3                 \n" \
        "pshufd $0b10100000, %%xmm2, %%xmm2\n" \
        "addps  %%xmm1, %%xmm0             \n" \
        "movaps %%xmm0, %0                 \n" \
        "addps  %%xmm3, %%xmm2             \n" \
        "movaps %%xmm2, %1                 \n" \
        : "+m"((re)[0]), "+m"((im)[0])         \
        : "m"(*NPNP) \
        : "%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm7" \
    )

#define module_radix4(re, im) \
    __asm__ __volatile__ \
    ( \
        "movaps %0, %%xmm0\n"                  \
        "movaps %1, %%xmm1\n"                  \
        "pshufd $0b00000000, %%xmm0, %%xmm2\n" \
        "pshufd $0b01010101, %%xmm0, %%xmm4\n" \
        "pshufd $0b00000000, %%xmm1, %%xmm3\n" \
        "mulps  %3, %%xmm4\n"                  \
        "pshufd $0b01010101, %%xmm1, %%xmm5\n" \
        "mulps  %3, %%xmm5\n"                  \
        "addps  %%xmm4, %%xmm2\n"              \
        "addps  %%xmm5, %%xmm3\n"              \
        "punpckhdq %%xmm1, %%xmm0\n"           \
        "pshufd $0b01000100, %%xmm0, %%xmm4\n" \
        "pshufd $0b11101110, %%xmm0, %%xmm1\n" \
        "mulps  %2, %%xmm4\n"                  \
        "pshufd $0b00111001, %%xmm4, %%xmm5\n" \
        "addps  %%xmm4, %%xmm2\n"              \
        "pshufd $0b00010001, %%xmm1, %%xmm4\n" \
        "mulps  %4, %%xmm1\n"                  \
        "addps  %%xmm1, %%xmm2\n"              \
        "mulps  %2, %%xmm4\n"                  \
        "addps  %%xmm5, %%xmm3\n"              \
        "addps  %%xmm4, %%xmm3\n"              \
        "movaps %%xmm2, %0\n"                  \
        "movaps %%xmm3, %1\n"                  \
        : "+m"((re)[0]), "+m"((im)[0])         \
        : "m"(*NNPP), "m"(*NPNP), "m"(*PNNP)   \
        : "%xmm0", "%xmm1", "%xmm2", "%xmm3",  \
          "%xmm4", "%xmm5", "%xmm6", "%xmm7"   \
    )

#define module_split8(re, im) \
    __asm__ __volatile__ \
    ( \
        "movaps %2, %%xmm0                 \n" \
        "movaps %3, %%xmm1                 \n" \
        "pshufd $0b01010101, %%xmm0, %%xmm2\n" \
        "pshufd $0b01010101, %%xmm1, %%xmm3\n" \
        "mulps  %4, %%xmm2                 \n" \
        "mulps  %5, %%xmm3                 \n" \
        "addps  %%xmm3, %%xmm2             \n" \
        "pshufd $0b11111111, %%xmm0, %%xmm3\n" \
        "pshufd $0b11111111, %%xmm1, %%xmm4\n" \
        "mulps  %6, %%xmm3                 \n" \
        "mulps  %7, %%xmm4                 \n" \
        "addps  %%xmm3, %%xmm2             \n" \
        "addps  %%xmm4, %%xmm2             \n" \
        "mulps  %8, %%xmm2                 \n" \
        "shufps $0b10001000, %%xmm1, %%xmm0\n" \
        "pshufd $0b10000010, %%xmm0, %%xmm3\n" \
        "pshufd $0b11010111, %%xmm0, %%xmm1\n" \
        "mulps  %5, %%xmm3                 \n" \
        "mulps  %6, %%xmm1                 \n" \
        "subps  %%xmm1, %%xmm3             \n" \
        "movaps %%xmm3, %%xmm0             \n" \
        "punpckhdq %%xmm2, %%xmm0          \n" \
        "punpckldq %%xmm2, %%xmm3          \n" \
        "movaps %0, %%xmm1                 \n" \
        "movaps %1, %%xmm2                 \n" \
        "movaps %%xmm0, %%xmm4             \n" \
        "movaps %%xmm3, %%xmm5             \n" \
        "addps  %%xmm1, %%xmm4             \n" \
        "addps  %%xmm2, %%xmm5             \n" \
        "movaps %%xmm4, %0                 \n" \
        "movaps %%xmm5, %1                 \n" \
        "subps  %%xmm0, %%xmm1             \n" \
        "subps  %%xmm3, %%xmm2             \n" \
        "movaps %%xmm1, %2                 \n" \
        "movaps %%xmm2, %3                 \n" \
        : "+m"((re)[0]), "+m"((im)[0]), \
          "+m"((re)[4]), "+m"((im)[4])  \
        : "m"(*NPNN), "m"(*PPNP), "m"(*PNNN), "m"(*PPPN), "m"(*VECN) \
        : "%xmm0", "%xmm1", "%xmm2", "%xmm3", \
          "%xmm4", "%xmm5", "%xmm6", "%xmm7"  \
    );

//Pipeline optimized
#define module_splitn_a(re, im, power, _k)\
    __asm__ __volatile__ \
    ( \
        "movaps %0, %%xmm0                 \n" \
        "movaps %1, %%xmm1                 \n" \
        "movaps %%xmm0, %%xmm4             \n" \
        "movaps %%xmm1, %%xmm5             \n" \
        "mulps  %4, %%xmm4                 \n" \
        "mulps  %5, %%xmm5                 \n" \
        "mulps  %4, %%xmm1                 \n" \
        "movaps %2, %%xmm2                 \n" \
        "subps  %%xmm5, %%xmm4             \n" \
        "mulps  %5, %%xmm0                 \n" \
        "movaps %3, %%xmm3                 \n" \
        "addps  %%xmm1, %%xmm0             \n" \
        "movaps %%xmm2, %%xmm1             \n" \
        "mulps  %6, %%xmm1                 \n" \
        "movaps %%xmm3, %%xmm5             \n" \
        "mulps  %7, %%xmm2                 \n" \
        "mulps  %7, %%xmm5                 \n" \
        "subps  %%xmm5, %%xmm1             \n" \
        "mulps  %6, %%xmm3                 \n" \
        "movaps %%xmm0, %%xmm5             \n" \
        "addps  %%xmm3, %%xmm2             \n" \
        "subps  %%xmm2, %%xmm5             \n" \
        "movaps %%xmm1, %%xmm3             \n" \
        "subps  %%xmm4, %%xmm3             \n" \
        "movaps %%xmm5, %2                 \n" \
        "addps  %%xmm1, %%xmm4             \n" \
        "movaps %%xmm3, %3                 \n" \
        "addps  %%xmm2, %%xmm0             \n" \
        "movaps %%xmm4, %0                 \n" \
        "movaps %%xmm0, %1                 \n" \
        : "+m"((re)[intpow(power) / 2 + _k]),     \
          "+m"((im)[intpow(power) / 2 + _k]),     \
          "+m"((re)[intpow(power) / 4 * 3 + _k]), \
          "+m"((im)[intpow(power) / 4 * 3 + _k])  \
        : "m"(w_##power##_re[_k]), "m"(w_##power##_im[_k]),  \
          "m"(w_##power##_3re[_k]), "m"(w_##power##_3im[_k]) \
        : "%xmm0", "%xmm1", "%xmm2", "%xmm3", \
          "%xmm4", "%xmm5", "%xmm6", "%xmm7"  \
    );
/*
#define module_splitn_a(re, im, power, _k)\
    __asm__ __volatile__ \
    ( \
        "movaps %0, %%xmm0                 \n" \
        "movaps %1, %%xmm1                 \n" \
        "movaps %2, %%xmm2                 \n" \
        "movaps %3, %%xmm3                 \n" \
        "movaps %%xmm0, %%xmm4             \n" \
        "mulps  %4, %%xmm4                 \n" \
        "movaps %%xmm1, %%xmm5             \n" \
        "mulps  %5, %%xmm5                 \n" \
        "subps  %%xmm5, %%xmm4             \n" \
        "mulps  %5, %%xmm0                 \n" \
        "mulps  %4, %%xmm1                 \n" \
        "addps  %%xmm1, %%xmm0             \n" \
        "movaps %%xmm2, %%xmm1             \n" \
        "mulps  %6, %%xmm1                 \n" \
        "movaps %%xmm3, %%xmm5             \n" \
        "mulps  %7, %%xmm5                 \n" \
        "subps  %%xmm5, %%xmm1             \n" \
        "mulps  %7, %%xmm2                 \n" \
        "mulps  %6, %%xmm3                 \n" \
        "addps  %%xmm3, %%xmm2             \n" \
        "movaps %%xmm0, %%xmm5             \n" \
        "subps  %%xmm2, %%xmm5             \n" \
        "movaps %%xmm1, %%xmm3             \n" \
        "subps  %%xmm4, %%xmm3             \n" \
        "addps  %%xmm1, %%xmm4             \n" \
        "addps  %%xmm2, %%xmm0             \n" \
        "movaps %%xmm0, %1                 \n" \
        "movaps %%xmm3, %3                 \n" \
        "movaps %%xmm4, %0                 \n" \
        "movaps %%xmm5, %2                 \n" \
        : "+m"((re)[intpow(power) / 2 + _k]),     \
          "+m"((im)[intpow(power) / 2 + _k]),     \
          "+m"((re)[intpow(power) / 4 * 3 + _k]), \
          "+m"((im)[intpow(power) / 4 * 3 + _k])  \
        : "m"(w_##power##_re[_k]), "m"(w_##power##_im[_k]),  \
          "m"(w_##power##_3re[_k]), "m"(w_##power##_3im[_k]) \
        : "%xmm0", "%xmm1", "%xmm2", "%xmm3", \
          "%xmm4", "%xmm5", "%xmm6", "%xmm7"  \
    );
*/

//Pipeline
#define module_splitn_b(re, im, power) \
    __asm__ __volatile__ \
    ( \
        "movaps %0, %%xmm0                 \n" \
        "movaps %2, %%xmm2                 \n" \
        "movaps %1, %%xmm1                 \n" \
        "movaps %3, %%xmm3                 \n" \
        "addps  %%xmm0, %%xmm2             \n" \
        "addps  %%xmm1, %%xmm3             \n" \
        "subps  %2, %%xmm0                 \n" \
        "subps  %3, %%xmm1                 \n" \
        "movaps %%xmm0, %2                 \n" \
        "movaps %%xmm1, %3                 \n" \
        "movaps %%xmm2, %0                 \n" \
        "movaps %%xmm3, %1                 \n" \
        : "+m"((re)[0]),                 "+m"((im)[0]),                \
          "+m"((re)[intpow(power) / 2]), "+m"((im)[intpow(power) / 2]) \
        : \
        : "%xmm0", "%xmm1", "%xmm2", "%xmm3", \
          "%xmm4", "%xmm5", "%xmm6", "%xmm7"  \
    );

/*
#define module_splitn_b(re, im, power) \
    __asm__ __volatile__ \
    ( \
        "movaps %0, %%xmm0                 \n" \
        "movaps %1, %%xmm1                 \n" \
        "movaps %2, %%xmm2                 \n" \
        "movaps %3, %%xmm3                 \n" \
        "addps  %%xmm0, %%xmm2             \n" \
        "addps  %%xmm1, %%xmm3             \n" \
        "subps  %2, %%xmm0                 \n" \
        "subps  %3, %%xmm1                 \n" \
        "movaps %%xmm0, %2                 \n" \
        "movaps %%xmm1, %3                 \n" \
        "movaps %%xmm2, %0                 \n" \
        "movaps %%xmm3, %1                 \n" \
        : "+m"((re)[0]),                 "+m"((im)[0]),                \
          "+m"((re)[intpow(power) / 2]), "+m"((im)[intpow(power) / 2]) \
        : \
        : "%xmm0", "%xmm1", "%xmm2", "%xmm3", \
          "%xmm4", "%xmm5", "%xmm6", "%xmm7"  \
    );
*/

#define module_split16(re, im) \
        module_splitn_a(re, im, 4, 0); \
        module_splitn_b(re, im, 4); \
        module_splitn_b(re + 4, im + 4, 4)

#define module_split32(re, im) \
        module_splitn_a(re, im, 5, 0); \
        module_splitn_a(re, im, 5, 4); \
        module_splitn_b(re, im, 5); \
        module_splitn_b(re + 4, im + 4, 5); \
        module_splitn_b(re + 8, im + 8, 5); \
        module_splitn_b(re + 12, im + 12, 5)

#define module_splitn(re, im, power) \
    for(k = 0; k < intpow(power) / 4; k += 4) \
    { \
        module_splitn_a(re, im, power, k); \
    } \
    for(k = 0; k < intpow(power) / 2; k += 4) \
    { \
        module_splitn_b(re + k, im + k, power); \
    } do {} while(0)

#endif

