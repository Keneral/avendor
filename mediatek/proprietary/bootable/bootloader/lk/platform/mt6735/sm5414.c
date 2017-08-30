#include <platform/mt_typedefs.h>
#include <platform/mt_reg_base.h>
#include <platform/mt_i2c.h>
#include <platform/mt_pmic.h>
#include <platform/mt_gpio.h>
#include <platform/sm5414.h>
#include <printf.h>
//#include <platform/mt_pumpexpress.h>


#if !defined(CONFIG_POWER_EXT)
#include <platform/upmu_common.h>
#endif

int g_log_en=0;

/**********************************************************
  *
  *   [I2C Slave Setting]
  *
  *********************************************************/
#define SM5414_SLAVE_ADDR_WRITE   0x92
#define SM5414_SLAVE_ADDR_READ    0x93
//#define PRECC_BATVOL 2800 //preCC 2.8V
#define PRECC_BATVOL 3000 // preCC 3.2V

#define GPIO_SM5414_CHGEN_PIN	(62 | 0x80000000)
#define GPIO_SM5414_SHDN_PIN	(80 | 0x80000000)
/**********************************************************
  *
  *   [Global Variable]
  *
  *********************************************************/
kal_uint8 sm5414_reg[SM5414_REG_NUM] = {0};
int g_sm5414_hw_exist=0;

/**********************************************************
  *
  *   [I2C Function For Read/Write sm5414]
  *
  *********************************************************/
#define SM5414_I2C_ID  I2C1
static struct mt_i2c_t sm5414_i2c;

kal_uint32 sm5414_write_byte(kal_uint8 cmd, kal_uint8 writeData)
{
	kal_uint32 ret_code = I2C_OK;
	kal_uint8 write_data[2];
	kal_uint16 len;

	write_data[0]= cmd;
	write_data[1] = writeData;

	sm5414_i2c.id = SM5414_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set I2C address to >>1 */
	sm5414_i2c.addr = (SM5414_SLAVE_ADDR_WRITE >> 1);
	sm5414_i2c.mode = ST_MODE;
	sm5414_i2c.speed = 100;
	len = 2;

	ret_code = i2c_write(&sm5414_i2c, write_data, len);

	if (I2C_OK != ret_code)
		dprintf(INFO, "%s: i2c_write: ret_code: %d\n", __func__, ret_code);

	return ret_code;
}

kal_uint32 sm5414_read_byte(kal_uint8 cmd, kal_uint8 *returnData)
{
	kal_uint32 ret_code = I2C_OK;
	kal_uint16 len;
	*returnData = cmd;

	sm5414_i2c.id = SM5414_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set I2C address to >>1 */
	sm5414_i2c.addr = (SM5414_SLAVE_ADDR_READ >> 1);
	sm5414_i2c.mode = ST_MODE;
	sm5414_i2c.speed = 100;
	len = 1;

	ret_code = i2c_write_read(&sm5414_i2c, returnData, len, len);

	if (I2C_OK != ret_code)
		dprintf(INFO, "%s: i2c_read: ret_code: %d\n", __func__, ret_code);

	return ret_code;
}

/**********************************************************
  *
  *   [Read / Write Function]
  *
  *********************************************************/
kal_uint32 sm5414_read_interface (kal_uint8 RegNum, kal_uint8 *val, kal_uint8 MASK, kal_uint8 SHIFT)
{
	kal_uint8 sm5414_reg = 0;
	kal_uint32 ret = 0;

	dprintf(INFO, "--------------------------------------------------LK\n");

	ret = sm5414_read_byte(RegNum, &sm5414_reg);
	dprintf(INFO, "[sm5414_read_interface] Reg[%x]=0x%x\n", RegNum, sm5414_reg);

	sm5414_reg &= (MASK << SHIFT);
	*val = (sm5414_reg >> SHIFT);

	dprintf(INFO, "[sm5414_read_interface] val=0x%x\n", *val);

	return ret;
}

