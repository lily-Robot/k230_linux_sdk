#include <asm/asm.h>
#include <asm/io.h>
#include <asm/types.h>
#include <lmb.h>
#include <cpu_func.h>
#include <stdio.h>
#include <common.h>
#include <command.h>
#include <image.h>
#include <gzip.h>
#include <asm/spl.h>
#include "sysctl.h"

#include <pufs_hmac.h>
#include <pufs_ecp.h>
#include <pufs_rt.h>
#include "pufs_sm2.h"
#include <pufs_sp38a.h>
#include <pufs_sp38d.h>
#include <linux/kernel.h>
// #include "sdk_autoconf.h"
// #include "k230_board_common.h"
#include <env_internal.h>
#include <linux/delay.h>
#include <platform.h>

void ddr_init_2667(void);
int ddr_init_training(void)
{
	#if defined(CONFIG_TARGET_K230_FPGA)
		return 0; //fpga not need init;
	#endif
	if( (readl((const volatile void __iomem *)0x980001bcULL) & 0x1 ) != 0 ){
		return 0; //have init ,not need reinit;
	}
	#ifdef CONFIG_LPDDR3_800
		printf("CONFIG_LPDDR3_800 \n");
		ddr_init_800();
    #elif defined(CONFIG_CANMV_01STUDIO_LPDDR3_2133)
		canmv_01studio_ddr_init_2133();
    #elif defined(CONFIG_CANMV_01STUDIO_LPDDR3_1600)
		canmv_01studio_ddr_init_1600();
	#elif defined(CONFIG_CANMV_01STUDIO_LPDDR3_800)
		canmv_01studio_ddr_init_800();
	#elif defined(CONFIG_CANMV_01STUDIO_LPDDR4_2667)
		ddr_init_2667();
	#elif defined(CONFIG_CANMV_01STUDIO_LPDDR4_3200)
		ddr4_init_3200();
	#endif
	return 0;
}

sysctl_boot_mode_e sysctl_boot_get_boot_mode(void)
{
	return SYSCTL_BOOT_SDIO0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
    #define USB_IDPULLUP0 		(1<<4)
    #define USB_DMPULLDOWN0 	(1<<8)
    #define USB_DPPULLDOWN0 	(1<<9)



    u32 usb_ctl3 = readl((const volatile void __iomem *)(SDIO0_BASE_ADDR + 0x7c));

	usb_ctl3 |= USB_IDPULLUP0;
	usb_ctl3 &= ~(USB_DMPULLDOWN0 | USB_DPPULLDOWN0);
	writel(usb_ctl3, ( volatile void __iomem *)(SDIO0_BASE_ADDR + 0x7c));
	//printf("usb_ctl3 =%x\n",usb_ctl3);

	usb_ctl3 = readl((const volatile void __iomem *)(SDIO0_BASE_ADDR + 0x9c));
	usb_ctl3 |= USB_IDPULLUP0;
	usb_ctl3 |= (USB_DMPULLDOWN0 | USB_DPPULLDOWN0);
	writel(usb_ctl3, ( volatile void __iomem *)(SDIO0_BASE_ADDR + 0x9c));

	//printf("usb_ctl3 =%x\n",usb_ctl3);
    return 0;
}
#endif
