/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Copyright (c) ARM Limited, 2014
 * Copyright (c) Siemens AG, 2016
 *
 * Authors:
 *  Jean-Philippe Brucker <jean-philippe.brucker@arm.com>
 *  Jan Kiszka <jan.kiszka@siemens.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 */
	.arm

	.section ".boot", "ax"
	.align 5
vectors:
	b	__reset_entry
	b	vector_undef
	b	vector_svc
	b	vector_pabt
	b	vector_dabt
	b	vector_unused
	b	vector_irq
	b	vector_fiq

.macro vector, name
	.weak vector_\name
	vector_\name:
	b	.
.endm
	vector undef
	vector svc
	vector pabt
	vector dabt
	vector unused
	vector irq
	vector fiq

	.globl __reset_entry
__reset_entry:
	ldr	r0, =vectors
	mcr	p15, 0, r0, c12, c0, 0	@ VBAR

	ldr	r0, =l1tab
	mcr	p15, 0, r0, c2, c0, 0	@ TTBR0

	mov	r0, #0x3500		@ T0SZ=0, IRGN0=ORGN0=1, SH0=3
	movt	r0, #0x8000		@ EAE
	mcr	p15, 0, r0, c2, c0, 2	@ TTBCR

	mrc	p15, 0, r0, c1, c0, 0	@ read SCTLR
	orr	r0, #0x5000		@ use round-robin, enable I cache
	orr	r0, #0x0005		@ enable data+unified caches and MMU
	mcr	p15, 0, r0, c1, c0, 0	@ write SCTLR

	mov	r0, #0
	ldr	r1, =bss_start
	ldr	r2, =bss_dwords
1:	str	r0, [r1]
	add	r1, #4
	subs	r2, #1
	bne	1b

	ldr	sp, =stack_top

	b	inmate_main

	.ltorg

	.section ".rodata"

	.align	12
l1tab:
	.word	l2tab + 0x3, 0	@ valid, table
	.quad	0x40000745	@ valid, EL0, RW, IS, access, device

	.align	12
l2tab:
	.quad	0x741		@ valid, EL0, RW, IS, access, WBRAWA
base=0x200000
.rept	511
	.quad	base + 0x745	@ valid, EL0, RW, IS, access, device
	base=base+0x200000
.endr
