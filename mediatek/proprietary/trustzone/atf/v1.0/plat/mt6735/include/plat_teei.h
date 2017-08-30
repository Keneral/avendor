/****************************************
 *	Copyright (c) 2015
 * 	microtrust
 * 	All rights reserved
 * 	Author: Steven Meng
 ****************************************/

#ifndef __PLAT_TEEI_H__
#define __PLAT_TEEI_H__

#include "devapc.h"
#include <platform.h>
#include <plat_private.h>

#define INTR_OFFSET 0


#define SEC_APP_INTR		(280+INTR_OFFSET)
#define SEC_DRV_INTR		(286+INTR_OFFSET)
#define SEC_RDRV_INTR		279+INTR_OFFSET

#define NSEC_CALL_INTR		284+INTR_OFFSET
#define NSEC_INTSCHED_INTR	285+INTR_OFFSET

#define NSEC_APP_INTR		282
#define NSEC_BOOT_INTR		283
#define NSEC_SCHED_INTR		284
#define NSEC_DRV_INTR		287
#define NSEC_RDRV_INTR		278
#define NSEC_LOG_INTR		277
#define NSEC_ERR_INTR		276

#define TEEI_BOOT_PARAMS (((atf_arg_t_ptr)(uintptr_t)TEE_BOOT_INFO_ADDR)->tee_boot_arg_addr)
#define TEEI_SECURE_PARAMS (TEEI_BOOT_PARAMS+0x100)

/* this should be checked and changed by platform */
#if defined(MACH_TYPE_MT6735) || defined(MACH_TYPE_MT6753)
#define TEEI_SPI0_APC_NUM	(54)
#elif defined(MACH_TYPE_MT6735M)
#define TEEI_SPI0_APC_NUM	(53)
#else
#error "Wrong MACH Type!!"
#endif
#define TEEI_USED_SPI_NUM	TEEI_SPI0_APC_NUM

void migrate_gic_context(uint32_t secure_state);
void disable_group(unsigned int grp);
void enable_group(unsigned int grp);
void trigger_soft_intr(unsigned int id);
void prepare_gic_for_nsec_boot();
void prepare_gic_for_sec_boot();
void teei_gic_setup();
void teei_ack_gic();
void teei_triggerSgiDump();
void plat_teei_dump();

extern void mt_irq_set_sens(unsigned int base, unsigned int irq, unsigned int sens);
extern void mt_irq_set_polarity(unsigned int irq, unsigned int polarity);
extern void set_module_apc(unsigned int module, E_MASK_DOM domain_num , APC_ATTR permission_control);


#endif