kal_uint32 sm5414_config_interface (kal_uint8 RegNum, kal_uint8 val, kal_uint8 MASK, kal_uint8 SHIFT)
{
	kal_uint8 sm5414_reg = 0;
	kal_uint32 ret = 0;

	dprintf(INFO, "--------------------------------------------------\n");

	ret = sm5414_read_byte(RegNum, &sm5414_reg);
	dprintf(INFO, "[sm5414_config_interface] Reg[%x]=0x%x\n", RegNum, sm5414_reg);

	sm5414_reg &= ~(MASK << SHIFT);
	sm5414_reg |= (val << SHIFT);

	ret = sm5414_write_byte(RegNum, sm5414_reg);
	dprintf(INFO, "[sm5414_config_interface] write Reg[%x]=0x%x\n", RegNum, sm5414_reg);

	// Check
	//sm5414_read_byte(RegNum, &sm5414_reg);
	//printk("[sm5414_config_interface] Check Reg[%x]=0x%x\n", RegNum, sm5414_reg);

	return ret;
}

//write one register directly
kal_uint32 sm5414_reg_config_interface (kal_uint8 RegNum, kal_uint8 val)
{
	kal_uint32 ret = 0;

	ret = sm5414_write_byte(RegNum, val);

	return ret;
}

/**********************************************************
  *
  *   [Internal Function]
  *
  *********************************************************/
//CTRL----------------------------------------------------
void sm5414_set_enboost(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CTRL), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CTRL_ENBOOST_MASK),
                                    (kal_uint8)(SM5414_CTRL_ENBOOST_SHIFT)
                                    );
    if(g_log_en>1)        
        dprintf(CRITICAL, "%d\n", ret);	    
}

void sm5414_set_chgen(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CTRL), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CTRL_CHGEN_MASK),
                                    (kal_uint8)(SM5414_CTRL_CHGEN_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}

void sm5414_set_suspen(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CTRL), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CTRL_SUSPEN_MASK),
                                    (kal_uint8)(SM5414_CTRL_SUSPEN_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}

void sm5414_set_reset(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CTRL), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CTRL_RESET_MASK),
                                    (kal_uint8)(SM5414_CTRL_RESET_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}

void sm5414_set_encomparator(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CTRL), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CTRL_ENCOMPARATOR_MASK),
                                    (kal_uint8)(SM5414_CTRL_ENCOMPARATOR_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}

//vbusctrl----------------------------------------------------
void sm5414_set_vbuslimit(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_VBUSCTRL), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_VBUSCTRL_VBUSLIMIT_MASK),
                                    (kal_uint8)(SM5414_VBUSCTRL_VBUSLIMIT_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}

//chgctrl1----------------------------------------------------
void sm5414_set_prechg(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CHGCTRL1), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CHGCTRL1_PRECHG_MASK),
                                    (kal_uint8)(SM5414_CHGCTRL1_PRECHG_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}
void sm5414_set_aiclen(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CHGCTRL1), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CHGCTRL1_AICLEN_MASK),
                                    (kal_uint8)(SM5414_CHGCTRL1_AICLEN_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}
void sm5414_set_autostop(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CHGCTRL1), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CHGCTRL1_AUTOSTOP_MASK),
                                    (kal_uint8)(SM5414_CHGCTRL1_AUTOSTOP_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}
void sm5414_set_aiclth(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CHGCTRL1), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CHGCTRL1_AICLTH_MASK),
                                    (kal_uint8)(SM5414_CHGCTRL1_AICLTH_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}

//chgctrl2----------------------------------------------------
void sm5414_set_fastchg(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CHGCTRL2), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CHGCTRL2_FASTCHG_MASK),
                                    (kal_uint8)(SM5414_CHGCTRL2_FASTCHG_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}

//chgctrl3----------------------------------------------------
void sm5414_set_weakbat(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CHGCTRL3), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CHGCTRL3_WEAKBAT_MASK),
                                    (kal_uint8)(SM5414_CHGCTRL3_WEAKBAT_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}
void sm5414_set_batreg(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CHGCTRL3), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CHGCTRL3_BATREG_MASK),
                                    (kal_uint8)(SM5414_CHGCTRL3_BATREG_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}

