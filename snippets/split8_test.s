	.file	"split8_test.c"
	.text
	.p2align 4,,15
	.globl	split8
	.type	split8, @function
split8:
.LFB38:
	.cfi_startproc
	movss	28(%rdi), %xmm5
	movss	20(%rsi), %xmm7
	movaps	%xmm5, %xmm3
	movaps	%xmm7, %xmm2
	movss	20(%rdi), %xmm6
	movss	28(%rsi), %xmm0
	subss	%xmm6, %xmm2
	addss	%xmm0, %xmm3
	movaps	%xmm0, %xmm1
	movss	24(%rdi), %xmm8
	addss	%xmm7, %xmm1
	movaps	%xmm8, %xmm11
	movss	16(%rdi), %xmm12
	addss	%xmm2, %xmm3
	movss	.LC0(%rip), %xmm4
	subss	%xmm5, %xmm2
	movss	16(%rsi), %xmm9
	addss	%xmm6, %xmm1
	movss	24(%rsi), %xmm13
	addss	%xmm12, %xmm11
	movaps	%xmm9, %xmm10
	mulss	%xmm4, %xmm3
	subss	%xmm12, %xmm8
	subss	%xmm0, %xmm2
	subss	%xmm7, %xmm0
	movss	(%rdi), %xmm7
	subss	%xmm5, %xmm1
	subss	%xmm13, %xmm10
	mulss	%xmm4, %xmm2
	addss	%xmm13, %xmm9
	subss	%xmm6, %xmm0
	movss	4(%rdi), %xmm6
	mulss	%xmm4, %xmm1
	subss	%xmm5, %xmm0
	movss	8(%rdi), %xmm5
	mulss	%xmm4, %xmm0
	movaps	%xmm7, %xmm4
	subss	%xmm11, %xmm4
	addss	%xmm7, %xmm11
	movss	%xmm4, 16(%rdi)
	movaps	%xmm6, %xmm4
	movss	%xmm11, (%rdi)
	subss	%xmm1, %xmm4
	addss	%xmm6, %xmm1
	movss	%xmm4, 20(%rdi)
	movaps	%xmm5, %xmm4
	movss	%xmm1, 4(%rdi)
	subss	%xmm10, %xmm4
	addss	%xmm5, %xmm10
	movss	%xmm4, 24(%rdi)
	movss	12(%rdi), %xmm4
	movaps	%xmm4, %xmm12
	movss	%xmm10, 8(%rdi)
	subss	%xmm3, %xmm12
	addss	%xmm4, %xmm3
	movss	%xmm12, 28(%rdi)
	movss	%xmm3, 12(%rdi)
	movss	(%rsi), %xmm5
	movaps	%xmm5, %xmm1
	movss	4(%rsi), %xmm4
	movss	8(%rsi), %xmm3
	addss	%xmm9, %xmm5
	subss	%xmm9, %xmm1
	movss	%xmm5, (%rsi)
	movss	%xmm1, 16(%rsi)
	movaps	%xmm4, %xmm1
	subss	%xmm2, %xmm1
	addss	%xmm4, %xmm2
	movss	%xmm1, 20(%rsi)
	movaps	%xmm3, %xmm1
	addss	%xmm8, %xmm3
	movss	%xmm2, 4(%rsi)
	subss	%xmm8, %xmm1
	movss	%xmm3, 8(%rsi)
	movss	%xmm1, 24(%rsi)
	movss	12(%rsi), %xmm1
	movaps	%xmm1, %xmm6
	subss	%xmm0, %xmm6
	addss	%xmm1, %xmm0
	movss	%xmm6, 28(%rsi)
	movss	%xmm0, 12(%rsi)
	ret
	.cfi_endproc
.LFE38:
	.size	split8, .-split8
	.p2align 4,,15
	.globl	split8_sse
	.type	split8_sse, @function
split8_sse:
.LFB39:
	.cfi_startproc
#APP
# 121 "split8_test.c" 1
	movaps 16(%rdi), %xmm0
