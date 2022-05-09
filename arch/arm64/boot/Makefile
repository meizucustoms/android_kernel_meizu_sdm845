#
# arch/arm64/boot/Makefile
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies.
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 2012, ARM Ltd.
# Author: Will Deacon <will.deacon@arm.com>
#
# Based on the ia64 boot/Makefile.
#

ifeq ($(CONFIG_MACH_MEIZU_SDM845), y)
# Meizu 16th
ifeq ($(CONFIG_MACH_MEIZU_M1882), y)
current-meizu-device := m1882
endif

# Meizu 16th Plus
ifeq ($(CONFIG_MACH_MEIZU_M1892), y)
current-meizu-device := m1892
endif

current-meizu-device-dtb := sdm845-v2.1-meizu-$(current-meizu-device).dtb
current-meizu-device-dtbo := sdm845-v2.1-meizu-$(current-meizu-device)-overlay.dtb
current-meizu-device-placeholder-dtbo := placeholder-dtbo-$(current-meizu-device)
current-meizu-device-overlayed-dtb := overlayed-meizu-$(current-meizu-device).dtb

$(obj)/dts/qcom/meizu/$(current-meizu-device-placeholder-dtbo).dtbo: $(src)/dts/qcom/meizu/$(current-meizu-device-placeholder-dtbo).dts FORCE
	$(call if_changed_dep,dtc)

# Overlayed DTB
$(obj)/dts/qcom/meizu/$(current-meizu-device-overlayed-dtb): $(obj)/dts/qcom/meizu/$(current-meizu-device-dtbo) $(obj)/dts/qcom/meizu/$(current-meizu-device-dtb) $(obj)/dts/qcom/meizu/$(current-meizu-device-placeholder-dtbo).dtbo
	$(DT_OVERLAY_APPLY) \
		$(obj)/dts/qcom/meizu/$(current-meizu-device-dtb) \
		$(obj)/dts/qcom/meizu/$(current-meizu-device-dtbo) \
		$(obj)/dts/qcom/meizu/$(current-meizu-device-overlayed-dtb)
endif

include $(srctree)/arch/arm64/boot/dts/Makefile

OBJCOPYFLAGS_Image :=-O binary -R .note -R .note.gnu.build-id -R .comment -S

targets := Image Image.bz2 Image.gz Image.lz4 Image.lzma Image.lzo

DTB_NAMES := $(subst $\",,$(CONFIG_BUILD_ARM64_APPENDED_DTB_IMAGE_NAMES))
ifneq ($(DTB_NAMES),)
DTB_LIST := $(addsuffix .dtb,$(DTB_NAMES))
DTB_OBJS := $(addprefix $(obj)/dts/,$(DTB_LIST))
else
DTB_OBJS := $(shell find $(obj)/dts/ -name \*.dtb)
endif

ifeq ($(CONFIG_MACH_MEIZU_SDM845), y)
DTB_OBJS := $(obj)/dts/qcom/meizu/$(current-meizu-device-overlayed-dtb)
endif

# Add RTIC DTB to the DTB list if RTIC MPGen is enabled
ifdef RTIC_MPGEN
DTB_OBJS += rtic_mp.dtb
endif

rtic_mp.dtb: vmlinux FORCE
	$(RTIC_MPGEN) --objcopy="${OBJCOPY}" --objdump="${OBJDUMP}" \
	--binpath="" --vmlinux="vmlinux" --config=${KCONFIG_CONFIG} \
	--cc="${CC}" --dts=rtic_mp.dts && \
	$(DTC) -O dtb -o rtic_mp.dtb -b 0 $(DTC_FLAGS) rtic_mp.dts

$(obj)/Image: vmlinux FORCE
	$(call if_changed,objcopy)

$(obj)/Image.bz2: $(obj)/Image FORCE
	$(call if_changed,bzip2)

$(obj)/Image-dtb-hdr: $(obj)/Image FORCE
	echo -n 'UNCOMPRESSED_IMG' > $@ && \
	$(call size_append, $(filter-out FORCE,$^)) >> $@

$(obj)/Image-dtb: $(obj)/Image-dtb-hdr $(obj)/Image $(DTB_OBJS) FORCE
	$(call if_changed,cat)

$(obj)/Image.gz: $(obj)/Image FORCE
	$(call if_changed,gzip)

$(obj)/Image.lz4: $(obj)/Image FORCE
	$(call if_changed,lz4)

$(obj)/Image.lzma: $(obj)/Image FORCE
	$(call if_changed,lzma)

$(obj)/Image.lzo: $(obj)/Image FORCE
	$(call if_changed,lzo)

$(obj)/Image.gz-dtb: $(obj)/Image.gz $(DTB_OBJS) FORCE
	$(call if_changed,cat)

install:
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh $(KERNELRELEASE) \
	$(obj)/Image System.map "$(INSTALL_PATH)"

zinstall:
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh $(KERNELRELEASE) \
	$(obj)/Image.gz System.map "$(INSTALL_PATH)"
