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

#include <linux/meizu/rstinfo.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>

int rstinfo_read(void *buf, int off, int len) 
{
    int real_off, real_len;
    char *tmpbuf;
    int ret = 0;

    real_off = GET_RB_SIZE(off >= 0 ? off : off + 511);
    real_len = GET_RB_SIZE(off + len + 511) - real_off;

    // If len and off match rstinfo block size
    if (real_len == len && real_off == off) {
        return sd_partition_rw("rstinfo", false, off, buf, len);
    }

    // Allocate temporary buffer
    tmpbuf = (char *)vmalloc(real_len);
    if (!tmpbuf) {
        pr_err("%s: no memory for tmpbuf\n", __func__);
        return -ENOMEM;
    }

    ret = sd_partition_rw("rstinfo", false, real_off, tmpbuf, real_len);
    if (ret) {
        pr_err("%s: failed to read to temporary buffer %d\n", ret);
        goto out;
    }

    // Adjust read data to defined offset
    memcpy(buf, &tmpbuf[off % 512], len);

out:
    vfree(tmpbuf);
    return ret;
}

int rstinfo_write(void *buf, int off, int len) 
{
    int real_off, real_len;
    char *tmpbuf;
    int ret = 0;

    real_off = GET_RB_SIZE(off >= 0 ? off : off + 511);
    real_len = GET_RB_SIZE(off + len + 511) - real_off;

    // If len and off match rstinfo block size
    if (real_len == len && real_off == off) {
        return sd_partition_rw("rstinfo", true, off, buf, len);
    }

    // Allocate temporary buffer
    tmpbuf = (char *)vmalloc(real_len);
    if (!tmpbuf) {
        pr_err("%s: no memory for tmpbuf\n", __func__);
        return -ENOMEM;
    }

    ret = sd_partition_rw("rstinfo", false, real_off, tmpbuf, real_len);
    if (ret) {
        pr_err("%s: failed to read to temporary buffer %d\n", ret);
        goto out;
    }

    // Copy write data to defined offset
    memcpy(&tmpbuf[off % 512], buf, len);

    ret = sd_partition_rw("rstinfo", true, real_off, tmpbuf, real_len);
out:
    vfree(tmpbuf);
    return ret;
}
