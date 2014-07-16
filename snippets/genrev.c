#include <stdio.h>
#include <sys/time.h>
#include <math.h>

#define sa16 static __attribute__ ((aligned(16)))

float sa16 buf_re[1024];
float sa16 buf_im[1024];
float sa16 buf2_re[1024];
float sa16 buf2_im[1024];

int sa16 shuffle_table[1024];

void bitrev(float* dst, float* src, int power)
{
    int N, i, j;
    int tmp = 0x00000000;
    
    N = pow(2, power);
    dst[0] = src[0];
    for(i = 0; i < N - 1; i ++)
    {
        j = power - 1;
        while((tmp & (1 << j)) != 0)
        {
            tmp &= ~ (1 << j);
            j --;
        }
        tmp |= (1 << j);
        dst[i + 1] = src[tmp];
    }
}

void bitrev_genstatic(int power)
{
    int N, i, j;
    int tmp = 0x00000000;
    
    N = pow(2, power);
    printf("void bitrev_%d(float* dst, float* src)\n", power);
    printf("{\n");
    printf("    dst[0] = src[0];");
    for(i = 0; i < N - 1; i ++)
    {
        j = power - 1;
        while((tmp & (1 << j)) != 0)
        {
            tmp &= ~ (1 << j);
            j --;
        }
        tmp |= (1 << j);
        if(i % 3 == 2)
            printf("\n    dst[%d] = src[%d];", i + 1, tmp);
        else
            printf(" dst[%d] = src[%d];", i + 1, tmp);
    }
    printf("\n}\n");
}

void bitrev_genstatic_asm(int power)
{
    int N, i, j;
    int tmp = 0x00000000;
    
    N = pow(2, power);
    printf("void bitrev_%d(float* dst, float* src)\n", power);
    printf("{\n");
    printf("    __asm__ __volatile__\n");
    printf("    (\n");
    printf("        \"movss %%1, %%%%xmm0\\n\"\n");
    printf("        \"movss %%%%xmm0, %%0\\n\"\n");
    
    for(i = 0; i < N - 1; i ++)
    {
        j = power - 1;
        while((tmp & (1 << j)) != 0)
        {
            tmp &= ~ (1 << j);
            j --;
        }
        tmp |= (1 << j);
        
        printf("        \"movss %d%%1, %%%%xmm%d\\n\"\n", tmp * 4, i % 8);
        printf("        \"movss %%%%xmm%d, %d%%0\\n\"\n", i % 8, i * 4 + 4);
    }
    printf("    : \"+m\"(*dst)\n");
    printf("    : \"m\"(*src)\n");
    printf("    : \"%%xmm0\", \"%%xmm1\", \"%%xmm2\", \"%%xmm3\", \"%%xmm4\", "
                 "\"%%xmm5\", \"%%xmm6\", \"%%xmm7\"\n");
    printf("    );\n");
    printf("}\n");
}

void bitrev_gentable(int* dst, int power)
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

void bitrev_10(float* dst, float* src);

double get_time_ms()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec + (t.tv_usec / 1000000.0)) * 1000.0;
}

int main(void)
{
    int i;
    //#define GEN
    
    #ifndef GEN
    bitrev_gentable(shuffle_table, 10);
    
        for(i = 0; i < 1024; i ++)
        {
            buf_re[i] = (i * 113 + 543) % 1024 * 0.009;
            buf_im[i] = (i * 317 + 173) % 1024 * 0.002;
        }
    double t1, t2;
    t1 = get_time_ms();
    for(i = 0; i < 100000; i ++)
    {
        //bitrev_dynamic(buf2_re, buf_re, shuffle_table, 1024);
        //bitrev_dynamic(buf2_im, buf_im, shuffle_table, 1024);
        bitrev_10(buf2_re, buf_re);
        bitrev_10(buf2_im, buf_im);
    }
    t2 = get_time_ms();
    for(i = 0; i < 8; i ++)
        printf("%f ", buf2_re[i]);
    puts("\n");
    for(i = 0; i < 8; i ++)
        printf("%f ", buf2_im[i]);
    puts("\n");
    
    printf("%f\n", t2 - t1);
    #else
    bitrev_genstatic_asm(10);
    #endif
    
    return 0;
}

