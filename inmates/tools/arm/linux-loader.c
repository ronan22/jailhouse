/*
 * Jailhouse ARM support
 *
 * Copyright (C) 2015 Huawei Technologies Duesseldorf GmbH
 * Copyright (c) Siemens AG, 2016
 *
 * Authors:
 *  Dmitry Voytik <dmitry.voytik@huawei.com>
 *  Jan Kiszka <jan.kiszka@siemens.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 */

#include <inmate.h>

#ifdef CONFIG_LINUX_LOADER_DEBUG
# define dbgprint	printk
#else
# define dbgprint(...)
#endif

/*
 * Example memory map:
 *     0x00000000       this binary
 *     0x40000000       Image
 *     0x42000000       dtb
 */

#define CMDLINE_BUFFER_SIZE	256
CMDLINE_BUFFER(CMDLINE_BUFFER_SIZE);

void inmate_main(void)
{
	void register (*entry)(unsigned long, unsigned long, unsigned long);
	unsigned long register dtb;

	dbgprint("\nJailhouse ARM Linux bootloader\n");

	entry = (void *)(unsigned long)cmdline_parse_int("kernel", 0);
	dtb = cmdline_parse_int("dtb", 0);

	if (!entry || !dtb) {
		dbgprint("ERROR: command line parameters kernel and dtb"
			 " are required\n");
		while (1)
			asm volatile ("wfi");
	}

	dbgprint("DTB:        0x%08lx\n", dtb);
	dbgprint("Image:      0x%08lx\n", entry);

	entry(0, -1, dtb);
}