movaps 16(%rsi), %xmm1
pshufd $0b01010101, %xmm0, %xmm2
pshufd $0b01010101, %xmm1, %xmm3
mulps  NPNN, %xmm2
mulps  PPNP, %xmm3
addps  %xmm3, %xmm2
pshufd $0b11111111, %xmm0, %xmm3
pshufd $0b11111111, %xmm1, %xmm4
mulps  PNNN, %xmm3
mulps  PPPN, %xmm4
addps  %xmm3, %xmm2
addps  %xmm4, %xmm2
mulps  VECN, %xmm2
shufps $0b10001000, %xmm1, %xmm0
pshufd $0b10000010, %xmm0, %xmm3
pshufd $0b11010111, %xmm0, %xmm1
mulps  PPNP, %xmm3
mulps  PNNN, %xmm1
subps  %xmm1, %xmm3
movaps %xmm3, %xmm0
punpckhdq %xmm2, %xmm0
punpckldq %xmm2, %xmm3
movaps (%rdi), %xmm1
movaps (%rsi), %xmm2
movaps %xmm0, %xmm4
movaps %xmm3, %xmm5
addps  %xmm1, %xmm4
addps  %xmm2, %xmm5
movaps %xmm4, (%rdi)
movaps %xmm5, (%rsi)
subps  %xmm0, %xmm1
subps  %xmm3, %xmm2
movaps %xmm1, 16(%rdi)
movaps %xmm2, 16(%rsi)

# 0 "" 2
#NO_APP
	ret
	.cfi_endproc
.LFE39:
	.size	split8_sse, .-split8_sse
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC20:
	.string	"%f "
.LC21:
	.string	"\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB40:
	.cfi_startproc
	movaps	.LC13(%rip), %xmm0
	xorl	%eax, %eax
	movl	$0x3f800000, w_3_re(%rip)
	movl	$0x80000000, w_3_im(%rip)
	movl	$0x3f3504f3, w_3_re+4(%rip)
	movl	$0xbf3504f3, w_3_im+4(%rip)
	movl	$0xb33bbd2e, w_3_re+8(%rip)
	movl	$0xbf800000, w_3_im+8(%rip)
	movl	$0xbf3504f3, w_3_re+12(%rip)
	movl	$0xbf3504f3, w_3_im+12(%rip)
	movl	$0xbf800000, w_3_re+16(%rip)
	movl	$0x33bbbd2e, w_3_im+16(%rip)
	movl	$0xbf3504f1, w_3_re+20(%rip)
	movl	$0x3f3504f5, w_3_im+20(%rip)
	movl	$0x324cde2e, w_3_re+24(%rip)
	movl	$0x3f800000, w_3_im+24(%rip)
	movl	$0x3f3504f7, w_3_re+28(%rip)
	movl	$0x3f3504ef, w_3_im+28(%rip)
	movaps	%xmm0, VECN(%rip)
	movl	$0xbf800000, NPNN(%rip)
	movl	$0x3f800000, PPNP(%rip)
	movl	$0xbf800000, PNNN(%rip)
	movl	$0xbf800000, PPPN(%rip)
	movdqa	.LC14(%rip), %xmm10
	movdqa	.LC1(%rip), %xmm1
	movdqa	.LC15(%rip), %xmm9
	movdqa	.LC16(%rip), %xmm6
	movapd	.LC17(%rip), %xmm5
	movdqa	.LC18(%rip), %xmm8
	movapd	.LC19(%rip), %xmm4
	jmp	.L5
	.p2align 4,,10
	.p2align 3
.L16:
	movdqa	%xmm7, %xmm1