//chgctrl4----------------------------------------------------
void sm5414_set_dislimit(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CHGCTRL4), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CHGCTRL4_DISLIMIT_MASK),
                                    (kal_uint8)(SM5414_CHGCTRL4_DISLIMIT_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}
void sm5414_set_topoff(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CHGCTRL4), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CHGCTRL4_TOPOFF_MASK),
                                    (kal_uint8)(SM5414_CHGCTRL4_TOPOFF_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}

//chgctrl5----------------------------------------------------
void sm5414_set_topofftimer(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CHGCTRL5), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CHGCTRL5_TOPOFFTIMER_MASK),
                                    (kal_uint8)(SM5414_CHGCTRL5_TOPOFFTIMER_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}
void sm5414_set_fasttimer(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CHGCTRL5), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CHGCTRL5_FASTTIMER_MASK),
                                    (kal_uint8)(SM5414_CHGCTRL5_FASTTIMER_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}
void sm5414_set_votg(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=sm5414_config_interface(   (kal_uint8)(SM5414_CHGCTRL5), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(SM5414_CHGCTRL5_VOTG_MASK),
                                    (kal_uint8)(SM5414_CHGCTRL5_VOTG_SHIFT)
                                    );
    if(g_log_en>1)        
    dprintf(CRITICAL, "%d\n", ret);	
}

/**********************************************************
  *
  *   [Internal Function]
  *
  *********************************************************/
void sm5414_chg_en(kal_uint32 enable)
{
    if(KAL_TRUE == enable)
    {
        //SM : Please set nCHGEN pin to low(Charging ON)
        mt_set_gpio_mode(GPIO_SM5414_CHGEN_PIN,GPIO_MODE_GPIO);  
        mt_set_gpio_dir(GPIO_SM5414_CHGEN_PIN,GPIO_DIR_OUT);
        mt_set_gpio_out(GPIO_SM5414_CHGEN_PIN,GPIO_OUT_ZERO);    
    }
    else
    {
        //SM : Please set nCHGEN pin to high(Charging OFF)
        mt_set_gpio_mode(GPIO_SM5414_CHGEN_PIN,GPIO_MODE_GPIO);  
        mt_set_gpio_dir(GPIO_SM5414_CHGEN_PIN,GPIO_DIR_OUT);
        mt_set_gpio_out(GPIO_SM5414_CHGEN_PIN,GPIO_OUT_ONE);    
    }
}

void sm5414_dump_register(void)
{
    kal_uint8 i=0;

    dprintf(CRITICAL, "sm5414_dump_register\r\n");
    for (i=SM5414_INT1;i<SM5414_REG_NUM;i++)
    {
        sm5414_read_byte(i, &sm5414_reg[i]);
        dprintf(CRITICAL, "[0x%x]=0x%x\r\n", i, sm5414_reg[i]);		  
    }

	dprintf(CRITICAL, "\n");
}

