#include <stdio.h>
#include <math.h>
#include <sys/time.h>


#define sa16 static __attribute__ ((aligned(16)))
#define make_w(power, N) \
    float sa16 w_##power##_re[N]; \
    float sa16 w_##power##_im[N]
#define init_w(power, N) \
    genw(w_##power##_re, w_##power##_im, N)

float sa16 buf_re[1024];
float sa16 buf_im[1024];

float sa16 NPNN[4] = {-1.0, -1.0, 1.0, -1.0};
float sa16 NPNP[4] = {1.0, -1.0, 1.0, -1.0};
float sa16 PNNN[4] = {-1.0, -1.0, -1.0, 1.0};
float sa16 PNNP[4] = {1.0, -1.0, -1.0, 1.0};
float sa16 PPNP[4] = {1.0, -1.0, 1.0, 1.0};
float sa16 PPPN[4] = {-1.0, 1.0, 1.0, 1.0};
float sa16 NNPP[4] = {1.0, 1.0, -1.0, -1.0};
float sa16 VECN[4];

make_w(3, 8);

static void genw(float* re, float* im, int N)
{
    int k;
    float omega = 2.0 * M_PI / (float)N;
    for(k = 0; k < N; k ++)
    {
        //e^{-j\frac{2\pi}{N}k} = \cos(\frac{2\pi}{N}k) - j\sin(\frac{2\pi}{N})
        re[k] = + cos(omega * k);
        im[k] = - sin(omega * k);
    }
}

static void split(float* re, float* im, float* w_re, float* w_im, int N)
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
        tmp_re   = x3_re[k] * w_re[3 * k] - x3_im[k] * w_im[3 * k];
        x3_im[k] = x3_re[k] * w_im[3 * k] + x3_im[k] * w_re[3 * k];
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


static void split8_sse(float* re, float* im)
{
    __asm__ __volatile__
    (
    "movaps %2, %%xmm0\n" //xmm0: R7 R6 R5 R4
    "movaps %3, %%xmm1\n" //xmm1: I7 I6 I5 I4
    "pshufd $0b01010101, %%xmm0, %%xmm2\n" //xmm2: R5 R5 R5 R5
    "pshufd $0b01010101, %%xmm1, %%xmm3\n" //xmm3: I5 I5 I5 I5
    "mulps  NPNN, %%xmm2\n" //xmm2: -R5 +R5 -R5 -R5
    "mulps  PPNP, %%xmm3\n" //xmm3: +I5 +I5 -I5 +I5
    "addps  %%xmm3, %%xmm2\n" //xmm2: -R5+I5 +R5+I5 -R5-I5 -R5+I5
    "pshufd $0b11111111, %%xmm0, %%xmm3\n" //xmm3: R7 R7 R7 R7
    "pshufd $0b11111111, %%xmm1, %%xmm4\n" //xmm4: I7 I7 I7 I7
    "mulps  PNNN, %%xmm3\n" //xmm3: +R7 -R7 -R7 -R7
    "mulps  PPPN, %%xmm4\n" //xmm4: +I7 +I7 +I7 -I7
    "addps  %%xmm3, %%xmm2\n" //xmm2: -R5+I5+R7 +R5+I5-R7 -R5-I5-R7 -R5+I5-R7
    "addps  %%xmm4, %%xmm2\n" //xmm2: -R5+I5+R7+I7 +R5+I5-R7+I7 -R5-I5-R7+I7 -R5+I5-R7-I7
    "mulps  VECN, %%xmm2\n" //xmm2: R3' R1' I3' I1'
    "shufps $0b10001000, %%xmm1, %%xmm0\n" //xmm0: I6 I4 R6 R4
    "pshufd $0b10000010, %%xmm0, %%xmm3\n" //xmm3: I4 R4 R4 I4
    "pshufd $0b11010111, %%xmm0, %%xmm1\n" //xmm1: I6 R6 R6 I6
    "mulps  PPNP, %%xmm3\n" //xmm3: +I4 +R4 -R4 +I4
    "mulps  PNNN, %%xmm1\n" //xmm1: +I6 -R6 -R6 -I6
    "subps  %%xmm1, %%xmm3\n" //xmm3: R2' R0' I2' I0'
    "movaps %%xmm3, %%xmm0\n" //xmm0: R2' R0' I2' I0'
    "punpckhdq %%xmm2, %%xmm0\n" //xmm0: R3' R2' R1' R0'
    "punpckldq %%xmm2, %%xmm3\n" //xmm3: I3' I2' I1' I0'
    "movaps %0, %%xmm1\n" //xmm1: R3 R2 R1 R0
    "movaps %1, %%xmm2\n" //xmm2: I3 I2 I1 I0
    "movaps %%xmm0, %%xmm4\n"
    "movaps %%xmm3, %%xmm5\n"
    "addps  %%xmm1, %%xmm4\n"
    "addps  %%xmm2, %%xmm5\n"
    "movaps %%xmm4, %0\n"
    "movaps %%xmm5, %1\n"
    "subps  %%xmm0, %%xmm1\n"
    "subps  %%xmm3, %%xmm2\n"
    "movaps %%xmm1, %2\n"
    "movaps %%xmm2, %3\n"
    : "+m"(re[0]), "+m"(im[0]), "+m"(re[4]), "+m"(im[4])
    : 
    : "%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6", "%xmm7"
    );
}


double get_time_ms()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec + (t.tv_usec / 1000000.0)) * 1000.0;
}

