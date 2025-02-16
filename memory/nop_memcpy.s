.globl nop_memcpy

.section .text
nop_memcpy:
  pushq %rbp
  movq %rsp, %rbp

  cmpq $0, %rdi
  jz .L0
  cmpq $0, %rsi
  jz .L0

  subq $24, %rsp
  movq %rcx, -8(%rbp)
  movq %rdi, -24(%rbp)

  movq %rdx, %rcx
  shrq $6, %rcx
  movq %rcx, -16(%rbp)
  shlq $6, -16(%rbp)
  subq -16(%rbp), %rdx
  movq %rsi, -16(%rbp)

.L1:
  cmpq $0, %rcx
  jz .L2
  vmovups (%rsi), %zmm0
  vmovups %zmm0, (%rdi)
  addq $64, %rsi
  addq $64, %rdi
  decq %rcx
  jmp .L1
.L2:

  movq %rdx, %rcx
  cld
  rep movsb

  movq -8(%rbp), %rcx
  movq -16(%rbp), %rsi
  movq -24(%rbp), %rdi
  movq %rsi, %rax
  addq $24, %rsp
.L0:
  leave
  ret
