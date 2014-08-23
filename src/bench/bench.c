#include "../ee-fft.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <fftw3.h>
#include <ffts/ffts.h>

#define EEFFT 1
#define FFTW  2
#define FFTS  3

#define FFTSIZE 1024
#define FFTPOWR 10
#define FFTLIB FFTS

#define sa16 static __attribute__ ((aligned(16)))
float sa16 dst_re[FFTSIZE];
float sa16 dst_im[FFTSIZE];

float* bufferlist_re[1000];
float* bufferlist_im[1000];

void init_rand()
{
    int i, j;
    printf("Random initializing...\n");
    for(i = 0; i < 1000; i ++)
    {
        bufferlist_re[i] = (float*)memalign(16, FFTSIZE * 4);
        bufferlist_im[i] = (float*)memalign(16, FFTSIZE * 4);
        for(j = 0; j < FFTSIZE; j ++)
        {
            bufferlist_re[i][j] = (float)rand() / RAND_MAX;
            bufferlist_im[i][j] = (float)rand() / RAND_MAX;
        }
    }
    printf("Initialized.\n");
}

fftwf_complex *fftw_in, *fftw_out;
fftwf_plan fftw_p;
ffts_plan_t* p;
float sa16 ffts_in[FFTSIZE * 2];
float sa16 ffts_out[FFTSIZE * 2];

void genericfft_init()
{
#if   FFTLIB == EEFFT
    eefft_init();
#elif FFTLIB == FFTW
    fftw_in  = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * FFTSIZE);
    fftw_out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * FFTSIZE);
    fftw_p = fftwf_plan_dft_1d(FFTSIZE, fftw_in, fftw_out, FFTW_FORWARD,
        FFTW_ESTIMATE);
#elif FFTLIB == FFTS
    p = ffts_init_1d(FFTSIZE, 1);
#endif
}

static void genericfft_fft(float* dre, float* dim, float* sre, float* sim,
    int power, int size)
{
#if   FFTLIB == EEFFT
    eefft_fft(dre, dim, sre, sim, power);
#elif FFTLIB == FFTW
    int i;
    for(i = 0; i < FFTSIZE; i ++)
    {
        fftw_in[i][0] = sre[i];
        fftw_in[i][1] = sim[i];
    }
    fftwf_execute(fftw_p);
#elif FFTLIB == FFTS
    int i;
    for(i = 0; i < FFTSIZE; i ++)
    {
        ffts_in[i * 2    ] = sre[i];
        ffts_in[i * 2 + 1] = sim[i];
    }
    ffts_execute(p, ffts_in, ffts_out);
#endif
}


int main(void)
{
    genericfft_init();
    init_rand();
    
    int i, j;
    
    double time1 = eefft_gettime();
    
    for(i = 0; i < 100; i ++)
    {
        for(j = 0; j < 1000; j ++)
            genericfft_fft(dst_re, dst_im,
                bufferlist_re[j], bufferlist_im[j], FFTPOWR, FFTSIZE);
    }
    
    double time2 = eefft_gettime();
    
#if FFTLIB == FFTW
    for(i = 0; i < FFTSIZE; i ++)
    {
        dst_re[i] = fftw_out[i][0];
        dst_im[i] = fftw_out[i][1];
    }
#elif FFTLIB == FFTS
    for(i = 0; i < FFTSIZE; i ++)
    {
        dst_re[i] = ffts_out[i * 2];
        dst_im[i] = ffts_out[i * 2 + 1];
    }
#endif
    for(i = 0; i < 10; i ++)
        printf("%f\n", log(sqrt(dst_re[i] * dst_re[i]
                              + dst_im[i] * dst_im[i])));
    
    double timecost_ms = (time2 - time1) / 100000.0;
    printf("Total time cost: %f ms.\n", time2 - time1);
    printf("Time cost for each %d-point complex fft: %f ms.\n", FFTSIZE,
        timecost_ms);
    printf("Speed: %f mflops.\n", 5 * FFTSIZE * log2(FFTSIZE) / timecost_ms
        / 1000.0);
    return 0;
}

