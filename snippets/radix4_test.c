#include <stdio.h>
#include <sys/time.h>

static float __attribute__ ((aligned(16))) PNNN[4] = {1.0, -1.0, -1.0, -1.0};

static float __attribute__ ((aligned(16))) NPNP[4] = {1.0, -1.0, 1.0, -1.0};
static float __attribute__ ((aligned(16))) PNNP[4] = {1.0, -1.0, -1.0, 1.0};
static float __attribute__ ((aligned(16))) NNPP[4] = {1.0, 1.0, -1.0, -1.0};
static float __attribute__ ((aligned(16))) buf_real[1024];
static float __attribute__ ((aligned(16))) buf_imag[1024];
static float *real, *imag;


double get_time_ms()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec + (t.tv_usec / 1000000.0)) * 1000.0;
}

#define module_radix4(Real, Imag) \
    do { \
    float Re_0, Re_1, Re_2, Re_3, Im_0, Im_1, Im_2, Im_3; \
    Re_0 = *(Real + 0); \
    Re_1 = *(Real + 1); \
    Re_2 = *(Real + 2); \
    Re_3 = *(Real + 3); \
    Im_0 = *(Imag + 0); \
    Im_1 = *(Imag + 1); \
    Im_2 = *(Imag + 2); \
    Im_3 = *(Imag + 3); \
    *(Real + 0) = Re_0 + Re_1 + Re_2 + Re_3; \
    *(Real + 1) = Re_0 - Re_1 + Im_2 - Im_3; \
    *(Real + 2) = Re_0 + Re_1 - Re_2 - Re_3; \
    *(Real + 3) = Re_0 - Re_1 - Im_2 + Im_3; \
    *(Imag + 0) = Im_0 + Im_1 + Im_2 + Im_3; \
    *(Imag + 1) = Im_0 - Im_1 - Re_2 + Re_3; \
    *(Imag + 2) = Im_0 + Im_1 - Im_2 - Im_3; \
    *(Imag + 3) = Im_0 - Im_1 + Re_2 - Re_3; } while(0)

