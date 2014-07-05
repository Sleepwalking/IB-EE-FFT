#include <stdio.h>
#include <sys/time.h>

static float __attribute__ ((aligned(16))) PNPN[4] = {1.0, - 1.0, 1.0, - 1.0};
static float __attribute__ ((aligned(16))) buf_real[1024];
static float __attribute__ ((aligned(16))) buf_imag[1024];
static float *real, *imag;


double get_time_ms()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec + (t.tv_usec / 1000000.0)) * 1000.0;
}

int main(void)
{    
    /*
        real = buf_real;
        imag = buf_imag;
    float tmp_re, tmp_im;
    tmp_re = real[0] + real[1];
    tmp_im = imag[0] + imag[1];
    real[1] = real[0] - real[1];
    imag[1] = imag[0] - imag[1];
    real[0] = tmp_re;
    imag[0] = tmp_im;*/
    
    #define radix2_2(R, I) \
        __asm__ __volatile__ \
        ( \
            "movaps %0, %%xmm0                 \n" \
            "pshufd $0b11110101, %%xmm0, %%xmm1\n" \
            "movaps %1, %%xmm2                 \n" \
            "pshufd $0b10100000, %%xmm0, %%xmm0\n" \
            "mulps  %%xmm7, %%xmm1             \n" \
            "pshufd $0b11110101, %%xmm2, %%xmm3\n" \
            "mulps  %%xmm7, %%xmm3             \n" \
            "pshufd $0b10100000, %%xmm2, %%xmm2\n" \
            "addps  %%xmm1, %%xmm0             \n" \
            "movaps %%xmm0, %0                 \n" \
            "addps  %%xmm3, %%xmm2             \n" \
            "movaps %%xmm2, %1                 \n" \
            : "+m"(R), "+m"(I) \
            : \
            : "%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm7" \
        )
    
    int i, j, k;
    real = buf_real;
    
    double t1, t2, t, n;
    t = 0;
    n = 0;
    for(k = 0; k < 25000; k ++)
    {
        for(i = 0; i < 1024; i ++)
        {
            buf_real[i] = i;
            buf_imag[i] = 512 - i;
        }
        
        __asm__ __volatile__
        (
            "movaps %0, %%xmm7\n"
            :
            : "m"(*PNPN)
            : "%xmm7"
        );
        t1 = get_time_ms();
        for(j = 0; j < 20; j ++)
        {
            for(i = 0; i < 1024 - 3; i += 4)
            {
                
                radix2_2(buf_real[i], buf_imag[i]);
                
                /*
                float tmp_re, tmp_im;
                tmp_re = buf_real[i] + buf_real[i+1];
                tmp_im = buf_imag[i] + buf_imag[i+1];
                buf_real[i+1] = buf_real[i] - buf_real[i+1];
                buf_imag[i+1] = buf_imag[i] - buf_imag[i+1];
                buf_real[i] = tmp_re;
                buf_imag[i] = tmp_im;*/
                
            }
        }
        t2 = get_time_ms();
        t += t2 - t1;
        n ++;
    }
    
    real = buf_real;
    imag = buf_imag;
    for(i = 0; i < 1; i += 2)
    printf("%f %f %f %f\n", real[i], real[i+1], imag[i], imag[i+1]);
    
    printf("%f\n", t / n * 100000.0 / 20.0);
    
    return 0;
}

