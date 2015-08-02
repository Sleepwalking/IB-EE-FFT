#include "ee-fft.h"
#include "module.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

#ifdef EEFFT_TIMMING
#include <sys/time.h>
#endif

#define sa16 static __attribute__ ((aligned(16)))

float sa16 PNNN[4] = {-1.0, -1.0, -1.0,  1.0};
float sa16 NPNP[4] = { 1.0, -1.0,  1.0, -1.0};
float sa16 PNNP[4] = { 1.0, -1.0, -1.0,  1.0};
float sa16 NNPP[4] = { 1.0,  1.0, -1.0, -1.0};
float sa16 NPNN[4] = {-1.0, -1.0,  1.0, -1.0};
float sa16 PPNP[4] = { 1.0, -1.0,  1.0,  1.0};
float sa16 PPPN[4] = {-1.0,  1.0,  1.0,  1.0};
float sa16 VECN[4];

float sa16 buf_re[intpow(21)];
float sa16 buf_im[intpow(21)];

#define make_w(power) \
    float sa16 w_##power##_re[intpow(power) / 4]; \
    float sa16 w_##power##_im[intpow(power) / 4]; \
    float sa16 w_##power##_3re[intpow(power) / 4]; \
    float sa16 w_##power##_3im[intpow(power) / 4]

#define init_w(power) \
    genw(w_##power##_re, w_##power##_im, w_##power##_3re, w_##power##_3im, \
        intpow(power))

#define loopcall(func) \
    func(4); func(5); func(6); func(7); \
    func(8); func(9); func(10); func(11); \
    func(12); func(13); func(14); func(15); \
    func(16); func(17); func(18); func(19); \
    func(20); func(21)

#define loopcall_11(func) \
    func(4); func(5); func(6); func(7); \
    func(8); func(9); func(10); func(11)

#define make_s(power) \
    int sa16 shuffle_table_##power[intpow(power)]

#define init_s(power) \
    genshuffle(shuffle_table_##power, power)

#define bitrev_12(dst, src) \
    bitrev_dynamic(dst, src, shuffle_table_12, intpow(12))
#define bitrev_13(dst, src) \
    bitrev_dynamic(dst, src, shuffle_table_13, intpow(13))
#define bitrev_14(dst, src) \
    bitrev_dynamic(dst, src, shuffle_table_14, intpow(14))
#define bitrev_15(dst, src) \
    bitrev_dynamic(dst, src, shuffle_table_15, intpow(15))
#define bitrev_16(dst, src) \
    bitrev_dynamic(dst, src, shuffle_table_16, intpow(16))
#define bitrev_17(dst, src) \
    bitrev_dynamic(dst, src, shuffle_table_17, intpow(17))
#define bitrev_18(dst, src) \
    bitrev_dynamic(dst, src, shuffle_table_18, intpow(18))
#define bitrev_19(dst, src) \
    bitrev_dynamic(dst, src, shuffle_table_19, intpow(19))
#define bitrev_20(dst, src) \
    bitrev_dynamic(dst, src, shuffle_table_20, intpow(20))
#define bitrev_21(dst, src) \
    bitrev_dynamic(dst, src, shuffle_table_21, intpow(21))

loopcall(make_w);
loopcall(make_s);

#include "block.h"
#include "bitrev.h"

static void genshuffle(int* dst, int power)
{
    int N, i, j;
    int tmp = 0x00000000;
    
    N = pow(2, power);
    dst[0] = 0;
    for(i = 0; i < N - 1; i ++)
    {
        j = power - 1;
        while((tmp & (1 << j)) != 0)
        {
            tmp &= ~ (1 << j);
            j --;
        }
        tmp |= (1 << j);
        dst[i + 1] = tmp;
    }
}

void bitrev_dynamic(float* dst, float* src, int* table, int size)
{
    int i;
    for(i = 0; i < size; i += 8)
    {
        dst[i] = src[table[i]];
        dst[i + 1] = src[table[i + 1]];
        dst[i + 2] = src[table[i + 2]];
        dst[i + 3] = src[table[i + 3]];
        dst[i + 4] = src[table[i + 4]];
        dst[i + 5] = src[table[i + 5]];
        dst[i + 6] = src[table[i + 6]];
        dst[i + 7] = src[table[i + 7]];
    }
}

static void genw(float* re, float* im, float* re3, float* im3, int N)
{
    int k;
    float omega = 2.0 * M_PI / (float)N;
    
    for(k = 0; k < N / 4; k ++)
    {
        re[k]  = + cos(omega * k);
        im[k]  = - sin(omega * k);
        re3[k] = + cos(omega * k * 3.0);
        im3[k] = - sin(omega * k * 3.0);
    }
}

void eefft_init()
{
    int i;
    for(i = 0; i < 4; i ++)
        VECN[i] = sqrt(2.0) / 2.0;
    loopcall(init_w);
    loopcall(init_s);
}

#define defrev(n) \
    case n: \
        bitrev_##n(buf_re, re); \
        bitrev_##n(buf_im, im); \
    break

static void bitrev(float* re, float* im, int power)
{
    switch(power)
    {
        defrev(1);
        defrev(2);
        defrev(3);
        loopcall(defrev);
        default:
        break;
    }
}

#define defsprdx(n) \
    case n: \
        fft_block_##n(buf_re, buf_im); \
    break

void eefft_fft(float* dst_re, float* dst_im, float* src_re, float* src_im,
    int power)
{
    int k, i;
    bitrev(src_re, src_im, power);
    
    switch(power)
    {
        defsprdx(1);
        defsprdx(2);
        defsprdx(3);
        loopcall(defsprdx);
        default:
        break;
    }
    memcpy(dst_re, buf_re, pow(2, power) * 4);
    memcpy(dst_im, buf_im, pow(2, power) * 4);
}

void eefft_ifft(float* dst_re, float* dst_im, float* src_re, float* src_im,
    int power)
{
    int k, i, N;
    float rcp;
    N = pow(2, power);
    rcp = 1.0 / N;
    bitrev(src_re, src_im, power);
    
    for(i = 0; i < N; i ++)
        buf_im[i] *= -1;
    
    switch(power)
    {
        defsprdx(1);
        defsprdx(2);
        defsprdx(3);
        loopcall(defsprdx);
        default:
        break;
    }
    
    for(i = 0; i < N; i ++)
    {
        buf_re[i] *= rcp;
        buf_im[i] *= rcp;
    }
    
    memcpy(dst_re, buf_re, pow(2, power) * 4);
    memcpy(dst_im, buf_im, pow(2, power) * 4);
}

#ifdef EEFFT_TIMMING
double eefft_gettime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec + (t.tv_usec / 1000000.0)) * 1000.0;
}
#endif

