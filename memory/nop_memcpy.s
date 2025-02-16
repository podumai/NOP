.globl nop_memcpy

.equ NULLPTR_CONSTANT, 0
.equ AVX512_STEP,      64
.equ TEMP_VAR,        -8

.section .text
nop_memcpy:
  pushq %rbp
  movq %rsp, %rbp

  cmpq $NULLPTR_CONSTANT, %rdi
  jz .L0
  cmpq $NULLPTR_CONSTANT, %rsi
  jz .L0

  subq $8, %rsp

  movq %rdx, %rcx
  shrq $6, %rcx
  movq %rcx, TEMP_VAR(%rbp)
  shlq $6, TEMP_VAR(%rbp)
  subq TEMP_VAR(%rbp), %rdx
  movq %rsi, TEMP_VAR(%rbp)

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

  movq %rdx, %rcx
  cld
  rep movsb

  movq TEMP_VAR(%rbp), %rax
  addq $8, %rsp
.L0:
  leave
  ret
