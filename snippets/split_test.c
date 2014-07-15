#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#define sa16 static __attribute__ ((aligned(16)))
#define make_w(power, N) \
    float sa16 w_##power##_re[N]; \
    float sa16 w_##power##_im[N]; \
    float sa16 w_##power##_3re[N]; \
    float sa16 w_##power##_3im[N]
#define init_w(power, N) \
    genw(w_##power##_re, w_##power##_im, w_##power##_3re, w_##power##_3im, N)

float sa16 buf_re[1024];
float sa16 buf_im[1024];

make_w(3, 8);
make_w(4, 16);
make_w(5, 32);
make_w(6, 64);
make_w(7, 128);
make_w(8, 256);
make_w(9, 512);

double get_time_ms()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec + (t.tv_usec / 1000000.0)) * 1000.0;
}

static void genw(float* re, float* im, float* re3, float* im3, int N)
{
    int k;
    float omega = 2.0 * M_PI / (float)N;
    
    for(k = 0; k < N / 4; k ++)
    {
        //e^{-j\frac{2\pi}{N}k} = \cos(\frac{2\pi}{N}k) - j\sin(\frac{2\pi}{N})
        re[k] = + cos(omega * k);
        im[k] = - sin(omega * k);
        re3[k] = + cos(omega * k * 3);
        im3[k] = - sin(omega * k * 3);
    }
}

static void split(float* re, float* im, float* w_re, float* w_im,
    float* w_3re, float* w_3im, int N)
{
    int k;
    float* x2_re, *x2_im, *x3_re, *x3_im;
    float tmp_re, tmp_im;
    
    x2_re = re + N / 2;
    x2_im = im + N / 2;
    x3_re = re + 3 * N / 4;
    x3_im = im + 3 * N / 4;
    
    for(k = 0; k < N / 4; k ++)
    {
        //X[k + N/2] <== X[k + N/2] * W_N^k
        tmp_re   = x2_re[k] * w_re[k] - x2_im[k] * w_im[k];
        x2_im[k] = x2_re[k] * w_im[k] + x2_im[k] * w_re[k];
        x2_re[k] = tmp_re;
        
        //X[k + 3N/4] <== X[k + 3N/4] * W_N^{3k}
        tmp_re   = x3_re[k] * w_3re[k] - x3_im[k] * w_3im[k];
        x3_im[k] = x3_re[k] * w_3im[k] + x3_im[k] * w_3re[k];
        x3_re[k] = tmp_re;
        
        //(no dependence)
        //X[k + 3N/4] <== j(X[k + 3N/4] - X[k + N/2])
        //X[k + N/2] <== X[k + N/2] + X[k + 3N/4]
        tmp_re = x3_re[k];
        tmp_im = x3_im[k];
        x3_re[k] = - tmp_im + x2_im[k];
        x3_im[k] = + tmp_re - x2_re[k];
        x2_re[k] += tmp_re;
        x2_im[k] += tmp_im;
    }
    for(k = 0; k < N / 2; k ++)
    {
        tmp_re = re[k];
        tmp_im = im[k];
        re[k] += x2_re[k];
        im[k] += x2_im[k];
        x2_re[k] = tmp_re - x2_re[k];
        x2_im[k] = tmp_im - x2_im[k];
    }
}

