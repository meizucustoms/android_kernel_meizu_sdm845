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
#include <linux/workqueue.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/pstore.h>
#include <linux/vmalloc.h>
#include <linux/platform_device.h>

extern struct platform_driver ramoops_driver;
extern struct pstore_info *psinfo;

static struct delayed_work rstinfo_work;
static bool is_sd_ready = false;

static void mz_rstinfo_store_ramoops(struct work_struct *ws) {
    int ret;
    ktime_t boottime;
    int64_t ms_boottime;
    int update_interval;

    char *buf, *tbuf;
    size_t buf_sz;

    u64 id = 0;
    enum pstore_type_id type = PSTORE_TYPE_CONSOLE;
    int count = 0;
    bool compressed = false;
    ssize_t ecc_notice_size = 0;
    struct timespec u_ts;

    if (!is_sd_ready) {
        tbuf = (char *)vmalloc(512);
        if (!tbuf)
            return;

        ret = rstinfo_read(tbuf, 0, 512);
        if (ret) {
            vfree(tbuf);
            schedule_delayed_work(&rstinfo_work, msecs_to_jiffies(50));
            return;
        }

        vfree(tbuf);
        is_sd_ready = true;
    }

    // Get boot time
    boottime = ktime_get_boottime();
    
    // Determine update interval
    ms_boottime = ktime_to_ms(boottime);

    if (ms_boottime < 2000) {
        update_interval = 50;
    } else if (ms_boottime < 10000) {
        update_interval = 100;
    } else if (ms_boottime < 20000) {
        update_interval = 500;
    } else {
        update_interval = 10000;
    }

    mutex_lock(&psinfo->read_mutex);

    ret = psinfo->open(psinfo);
    if (ret)
        goto unlock;

    ret = psinfo->read(&id, &type, &count, &u_ts,
                       &buf, &compressed, &ecc_notice_size, psinfo);

    psinfo->close(psinfo);

unlock:
    mutex_unlock(&psinfo->read_mutex);

    if (!ret) {
        buf_sz = strlen(buf);

        if (buf_sz > RSTINFO_MICRO_PSTORE_SIZE)
            buf_sz = sprintf(buf, "ERROR: NO MORE SPACE FOR PSTORE!\n\0\0\0\0\0\0");

        rstinfo_write(buf, RSTINFO_MICRO_PSTORE_OFFSET, buf_sz);
    }

    // Re-schedule this function
    schedule_delayed_work(&rstinfo_work, msecs_to_jiffies(update_interval));

    return;
}

static int mz_rstinfo_init(void)
{
    char *buf;

    buf = vmalloc(RSTINFO_MICRO_PSTORE_SIZE);
    if (!buf)
        return -ENOMEM;

    // Clear 10MB to store ramoops
    memset(buf, 0, RSTINFO_MICRO_PSTORE_SIZE);
    rstinfo_write(buf, RSTINFO_MICRO_PSTORE_OFFSET, RSTINFO_MICRO_PSTORE_SIZE);

    // Start ramoops driver
    platform_driver_register(&ramoops_driver);

    // Store ramoops to memory
    INIT_DELAYED_WORK(&rstinfo_work, mz_rstinfo_store_ramoops);
    schedule_delayed_work(&rstinfo_work, msecs_to_jiffies(10));

    return 0;
}

module_init(mz_rstinfo_init);