void sm5414_hw_init(void)
{
    int reg_data = 0;
    
    printf("[sm5414] ChargerHwInit_sm5414\n" );

    //SM : set nCHGEN pin to low(Charging ON)
    mt_set_gpio_mode(GPIO_SM5414_CHGEN_PIN,GPIO_MODE_GPIO);  
    mt_set_gpio_dir(GPIO_SM5414_CHGEN_PIN,GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_SM5414_CHGEN_PIN,GPIO_OUT_ZERO);
    
    //SM : set nSHDN pin to high(I2C Function)
    mt_set_gpio_mode(GPIO_SM5414_SHDN_PIN,GPIO_MODE_GPIO);  
    mt_set_gpio_dir(GPIO_SM5414_SHDN_PIN,GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_SM5414_SHDN_PIN,GPIO_OUT_ONE);
    
    //You can customize INTMSK1/2/3 for your system
    //INTMSK1
    reg_data = 0xFF;
    sm5414_write_byte(SM5414_INTMASK1, &reg_data);
    //INTMSK2
    reg_data = 0xFF;
    sm5414_write_byte(SM5414_INTMASK2, &reg_data);
    //INTMSK3
    reg_data = 0xFF;
    sm5414_write_byte(SM5414_INTMASK3, &reg_data);

    // Setting default settings for CTRL register - Encomparator bit default is meant to be enabled
    sm5414_set_encomparator(ENCOMPARATOR_EN);//ENCOMPARATOR : Enable

    sm5414_set_topoff(TOPOFF_150mA); //Termination current 150mA
    sm5414_set_batreg(BATREG_4_4_0_0_V); //VREG 4.40V

#if defined(SM5414_TOPOFF_TIMER_SUPPORT)
    sm5414_set_autostop(AUTOSTOP_EN);
    sm5414_set_topofftimer(TOPOFFTIMER_10MIN);
#else
    sm5414_set_autostop(AUTOSTOP_DIS);
#endif

    //SM : Below register value are set to high for booting in case 0 voltage battery
    //     If system can idendify cable type, you had better set below value to suitable value.
	sm5414_set_prechg(PRECHG_450mA);
	/* Fastchg 2000mA*/
	sm5414_set_fastchg(FASTCHG_2000mA);
	/*Vbuslimit = 1800mA*/
	sm5414_set_vbuslimit(VBUSLIMIT_2000mA);
}

static CHARGER_TYPE g_chr_type_num = CHARGER_UNKNOWN;
int hw_charging_get_charger_type(void);
extern int g_std_ac_large_current_en;

void sm5414_charging_enable(kal_uint32 bEnable)
{
#if 0
	if (KAL_TRUE == bEnable)
		sm5414_chg_en(0x1); // charger enable
	else
		sm5414_chg_en(0);  // charger disable

	dprintf(CRITICAL, "[BATTERY:sm5414] charger enable/disable %d !\r\n", bEnable);
#endif
	int temp_CC_value = 0;
	kal_int32 bat_val = 0;

	if (CHARGER_UNKNOWN == g_chr_type_num && KAL_TRUE == upmu_is_chr_det()) {
		hw_charging_get_charger_type();
		dprintf(CRITICAL, "[BATTERY:sm5414] charger type: %d\n", g_chr_type_num);
	}

	bat_val = get_i_sense_volt(1);
	if (g_chr_type_num == STANDARD_CHARGER) {
		if (g_std_ac_large_current_en==1) {
			temp_CC_value = 2000;
			sm5414_set_vbuslimit(VBUSLIMIT_2000mA);  //IN current limit at 2A
			if (bat_val < PRECC_BATVOL)
            {         
				sm5414_set_prechg(PRECHG_450mA);  //Pre-Charging Current Limit at 450ma
                sm5414_set_fastchg(FASTCHG_2000mA);  //Fast Charging Current Limit at 2A
            }
			else
            { 
				sm5414_set_prechg(PRECHG_450mA);  //Pre-Charging Current Limit at 450ma
				sm5414_set_fastchg(FASTCHG_2000mA);  //Fast Charging Current Limit at 2A
            }

		} else {
			temp_CC_value = 1200;
			sm5414_set_vbuslimit(VBUSLIMIT_1200mA);  //IN current limit at 1200mA or 1.5mA for sm5414
			if (bat_val < PRECC_BATVOL)
            {         
				sm5414_set_prechg(PRECHG_450mA);  //Pre-Charging Current Limit at 450ma
                sm5414_set_fastchg(FASTCHG_1000mA);  //Fast Charging Current Limit at 2A
            }
			else
            { 
				sm5414_set_prechg(PRECHG_450mA);  //Pre-Charging Current Limit at 450ma
				sm5414_set_fastchg(FASTCHG_1000mA);  //Fast Charging Current Limit at 2A
            }

		}
	} else if (g_chr_type_num == STANDARD_HOST \
	           || g_chr_type_num == CHARGING_HOST \
	           || g_chr_type_num == NONSTANDARD_CHARGER) {
		temp_CC_value = 500;
		sm5414_set_vbuslimit(VBUSLIMIT_500mA); //IN current limit at 500mA
		sm5414_set_fastchg(FASTCHG_500mA);  //Fast Charging Current Limit at 500mA
	} else {
		temp_CC_value = 500;
		sm5414_set_vbuslimit(VBUSLIMIT_500mA); //IN current limit at 500mA
		sm5414_set_fastchg(FASTCHG_500mA);  //Fast Charging Current Limit at 500mA
	}

	if (KAL_TRUE == bEnable)
		sm5414_chg_en(0x1); // charger enable
	else
		sm5414_chg_en(0);  // charger disable

	dprintf(INFO, "[BATTERY:sm5414] sm5414_set_ac_current(), CC value(%dmA) \r\n", temp_CC_value);
	dprintf(INFO, "[BATTERY:sm5414] charger enable/disable %d !\r\n", bEnable);

}