int main(void)
{
    init_w(8, 256);
    int i, j, k, l;
    
    double t1, t2, t, n;
    t = 0;
    n = 0;
    for(l = 0; l < 25000; l ++)
    {
        for(i = 0; i < 1024; i ++)
        {
            buf_re[i] = (i * 113 + 543) % 1024 * 0.009;
            buf_im[i] = (i * 317 + 173) % 1024 * 0.002;
        }
        
        
        float v_re[4];
        float v_im[4];
        t1 = get_time_ms();
        int N = 256;
        
        float* w_re = w_8_re;
        float* w_im = w_8_im;
        float* w_3re = w_8_3re;
        float* w_3im = w_8_3im;
        for(j = 0; j < 20; j ++)
        {
            for(i = 0; i < 1024 - 255; i += 256)
            {
#define ASM

    
#ifndef ASM
    float* x2_re, *x2_im, *x3_re, *x3_im, *re, *im;
    float tmp_re, tmp_im;
    
    re = buf_re + i;
    im = buf_im + i;
    x2_re = buf_re + N / 2 + i;
    x2_im = buf_im + N / 2 + i;
    x3_re = buf_re + 3 * N / 4 + i;
    x3_im = buf_im + 3 * N / 4 + i;
    
    for(k = 0; k < N / 4; k ++)
#else
    for(k = 0; k < N / 4 - 3; k += 4)
#endif
    {
#ifndef ASM
    
        //X[k + N/2] <== X[k + N/2] * W_N^k
        tmp_re   = x2_re[k] * w_re[k] - x2_im[k] * w_im[k];
        x2_im[k] = x2_re[k] * w_im[k] + x2_im[k] * w_re[k];
        x2_re[k] = tmp_re;
        
        //X[k + 3N/4] <== X[k + 3N/4] * W_N^{3k}
        tmp_re   = x3_re[k] * w_3re[k] - x3_im[k] * w_3im[k];
        x3_im[k] = x3_re[k] * w_3im[k] + x3_im[k] * w_3re[k];
        x3_re[k] = tmp_re;
        
        //(no dependence)
        //X[k + 3N/4] <== j(X[k + 3N/4] - X[k + N/2])
        //X[k + N/2] <== X[k + N/2] + X[k + 3N/4]
        tmp_re = x3_re[k];
        tmp_im = x3_im[k];
        x3_re[k] = - tmp_im + x2_im[k];
        x3_im[k] = + tmp_re - x2_re[k];
        x2_re[k] += tmp_re;
        x2_im[k] += tmp_im;
#else

__asm__ __volatile__
(
//%0: x2_re[k] | %1: x2_im[k] | %2: x3_re[k] | %3: x3_im[k]
//%4:  w_re[k] | %5:  w_im[k] | %6: w_3re[k] | %7: w_3im[k]
"movaps %0, %%xmm0\n"     //xmm0: x2_re
"movaps %1, %%xmm1\n"     //xmm1: x2_im
"movaps %2, %%xmm2\n"     //xmm2: x3_re
"movaps %3, %%xmm3\n"     //xmm3: x3_im
"movaps %%xmm0, %%xmm4\n" //xmm4: x2_re
"mulps  %4, %%xmm4\n"     //xmm4: x2_re .* w_re
"movaps %%xmm1, %%xmm5\n" //xmm5: x2_im
"mulps  %5, %%xmm5\n"     //xmm5: x2_im .* w_im
"subps  %%xmm5, %%xmm4\n" //xmm4: x2_re'
"mulps  %5, %%xmm0\n"     //xmm0: x2_re .* w_im
"mulps  %4, %%xmm1\n"     //xmm1: x2_im .* w_re
"addps  %%xmm1, %%xmm0\n" //xmm0: x2_im'
"movaps %%xmm2, %%xmm1\n" //xmm1: x3_re
"mulps  %6, %%xmm1\n"     //xmm1: x3_re .* w_3re
"movaps %%xmm3, %%xmm5\n" //xmm5: x3_im
"mulps  %7, %%xmm5\n"     //xmm5: x3_im .* w_3im
"subps  %%xmm5, %%xmm1\n" //xmm1: x3_re'
"mulps  %7, %%xmm2\n"     //xmm2: x3_re .* w_3im
"mulps  %6, %%xmm3\n"     //xmm3: x3_im .* w_3re
"addps  %%xmm3, %%xmm2\n" //xmm2: x3_im'
"movaps %%xmm0, %%xmm5\n" //xmm5: x2_im'
"subps  %%xmm2, %%xmm5\n" //xmm5: x3_re''
"movaps %%xmm1, %%xmm3\n" //xmm3: x3_re'
"subps  %%xmm4, %%xmm3\n" //xmm3: x3_im''
"addps  %%xmm1, %%xmm4\n" //xmm4: x2_re''
"addps  %%xmm2, %%xmm0\n" //xmm0: x2_im''
"movaps %%xmm0, %1\n"
"movaps %%xmm3, %3\n"
"movaps %%xmm4, %0\n"
"movaps %%xmm5, %2\n"
    : "+m"(buf_re[i + k + N / 2]), "+m"(buf_im[i + k + N / 2]),
      "+m"(buf_re[i + k + 3 * N / 4]), "+m"(buf_im[i + k + 3 * N / 4])
    : "m"(w_8_re[k]), "m"(w_8_im[k]), "m"(w_8_3re[k]), "m"(w_8_3im[k])
    : "%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6", "%xmm7"
);
#endif
    }
#ifndef ASM
    for(k = 0; k < N / 2; k ++)
#else
    for(k = 0; k < N / 2 - 3; k += 4)
#endif
    {
#ifndef ASM
        tmp_re = re[k];
        tmp_im = im[k];
        re[k] += x2_re[k];
        im[k] += x2_im[k];
        x2_re[k] = tmp_re - x2_re[k];
        x2_im[k] = tmp_im - x2_im[k];
#else
__asm__ __volatile__
(
//%0: re[k] | %1: im[k] | %2: x2_re[k] | %3: x2_re[k]
"movaps %0, %%xmm0\n"     //xmm0: re
"movaps %1, %%xmm1\n"     //xmm1: im
"movaps %2, %%xmm2\n"     //xmm2: x2_re
"movaps %3, %%xmm3\n"     //xmm3: x2_im
"addps  %%xmm0, %%xmm2\n" //xmm2: x2_re + re = re'
"addps  %%xmm1, %%xmm3\n" //xmm3: x2_im + im = im'
"subps  %2, %%xmm0\n"     //xmm0: re - x2_re = x2_re'
"subps  %3, %%xmm1\n"     //xmm1: im - x2_im = x2_im'
"movaps %%xmm0, %2\n"
"movaps %%xmm1, %3\n"
"movaps %%xmm2, %0\n"
"movaps %%xmm3, %1\n"
    : "+m"(buf_re[i + k]), "+m"(buf_im[i + k]),
      "+m"(buf_re[i + k + N / 2]), "+m"(buf_im[i + k + N / 2])
    :
    : "%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6", "%xmm7"
);
#endif
    }
    
            }
        }
        t2 = get_time_ms();
        t += t2 - t1;
        n ++;
    }
    for(i = 0; i < 8; i ++)
        printf("%f ", buf_re[i]);
    puts("\n");
    for(i = 0; i < 8; i ++)
        printf("%f ", buf_im[i]);
    puts("\n");
    
    printf("%f\n", t / n * 100000.0 / 20.0);
    return 0;
}