.L5:
	movdqa	%xmm1, %xmm2
	addq	$16, %rax
	movdqa	%xmm1, %xmm7
	pslld	$4, %xmm2
	movdqa	%xmm2, %xmm0
	paddd	%xmm10, %xmm7
	pslld	$3, %xmm0
	psubd	%xmm2, %xmm0
	paddd	%xmm1, %xmm0
	paddd	%xmm9, %xmm0
	pand	%xmm6, %xmm0
	cvtdq2pd	%xmm0, %xmm3
	pshufd	$238, %xmm0, %xmm0
	mulpd	%xmm5, %xmm3
	cvtdq2pd	%xmm0, %xmm0
	mulpd	%xmm5, %xmm0
	cvtpd2ps	%xmm3, %xmm3
	cvtpd2ps	%xmm0, %xmm0
	movlhps	%xmm0, %xmm3
	movdqa	%xmm2, %xmm0
	movaps	%xmm3, buf_re-16(%rax)
	pslld	$2, %xmm0
	paddd	%xmm2, %xmm0
	psubd	%xmm1, %xmm0
	pslld	$2, %xmm0
	paddd	%xmm1, %xmm0
	paddd	%xmm8, %xmm0
	pand	%xmm6, %xmm0
	cvtdq2pd	%xmm0, %xmm1
	pshufd	$238, %xmm0, %xmm0
	mulpd	%xmm4, %xmm1
	cvtdq2pd	%xmm0, %xmm0
	mulpd	%xmm4, %xmm0
	cvtpd2ps	%xmm1, %xmm1
	cvtpd2ps	%xmm0, %xmm0
	movlhps	%xmm0, %xmm1
	movaps	%xmm1, buf_im-16(%rax)
	cmpq	$4096, %rax
	jne	.L16
	movss	w_3_im(%rip), %xmm5
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movss	w_3_re(%rip), %xmm4
	movl	$buf_re+32, %r12d
	movaps	%xmm4, %xmm3
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movaps	%xmm5, %xmm0
	movss	buf_re+16(%rip), %xmm2
	movaps	%xmm5, %xmm7
	movss	buf_im+16(%rip), %xmm1
	mulss	%xmm2, %xmm3
	mulss	%xmm1, %xmm0
	movss	buf_im+24(%rip), %xmm6
	mulss	%xmm5, %xmm2
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	$buf_re, %ebx
	mulss	%xmm4, %xmm1
	mulss	%xmm6, %xmm7
	subss	%xmm0, %xmm3
	movss	buf_re+24(%rip), %xmm0
	addss	%xmm2, %xmm1
	movaps	%xmm4, %xmm2
	mulss	%xmm6, %xmm4
	movss	w_3_re+12(%rip), %xmm6
	mulss	%xmm0, %xmm2
	mulss	%xmm5, %xmm0
	subss	%xmm7, %xmm2
	addss	%xmm4, %xmm0
	movaps	%xmm1, %xmm4
	subss	%xmm0, %xmm4
	addss	%xmm1, %xmm0
	movss	buf_im+20(%rip), %xmm1
	movss	%xmm4, buf_re+24(%rip)
	movaps	%xmm2, %xmm4
	addss	%xmm3, %xmm2
	movss	%xmm0, buf_im+16(%rip)
	subss	%xmm3, %xmm4
	movss	w_3_re+4(%rip), %xmm0
	movaps	%xmm0, %xmm3
	movss	%xmm2, buf_re+16(%rip)
	movss	%xmm4, buf_im+24(%rip)
	movss	w_3_im+4(%rip), %xmm4
	movaps	%xmm4, %xmm5
	movss	buf_re+20(%rip), %xmm2
	mulss	%xmm2, %xmm3
	mulss	%xmm1, %xmm5
	mulss	%xmm4, %xmm2
	movss	buf_re+28(%rip), %xmm4
	mulss	%xmm0, %xmm1
	movss	buf_im+28(%rip), %xmm0
	subss	%xmm5, %xmm3
	movss	w_3_im+12(%rip), %xmm5
	movaps	%xmm5, %xmm7
	addss	%xmm2, %xmm1
	movaps	%xmm6, %xmm2
	mulss	%xmm0, %xmm7
	mulss	%xmm4, %xmm2
	mulss	%xmm6, %xmm0
	mulss	%xmm5, %xmm4
	subss	%xmm7, %xmm2
	addss	%xmm4, %xmm0
	movaps	%xmm1, %xmm4
	subss	%xmm0, %xmm4
	addss	%xmm1, %xmm0
	movaps	buf_im(%rip), %xmm1
	movss	%xmm4, buf_re+28(%rip)
	movaps	%xmm2, %xmm4
	addss	%xmm3, %xmm2
	movss	%xmm0, buf_im+20(%rip)
	subss	%xmm3, %xmm4
	movaps	buf_re(%rip), %xmm3
	movss	%xmm2, buf_re+20(%rip)
	movaps	%xmm3, %xmm0
	movss	%xmm4, buf_im+28(%rip)
	movaps	%xmm1, %xmm4
	movaps	buf_re+16(%rip), %xmm2
	addps	%xmm2, %xmm0
	subps	%xmm2, %xmm3
	movaps	%xmm0, buf_re(%rip)
	movaps	%xmm3, buf_re+16(%rip)
	movaps	buf_im+16(%rip), %xmm0
	addps	%xmm0, %xmm4
	subps	%xmm0, %xmm1
	movaps	%xmm4, buf_im(%rip)
	movaps	%xmm1, buf_im+16(%rip)
	.p2align 4,,10
	.p2align 3
