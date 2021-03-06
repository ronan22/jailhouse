/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Copyright (c) Siemens AG, 2013
 *
 * Authors:
 *  Jan Kiszka <jan.kiszka@siemens.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 */

#ifndef _JAILHOUSE_ASM_PERCPU_H
#define _JAILHOUSE_ASM_PERCPU_H

#include <jailhouse/types.h>

#define NUM_ENTRY_REGS			13

#ifndef __ASSEMBLY__

#include <jailhouse/cell.h>
#include <asm/irqchip.h>
#include <asm/processor.h>
#include <asm/spinlock.h>
#include <asm/sysregs.h>

/* Round up sizeof(struct per_cpu) to the next power of two. */
#define PERCPU_SIZE_SHIFT \
	(BITS_PER_LONG - __builtin_clzl(sizeof(struct per_cpu) - 1))

struct per_cpu {
	u8 stack[PAGE_SIZE];
	unsigned long linux_sp;
	unsigned long linux_ret;
	unsigned long linux_flags;
	unsigned long linux_reg[NUM_ENTRY_REGS];

	unsigned int cpu_id;
	unsigned int virt_id;

	/* synchronizes parallel insertions of SGIs into the pending ring */
	spinlock_t pending_irqs_lock;
	u16 pending_irqs[MAX_PENDING_IRQS];
	unsigned int pending_irqs_head;
	/* removal from the ring happens lockless, thus tail is volatile */
	volatile unsigned int pending_irqs_tail;
	/* Only GICv3: physical redistributor base */
	void *gicr_base;

	struct cell *cell;

	u32 stats[JAILHOUSE_NUM_CPU_STATS];

	bool initialized;

	/**
	 * Lock protecting CPU state changes done for control tasks.
	 *
	 * The lock protects the following fields (unless CPU is suspended):
	 * @li per_cpu::suspend_cpu
	 * @li per_cpu::cpu_suspended (except for spinning on it to become
	 *                             true)
	 * @li per_cpu::flush_vcpu_caches
	 */
	spinlock_t control_lock;

	/** Set to true for instructing the CPU to suspend. */
	volatile bool suspend_cpu;
	/** True if CPU is waiting for power-on. */
	volatile bool wait_for_poweron;
	/** True if CPU is suspended. */
	volatile bool cpu_suspended;
	/** Set to true for pending reset. */
	bool reset;
	/** Set to true for pending park. */
	bool park;
	/** Set to true for a pending TLB flush for the paging layer that does
	 *  host physical <-> guest physical memory mappings. */
	bool flush_vcpu_caches;

	unsigned long cpu_on_entry;
	unsigned long cpu_on_context;

	int shutdown_state;
	unsigned long mpidr;
	bool failed;
} __attribute__((aligned(PAGE_SIZE)));

static inline struct per_cpu *this_cpu_data(void)
{
	struct per_cpu *cpu_data;

	arm_read_sysreg(TPIDR_EL2, cpu_data);
	return cpu_data;
}

#define DEFINE_PER_CPU_ACCESSOR(field)					\
static inline typeof(((struct per_cpu *)0)->field) this_##field(void)	\
{									\
	return this_cpu_data()->field;					\
}

DEFINE_PER_CPU_ACCESSOR(cpu_id)
DEFINE_PER_CPU_ACCESSOR(cell)

static inline struct per_cpu *per_cpu(unsigned int cpu)
{
	extern u8 __page_pool[];

	return (struct per_cpu *)(__page_pool + (cpu << PERCPU_SIZE_SHIFT));
}

static inline struct registers *guest_regs(struct per_cpu *cpu_data)
{
	/* Assumes that the trap handler is entered with an empty stack */
	return (struct registers *)(cpu_data->stack + sizeof(cpu_data->stack)
			- sizeof(struct registers));
}

static inline unsigned int arm_cpu_phys2virt(unsigned int cpu_id)
{
	return per_cpu(cpu_id)->virt_id;
}

unsigned int arm_cpu_virt2phys(struct cell *cell, unsigned int virt_id);
#endif /* !__ASSEMBLY__ */

#endif /* !_JAILHOUSE_ASM_PERCPU_H */
