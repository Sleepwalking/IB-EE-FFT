#ifndef EE_FFT_H
#define EE_FFT_H

/*
    ee-fft
    
    Copyright (c) 2014 Hua Kanru(Sleepwalking)
    
    Fast Fourier Transform implementation on x86/x86_64 with SSE2
    
    As a part of the author's International Bacculaurate Extend Essay
    
    License: MIT
*/

void eefft_init();

void eefft_fft(float* dst_re, float* dst_im, float* src_re, float* src_im,
    int power);
void eefft_ifft(float* dst_re, float* dst_im, float* src_re, float* src_im,
    int power);

double eefft_gettime();

#endif