.L7:
	movss	(%rbx), %xmm0
	movl	$.LC20, %esi
	movl	$1, %edi
	movl	$1, %eax
	addq	$4, %rbx
	cvtps2pd	%xmm0, %xmm0
	call	__printf_chk
	cmpq	$buf_re+32, %rbx
	jne	.L7
	movl	$.LC21, %edi
	movl	$buf_im+32, %ebp
	movl	$buf_im, %ebx
	call	puts
	.p2align 4,,10
	.p2align 3
.L9:
	movss	(%rbx), %xmm0
	movl	$.LC20, %esi
	movl	$1, %edi
	movl	$1, %eax
	addq	$4, %rbx
	cvtps2pd	%xmm0, %xmm0
	call	__printf_chk
	cmpq	$buf_im+32, %rbx
	jne	.L9
	movl	$.LC21, %edi
	movl	$buf_re, %ebx
	call	puts
	movaps	.LC22(%rip), %xmm0
	movl	$buf_im, %esi
	movl	$buf_re, %edi
	movaps	%xmm0, buf_re(%rip)
	movaps	.LC23(%rip), %xmm0
	movaps	%xmm0, buf_im(%rip)
	movaps	.LC24(%rip), %xmm0
	movaps	%xmm0, buf_re+16(%rip)
	movaps	.LC25(%rip), %xmm0
	movaps	%xmm0, buf_im+16(%rip)
	call	split8_sse
	.p2align 4,,10
	.p2align 3
.L11:
	movss	(%rbx), %xmm0
	movl	$.LC20, %esi
	movl	$1, %edi
	movl	$1, %eax
	addq	$4, %rbx
	cvtps2pd	%xmm0, %xmm0
	call	__printf_chk
	cmpq	%rbx, %r12
	jne	.L11
	movl	$.LC21, %edi
	movl	$buf_im, %ebx
	call	puts
	.p2align 4,,10
	.p2align 3
.L13:
	movss	(%rbx), %xmm0
	movl	$.LC20, %esi
	movl	$1, %edi
	movl	$1, %eax
	addq	$4, %rbx
	cvtps2pd	%xmm0, %xmm0
	call	__printf_chk
	cmpq	%rbx, %rbp
	jne	.L13
	movl	$.LC21, %edi
	call	puts
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%r12
	.cfi_restore 12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE40:
	.size	main, .-main
	.local	w_3_im
	.comm	w_3_im,32,16
	.local	w_3_re
	.comm	w_3_re,32,16
	.local	VECN
	.comm	VECN,16,16
	.data
	.align 16
	.type	PPPN, @object
	.size	PPPN, 16
PPPN:
	.long	3212836864
	.long	1065353216
	.long	1065353216
	.long	1065353216
	.align 16
	.type	PPNP, @object
	.size	PPNP, 16
PPNP:
	.long	1065353216
	.long	3212836864
	.long	1065353216
	.long	1065353216
	.align 16
	.type	PNNN, @object
	.size	PNNN, 16
PNNN:
	.long	3212836864
	.long	3212836864
	.long	3212836864
	.long	1065353216
	.align 16
	.type	NPNN, @object
	.size	NPNN, 16
NPNN:
	.long	3212836864
	.long	3212836864
	.long	1065353216
	.long	3212836864
	.local	buf_im
	.comm	buf_im,4096,16
	.local	buf_re
	.comm	buf_re,4096,16
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	1060439283
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC1:
	.long	0
	.long	1
	.long	2
	.long	3
	.align 16
.LC13:
	.long	1060439283
	.long	1060439283
	.long	1060439283
	.long	1060439283
	.align 16
.LC14:
	.long	4
	.long	4
	.long	4
	.long	4
	.align 16
.LC15:
	.long	543
	.long	543
	.long	543
	.long	543
	.align 16
.LC16:
	.long	1023
	.long	1023
	.long	1023
	.long	1023
	.align 16
.LC17:
	.long	3435973837
	.long	1072483532
	.long	3435973837
	.long	1072483532
	.align 16
.LC18:
	.long	173
	.long	173
	.long	173
	.long	173
	.align 16
.LC19:
	.long	2576980378
	.long	1070176665
	.long	2576980378
	.long	1070176665
	.align 16
.LC22:
	.long	1083179008
	.long	0
	.long	1076677837
	.long	1085066445
	.align 16
.LC23:
	.long	1045220557
	.long	0
	.long	1068708659
	.long	1067030938
	.align 16
.LC24:
	.long	1063675494
	.long	1080452710
	.long	1086953882
	.long	1072064102
	.align 16
.LC25:
	.long	1065353216
	.long	1061997773
	.long	1058642330
	.long	1053609165
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
