#include <stdio.h>

static void split(float* real, float* imag, int N)
{
    int i;
    int q = N / 4;
    
    for(i = 0; i < q; i ++)
    {
        /*
          x[i + N / 2] *= W_N^{3k};
          x[i + N / 2 + N / 4] *= W_N^k;
          tmp = x[i + N / 2] + x[i + N / 2 + N / 4];
          x[i + N / 2 + N / 4] -= W[i + N / 2];
          x[i + N / 2] = tmp;
        */
    }
}

int main(void)
{
    
    return 0;
}