#if defined(CONFIG_POWER_EXT)

int hw_charging_get_charger_type(void)
{
	g_chr_type_num = STANDARD_HOST;

	return STANDARD_HOST;
}

#else

extern void Charger_Detect_Init(void);
extern void Charger_Detect_Release(void);
extern void mdelay (unsigned long msec);
extern void pmic_set_register_value(PMU_FLAGS_LIST_ENUM flagname,kal_uint32 val);
extern kal_uint16 pmic_get_register_value(PMU_FLAGS_LIST_ENUM flagname);
static void hw_bc11_dump_register(void)
{
	dprintf(INFO, "Reg[0x%x]=0x%x,Reg[0x%x]=0x%x\n",
	        MT6328_CHR_CON20, upmu_get_reg_value(MT6328_CHR_CON20),
	        MT6328_CHR_CON21, upmu_get_reg_value(MT6328_CHR_CON21)
	       );
}

static void hw_bc11_init(void)
{
	mdelay(300);

	Charger_Detect_Init();

	//RG_bc11_BIAS_EN=1
	pmic_set_register_value(PMIC_RG_BC11_BIAS_EN, 0x1);//mt6351_upmu_set_rg_bc11_bias_en(0x1);
	//RG_bc11_VSRC_EN[1:0]=00
	pmic_set_register_value(PMIC_RG_BC11_VSRC_EN, 0x0);//mt6351_upmu_set_rg_bc11_vsrc_en(0x0);
	//RG_bc11_VREF_VTH = [1:0]=00
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH, 0x0);//mt6351_upmu_set_rg_bc11_vref_vth(0x0);
	//RG_bc11_CMP_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN, 0x0);//mt6351_upmu_set_rg_bc11_cmp_en(0x0);
	//RG_bc11_IPU_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_IPU_EN, 0x0);//mt6351_upmu_set_rg_bc11_ipu_en(0x0);
	//RG_bc11_IPD_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN, 0x0);//mt6351_upmu_set_rg_bc11_ipd_en(0x0);
	//bc11_RST=1
	pmic_set_register_value(PMIC_RG_BC11_RST, 0x1);//mt6351_upmu_set_rg_bc11_rst(0x1);
	//bc11_BB_CTRL=1
	pmic_set_register_value(PMIC_RG_BC11_BB_CTRL, 0x1);//mt6351_upmu_set_rg_bc11_bb_ctrl(0x1);

	mdelay(50);

	if (g_log_en>1) {
		dprintf(INFO, "hw_bc11_init() \r\n");
		hw_bc11_dump_register();
	}

}