int main(void)
{    
        
    int i, j, k;
    
    double t1, t2, t, n;
    t = 0;
    n = 0;
    NPNP[0] = 1;
    PNNP[0] = 1;
    NNPP[0] = 1;
    for(k = 0; k < 25000; k ++)
    {
        for(i = 0; i < 1024; i ++)
        {
            buf_real[i] = i;
            buf_imag[i] = -i;
        }
        
        __asm__ __volatile__
        (
            "movaps %0, %%xmm7\n"
            "movaps %1, %%xmm6\n"
            :
            : "m"(*NPNP), "m"(*PNNP)
            : "%xmm7", "%xmm6"
        );
        t1 = get_time_ms();
        for(j = 0; j < 20; j ++)
        {
            for(i = 0; i < 1024 - 3; i += 4)
            {
            
                /*
                # R0 R1 R2 R3 I1 I3
                R0 <- +R0 +R1 +R2 +R3
                R1 <- +R0 +I1 -R2 -I3
                R2 <- +R0 -R1 +R2 -R3
                R3 <- +R0 -I1 -R2 +R3

                # I0 I1 I2 I3 R1 R3
                I0 <- +I0 +I1 +I2 +I3
                I1 <- +I0 -R1 -I2 +R3
                I2 <- +I0 -I1 +I2 -I3
                I3 <- +I0 +R1 -I2 -I3
                */
                //#define ASM
                
                #ifndef ASM
                /*
                float R0, R1, R2, R3;
                float I0, I1, I2, I3;
                
                R0 = buf_real[i];
                R1 = buf_real[i+1];
                R2 = buf_real[i+2];
                R3 = buf_real[i+3];
                I0 = buf_imag[i];
                I1 = buf_imag[i+1];
                I2 = buf_imag[i+2];
                I3 = buf_imag[i+3];
                
                buf_real[i  ] = +R0 +R2 +R1 +R3;
                buf_real[i+1] = +R0 -R2 +I1 -I3;
                buf_real[i+2] = +R0 -R1 +R2 -R3;
                buf_real[i+3] = +R0 -R2 +R3 -I1;
                
                buf_imag[i  ] = +I0 +I2 +I1 +I3;
                buf_imag[i+1] = +I0 -I2 +R3 -R1;
                buf_imag[i+2] = +I0 -I1 +I2 -I3;
                buf_imag[i+3] = +I0 -I2 +R1 -I3;*/
                module_radix4(buf_real + i, buf_imag + i);
                
                #else
__asm__ __volatile__
(
//Plan B
/*
"movaps %0, %%xmm0\n"
"pshufd $0b00000000, %%xmm0, %%xmm2\n" //xmm2 = R0 R0 R0 R0
//"movaps %1, %%xmm1\n"
"pshufd $0b10101010, %%xmm0, %%xmm4\n" //xmm4 = R2 R2 R2 R2
"mulps  %%xmm7, %%xmm4\n" //xmm4 = -R2 R2 -R2 R2
"pshufd $0b00000000, %1, %%xmm3\n" //xmm3 = I0 I0 I0 I0
"addps  %%xmm4, %%xmm2\n" //xmm2 = R0-R2 R0+R2 R0-R2 R0+R2
"pshufd $0b10101010, %1, %%xmm5\n" //xmm5 = R2 I2 I2 I2
"mulps  %%xmm7, %%xmm5\n" //xmm5 = -I2 I2 -I2 I2
"movaps %1, %%xmm6\n" //xmm6 = I3 I2 I1 I0
"shufps $0b01110111, %%xmm0, %%xmm6\n" //xmm6 = R1 R3 I1 I3
"addps  %%xmm5, %%xmm3\n" //xmm3 = I0-I2 I0+I2 I0-I2 I0+I2
"pshufd $0b01110111, %%xmm6, %%xmm4\n" //xmm4 = I1 R1 I1 R1
"pshufd $0b11011101, %%xmm6, %%xmm5\n" //xmm5 = R1 I1 R1 I1
"mulps  NNPP, %%xmm4\n" //xmm4 = -I1 -R1 I1 R1
"mulps  PNNP, %%xmm5\n" //xmm5 = R1 -I1 -R1 I1
"addps  %%xmm4, %%xmm2\n" //xmm2 = R0-R2-I1 R0+R2-R1 R0-R2+I1 R0+R2+R1
"pshufd $0b10100010, %%xmm6, %%xmm4\n" //xmm4 = R3 R3 I3 R3
"addps  %%xmm5, %%xmm3\n" //xmm3 = I0-I2+R1 I0+I2-I1 I0-I2-R1 I0+I2+I1
"pshufd $0b00001000, %%xmm6, %%xmm5\n" //xmm5 = I3 I3 R3 I3
"mulps  PNNP, %%xmm4\n" //xmm4 = R3 -R3 -I3 R3
"mulps  NNPP, %%xmm5\n" //xmm5 = -I3 -I3 R3 I3
"addps  %%xmm4, %%xmm2\n" //xmm2 = R0-R2-I1+R3 R0+R2-R1-R3 R0-R2+I1-I3 R0+R2+R1+R3
"addps  %%xmm5, %%xmm3\n" //xmm3 = I0-I2+R1-I3 I0+I2-I1-I3 I0-I2-R1+R3 I0+I2+I1-I3
"movaps %%xmm2, %0\n"
"movaps %%xmm3, %1\n"
*/
//Plan C
"movaps %0, %%xmm0\n"
"pshufd $0b00000000, %%xmm0, %%xmm2\n" //xmm2 = R0 R0 R0 R0
"pshufd $0b10101010, %%xmm0, %%xmm4\n" //xmm4 = R2 R2 R2 R2
"mulps  %%xmm7, %%xmm4\n" //xmm4 = -R2 R2 -R2 R2
"pshufd $0b00000000, %1, %%xmm3\n" //xmm3 = I0 I0 I0 I0
"addps  %%xmm4, %%xmm2\n" //xmm2 = R0-R2 R0+R2 R0-R2 R0+R2
"pshufd $0b10101010, %1, %%xmm5\n" //xmm5 = R2 I2 I2 I2
"mulps  %%xmm7, %%xmm5\n" //xmm5 = -I2 I2 -I2 I2
"movaps %1, %%xmm1\n" //xmm6 = I3 I2 I1 I0
"shufps $0b01110111, %%xmm0, %%xmm1\n" //xmm6 = R1 R3 I1 I3
"addps  %%xmm5, %%xmm3\n" //xmm3 = I0-I2 I0+I2 I0-I2 I0+I2
"pshufd $0b01110111, %%xmm1, %%xmm4\n" //xmm4 = I1 R1 I1 R1
"pshufd $0b11011101, %%xmm1, %%xmm5\n" //xmm5 = R1 I1 R1 I1
"mulps  NNPP, %%xmm4\n" //xmm4 = -I1 -R1 I1 R1
"mulps  %%xmm6, %%xmm5\n" //xmm5 = R1 -I1 -R1 I1
"addps  %%xmm4, %%xmm2\n" //xmm2 = R0-R2-I1 R0+R2-R1 R0-R2+I1 R0+R2+R1
"pshufd $0b10100010, %%xmm1, %%xmm4\n" //xmm4 = R3 R3 I3 R3
"addps  %%xmm5, %%xmm3\n" //xmm3 = I0-I2+R1 I0+I2-I1 I0-I2-R1 I0+I2+I1
"pshufd $0b00001000, %%xmm1, %%xmm5\n" //xmm5 = I3 I3 R3 I3
"mulps  %%xmm6, %%xmm4\n" //xmm4 = R3 -R3 -I3 R3
"mulps  NNPP, %%xmm5\n" //xmm5 = -I3 -I3 R3 I3
"addps  %%xmm4, %%xmm2\n" //xmm2 = R0-R2-I1+R3 R0+R2-R1-R3 R0-R2+I1-I3 R0+R2+R1+R3
"addps  %%xmm5, %%xmm3\n" //xmm3 = I0-I2+R1-I3 I0+I2-I1-I3 I0-I2-R1+R3 I0+I2+I1-I3
"movaps %%xmm2, %0\n"
"movaps %%xmm3, %1\n"
: "+m"(buf_real[i]), "+m"(buf_imag[i])
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
    
    real = buf_real;
    imag = buf_imag;
//    for(i = 0; i < 1; i += 2)
    i = 0;
    printf("%f %f %f %f\n", real[i], real[i+1], real[i+2], real[i+3]);
    printf("%f %f %f %f\n", imag[i], imag[i+1], imag[i+2], imag[i+3]);
    
    printf("%f\n", t / n * 100000.0 / 20.0);
    
    return 0;
}

