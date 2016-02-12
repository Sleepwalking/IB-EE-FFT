#include <stdio.h>
#include <math.h>
#include <unistd.h>

/*
NAME
    genrev - generates bit-reverse C codes for fft implementation.

SYNOPSIS
    genrev [-a] [-b] [-n name] fftpower

OPTIONS
    -a
        Enable assembly code generation in gcc asm extension's format.
    
    -b
        Leave one blank line in the end.
    
    -n
        Specify the name of the function. In the generated code, an underline
        and fftpower will be added after this name. e.g. "name_4".
        
        The default name is bitrev.
*/

void bitrev_genstatic_asm(int power, char* name)
{
    int N, i, j;
    int tmp = 0x00000000;
    
    N = pow(2, power);
    printf("static void %s_%d(float* dst, float* src)\n", name, power);
    printf("{\n");
    printf("    __asm__ __volatile__\n");
    printf("    (\n");
    printf("        \"mov %%0, %%%%rsi\\n\"\n");
    printf("        \"mov %%1, %%%%rdi\\n\"\n");
    printf("        \"movss (%%%%rdi), %%%%xmm0\\n\"\n");
    printf("        \"movss %%%%xmm0, (%%%%rsi)\\n\"\n");
    
    for(i = 0; i < N - 1; i ++)
    {
        j = power - 1;
        while((tmp & (1 << j)) != 0)
        {
            tmp &= ~ (1 << j);
            j --;
        }
        tmp |= (1 << j);
        
        printf("        \"movss %d(%%%%rdi), %%%%xmm%d\\n\"\n", tmp * 4, i % 8);
        printf("        \"movss %%%%xmm%d, %d(%%%%rsi)\\n\"\n", i % 8, i * 4 + 4);
    }
    printf("    : \"+m\"(dst)\n");
    printf("    : \"m\"(src)\n");
    printf("    : \"%%xmm0\", \"%%xmm1\", \"%%xmm2\", \"%%xmm3\", \"%%xmm4\", "
                 "\"%%xmm5\", \"%%xmm6\", \"%%xmm7\", \"%%rsi\", \"%%rdi\"\n");
    printf("    );\n");
    printf("}\n");
}

void bitrev_genstatic(int power, char* name)
{
    int N, i, j;
    int tmp = 0x00000000;
    
    N = pow(2, power);
    printf("static void %s_%d(float* dst, float* src)\n", name, power);
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

int main(int argc, char** argv)
{
    int c;
    int aflag = 0;
    int bflag = 0;
    int power = 0;
    char* name = "bitrev";
    
    while((c = getopt(argc, argv, "abn:")) != -1)
    {
        switch(c)
        {
            case 'a':
                aflag = 1;
            break;
            case 'b':
                bflag = 1;
            break;
            case 'n':
                name = optarg;
            break;
            default:
                fprintf(stderr, "Usage: genrev [-a] [-b] fftpower\n");
            return 1;
        }
    }
    
    if(optind < argc - 1)
        fprintf(stderr, "Redundant argument '%s'.\n", argv[optind + 1]);
    
    if(optind >= argc)
    {
        fprintf(stderr, "Missing argument 'fftpower'.\n");
        fprintf(stderr, "Usage: genrev [-a] [-b] fftpower\n");
        return 1;
    }
    
    power = atoi(argv[optind]);
    if(aflag)
        bitrev_genstatic_asm(power, name);
    else
        bitrev_genstatic(power, name);
    
    if(bflag)
        printf("\n");
    
    return 0;
}

