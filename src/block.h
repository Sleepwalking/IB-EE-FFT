#ifndef EE_FFT_BLOCK_H
#define EE_FFT_BLOCK_H

//This header file should only be included by ee-fft.c

//64 point fft block for optimum cache usage.
/*
static void fft_block_6(float* re, float* im)
{
    int k;
    
    module_radix2(re + 4, im + 4);
    module_radix2(re + 20, im + 20);
    module_radix2(re + 28, im + 28);
    module_radix2(re + 36, im + 36);
    module_radix2(re + 52, im + 52);
    
    module_radix4(re, im);
    module_radix4(re + 8, im + 8);
    module_radix4(re + 12, im + 12);
    module_radix4(re + 16, im + 16);
    module_radix4(re + 24, im + 24);
    module_radix4(re + 32, im + 32);
    module_radix4(re + 40, im + 40);
    module_radix4(re + 44, im + 44);
    module_radix4(re + 48, im + 48);
    module_radix4(re + 56, im + 56);
    module_radix4(re + 60, im + 60);
    
    module_split8(re, im);
    module_split8(re + 16, im + 16);
    module_split8(re + 24, im + 24);
    module_split8(re + 32, im + 32);
    module_split8(re + 48, im + 48);
    
    module_splitn(re, im, 4);
    module_splitn(re + 32, im + 32, 4);
    module_splitn(re + 48, im + 48, 4);
    module_splitn(re, im, 5);
    module_splitn(re, im, 6);
}*/


#define fft_block_1(re, im) \
    module_radix2(re, im)

#define fft_block_2(re, im) \
    module_radix4(re, im)

#define fft_block_3(re, im) \
    fft_block_1(re + intpow(3) / 2, im + intpow(3) / 2); \
    fft_block_2(re, im); \
    module_split8(re, im)

#define fft_block_4(re, im) \
    fft_block_2(re + intpow(4) / 2, im + intpow(4) / 2); \
    fft_block_2(re + intpow(4) / 4 * 3, im + intpow(4) / 4 * 3); \
    fft_block_3(re, im); \
    module_split16(re, im)
//    module_splitn(re, im, 4)
/*
#define fft_block_5(re, im) \
    fft_block_3(re + intpow(5) / 2, im + intpow(5) / 2); \
    fft_block_3(re + intpow(5) / 4 * 3, im + intpow(5) / 4 * 3); \
    fft_block_4(re, im); \
    module_split32(re, im)
//    module_splitn(re, im, 5)


#define fft_block_6(re, im) \
    fft_block_4(re + intpow(6) / 2, im + intpow(6) / 2); \
    fft_block_4(re + intpow(6) / 4 * 3, im + intpow(6) / 4 * 3); \
    fft_block_5(re, im); \
    module_splitn(re, im, 6)
*/
#define fft_block_7(re, im) \
    fft_block_5(re + intpow(7) / 2, im + intpow(7) / 2); \
    fft_block_5(re + intpow(7) / 4 * 3, im + intpow(7) / 4 * 3); \
    fft_block_6(re, im); \
    module_splitn(re, im, 7)
/*
#define fft_block_8(re, im) \
    fft_block_6(re + intpow(8) / 2, im + intpow(8) / 2); \
    fft_block_6(re + intpow(8) / 4 * 3, im + intpow(8) / 4 * 3); \
    fft_block_7(re, im); \
    module_splitn(re, im, 8)
*/
#define fft_block_9(re, im) \
    fft_block_7(re + intpow(9) / 2, im + intpow(9) / 2); \
    fft_block_7(re + intpow(9) / 4 * 3, im + intpow(9) / 4 * 3); \
    fft_block_8(re, im); \
    module_splitn(re, im, 9)

#define fft_block_10(re, im) \
    fft_block_8(re + intpow(10) / 2, im + intpow(10) / 2); \
    fft_block_8(re + intpow(10) / 4 * 3, im + intpow(10) / 4 * 3); \
    fft_block_9(re, im); \
    module_splitn(re, im, 10)

/*
#define fft_block_11(re, im) \
    fft_block_9(re + intpow(11) / 2, im + intpow(11) / 2); \
    fft_block_9(re + intpow(11) / 4 * 3, im + intpow(11) / 4 * 3); \
    fft_block_10(re, im); \
    module_splitn(re, im, 11)
*/

#define fft_block_12(re, im) \
    fft_block_10(re + intpow(12) / 2, im + intpow(12) / 2); \
    fft_block_10(re + intpow(12) / 4 * 3, im + intpow(12) / 4 * 3); \
    fft_block_11(re, im); \
    module_splitn(re, im, 12)

