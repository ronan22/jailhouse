#
# Jailhouse, a Linux-based partitioning hypervisor
#
# Copyright (c) Siemens AG, 2013
#
# Authors:
#  Jan Kiszka <jan.kiszka@siemens.com>
#
# This work is licensed under the terms of the GNU GPL, version 2.  See
# the COPYING file in the top-level directory.
#

LINUXINCLUDE := -I$(src)/../hypervisor/arch/$(SRCARCH)/include \
		-I$(src)/../hypervisor/include
KBUILD_CFLAGS := -Wextra

ifneq ($(wildcard $(obj)/../hypervisor/include/jailhouse/config.h),)
KBUILD_CFLAGS += -include $(obj)/../hypervisor/include/jailhouse/config.h
endif

DTC := ${KDIR}/scripts/dtc/dtc

OBJCOPYFLAGS := -O binary

CONFIGS = $(shell cd $(src); ls *.c)

ifneq ($(ARCH),x86)
DTS = $(shell cd $(src); ls *.dts)
endif

always := $(CONFIGS:.c=.cell) $(DTS:.dts=.dtb)

targets += $(CONFIGS:.c=.o) $(CONFIGS:.c=.cell) $(DTS:.dts=.dtb)

# prevent deleting intermediate files which would cause rebuilds
.SECONDARY: $(addprefix $(obj)/,$(CONFIGS:.c=.o))

$(obj)/%.cell: $(obj)/%.o
	$(call if_changed,objcopy)