int main(void)
{
    init_w(3, 8);
    int i, j, k;
    
    for(i = 0; i < 4; i ++)
        VECN[i] = sqrt(2.0) / 2.0;
    NPNN[0] = -1.0;
    PPNP[0] =  1.0;
    PNNN[0] = -1.0;
    PPPN[0] = -1.0;
    PNNN[0] = -1.0;
    
    //split(buf_re, buf_im, w_3_re, w_3_im, 8);
    
    
    double t1, t2, t, n;
    float N = VECN[0];
    t = 0;
    n = 0;
    NPNP[0] = 1;
    PNNP[0] = 1;
    NNPP[0] = 1;
    for(k = 0; k < 25000; k ++)
    {
        for(i = 0; i < 1024; i ++)
        {
            buf_re[i] = (i * 113 + 543) % 1024 * 0.009;
            buf_im[i] = (i * 317 + 173) % 1024 * 0.002;
        }
        
        
        float v_re[4];
        float v_im[4];
        t1 = get_time_ms();
        for(j = 0; j < 20; j ++)
        {
            for(i = 0; i < 1024 - 7; i += 8)
            {
#define ASM
#ifndef ASM
                float* re = buf_re + i;
                float* im = buf_im + i;
                v_re[0] = re[4] + re[6];
                v_re[1] = N * (re[5] + im[5] - re[7] + im[7]);
                v_re[2] = + im[4] - im[6];
                v_re[3] = N * (- re[5] + im[5] + re[7] + im[7]);

                v_im[0] = im[4] + im[6];
                v_im[1] = N * (im[5] - re[5] - re[7] - im[7]);
                v_im[2] = - re[4] + re[6];
                v_im[3] = N * (- re[5] - im[5] + im[7] - re[7]);

                re[4] = re[0] - v_re[0];
                re[5] = re[1] - v_re[1];
                re[6] = re[2] - v_re[2];
                re[7] = re[3] - v_re[3];

                re[0] += v_re[0];
                re[1] += v_re[1];
                re[2] += v_re[2];
                re[3] += v_re[3];

                im[4] = im[0] - v_im[0];
                im[5] = im[1] - v_im[1];
                im[6] = im[2] - v_im[2];
                im[7] = im[3] - v_im[3];

                im[0] += v_im[0];
                im[1] += v_im[1];
                im[2] += v_im[2];
                im[3] += v_im[3];
#else
__asm__ __volatile__
(
    "movaps %2, %%xmm0\n" //xmm0: R7 R6 R5 R4
    "movaps %3, %%xmm1\n" //xmm1: I7 I6 I5 I4
    "pshufd $0b01010101, %%xmm0, %%xmm2\n" //xmm2: R5 R5 R5 R5
    "pshufd $0b01010101, %%xmm1, %%xmm3\n" //xmm3: I5 I5 I5 I5
    "mulps  NPNN, %%xmm2\n" //xmm2: -R5 +R5 -R5 -R5
    "mulps  PPNP, %%xmm3\n" //xmm3: +I5 +I5 -I5 +I5
    "addps  %%xmm3, %%xmm2\n" //xmm2: -R5+I5 +R5+I5 -R5-I5 -R5+I5
    "pshufd $0b11111111, %%xmm0, %%xmm3\n" //xmm3: R7 R7 R7 R7
    "pshufd $0b11111111, %%xmm1, %%xmm4\n" //xmm4: I7 I7 I7 I7
    "mulps  PNNN, %%xmm3\n" //xmm3: +R7 -R7 -R7 -R7
    "mulps  PPPN, %%xmm4\n" //xmm4: +I7 +I7 +I7 -I7
    "addps  %%xmm3, %%xmm2\n" //xmm2: -R5+I5+R7 +R5+I5-R7 -R5-I5-R7 -R5+I5-R7
    "addps  %%xmm4, %%xmm2\n" //xmm2: -R5+I5+R7+I7 +R5+I5-R7+I7 -R5-I5-R7+I7 -R5+I5-R7-I7
    "mulps  VECN, %%xmm2\n" //xmm2: R3' R1' I3' I1'
    "shufps $0b10001000, %%xmm1, %%xmm0\n" //xmm0: I6 I4 R6 R4
    "pshufd $0b10000010, %%xmm0, %%xmm3\n" //xmm3: I4 R4 R4 I4
    "pshufd $0b11010111, %%xmm0, %%xmm1\n" //xmm1: I6 R6 R6 I6
    "mulps  PPNP, %%xmm3\n" //xmm3: +I4 +R4 -R4 +I4
    "mulps  PNNN, %%xmm1\n" //xmm1: +I6 -R6 -R6 -I6
    "subps  %%xmm1, %%xmm3\n" //xmm3: R2' R0' I2' I0'
    "movaps %%xmm3, %%xmm0\n" //xmm0: R2' R0' I2' I0'
    "punpckhdq %%xmm2, %%xmm0\n" //xmm0: R3' R2' R1' R0'
    "punpckldq %%xmm2, %%xmm3\n" //xmm3: I3' I2' I1' I0'
    "movaps %0, %%xmm1\n" //xmm1: R3 R2 R1 R0
    "movaps %1, %%xmm2\n" //xmm2: I3 I2 I1 I0
    "movaps %%xmm0, %%xmm4\n"
    "movaps %%xmm3, %%xmm5\n"
    "addps  %%xmm1, %%xmm4\n"
    "addps  %%xmm2, %%xmm5\n"
    "movaps %%xmm4, %0\n"
    "movaps %%xmm5, %1\n"
    "subps  %%xmm0, %%xmm1\n"
    "subps  %%xmm3, %%xmm2\n"
    "movaps %%xmm1, %2\n"
    "movaps %%xmm2, %3\n"
    : "+m"(buf_re[i]), "+m"(buf_im[i]), "+m"(buf_re[i + 4]), "+m"(buf_im[i + 4])
    : 
    : "%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6", "%xmm7"
);
#endif
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

