#include "ee-fft.h"
#include "module.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

#define sa16 static __attribute__ ((aligned(16)))

float sa16 PNNN[4] = {-1.0, -1.0, -1.0,  1.0};
float sa16 NPNP[4] = { 1.0, -1.0,  1.0, -1.0};
float sa16 PNNP[4] = { 1.0, -1.0, -1.0,  1.0};
float sa16 NNPP[4] = { 1.0,  1.0, -1.0, -1.0};
float sa16 NPNN[4] = {-1.0, -1.0,  1.0, -1.0};
float sa16 PPNP[4] = { 1.0, -1.0,  1.0,  1.0};
float sa16 PPPN[4] = {-1.0,  1.0,  1.0,  1.0};
float sa16 VECN[4];

float sa16 buf_re[65536];
float sa16 buf_im[65536];

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
    func(12); func(13); func(14); func(15); func(16)

#define loopcall_11(func) \
    func(4); func(5); func(6); func(7); \
    func(8); func(9); func(10); func(11)

loopcall(make_w);

#include "block.h"
#include "bitrev.h"

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
        loopcall_11(defrev);
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
        loopcall_11(defsprdx);
        default:
        break;
    }
    memcpy(dst_re, buf_re, pow(2, power) * 4);
    memcpy(dst_im, buf_im, pow(2, power) * 4);
}

