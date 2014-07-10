#include <stdio.h>
#include <math.h>

#define sa16 static __attribute__ ((aligned(16)))
#define make_w(power, N) \
    float sa16 w_##power##_re[N]; \
    float sa16 w_##power##_im[N]
#define init_w(power, N) \
    genw(w_##power##_re, w_##power##_im, N)

float sa16 buf_re[1024];
float sa16 buf_im[1024];

make_w(3, 8);
make_w(4, 16);
make_w(5, 32);
make_w(6, 64);

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

int main(void)
{
    init_w(3, 8);
    int i;
    for(i = 0; i < 8; i ++)
    {
        buf_re[i] = 1;
        buf_im[i] = 1;
    }
    split(buf_re, buf_im, w_3_re, w_3_im, 8);
    for(i = 0; i < 8; i ++)
        printf("%f ", buf_re[i]);
    puts("\n");
    for(i = 0; i < 8; i ++)
        printf("%f ", buf_im[i]);
    puts("\n");
    return 0;
}

