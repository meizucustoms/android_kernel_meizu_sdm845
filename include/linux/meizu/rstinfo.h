/* 
 * Meizu Reset Info driver
 *
 * Copyright (c) 2022, MeizuCustoms enthusiasts
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */ 

#include <linux/types.h>

#ifndef __MZ_RSTINFO_H__
#define __MZ_RSTINFO_H__

#define RSTINFO_BLOCK_SIZE 512
#define GET_RB_SIZE(size) (((size) / 512) * 512)

enum rstinfo_restart_reason {
  RSTINFO_REASON_UNKNOWN = 0x0,
  RSTINFO_REASON_NORMAL = 0x1,
  RSTINFO_REASON_PANIC = 0x2,
  RSTINFO_REASON_HARDWARE_ERROR = 0x3,
  RSTINFO_REASON_ANDROID_REBOOT = 0x4,
  RSTINFO_REASON_ANDROID_ROOT = 0x5,
  RSTINFO_REASON_MAX = 0x6,
};

enum rstlog_entity_type {
  RSTLOG_UNKNOWN = 0x0,
  RSTLOG_KMSG = 0x1,
  RSTLOG_REBOOTREASON = 0x2,
};

struct reset_info {
  uint32_t backed_rstinfo;
  uint32_t restart_reason;
  uint32_t gcc_reset_status;
  uint64_t pmic_reg[3];
  uint32_t pmic_fault[3];
};

struct rstinfo_pon_reason {
  int pon_index[3];
  int poff_index[3];
  enum rstinfo_restart_reason restart_reason;
};

struct rstinfo_reason {
  struct rstinfo_pon_reason now;
  struct rstinfo_pon_reason last;
  uint32_t reason;
  uint32_t gcc_reset_status;
  uint8_t dev_name[3];
  struct reset_info regs_now;
  struct reset_info regs_old;
};

struct rstlog_entity_header {
  uint32_t magic;
  uint32_t length;
  enum rstlog_entity_type type;
};

struct rstcnt_entity {
  uint16_t exptype;
  uint16_t subtype;
  uint64_t time;
  uint8_t linux_ver[64];
  uint8_t android_ver[64];
  uint8_t build_time[64];
};

struct rstlog_header {
  uint32_t slot_index;
  uint32_t entity_max_size;
  uint64_t logsum;
};

struct rstcnt_header {
  uint32_t index;
  uint32_t index_max;
  uint64_t expcnt[6];
  uint64_t expsum;
};

struct rstinfo_header {
  uint32_t magic;
  uint32_t version;
  uint32_t blksize;
  uint32_t rstcnt_offset;
  uint32_t rstcnt_size;
  uint32_t rstlog_offset;
  uint32_t rstlog_size;
  struct rstcnt_header rstcnt_head;
  struct rstlog_header rstlog_head;
};

#define RSTINFO_RINFO_OFFSET 0x200
#define RSTINFO_RINFO_SIZE   sizeof(struct reset_info)

#define RSTINFO_PSTORE_OFFSET 0x900400
#define RSTINFO_PSTORE_SIZE 0x200000

#define RSTINFO_MICRO_PSTORE_OFFSET 0x150000
#define RSTINFO_MICRO_PSTORE_SIZE 0x50000

extern int sd_partition_rw(const char *part_name, int write, loff_t offset,
						void *buffer, size_t len);

int rstinfo_read(void *buf, int off, int len);
int rstinfo_write(void *buf, int off, int len);;

#endif /* __MZ_RSTINFO_H__ */
