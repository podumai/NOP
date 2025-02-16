.globl nop_memset

.equ NULLPTR_CONST, 0
.equ AVX512_STEP,   64

.section .text
nop_memset:
  pushq %rbp
  movq %rsp, %rbp
  movq %rdi, %r8

  cmpq $NULLPTR_CONST, %rdi
  jz .L0

  movq %rdx, %rcx
  shrq $6, %rcx
  movq %rcx, %rax
  shlq $6, %rax
  subq %rax, %rdx

  subq $2, %rsp
  movw %si, -2(%rbp)
  vpbroadcastb -2(%rbp), %zmm0
  addq $2, %rsp

.L1:
  cmpq $0, %rcx
  jz .L2
  vmovups %zmm0, (%rdi)
  addq $AVX512_STEP, %rdi
  decq %rcx
  jmp .L1
.L2:

  movq %rsi, %rax

.L3:
  cmpq $0, %rdx
  jz .L0
  movb %al, (%rdi)
  incq %rdi
  decq %rdx
  jmp .L3

.L0:
  movq %r8, %rax
  leave
  ret
