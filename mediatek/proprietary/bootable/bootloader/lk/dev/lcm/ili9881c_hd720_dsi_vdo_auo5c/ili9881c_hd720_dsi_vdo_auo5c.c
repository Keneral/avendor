#include "lcm_drv.h"
#ifdef BUILD_LK
#include <platform/upmu_common.h>
#include <platform/upmu_hw.h>
#include <platform/mt_gpio.h>
#include <platform/mt_i2c.h>
#include <platform/mt_pmic.h>
#include <string.h>
#include <cust_gpio_boot.h>
#include <cust_gpio_usage.h>
#include <cust_i2c.h>
#else
#include <linux/string.h>
#include <linux/kernel.h>
#include "mt-plat/upmu_common.h"
#include "mt-plat/mt_gpio.h"
#ifdef CONFIG_MTK_LEGACY
#include <mach/mt_pm_ldo.h>
#include <mach/mt_gpio.h>
#ifndef CONFIG_FPGA_EARLY_PORTING
#include <cust_gpio_usage.h>
#include <cust_i2c.h>
#endif
#endif
#endif
#include "mt_gpio.h"
#include "gpio_const.h"
// ---------------------------------------------------------------------------
//  Define Log print
// ---------------------------------------------------------------------------
#define DEBUG
#ifdef BUILD_LK
#ifdef DEBUG
#define LCM_DEBUG(fmt, args...)  _dprintf(fmt, ##args)
#else
#define LCM_DEBUG(fmt, args...) do {} while(0)
#endif
#define LCM_ERROR(fmt, args...)  _dprintf(fmt, ##args)
#else
#ifdef DEBUG
#define LCM_DEBUG(fmt, args...)  printk(fmt, ##args)
#else
#define LCM_DEBUG(fmt, args...) do {} while(0)
#endif
#define LCM_ERROR(fmt, args...)  printk(fmt, ##args)
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
#define LCM_DSI_CMD_MODE									0
#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1280)
#define UDELAY(n) 											(lcm_util.udelay(n))
#define REGFLAG_PORT_SWAP									0xFFFA
#define REGFLAG_UDELAY             							0xFB
#define REGFLAG_DELAY             							0xFFFC
#define REGFLAG_END_OF_TABLE      							0xFFFD   // END OF REGISTERS MARKER
#define AUXADC_LCM_VOLTAGE_CHANNEL     12
/* LCM id voltage is 1.2V */
#define LCM_ID_MAX_VOLTAGE   1350
#define LCM_ID_MIN_VOLTAGE   1050
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------
struct NT5038_SETTING_TABLE {
	unsigned char cmd;
	unsigned char data;
};
static struct NT5038_SETTING_TABLE nt5038_cmd_data[3] = {
	{ 0x00, 0x0a },
	{ 0x01, 0x0a },
	{ 0x03, 0x30 }
};
static LCM_UTIL_FUNCS lcm_util;
extern int IMM_GetOneChannelValue(int dwChannel, int data[4], int* rawdata);
// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq((unsigned int *)pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)										lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)
#define dsi_swap_port(swap)   								lcm_util.dsi_swap_port(swap)
#define MDELAY(n)                          (lcm_util.mdelay(n))
#define LCM_RESET_PIN                                       (GPIO146|0x80000000)
#define SET_RESET_PIN(v)                                  (mt_set_gpio_out(LCM_RESET_PIN,(v)))
#ifndef GPIO_LCD_ENP_PIN
#define GPIO_LCD_ENP_PIN                               (GPIO83|0x80000000)
#endif
#ifndef GPIO_LCD_ENN_PIN
#define GPIO_LCD_ENN_PIN                               (GPIO42|0x80000000)
#endif
#ifndef BUILD_LK
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
/*****************************************************************************
* Define
*****************************************************************************/
#define I2C_I2C_LCD_BIAS_CHANNEL 1
#define DCDC_I2C_BUSNUM  I2C_I2C_LCD_BIAS_CHANNEL//for I2C channel 0
#define DCDC_I2C_ID_NAME "nt5038"
#define DCDC_I2C_ADDR 0x3E
/*****************************************************************************
* GLobal Variable
*****************************************************************************/
#if defined(CONFIG_MTK_LEGACY)
static struct i2c_board_info __initdata nt5038_board_info = {I2C_BOARD_INFO(DCDC_I2C_ID_NAME, DCDC_I2C_ADDR)};
#else
static const struct of_device_id lcm_of_match[] = {
	{.compatible = "mediatek,I2C_LCD_BIAS"},
	{},
};
#endif
static struct i2c_client *nt5038_i2c_client = NULL;
/*****************************************************************************
* Function Prototype
*****************************************************************************/
static int nt5038_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int nt5038_remove(struct i2c_client *client);
/*****************************************************************************
* Data Structure
*****************************************************************************/
struct nt5038_dev	{
struct i2c_client	*client;
};
static const struct i2c_device_id nt5038_id[] = {
{ DCDC_I2C_ID_NAME, 0 },
{ }
};
//#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
//static struct i2c_client_address_data addr_data = { .forces = forces,};
//#endif
//DC-DC nt5038 i2c driver
static struct i2c_driver nt5038_iic_driver = {
    .id_table	= nt5038_id,
    .probe		= nt5038_probe,
    .remove		= nt5038_remove,
    .driver		= {
    .owner	= THIS_MODULE,
    .name	= "nt5038",
#if !defined(CONFIG_MTK_LEGACY)
		.of_match_table = lcm_of_match,
#endif
},
};
/*****************************************************************************
* Extern Area
*****************************************************************************/
/*****************************************************************************
* Function
*****************************************************************************/
static int nt5038_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
//    LCD_DEBUG( "nt5038_iic_probe\n");
//    LCD_DEBUG("TPS: info==>name=%s addr=0x%x\n",client->name,client->addr);
    nt5038_i2c_client  = client;
    return 0;
}
static int nt5038_remove(struct i2c_client *client)
{
//    LCD_DEBUG( "nt5038_remove\n");
    nt5038_i2c_client = NULL;
    i2c_unregister_device(client);
    return 0;
}
static int nt5038_i2c_write_byte(unsigned char addr, unsigned char value)
{
	int ret = 0;
	struct i2c_client *client = nt5038_i2c_client;
	char write_data[2]={0};
	if(client == NULL)
	{
		LCM_ERROR("ERROR!!nt5038_i2c_client is null\n");
		return 0;
	}
	write_data[0]= addr;
	write_data[1] = value;
	ret=i2c_master_send(client, write_data, 2);
	if(ret<0)
		LCM_DEBUG("nt5038 write data fail !!\n");
	return ret ;
}
static int __init nt5038_iic_init(void)
{
    i2c_register_board_info(DCDC_I2C_BUSNUM, &nt5038_board_info, 1);
    i2c_add_driver(&nt5038_iic_driver);
    return 0;
}
static void __exit nt5038_iic_exit(void)
{
    i2c_del_driver(&nt5038_iic_driver);
}
module_init(nt5038_iic_init);
module_exit(nt5038_iic_exit);
MODULE_AUTHOR("Xiaokuan Shi");
MODULE_DESCRIPTION("MTK NT5038 I2C Driver");
MODULE_LICENSE("GPL");
#else
#define NT5038_SLAVE_ADDR_WRITE  0x7C
static struct mt_i2c_t NT5038_i2c;
static int nt5038_i2c_write_byte(kal_uint8 addr, kal_uint8 value)
{
	kal_uint32 ret_code = I2C_OK;
	kal_uint8 write_data[2];
	kal_uint16 len;
	write_data[0]= addr;
	write_data[1] = value;
	NT5038_i2c.id = I2C_I2C_LCD_BIAS_CHANNEL;
	NT5038_i2c.addr = (NT5038_SLAVE_ADDR_WRITE >> 1);
	NT5038_i2c.mode = ST_MODE;
	NT5038_i2c.speed = 100;
	len = 2;
	ret_code = i2c_write(&NT5038_i2c, write_data, len);
	printf("%s: i2c_write: addr:0x%x, value:0x%x ret_code: %d\n", __func__, addr, value, ret_code);
	return ret_code;
}
#endif
void ili9881c_DCS_write_1A_1P(unsigned char cmd, unsigned char para)
{
    unsigned int data_array[16];
    data_array[0] = (0x00022902);
    data_array[1] = (0x00000000 | (para << 8) | (cmd));
    dsi_set_cmdq(data_array, 2, 1);
}
static void init_lcm_registers(void)
{
	unsigned int array[16];

    array[0] = 0x00043902;
    array[1] = 0x038198FF;
    dsi_set_cmdq(array, 2, 1);

    ili9881c_DCS_write_1A_1P(0x01,0x08);
    ili9881c_DCS_write_1A_1P(0x02,0x00);
    ili9881c_DCS_write_1A_1P(0x03,0x73);
    ili9881c_DCS_write_1A_1P(0x04,0x73);
    ili9881c_DCS_write_1A_1P(0x05,0x14);
    ili9881c_DCS_write_1A_1P(0x06,0x06);
    ili9881c_DCS_write_1A_1P(0x07,0x02);
    ili9881c_DCS_write_1A_1P(0x08,0x05);
    ili9881c_DCS_write_1A_1P(0x09,0x14);
    ili9881c_DCS_write_1A_1P(0x0a,0x14);
    ili9881c_DCS_write_1A_1P(0x0b,0x00);
    ili9881c_DCS_write_1A_1P(0x0c,0x14);
    ili9881c_DCS_write_1A_1P(0x0d,0x14);
    ili9881c_DCS_write_1A_1P(0x0e,0x00);
    ili9881c_DCS_write_1A_1P(0x0f,0x0C);
    ili9881c_DCS_write_1A_1P(0x10,0x0C);
    ili9881c_DCS_write_1A_1P(0x11,0x0C);
    ili9881c_DCS_write_1A_1P(0x12,0x0C);
    ili9881c_DCS_write_1A_1P(0x13,0x14);
    ili9881c_DCS_write_1A_1P(0x14,0x0c);
    ili9881c_DCS_write_1A_1P(0x15,0x00);
    ili9881c_DCS_write_1A_1P(0x16,0x00);
    ili9881c_DCS_write_1A_1P(0x17,0x00);
    ili9881c_DCS_write_1A_1P(0x18,0x00);
    ili9881c_DCS_write_1A_1P(0x19,0x00);
    ili9881c_DCS_write_1A_1P(0x1a,0x00);
    ili9881c_DCS_write_1A_1P(0x1b,0x00);
    ili9881c_DCS_write_1A_1P(0x1c,0x00);
    ili9881c_DCS_write_1A_1P(0x1d,0x00);
    ili9881c_DCS_write_1A_1P(0x1e,0xc8);
    ili9881c_DCS_write_1A_1P(0x1f,0x80);
    ili9881c_DCS_write_1A_1P(0x20,0x02);
    ili9881c_DCS_write_1A_1P(0x21,0x00);
    ili9881c_DCS_write_1A_1P(0x22,0x02);
    ili9881c_DCS_write_1A_1P(0x23,0x00);
    ili9881c_DCS_write_1A_1P(0x24,0x00);
    ili9881c_DCS_write_1A_1P(0x25,0x00);
    ili9881c_DCS_write_1A_1P(0x26,0x00);
    ili9881c_DCS_write_1A_1P(0x27,0x00);
    ili9881c_DCS_write_1A_1P(0x28,0xfb);
    ili9881c_DCS_write_1A_1P(0x29,0x43);
    ili9881c_DCS_write_1A_1P(0x2a,0x00);
    ili9881c_DCS_write_1A_1P(0x2b,0x00);
    ili9881c_DCS_write_1A_1P(0x2c,0x07);
    ili9881c_DCS_write_1A_1P(0x2d,0x07);
    ili9881c_DCS_write_1A_1P(0x2e,0xff);
    ili9881c_DCS_write_1A_1P(0x2f,0xff);
    ili9881c_DCS_write_1A_1P(0x30,0x11);
    ili9881c_DCS_write_1A_1P(0x31,0x00);
    ili9881c_DCS_write_1A_1P(0x32,0x00);
    ili9881c_DCS_write_1A_1P(0x33,0x00);
    ili9881c_DCS_write_1A_1P(0x34,0x84);
    ili9881c_DCS_write_1A_1P(0x35,0x80);
    ili9881c_DCS_write_1A_1P(0x36,0x07);
    ili9881c_DCS_write_1A_1P(0x37,0x00);
    ili9881c_DCS_write_1A_1P(0x38,0x00);
    ili9881c_DCS_write_1A_1P(0x39,0x00);
    ili9881c_DCS_write_1A_1P(0x3a,0x00);
    ili9881c_DCS_write_1A_1P(0x3b,0x00);
    ili9881c_DCS_write_1A_1P(0x3c,0x00);
    ili9881c_DCS_write_1A_1P(0x3d,0x00);
    ili9881c_DCS_write_1A_1P(0x3e,0x00);
    ili9881c_DCS_write_1A_1P(0x3f,0x00);
    ili9881c_DCS_write_1A_1P(0x40,0x00);
    ili9881c_DCS_write_1A_1P(0x41,0x88);
    ili9881c_DCS_write_1A_1P(0x42,0x00);
    ili9881c_DCS_write_1A_1P(0x43,0x80);
    ili9881c_DCS_write_1A_1P(0x44,0x08);

    ili9881c_DCS_write_1A_1P(0x50,0x01);
    ili9881c_DCS_write_1A_1P(0x51,0x23);
    ili9881c_DCS_write_1A_1P(0x52,0x45);
    ili9881c_DCS_write_1A_1P(0x53,0x67);
    ili9881c_DCS_write_1A_1P(0x54,0x89);
    ili9881c_DCS_write_1A_1P(0x55,0xab);
    ili9881c_DCS_write_1A_1P(0x56,0x01);
    ili9881c_DCS_write_1A_1P(0x57,0x23);
    ili9881c_DCS_write_1A_1P(0x58,0x45);
    ili9881c_DCS_write_1A_1P(0x59,0x67);
    ili9881c_DCS_write_1A_1P(0x5a,0x89);
    ili9881c_DCS_write_1A_1P(0x5b,0xab);
    ili9881c_DCS_write_1A_1P(0x5c,0xcd);
    ili9881c_DCS_write_1A_1P(0x5d,0xef);

    ili9881c_DCS_write_1A_1P(0x5e,0x10);
    ili9881c_DCS_write_1A_1P(0x5f,0x02);
    ili9881c_DCS_write_1A_1P(0x60,0x08);
    ili9881c_DCS_write_1A_1P(0x61,0x09);
    ili9881c_DCS_write_1A_1P(0x62,0x10);
    ili9881c_DCS_write_1A_1P(0x63,0x12);
    ili9881c_DCS_write_1A_1P(0x64,0x11);
    ili9881c_DCS_write_1A_1P(0x65,0x13);
    ili9881c_DCS_write_1A_1P(0x66,0x0c);
    ili9881c_DCS_write_1A_1P(0x67,0x02);
    ili9881c_DCS_write_1A_1P(0x68,0x02);
    ili9881c_DCS_write_1A_1P(0x69,0x02);
    ili9881c_DCS_write_1A_1P(0x6a,0x02);
    ili9881c_DCS_write_1A_1P(0x6b,0x02);
    ili9881c_DCS_write_1A_1P(0x6c,0x0e);
    ili9881c_DCS_write_1A_1P(0x6d,0x0d);
    ili9881c_DCS_write_1A_1P(0x6e,0x0f);
    ili9881c_DCS_write_1A_1P(0x6f,0x02);
    ili9881c_DCS_write_1A_1P(0x70,0x02);
    ili9881c_DCS_write_1A_1P(0x71,0x06);
    ili9881c_DCS_write_1A_1P(0x72,0x07);
    ili9881c_DCS_write_1A_1P(0x73,0x02);
    ili9881c_DCS_write_1A_1P(0x74,0x02);
    ili9881c_DCS_write_1A_1P(0x75,0x02);
    ili9881c_DCS_write_1A_1P(0x76,0x07);
    ili9881c_DCS_write_1A_1P(0x77,0x06);
    ili9881c_DCS_write_1A_1P(0x78,0x11);
    ili9881c_DCS_write_1A_1P(0x79,0x13);
    ili9881c_DCS_write_1A_1P(0x7a,0x10);
    ili9881c_DCS_write_1A_1P(0x7b,0x12);
    ili9881c_DCS_write_1A_1P(0x7c,0x0f);
    ili9881c_DCS_write_1A_1P(0x7d,0x02);
    ili9881c_DCS_write_1A_1P(0x7e,0x02);
    ili9881c_DCS_write_1A_1P(0x7f,0x02);
    ili9881c_DCS_write_1A_1P(0x80,0x02);
    ili9881c_DCS_write_1A_1P(0x81,0x02);
    ili9881c_DCS_write_1A_1P(0x82,0x0d);
    ili9881c_DCS_write_1A_1P(0x83,0x0e);
    ili9881c_DCS_write_1A_1P(0x84,0x0c);
    ili9881c_DCS_write_1A_1P(0x85,0x02);
    ili9881c_DCS_write_1A_1P(0x86,0x02);
    ili9881c_DCS_write_1A_1P(0x87,0x09);
    ili9881c_DCS_write_1A_1P(0x88,0x08);
    ili9881c_DCS_write_1A_1P(0x89,0x02);
    ili9881c_DCS_write_1A_1P(0x8A,0x02);

	array[0] = 0x00043902;
	array[1] = 0x048198FF;
	dsi_set_cmdq(array, 2, 1);
    ili9881c_DCS_write_1A_1P(0x00,0x00);
    ili9881c_DCS_write_1A_1P(0x6C,0x15);
    ili9881c_DCS_write_1A_1P(0x6E,0x2D);
    ili9881c_DCS_write_1A_1P(0x6F,0x45);
    ili9881c_DCS_write_1A_1P(0x3A,0xA4);
    ili9881c_DCS_write_1A_1P(0x8D,0x14);
    ili9881c_DCS_write_1A_1P(0x87,0xBA);
    ili9881c_DCS_write_1A_1P(0x26,0x76);
    ili9881c_DCS_write_1A_1P(0xB2,0xD1);
    ili9881c_DCS_write_1A_1P(0xB5,0x06);
    ili9881c_DCS_write_1A_1P(0x69,0x57);
    ili9881c_DCS_write_1A_1P(0x33,0x44);
    ili9881c_DCS_write_1A_1P(0x88,0x0B);

	array[0] = 0x00043902;
	array[1] = 0x018198FF;
	dsi_set_cmdq(array, 2, 1);
    ili9881c_DCS_write_1A_1P(0x22,0x0A);
    ili9881c_DCS_write_1A_1P(0x31,0x0B);

    ili9881c_DCS_write_1A_1P(0x55,0x5F);
     ili9881c_DCS_write_1A_1P(0x50,0x95);
     ili9881c_DCS_write_1A_1P(0x51,0x90);
    ili9881c_DCS_write_1A_1P(0x60,0x14);
    ili9881c_DCS_write_1A_1P(0x61,0x00);
    ili9881c_DCS_write_1A_1P(0x62,0x19);
    ili9881c_DCS_write_1A_1P(0x63,0x10);
    ili9881c_DCS_write_1A_1P(0xA0,0x00);
    ili9881c_DCS_write_1A_1P(0xA1,0x18);
    ili9881c_DCS_write_1A_1P(0xA2,0x25);
    ili9881c_DCS_write_1A_1P(0xA3,0x12);
    ili9881c_DCS_write_1A_1P(0xA4,0x14);
    ili9881c_DCS_write_1A_1P(0xA5,0x27);
    ili9881c_DCS_write_1A_1P(0xA6,0x1C);
    ili9881c_DCS_write_1A_1P(0xA7,0x1E);
    ili9881c_DCS_write_1A_1P(0xA8,0x7D);
    ili9881c_DCS_write_1A_1P(0xA9,0x1B);
    ili9881c_DCS_write_1A_1P(0xAA,0x26);
    ili9881c_DCS_write_1A_1P(0xAB,0x70);
    ili9881c_DCS_write_1A_1P(0xAC,0x1C);
    ili9881c_DCS_write_1A_1P(0xAD,0x1B);
    ili9881c_DCS_write_1A_1P(0xAE,0x50);
    ili9881c_DCS_write_1A_1P(0xAF,0x23);
    ili9881c_DCS_write_1A_1P(0xB0,0x2B);
    ili9881c_DCS_write_1A_1P(0xB1,0x4E);
    ili9881c_DCS_write_1A_1P(0xB2,0x5F);
    ili9881c_DCS_write_1A_1P(0xB3,0x3F);
    ili9881c_DCS_write_1A_1P(0xC0,0x0A);
    ili9881c_DCS_write_1A_1P(0xC1,0x18);
    ili9881c_DCS_write_1A_1P(0xC2,0x25);
    ili9881c_DCS_write_1A_1P(0xC3,0x12);
    ili9881c_DCS_write_1A_1P(0xC4,0x14);
    ili9881c_DCS_write_1A_1P(0xC5,0x27);
    ili9881c_DCS_write_1A_1P(0xC6,0x1B);
    ili9881c_DCS_write_1A_1P(0xC7,0x1E);
    ili9881c_DCS_write_1A_1P(0xC8,0x7C);
    ili9881c_DCS_write_1A_1P(0xC9,0x1B);
    ili9881c_DCS_write_1A_1P(0xCA,0x26);
    ili9881c_DCS_write_1A_1P(0xCB,0x70);
    ili9881c_DCS_write_1A_1P(0xCC,0x1B);
    ili9881c_DCS_write_1A_1P(0xCD,0x1B);
    ili9881c_DCS_write_1A_1P(0xCE,0x50);
    ili9881c_DCS_write_1A_1P(0xCF,0x23);
    ili9881c_DCS_write_1A_1P(0xD0,0x2A);
    ili9881c_DCS_write_1A_1P(0xD1,0x4E);
    ili9881c_DCS_write_1A_1P(0xD2,0x5F);
    ili9881c_DCS_write_1A_1P(0xD3,0x3F);
    array[0] = 0x00043902;
    array[1] = 0x008198FF;
    dsi_set_cmdq(array, 2, 1);
    ili9881c_DCS_write_1A_1P(0x35,0x00);
    array[0] = 0x00110500;
    dsi_set_cmdq(array, 1, 1);
    MDELAY(120);
    array[0] = 0x00290500;
    dsi_set_cmdq(array, 1, 1);
    MDELAY(20);
}
// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------
static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}
static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS));
    params->type   = LCM_TYPE_DSI;
    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;
    params->lcm_if = LCM_INTERFACE_DSI0;
    params->lcm_cmd_if = LCM_INTERFACE_DSI0;