#define fft_block_13(re, im) \
    fft_block_11(re + intpow(13) / 2, im + intpow(13) / 2); \
    fft_block_11(re + intpow(13) / 4 * 3, im + intpow(13) / 4 * 3); \
    fft_block_12(re, im); \
    module_splitn(re, im, 13)

/*
#define fft_block_14(re, im) \
    fft_block_12(re + intpow(14) / 2, im + intpow(14) / 2); \
    fft_block_12(re + intpow(14) / 4 * 3, im + intpow(14) / 4 * 3); \
    fft_block_13(re, im); \
    module_splitn(re, im, 14)
*/

#define fft_block_15(re, im) \
    fft_block_13(re + intpow(15) / 2, im + intpow(15) / 2); \
    fft_block_13(re + intpow(15) / 4 * 3, im + intpow(15) / 4 * 3); \
    fft_block_14(re, im); \
    module_splitn(re, im, 15)

#define fft_block_16(re, im) \
    fft_block_14(re + intpow(16) / 2, im + intpow(16) / 2); \
    fft_block_14(re + intpow(16) / 4 * 3, im + intpow(16) / 4 * 3); \
    fft_block_15(re, im); \
    module_splitn(re, im, 16)

static void fft_block_5(float* re, float* im)
{
    int k;
    fft_block_3(re + intpow(5) / 2, im + intpow(5) / 2);
    fft_block_3(re + intpow(5) / 4 * 3, im + intpow(5) / 4 * 3);
    fft_block_4(re, im);
    module_splitn(re, im, 5);
}

static void fft_block_6(float* re, float* im)
{
    int k;
    fft_block_4(re + intpow(6) / 2, im + intpow(6) / 2);
    fft_block_4(re + intpow(6) / 4 * 3, im + intpow(6) / 4 * 3);
    fft_block_5(re, im);
    module_splitn(re, im, 6);
}

/*
static void fft_block_7(float* re, float* im)
{
    int k;
    fft_block_5(re + intpow(7) / 2, im + intpow(7) / 2);
    fft_block_5(re + intpow(7) / 4 * 3, im + intpow(7) / 4 * 3);
    fft_block_6(re, im);
    module_splitn(re, im, 7);
}*/

static void fft_block_8(float* re, float* im)
{
    int k;
    fft_block_6(re + intpow(8) / 2, im + intpow(8) / 2);
    fft_block_6(re + intpow(8) / 4 * 3, im + intpow(8) / 4 * 3);
    fft_block_7(re, im);
    module_splitn(re, im, 8);
}

static void fft_block_11(float* re, float* im)
{
    int k;
    fft_block_9(re + intpow(11) / 2, im + intpow(11) / 2);
    fft_block_9(re + intpow(11) / 4 * 3, im + intpow(11) / 4 * 3);
    fft_block_10(re, im);
    module_splitn(re, im, 11);
}

static void fft_block_14(float* re, float* im)
{
    int k;
    fft_block_12(re + intpow(14) / 2, im + intpow(14) / 2);
    fft_block_12(re + intpow(14) / 4 * 3, im + intpow(14) / 4 * 3);
    fft_block_13(re, im);
    module_splitn(re, im, 14);
}

static void fft_block_17(float* re, float* im)
{
    int k;
    fft_block_15(re + intpow(17) / 2, im + intpow(17) / 2);
    fft_block_15(re + intpow(17) / 4 * 3, im + intpow(17) / 4 * 3);
    fft_block_16(re, im);
    module_splitn(re, im, 17);
}

static void fft_block_18(float* re, float* im)
{
    int k;
    fft_block_16(re + intpow(18) / 2, im + intpow(18) / 2);
    fft_block_16(re + intpow(18) / 4 * 3, im + intpow(18) / 4 * 3);
    fft_block_17(re, im);
    module_splitn(re, im, 18);
}

static void fft_block_19(float* re, float* im)
{
    int k;
    fft_block_17(re + intpow(19) / 2, im + intpow(19) / 2);
    fft_block_17(re + intpow(19) / 4 * 3, im + intpow(19) / 4 * 3);
    fft_block_18(re, im);
    module_splitn(re, im, 19);
}

static void fft_block_20(float* re, float* im)
{
    int k;
    fft_block_18(re + intpow(20) / 2, im + intpow(20) / 2);
    fft_block_18(re + intpow(20) / 4 * 3, im + intpow(20) / 4 * 3);
    fft_block_19(re, im);
    module_splitn(re, im, 20);
}

static void fft_block_21(float* re, float* im)
{
    int k;
    fft_block_19(re + intpow(21) / 2, im + intpow(21) / 2);
    fft_block_19(re + intpow(21) / 4 * 3, im + intpow(21) / 4 * 3);
    fft_block_20(re, im);
    module_splitn(re, im, 21);
}

#endif

