/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Test configuration for Jetson TK1 board
 * (NVIDIA Tegra K1 quad-core Cortex-A15, 2G RAM)
 *
 * Copyright (c) Siemens AG, 2015
 *
 * Authors:
 *  Jan Kiszka <jan.kiszka@siemens.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 * NOTE: Add "mem=1920M vmalloc=512M" to the kernel command line.
 */

#include <jailhouse/types.h>
#include <jailhouse/cell-config.h>

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

struct {
	struct jailhouse_system header;
	__u64 cpus[1];
	struct jailhouse_memory mem_regions[15];
	struct jailhouse_irqchip irqchips[2];
	struct jailhouse_pci_device pci_devices[2];
	struct jailhouse_pci_capability pci_caps[3];
} __attribute__((packed)) config = {
	.header = {
		.signature = JAILHOUSE_SYSTEM_SIGNATURE,
		.hypervisor_memory = {
			.phys_start = 0xfc000000,
			.size = 0x4000000 - 0x100000, /* -1MB (PSCI) */
		},
		.debug_console = {
			.phys_start = 0x70006000,
			.size = 0x1000,
			.flags = JAILHOUSE_MEM_IO,
		},
		.platform_info = {
			.pci_mmconfig_base = 0x02000000,
			.pci_mmconfig_end_bus = 255,
			.arm = {
				.gicd_base = 0x50041000,
				.gicc_base = 0x50042000,
				.gich_base = 0x50044000,
				.gicv_base = 0x50046000,
				.maintenance_irq = 25,
			},
		},
		.root_cell = {
			.name = "Jetson-TK1",

			.cpu_set_size = sizeof(config.cpus),
			.num_memory_regions = ARRAY_SIZE(config.mem_regions),
			.num_irqchips = ARRAY_SIZE(config.irqchips),
			.num_pci_devices = ARRAY_SIZE(config.pci_devices),
			.num_pci_caps = ARRAY_SIZE(config.pci_caps),
		},
	},

	.cpus = {
		0xf,
	},

	.mem_regions = {
		/* PCIe host */ {
			.phys_start = 0x01001000,
			.virt_start = 0x01001000,
			.size = 0x3000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* PCIe I/O */ {
			.phys_start = 0x12000000,
			.virt_start = 0x12000000,
			.size = 0x10000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* PCIe MMIO */ {
			.phys_start = 0x13000000,
			.virt_start = 0x13000000,
			.size = 0x33000000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* HACK: Legacy Interrupt Controller */ {
			.phys_start = 0x60004000,
			.virt_start = 0x60004000,
			.size = 0x00001000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* HACK: Clock and Reset Controller */ {
			.phys_start = 0x60006000,
			.virt_start = 0x60006000,
			.size = 0x00001000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* GPIO */ {
			.phys_start = 0x6000d000,
			.virt_start = 0x6000d000,
			.size = 0x00001000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* pinmux */ {
			.phys_start = 0x70000000,
			.virt_start = 0x70000000,
			.size = 0x00001000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* I2C5/6, SPI */ {
			.phys_start = 0x7000d000,
			.virt_start = 0x7000d000,
			.size = 0x00001000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* HACK: Memory Controller */ {
			.phys_start = 0x70019000,
			.virt_start = 0x70019000,
			.size = 0x00001000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* XUSB */ {
			.phys_start = 0x70090000,
			.virt_start = 0x70090000,
			.size = 0x8000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* MMC0/1 */ {
			.phys_start = 0x700b0000,
			.virt_start = 0x700b0000,
			.size = 0x00001000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* RTC + PMC */ {
			.phys_start = 0x7000e000,
			.virt_start = 0x7000e000,
			.size = 0x00001000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* USB */ {
			.phys_start = 0x7d004000,
			.virt_start = 0x7d004000,
			.size = 0x00008000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* UART */ {
			.phys_start = 0x70006000,
			.virt_start = 0x70006000,
			.size = 0x1000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* RAM */ {
			.phys_start = 0x80000000,
			.virt_start = 0x80000000,
			.size = 0x7c000000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE,
		},
	},

	.irqchips = {
		/* GIC */ {
			.address = 0x50041000,
			.pin_base = 32,
			.pin_bitmap = {
				0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
			},
		},
		/* GIC */ {
			.address = 0x50041000,
			.pin_base = 160,
			.pin_bitmap = {
				0xffffffff, 0xffffffff
			},
		},
	},

	.pci_devices = {
		/* 00:02.0 */ {
			.type = JAILHOUSE_PCI_TYPE_BRIDGE,
			.bdf = 0x0010,
			.caps_start = 0,
			.num_caps = 1,
			.num_msi_vectors = 2,
			.msi_64bits = 1,
		},
		/* 01:00.0 */ {
			.type = JAILHOUSE_PCI_TYPE_DEVICE,
			.bdf = 0x0100,
			.bar_mask = {
				0xffffff00, 0x00000000, 0xfffff000, 0xffffffff,
				0xffff0000, 0xffffffff
			},
			.caps_start = 1,
			.num_caps = 2,
			.num_msi_vectors = 1,
			.msi_64bits = 1,
		},
	},

	.pci_caps = {
		/* 00:02.0 */ {
			.id = 0x5,
			.start = 0x50,
			.len = 14,
			.flags = JAILHOUSE_PCICAPS_WRITE,
		},
		/* 01:00.0 */ {
			.id = 0x5,
			.start = 0x50,
			.len = 14,
			.flags = JAILHOUSE_PCICAPS_WRITE,
		},
		{
			.id = 0x8,
			.start = 0x70,
			.len = 0x3c,
			.flags = JAILHOUSE_PCICAPS_WRITE,
		},
	},
};