static U32 hw_bc11_DCD(void)
{
	U32 wChargerAvail = 0;

	//RG_bc11_IPU_EN[1.0] = 10
	pmic_set_register_value(PMIC_RG_BC11_IPU_EN, 0x2);
	//RG_bc11_IPD_EN[1.0] = 01
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN, 0x1);//mt6351_upmu_set_rg_bc11_ipd_en(0x1);
	//RG_bc11_VREF_VTH = [1:0]=01
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH, 0x1);//mt6351_upmu_set_rg_bc11_vref_vth(0x1);
	//RG_bc11_CMP_EN[1.0] = 10
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN, 0x2);//mt6351_upmu_set_rg_bc11_cmp_en(0x2);

	mdelay(80);

	wChargerAvail = pmic_get_register_value(PMIC_RGS_BC11_CMP_OUT);//mt6351_upmu_get_rgs_bc11_cmp_out();

	if (g_log_en>1) {
		dprintf(INFO, "hw_bc11_DCD() \r\n");
		hw_bc11_dump_register();
	}

	//RG_bc11_IPU_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_IPU_EN, 0x0);//mt6351_upmu_set_rg_bc11_ipu_en(0x0);
	//RG_bc11_IPD_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN, 0x0);//mt6351_upmu_set_rg_bc11_ipd_en(0x0);
	//RG_bc11_CMP_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN, 0x0);//mt6351_upmu_set_rg_bc11_cmp_en(0x0);
	//RG_bc11_VREF_VTH = [1:0]=00
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH, 0x0);//mt6351_upmu_set_rg_bc11_vref_vth(0x0);

	return wChargerAvail;
}


static U32 hw_bc11_stepA1(void)
{
	U32 wChargerAvail = 0;

	//RG_bc11_IPD_EN[1.0] = 01
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN, 0x1);//mt6351_upmu_set_rg_bc11_ipd_en(0x1);
	//RG_bc11_VREF_VTH = [1:0]=00
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH, 0x0);//mt6351_upmu_set_rg_bc11_vref_vth(0x0);
	//RG_bc11_CMP_EN[1.0] = 01
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN, 0x1);//mt6351_upmu_set_rg_bc11_cmp_en(0x1);

	mdelay(80);

	wChargerAvail = pmic_get_register_value(PMIC_RGS_BC11_CMP_OUT);//mt6351_upmu_get_rgs_bc11_cmp_out();

	if (g_log_en>1) {
		dprintf(INFO, "hw_bc11_stepA1() \r\n");
		hw_bc11_dump_register();
	}

	//RG_bc11_IPD_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN, 0x0);//mt6351_upmu_set_rg_bc11_ipd_en(0x0);
	//RG_bc11_CMP_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN, 0x0);//mt6351_upmu_set_rg_bc11_cmp_en(0x0);

	return  wChargerAvail;
}


static U32 hw_bc11_stepA2(void)
{
	U32 wChargerAvail = 0;

	//RG_bc11_VSRC_EN[1.0] = 10
	pmic_set_register_value(PMIC_RG_BC11_VSRC_EN, 0x2);//mt6351_upmu_set_rg_bc11_vsrc_en(0x2);
	//RG_bc11_IPD_EN[1:0] = 01
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN, 0x1);//mt6351_upmu_set_rg_bc11_ipd_en(0x1);
	//RG_bc11_VREF_VTH = [1:0]=00
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH, 0x0);//mt6351_upmu_set_rg_bc11_vref_vth(0x0);
	//RG_bc11_CMP_EN[1.0] = 01
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN, 0x1);//mt6351_upmu_set_rg_bc11_cmp_en(0x1);

	mdelay(80);

	wChargerAvail = pmic_get_register_value(PMIC_RGS_BC11_CMP_OUT);//mt6351_upmu_get_rgs_bc11_cmp_out();

	if (g_log_en>1) {
		dprintf(INFO, "hw_bc11_stepA2() \r\n");
		hw_bc11_dump_register();
	}

	//RG_bc11_VSRC_EN[1:0]=00
	pmic_set_register_value(PMIC_RG_BC11_VSRC_EN, 0x2);//mt6351_upmu_set_rg_bc11_vsrc_en(0x0);
	//RG_bc11_IPD_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN, 0x0);//mt6351_upmu_set_rg_bc11_ipd_en(0x0);
	//RG_bc11_CMP_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN, 0x0);//mt6351_upmu_set_rg_bc11_cmp_en(0x0);

	return  wChargerAvail;
}


