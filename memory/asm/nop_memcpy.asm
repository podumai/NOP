.globl nop_memcpy

.equ NULLPTR_CONSTANT, 0
.equ AVX512_STEP,      64

.section .text
nop_memcpy:
  pushq %rbp
  movq %rsp, %rbp
  movq %rdi, %r8

  cmpq $NULLPTR_CONSTANT, %rdi
  jz .L0
  cmpq $NULLPTR_CONSTANT, %rsi
  jz .L0

  movq %rdx, %rcx
  shrq $6, %rcx
  movq %rcx, %rax
  shlq $6, %rax
  subq %rax, %rdx

.L1:
  cmpq $0, %rcx
  jz .L2
  vmovups (%rsi), %zmm0
  vmovups %zmm0, (%rdi)
  addq $AVX512_STEP, %rsi
  addq $AVX512_STEP, %rdi
  decq %rcx
  jmp .L1
.L2:

.L3:
  cmpq $0, %rdx
  jz .L0
  movb (%rsi), %al
  movb %al, (%rdi)
  incq %rsi
  incq %rdi
  decq %rdx
  jmp .L3

.L0:
  movq %r8, %rax
  leave
  ret
