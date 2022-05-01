# Copyright (c) 2022, MeizuCustoms enthusiasts
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 and
# only version 2 as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#

# Meizu 16th
ifeq ($(CONFIG_MACH_MEIZU_M1882), y)
# DTBO
dtbo-$(CONFIG_ARCH_SDM845) := \
	sdm845-v2.1-mtp-meizu-m1882-overlay.dtbo
sdm845-v2.1-mtp-meizu-m1882-overlay.dtbo-base := sdm845-v2.1-meizu-m1882.dtb
# DTB
dtb-$(CONFIG_ARCH_SDM845) := \
	sdm845-v2.1-meizu-m1882.dtb
endif

# Meizu 16th Plus
ifeq ($(CONFIG_MACH_MEIZU_M1892), y)
# DTBO
dtbo-$(CONFIG_ARCH_SDM845) := \
	sdm845-v2.1-mtp-meizu-m1892-overlay.dtbo
sdm845-v2.1-mtp-meizu-m1892-overlay.dtbo-base := sdm845-v2.1-meizu-m1892.dtb
# DTB
dtb-$(CONFIG_ARCH_SDM845) := \
	sdm845-v2.1-meizu-m1892.dtb
endif

always		:= $(dtb-y)
subdir-y	:= $(dts-dirs)
clean-files	:= *.dtb
