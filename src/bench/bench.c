#include "../ee-fft.h"
#include <math.h>
#include <stdio.h>

float a_re[1024];
float a_im[1024];
float b_re[1024];
float b_im[1024];

int main(void)
{
    eefft_init();
    
    int i;
    for(i = 0; i < 1024; i ++)
    {
        a_re[i] = sin(i);
        a_im[i] = 0;
    }
    
    //for(i = 0; i < 50; i ++)
    //    printf("%f\n", sqrt(a_re[i] * a_re[i] + a_im[i] * a_im[i]));
    for(i = 0; i < 100000; i ++)
    eefft_fft(b_re, b_im, a_re, a_im, 10);
    
    for(i = 0; i < 1024; i ++)
        printf("%f\n", log(sqrt(b_re[i] * b_re[i] + b_im[i] * b_im[i])));
    
    return 0;
}

