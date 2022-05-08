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
current-meizu-device := m1882
endif

# Meizu 16th Plus
ifeq ($(CONFIG_MACH_MEIZU_M1892), y)
current-meizu-device := m1892
endif

# DTB
dtb-$(CONFIG_ARCH_SDM845) := \
	sdm845-v2.1-meizu-$(current-meizu-device).dtb

# Placeholder DTBO
dtbo-$(CONFIG_ARCH_SDM845) := \
	placeholder-dtbo-$(current-meizu-device).dtbo
placeholder-dtbo-$(current-meizu-device).dtbo-base := sdm845-v2.1-meizu-$(current-meizu-device).dtb

always		:= $(dtb-y)
subdir-y	:= $(dts-dirs)
clean-files	:= *.dtb