#if (LCM_DSI_CMD_MODE)
    params->dsi.mode   = CMD_MODE;
#else
    params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif
    params->dsi.LANE_NUM = LCM_THREE_LANE;
    params->dsi.data_format.color_order 	= LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq   	= LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding     	= LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format      	= LCM_DSI_FORMAT_RGB888;
    params->dsi.packet_size=256;
    params->dsi.ssc_disable=0;
    params->dsi.ssc_range = 1;
    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
    params->dsi.vertical_sync_active = 8;
    params->dsi.vertical_backporch = 24,
    params->dsi.vertical_frontporch = 24,
    params->dsi.vertical_active_line = FRAME_HEIGHT;
    params->dsi.horizontal_sync_active =20;
    params->dsi.horizontal_backporch = 40,
    params->dsi.horizontal_frontporch = 50,
    params->dsi.horizontal_active_pixel = FRAME_WIDTH;
    params->dsi.PLL_CLOCK = 261;
}
static void lcm_init_power(void)
{
	pmic_config_interface(0x0a5e, 0x8030, 0xffff, 0x0);
#ifdef BUILD_LK
	pmic_set_register_value(PMIC_RG_VEFUSE_VOSEL,3);
	MDELAY(5);
	pmic_set_register_value(PMIC_RG_VEFUSE_EN,1);
	MDELAY(5);
#endif
    mt_set_gpio_mode(GPIO_LCD_ENP_PIN, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_LCD_ENP_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_LCD_ENP_PIN, GPIO_OUT_ONE);
	MDELAY(5);
	mt_set_gpio_mode(GPIO_LCD_ENN_PIN, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_LCD_ENN_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_LCD_ENN_PIN, GPIO_OUT_ONE);
	MDELAY(5);
	nt5038_i2c_write_byte(nt5038_cmd_data[0].cmd, nt5038_cmd_data[0].data);
	MDELAY(1);
	nt5038_i2c_write_byte(nt5038_cmd_data[1].cmd, nt5038_cmd_data[1].data);
	MDELAY(1);
	nt5038_i2c_write_byte(nt5038_cmd_data[2].cmd, nt5038_cmd_data[2].data);
	MDELAY(1);
}
static void lcm_resume_power(void)
{
	pmic_set_register_value(PMIC_RG_VEFUSE_EN, 1);
	MDELAY(5);
 	mt_set_gpio_mode(GPIO_LCD_ENP_PIN, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_LCD_ENP_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_LCD_ENP_PIN, GPIO_OUT_ONE);
	MDELAY(5);
	mt_set_gpio_mode(GPIO_LCD_ENN_PIN, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_LCD_ENN_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_LCD_ENN_PIN, GPIO_OUT_ONE);
	MDELAY(5);
}
static void lcm_suspend_power(void)
{
	SET_RESET_PIN(0);
	MDELAY(5);
	mt_set_gpio_mode(GPIO_LCD_ENN_PIN, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_LCD_ENN_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_LCD_ENN_PIN, GPIO_OUT_ZERO);
	MDELAY(5);
	mt_set_gpio_mode(GPIO_LCD_ENP_PIN, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_LCD_ENP_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_LCD_ENP_PIN, GPIO_OUT_ZERO);
	MDELAY(5);
	pmic_set_register_value(PMIC_RG_VEFUSE_EN, 0);
	MDELAY(5);
}
static void lcm_init(void)
{
    mt_set_gpio_mode(LCM_RESET_PIN,GPIO_MODE_GPIO);
    mt_set_gpio_dir(LCM_RESET_PIN,GPIO_DIR_OUT);
    mt_set_gpio_pull_enable(LCM_RESET_PIN,GPIO_PULL_DISABLE);
    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(120);
     init_lcm_registers();
}
static void lcm_suspend(void)
{
	unsigned int data_array[16];
	data_array[0] = 0x00280500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(20);
	data_array[0] = 0x00100500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(120);
}
static void lcm_resume(void)
{
    mt_set_gpio_mode(LCM_RESET_PIN,GPIO_MODE_GPIO);
    MDELAY(2);
    mt_set_gpio_dir(LCM_RESET_PIN,GPIO_DIR_OUT);
    MDELAY(2);
    mt_set_gpio_pull_enable(LCM_RESET_PIN,GPIO_PULL_DISABLE);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(10);
   init_lcm_registers();
}
unsigned int lcm_compare_auo5c_id(void)
{
    int data[4] = {0,0,0,0};
    int res = 0;
    int rawdata = 0;
    int lcm_vol = 0;
#ifdef AUXADC_LCM_VOLTAGE_CHANNEL
    res = IMM_GetOneChannelValue(AUXADC_LCM_VOLTAGE_CHANNEL,data,&rawdata);
    if(res < 0)
    {
        LCM_ERROR("(%s) ili9881c_hd_dsi_vdo_auo5c get lcm chip id vol fail\n", __func__);
        return 0;
    }
#endif
    lcm_vol = data[0]*1000+data[1]*10;
    LCM_ERROR("(%s) ili9881c_hd_dsi_vdo_auo5c lcm chip id adc raw data:%d, lcm_vol:%d\n", __func__, rawdata, lcm_vol);
  if ((lcm_vol < LCM_ID_MAX_VOLTAGE) && (lcm_vol > LCM_ID_MIN_VOLTAGE))
        return 1;
    else
        return 0;
}
LCM_DRIVER ili9881c_hd720_dsi_vdo_auo5c_lcm_drv=
{
    .name           = "ili9881c_hd720_dsi_vdo_auo5c",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_auo5c_id,
    .init_power		= lcm_init_power,
    .resume_power   = lcm_resume_power,
    .suspend_power  = lcm_suspend_power,
};