static U32 hw_bc11_stepB2(void)
{
	U32 wChargerAvail = 0;

	//RG_bc11_IPU_EN[1:0]=10
	pmic_set_register_value(PMIC_RG_BC11_IPU_EN, 0x2);//mt6351_upmu_set_rg_bc11_ipu_en(0x2);
	//RG_bc11_VREF_VTH = [1:0]=01
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH, 0x0);//mt6351_upmu_set_rg_bc11_vref_vth(0x1);
	//RG_bc11_CMP_EN[1.0] = 01
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN, 0x1);//mt6351_upmu_set_rg_bc11_cmp_en(0x1);

	mdelay(80);

	wChargerAvail = pmic_get_register_value(PMIC_RGS_BC11_CMP_OUT);//mt6351_upmu_get_rgs_bc11_cmp_out();

	if (g_log_en>1) {
		dprintf(INFO, "hw_bc11_stepB2() \r\n");
		hw_bc11_dump_register();
	}

	//RG_bc11_IPU_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_IPU_EN, 0x0);//mt6351_upmu_set_rg_bc11_ipu_en(0x0);
	//RG_bc11_CMP_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN, 0x0);//mt6351_upmu_set_rg_bc11_cmp_en(0x0);
	//RG_bc11_VREF_VTH = [1:0]=00
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH, 0x0); //mt6351_upmu_set_rg_bc11_vref_vth(0x0);

	return  wChargerAvail;
}


static void hw_bc11_done(void)
{
	//RG_bc11_VSRC_EN[1:0]=00
	pmic_set_register_value(PMIC_RG_BC11_VSRC_EN, 0x0);//mt6351_upmu_set_rg_bc11_vsrc_en(0x0);
	//RG_bc11_VREF_VTH = [1:0]=0
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH, 0x0);//mt6351_upmu_set_rg_bc11_vref_vth(0x0);
	//RG_bc11_CMP_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN, 0x0);//mt6351_upmu_set_rg_bc11_cmp_en(0x0);
	//RG_bc11_IPU_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_IPU_EN, 0x0);//mt6351_upmu_set_rg_bc11_ipu_en(0x0);
	//RG_bc11_IPD_EN[1.0] = 00
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN, 0x0);//mt6351_upmu_set_rg_bc11_ipd_en(0x0);
	//RG_bc11_BIAS_EN=0
	pmic_set_register_value(PMIC_RG_BC11_BIAS_EN, 0x0);//mt6351_upmu_set_rg_bc11_bias_en(0x0);

	Charger_Detect_Release();

	if (g_log_en>1) {
		dprintf(INFO, "hw_bc11_done() \r\n");
		hw_bc11_dump_register();
	}

}

int hw_charging_get_charger_type(void)
{
	if (CHARGER_UNKNOWN != g_chr_type_num)
		return g_chr_type_num;

	/********* Step initial  ***************/
	hw_bc11_init();

	/********* Step DCD ***************/
	if (1 == hw_bc11_DCD()) {
		/********* Step A1 ***************/
		if (1 == hw_bc11_stepA1()) {
			g_chr_type_num = APPLE_2_1A_CHARGER;
			dprintf(INFO, "step A1 : Apple 2.1A CHARGER!\r\n");
		} else {
			g_chr_type_num = NONSTANDARD_CHARGER;
			dprintf(INFO, "step A1 : Non STANDARD CHARGER!\r\n");
		}
	} else {
		/********* Step A2 ***************/
		if (1 == hw_bc11_stepA2()) {
			/********* Step B2 ***************/
			if (1 == hw_bc11_stepB2()) {
				g_chr_type_num = STANDARD_CHARGER;
				dprintf(INFO, "step B2 : STANDARD CHARGER!\r\n");
			} else {
				g_chr_type_num = CHARGING_HOST;
				dprintf(INFO, "step B2 :  Charging Host!\r\n");
			}
		} else {
			g_chr_type_num = STANDARD_HOST;
			dprintf(INFO, "step A2 : Standard USB Host!\r\n");
		}

	}

	/********* Finally setting *******************************/
	hw_bc11_done();

	return g_chr_type_num;
}

void pumpex_reset_adapter(void)
{	
    sm5414_set_vbuslimit(VBUSLIMIT_500mA); //IN current limit at 500mA	
	mdelay(250);
}
#endif

