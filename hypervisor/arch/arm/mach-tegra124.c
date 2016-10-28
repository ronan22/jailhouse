/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Copyright (c) Siemens AG, 2016
 *
 * Authors:
 *  Jan Kiszka <jan.kiszka@siemens.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 */

#include <jailhouse/pci.h>

/*
 * linux/drivers/pci/host/pci-tegra.c says that the Tegra K1 maps its config
 * space differently than PCIe specifies:
 *
 * [27:24] extended register number
 * [23:16] bus number
 * [15:11] device number
 * [10: 8] function number
 * [ 7: 0] register number
 */

u32 pci_mmconfig_address_to_reg(u64 address)
{
	return ((address >> 16) & 0xf00) | (address & 0xff);
}

u16 pci_mmconfig_address_to_bdf(u64 address)
{
	return address >> 8;
}
