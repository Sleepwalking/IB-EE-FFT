Some points to concern:

* DIT or DIF? - DIT
* In-place or out-of-place? - Out-of-place
* Radix 2 & 4 for base level or up to Radix 8 or even more?
* Unroll to which level?
* Loop to which size?
* Combined or separated complex structure?

Some possible ways to optimize:

* SIMD/SSE
* Combine aligning fetch with bit reverse process.
* Combine unaligning storage with the last split process.
* Rewrite 8 point split radix in assembly.
* Keep switching between two pieces of allocated memory to avoid delay caused by memory occupation.
