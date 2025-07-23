
#include "gd32f4x.h"

const uint8_t CRC8Table[]=
{			
	0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,
	0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,
	0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,
	0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,
	0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,
	0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,
	0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,
	0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,
	0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,
	0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,
	0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,
	0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,
	0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,
	0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,
	0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,
	0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3
};

/*CRC高位字节值表*/
const unsigned int ExternalCommRxA_CRCTable_high[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/*CRC低位字节值表*/
const unsigned int ExternalCommRxA_CRCTable_low[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};


volatile uint8_t  Usart0TxBuff[USART2_TXBUFF_COUNT_MAX] = {0};	/*串口0的数据发送缓冲区*/
uint8_t  Usart0RxBuff[USART0_RXBUFF_COUNT_MAX] = {0};			/*串口0的数据接收缓冲区*/

volatile static uint16_t Usart0RxNum = 0;						/*串口0接收到的数据个数*/

volatile uint8_t  Usart2TxBuff[USART2_TXBUFF_COUNT_MAX] = {0};	/*串口2的数据发送缓冲区*/
__attribute__((aligned(4))) uint8_t  Usart2RxBuff[USART2_RXBUFF_COUNT_MAX] = {0};			/*串口2的数据接收缓冲区*/

volatile static uint16_t Usart2RxNum = 0;						/*串口2接收到的数据个数*/

/* volatile uint8_t  Uart3TxBuff[UART3_TXBUFF_COUNT_MAX] = {0};*/	/*串口3的数据发送缓冲区*/
uint8_t  Uart3RxBuff[UART3_RXBUFF_COUNT_MAX] = {0};			    /*串口3的数据接收缓冲区*/

volatile static uint16_t Uart3RxNum = 0;						/*串口3接收到的数据个数*/

uint32_t uiDMAData[DMA1_CH0_BUFF_MAX] ={0};			            /*存储DMA数据 ADC用*/

/* 系统时钟************************************************************************** */
/* us延时倍乘数 */
static uint32_t g_fac_us = 0; 

/**
 * @函数名称: delay_init()
 * @功能描述: 精准延时函数初始化
 * @输入: sysclk: 系统时钟频率, 即CPU频率(rcc_c_ck) 200Mhz
 * @输出: 无
 * @返回值: 无
 */
static void gd32f4x_delay_init(uint16_t sysclk)
{
    uint32_t reload;
    /* SYSTICK使用内核时钟源,同CPU同频率 */
    systick_clksource_set(SYSTICK_CLKSOURCE_HCLK);
    /* 不论是否使用OS,g_fac_us都需要使用 */
    g_fac_us = sysclk;
    /* 每秒钟的计数次数 单位为M */
    reload = sysclk;
    /* 根据delay_ostickspersec设定溢出时间,reload为24位
    寄存器,最大值:16777216,在168M下,约合0.099s左右 */
    reload *= 1000000 / configTICK_RATE_HZ;
    /* 开启SYSTICK中断 */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  
    /* 每1/delay_ostickspersec秒中断一次 */
    SysTick->LOAD = reload;
    /* 开启SYSTICK */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/**
 * @函数名称: delay_us()
 * @功能描述: 微秒延时接口
 * @输入: nus 微秒计数
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    /* LOAD的值 */
    uint32_t reload = SysTick->LOAD;
    /* 需要的节拍数 */
    ticks = nus * g_fac_us;
    /* 刚进入时的计数器值 */
    told = SysTick->VAL;
    while (1) {
        tnow = SysTick->VAL;
        if (tnow != told) {
            if (tnow < told) {
                /* 这里注意一下SYSTICK是一个递减的计数器就可以了 */
                tcnt += told - tnow;
            }
            else {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) {
                /* 时间超过/等于要延迟的时间,则退出 */
                break;
            }
        }
    }
}

/**
 * @函数名称: gd32f4x_delay_ms()
 * @功能描述: 毫秒延时接口
 * @输入: ms 毫秒计数
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_delay_ms(uint16_t nms)
{
    uint32_t i;
    for (i = 0; i < nms; i++) {
        gd32f4x_delay_us(1000);
    }
}

/**
 * @函数名称: system_clock_init()
 * @功能描述: 系统时钟初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_system_clock_init(void)
{
    /* 设置系统时钟200MHz */
    SystemInit();
    /* 延时函数初始化 */
    gd32f4x_delay_init(200);
}

/* 服务FreeRTOS定时器中断（除了自动执行的端口）*/
extern void xPortSysTickHandler(void);
void SysTick_Handler(void) 
{     
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )      
    if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {   
        #endif        
        xPortSysTickHandler();   
        #if (INCLUDE_xTaskGetSchedulerState  == 1 )     
    }
    #endif  
}

/* IO-输入/输出************************************************************************** */
/**
 * @函数名称: gpio_init()
 * @功能描述: gpio初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_gpio_init(void)
{
    gpio_deinit(GPIOA);
	gpio_deinit(GPIOB);
	gpio_deinit(GPIOC);
	gpio_deinit(GPIOD);
	gpio_deinit(GPIOE);
	/*复位外部中断*/ 
	exti_deinit();
    /*使能外部中断时钟*/
	rcu_periph_clock_enable(RCU_SYSCFG);
	 
	 /*****************************************GPIOA初始化*******************************************/
    /*使能GPIOA时钟*/
	rcu_periph_clock_enable(RCU_GPIOA);
	
	/*wkup待机模式下按键唤醒功能*/
	gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_MODE_AF, GPIO_PIN_0);/*设置PA0的工作模式为备用功能模式*/
	pmu_wakeup_pin_enable();/*使能PA0的wkup功能*/
	
	/*
	  *@ EN_SW1置高,Vbat开始给板卡通电,辅电工作
	 */
	gpio_mode_set(EN_SW1_APS_VBT_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,EN_SW1_APS_VBT_PIN);/*设置工作模式，输入还是输出*/
	gpio_output_options_set(EN_SW1_APS_VBT_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,EN_SW1_APS_VBT_PIN);/*输出模式，推挽输出*/
	gpio_bit_reset(EN_SW1_APS_VBT_GPIO,EN_SW1_APS_VBT_PIN);/*输出高电平*/
	
	/*
	  *@ EN_SW2置高,Vbat开始防反接电路工作
	 */
	gpio_mode_set(EN_SW2_ARC_VBT_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,EN_SW2_ARC_VBT_PIN);
	gpio_output_options_set(EN_SW2_ARC_VBT_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,EN_SW2_ARC_VBT_PIN);
	gpio_bit_reset(EN_SW2_ARC_VBT_GPIO,EN_SW2_ARC_VBT_PIN);
	
	/*
	  *@ CAN,485通讯芯片供电电源开关,高电平使能输出,低电平关闭输出
	 */
	gpio_mode_set(PERIPHERAL_POWER_VDD_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,PERIPHERAL_POWER_VDD_PIN);
	gpio_output_options_set(PERIPHERAL_POWER_VDD_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,PERIPHERAL_POWER_VDD_PIN);
	gpio_bit_reset(PERIPHERAL_POWER_VDD_GPIO,PERIPHERAL_POWER_VDD_PIN);

    /*
	  *@ LED 7 
	 */
	gpio_mode_set(LED_J49_7_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,LED_J49_7_PIN);
	gpio_output_options_set(LED_J49_7_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,LED_J49_7_PIN);
	gpio_bit_reset(LED_J49_7_GPIO,LED_J49_7_PIN);

	 /*****************************************GPIOB初始化*******************************************/
	rcu_periph_clock_enable(RCU_GPIOB);
	
	/*
	  *@ 蜂鸣器
	 */
	gpio_mode_set(BUZZER_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,BUZZER_PIN);
	gpio_output_options_set(BUZZER_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BUZZER_PIN);
	gpio_bit_reset(BUZZER_PORT,BUZZER_PIN);
    
	/*
	  *@ J4的EH_DCH，EN_DCH置高，低压Vbat驱动端开始工作，升压输出
	 */
	gpio_mode_set(LOW_VOLTAGE_DRIVE_EN_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,LOW_VOLTAGE_DRIVE_EN_PIN);
	gpio_output_options_set(LOW_VOLTAGE_DRIVE_EN_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,LOW_VOLTAGE_DRIVE_EN_PIN);
	gpio_bit_reset(LOW_VOLTAGE_DRIVE_EN_GPIO,LOW_VOLTAGE_DRIVE_EN_PIN);
	
	/*
	  *@ RS485(/RE)收发器的接收使能引脚
	 */
	gpio_mode_set(PERIPHERAL_RS485_EN_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,PERIPHERAL_RS485_EN_PIN);
	gpio_output_options_set(PERIPHERAL_RS485_EN_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,PERIPHERAL_RS485_EN_PIN);
	gpio_bit_set(PERIPHERAL_RS485_EN_GPIO,PERIPHERAL_RS485_EN_PIN);

	
	 
	 /*****************************************GPIOC初始化*******************************************/ 
	rcu_periph_clock_enable(RCU_GPIOC);
	
	/*
	  *@ 12V转8V电源VPRO的供电使能输出,VPRO的供电使能,常低,拉高延时1s供电
	 */
	gpio_mode_set(VPRO12V8V_CONTROL_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,VPRO12V8V_CONTROL_PIN);
	gpio_output_options_set(VPRO12V8V_CONTROL_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,VPRO12V8V_CONTROL_PIN);
	gpio_bit_set(VPRO12V8V_CONTROL_GPIO,VPRO12V8V_CONTROL_PIN);

	/*
	  *@ 输入,J4的S_HV,高压存在后,S_HV=0,母线没高压S_HV=1；
	 */
	gpio_mode_set(BUS_HIGH_VOLTAGE_STATE_GPIO,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,BUS_HIGH_VOLTAGE_STATE_PIN);
	
	nvic_irq_enable(EXTI5_9_IRQn, 2u, 2u);
	syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN5);
	exti_init(BUS_HIGH_VOLTAGE_EXTI_X, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	exti_interrupt_enable(BUS_HIGH_VOLTAGE_EXTI_X);
	exti_interrupt_flag_clear(BUS_HIGH_VOLTAGE_EXTI_X);

    /*
	  *@ 输入,插入检测 当外部给SL0_IN,高电平时,也就是插入后,你读取MS0就是高电平
	 */
	gpio_mode_set(MS0_DET_STATE_GPIO,GPIO_MODE_INPUT,GPIO_PUPD_PULLDOWN,MS0_DET_STATE_PIN);

    nvic_irq_enable(EXTI5_9_IRQn, 2u, 1u);
	syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN7);
	exti_init(MS0_DET_EXTI_X, EXTI_INTERRUPT, EXTI_TRIG_RISING);
	exti_interrupt_enable(MS0_DET_EXTI_X);
	exti_interrupt_flag_clear(MS0_DET_EXTI_X);

	/*
	  *@ 输出，发出插入信号
	 */
	gpio_mode_set(MS0_OUT_EN_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,MS0_OUT_EN_PIN);
	gpio_output_options_set(MS0_OUT_EN_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,MS0_OUT_EN_PIN);
	gpio_bit_set(MS0_OUT_EN_GPIO,MS0_OUT_EN_PIN);

    /*
	  *@ 输出，LED6
	 */
    gpio_mode_set(LED_J49_6_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_J49_6_PIN);
    gpio_output_options_set(LED_J49_6_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_J49_6_PIN);
    gpio_bit_reset(LED_J49_6_GPIO,LED_J49_6_PIN);

	 /*****************************************GPIOD初始化*******************************************/ 
	rcu_periph_clock_enable(RCU_GPIOD);
		
	/*
	  *@ CTL 充放电和预充MOS强制关闭
	 */
	gpio_mode_set(AFESH367309MOS_CONTR_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,AFESH367309MOS_CONTR_PIN);
	gpio_output_options_set(AFESH367309MOS_CONTR_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,AFESH367309MOS_CONTR_PIN);
	gpio_bit_set(AFESH367309MOS_CONTR_GPIO,AFESH367309MOS_CONTR_PIN); 	
	
	/*
	  *@ ALARM AFE芯片与MCU通信引脚，中断方式
	 */	
	gpio_mode_set(AFE_SH367309_ALARM_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, AFE_SH367309_ALARM_PIN);
	nvic_irq_enable(EXTI10_15_IRQn, 2u, 1u);
	syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN15);
	exti_init(ALARM_GPIO_EXTI_X, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	exti_interrupt_enable(ALARM_GPIO_EXTI_X);
	exti_interrupt_flag_clear(ALARM_GPIO_EXTI_X);

	/*
	  *@ SHIP 仓运初始化
	 */
	gpio_mode_set(WAREHOUSE_STAT_CONTR_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,WAREHOUSE_STAT_CONTR_PIN);
	gpio_output_options_set(WAREHOUSE_STAT_CONTR_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,WAREHOUSE_STAT_CONTR_PIN);
	gpio_bit_set(WAREHOUSE_STAT_CONTR_GPIO,WAREHOUSE_STAT_CONTR_PIN);
	
	 /*****************************************GPIOE初始化*******************************************/
	rcu_periph_clock_enable(RCU_GPIOE);
	
	/*
	  *@ EN_SKY：开机按钮按下检测功能，开机按钮按下EN_SKY=0,没按下EN_SKY=1
	 */	
	gpio_mode_set(EN_SKY_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, EN_SKY_PIN);
	nvic_irq_enable(EXTI2_IRQn, 2u, 1u);
	syscfg_exti_line_config(EXTI_SOURCE_GPIOE, EXTI_SOURCE_PIN2);
	exti_init(EN_SKY_EXTI_X, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	exti_interrupt_enable(EN_SKY_EXTI_X);
	exti_interrupt_flag_clear(EN_SKY_EXTI_X);

	/*
	  *@ EN_PWR：当开机按钮按下后辅电供电，系统自检OK后EN_PWRs输出1高，保持辅电一直提供
	 */	
	gpio_mode_set(EN_PWR_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,EN_PWR_PIN);
	gpio_output_options_set(EN_PWR_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,EN_PWR_PIN);
	gpio_bit_reset(EN_PWR_GPIO,EN_PWR_PIN);

	/*
	  *@ 输出，J4的EH_CH，EN_CH置高，高压驱动端开始工作，降压输出
	 */
	gpio_mode_set(HIGH_VOLTAGE_DRIVE_EN_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,HIGH_VOLTAGE_DRIVE_EN_PIN);
	gpio_output_options_set(HIGH_VOLTAGE_DRIVE_EN_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,HIGH_VOLTAGE_DRIVE_EN_PIN);
	gpio_bit_reset(HIGH_VOLTAGE_DRIVE_EN_GPIO,HIGH_VOLTAGE_DRIVE_EN_PIN);
}

/* afe alarm中断处理函数 */
afe_alarm_interrupt_callback afe_alarm_handle = NULL;

/**
 * @函数名称: gd32f4x_gpio_set_afe_alarm_callback()
 * @功能描述: 注册afe alarm中断回调处理函数
 * @输入: alarm_calback 中断回调处理函数
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_gpio_set_afe_alarm_callback(afe_alarm_interrupt_callback alarm_calback)
{
    if (NULL != alarm_calback) {
        afe_alarm_handle = alarm_calback;
    }
}

void gd32f4x_afe_alarm_irq_enable(void)
{
    exti_interrupt_enable(ALARM_GPIO_EXTI_X);
}

void gd32f4x_afe_alarm_irq_disable(void)
{
    exti_interrupt_disable(ALARM_GPIO_EXTI_X);
}

void gd32f4x_bus_high_voltage_irq_enable(void)
{
    exti_interrupt_enable(BUS_HIGH_VOLTAGE_EXTI_X);
}

void gd32f4x_bus_high_voltage_irq_disable(void)
{
    exti_interrupt_disable(BUS_HIGH_VOLTAGE_EXTI_X);
}


void gd32f4x_sky_irq_enable(void)
{
    exti_interrupt_enable(EN_SKY_EXTI_X);
}

void gd32f4x_sky_irq_disable(void)
{
    exti_interrupt_disable(EN_SKY_EXTI_X);
}

void gd32f4x_input_irq_enable(void)
{
    exti_interrupt_enable(MS0_DET_EXTI_X);
}

void gd32f4x_input_irq_disable(void)
{
    exti_interrupt_disable(MS0_DET_EXTI_X);
}
/**
 * @函数名称: gd32f4x_key_read()
 * @功能描述: 按键读取
 * @输入: 无
 * @输出: 无
 * @返回值: 电平0 1 
 */
uint8_t gd32f4x_key_read(void)
{
    if(0 == gpio_input_bit_get(EN_SKY_GPIO, EN_SKY_PIN)) {

        return 0;
    }
    return 1;
}


/* AFE告警触发的外部中断函数 */
void EXTI10_15_IRQHandler(void)
{
	uint32_t ulReturn;
    /* 进入临界代码段，让系统管理中断 */
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
    if (exti_interrupt_flag_get(ALARM_GPIO_EXTI_X) == SET) {

        /* 处理告警的回调函数 */
        if (afe_alarm_handle != NULL) {
            /* 中断回调函数 */
            afe_alarm_handle();
        }
        // DRIVER_LOG(" ALARM INPUT \r\n ");
        exti_interrupt_flag_clear(ALARM_GPIO_EXTI_X);
    }
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}


uint32_t exitirq_count = 0;
/* 母线电压检测的外部中断函数 以及后面添加的输入检测 */
void EXTI5_9_IRQHandler(void)
{
	uint32_t ulReturn;
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(exti_interrupt_flag_get(BUS_HIGH_VOLTAGE_EXTI_X) == SET)
	{
        // DRIVER_LOG("BUS_HIGH_VOLTAGE_EXTI_X INPUT\r\n");
        exitirq_count++;
		exti_interrupt_flag_clear(BUS_HIGH_VOLTAGE_EXTI_X);
	}
	if(exti_interrupt_flag_get(MS0_DET_EXTI_X) == SET)
	{
        // DRIVER_LOG("MS0_DET_EXTI_X INPUT\r\n");
		exti_interrupt_flag_clear(MS0_DET_EXTI_X);
	}
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}

/* 串口调试************************************************************************** */
/**
 * @函数名称: usart0_init()
 * @功能描述: usart0初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
// void gd32f4x_usart0_init(uint32_t baudrate) 
// {
//     rcu_periph_clock_enable(RCU_USART2);  /* 使能串口0的时钟*/
//     rcu_periph_clock_enable(RCU_GPIOC);
//     /* 接收引脚配置*/
//     gpio_mode_set(USART0_RX_GPIO, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART0_RX_PIN);  
//     /*	gpio_output_options_set(USART5_RX_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,USART5_RX_PIN);*/
//     /*设置GPIO输出模式和速度*/
//     gpio_af_set(USART0_RX_GPIO, GPIO_AF_7, USART0_RX_PIN);  

//     /* 发送引脚配置*/
//     gpio_mode_set(USART0_TX_GPIO, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART0_TX_PIN); 
//     gpio_output_options_set(USART0_TX_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART0_TX_PIN);  
//     gpio_af_set(USART0_TX_GPIO, GPIO_AF_7, USART0_TX_PIN);  

//     usart_deinit(USART2);                             
//     usart_oversample_config(USART2, USART_OVSMOD_8);  
//     usart_baudrate_set(USART2, baudrate);            
//     usart_word_length_set(USART2, USART_WL_8BIT); 
//     usart_stop_bit_set(USART2, USART_STB_1BIT);   
//     usart_parity_config(USART2, USART_PM_NONE);    

//     usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);  
//     usart_receive_config(USART2, USART_RECEIVE_ENABLE);   
//     usart_enable(USART2);                                 

//     usart_dma_receive_config(USART2, USART_DENR_ENABLE); 
//     usart_dma_transmit_config(USART2, USART_DENT_ENABLE); 

//     usart_interrupt_enable(USART2, USART_INT_IDLE);  
//     /*	usart_interrupt_enable(USART0, USART_INT_TBE);*/
    

//     nvic_irq_enable(USART2_IRQn, 3, 2);  
// }

void gd_eval_com_init(void)
{
	rcu_periph_clock_enable(RCU_USART2);  
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_mode_set(USART2_RX_GPIO, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART2_RX_PIN);  
    /*	gpio_output_options_set(USART5_RX_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,USART5_RX_PIN);*/
    gpio_af_set(USART2_RX_GPIO, GPIO_AF_7, USART2_RX_PIN);  

    gpio_mode_set(USART2_TX_GPIO, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART2_TX_PIN); 
    gpio_output_options_set(USART2_TX_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART2_TX_PIN);  
    gpio_af_set(USART2_TX_GPIO, GPIO_AF_7, USART2_TX_PIN);  

    usart_deinit(USART2);                             
    usart_oversample_config(USART2, USART_OVSMOD_8);  
    usart_baudrate_set(USART2, 115200);            
    usart_word_length_set(USART2, USART_WL_8BIT); 
    usart_stop_bit_set(USART2, USART_STB_1BIT);   
    usart_parity_config(USART2, USART_PM_NONE);    

    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);  
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);   
    usart_enable(USART2);                                 

    usart_dma_receive_config(USART2, USART_DENR_ENABLE); 
    usart_dma_transmit_config(USART2, USART_DENR_ENABLE); 

    usart_interrupt_enable(USART2, USART_INT_IDLE);  
    /*	usart_interrupt_enable(USART0, USART_INT_TBE);*/
    

    nvic_irq_enable(USART2_IRQn, 0, 0);  
}

/* 串口调试************************************************************************** */
/**
 * @函数名称: usart2_init()
 * @功能描述: usart2初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_usart2_init(uint32_t baudrate) 
{
    rcu_periph_clock_enable(RCU_USART2);  
    rcu_periph_clock_enable(RCU_GPIOD);   
    /* 接收引脚配置*/
    gpio_mode_set(USART2_RX_GPIO, GPIO_MODE_AF, GPIO_PUPD_PULLUP,USART2_RX_PIN);  /* 配置串口2的RX引脚复用功能，上拉输入*/
    /*	gpio_output_options_set(USART2_RX_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,USART2_RX_PIN);*/
    /*设置GPIO输出模式和速度*/
    gpio_af_set(USART2_RX_GPIO, GPIO_AF_7, USART2_RX_PIN); 

    /* 发送引脚配置*/
    gpio_mode_set(USART2_TX_GPIO, GPIO_MODE_AF, GPIO_PUPD_NONE, USART2_TX_PIN); 
    gpio_output_options_set(USART2_TX_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART2_TX_PIN);  
    gpio_af_set(USART2_TX_GPIO, GPIO_AF_7, USART2_TX_PIN);  

    usart_deinit(USART2);                          
    usart_oversample_config(USART2, USART_OVSMOD_8); 
    usart_baudrate_set(USART2, baudrate);             
    usart_word_length_set(USART2, USART_WL_8BIT);  
    usart_stop_bit_set(USART2, USART_STB_1BIT);    
    usart_parity_config(USART2, USART_PM_NONE);   

    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);  
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);    
    usart_enable(USART2);                                 

    usart_dma_receive_config(USART2, USART_DENR_ENABLE); 
    usart_dma_transmit_config(USART2, USART_DENT_ENABLE); 

    usart_flag_clear(USART2, USART_FLAG_TC);
    usart_flag_clear(USART2, USART_INT_IDLE);
    usart_interrupt_enable(USART2, USART_INT_IDLE);
    /*	usart_interrupt_enable(USART2, USART_INT_TBE);*/
  

    nvic_irq_enable(USART2_IRQn, 2, 4); 
                       
}

/* 串口调试************************************************************************** */
/**
 * @函数名称: usart3_init()
 * @功能描述: usart3初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_uart3_init(uint32_t baudrate)
{
    rcu_periph_clock_enable(RCU_UART3);
    rcu_periph_clock_enable(RCU_GPIOC); 
    /* 接收引脚配置*/
    gpio_mode_set(UART3_RX_GPIO, GPIO_MODE_AF, GPIO_PUPD_PULLUP, UART3_RX_PIN);  /* 配置串口2的RX引脚复用功能，上拉输入*/
    /*	gpio_output_options_set(USART2_RX_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,USART3_RX_PIN);*/
    /*设置GPIO输出模式和速度*/
    gpio_af_set(UART3_RX_GPIO, GPIO_AF_8, UART3_RX_PIN);  

    /*发送引脚配置*/
    gpio_mode_set(UART3_TX_GPIO, GPIO_MODE_AF, GPIO_PUPD_NONE, UART3_TX_PIN); 
    gpio_output_options_set(UART3_TX_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, UART3_TX_PIN); 
    gpio_af_set(UART3_TX_GPIO, GPIO_AF_8, UART3_TX_PIN);

    usart_deinit(UART3);                            
    usart_oversample_config(UART3, USART_OVSMOD_8); 
    usart_baudrate_set(UART3, baudrate);          
    usart_word_length_set(UART3, USART_WL_8BIT);
    usart_stop_bit_set(UART3, USART_STB_1BIT);
    usart_parity_config(UART3, USART_PM_NONE);

    usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
    usart_receive_config(UART3, USART_RECEIVE_ENABLE);   
    usart_enable(UART3);                                

    usart_dma_receive_config(UART3, USART_DENR_ENABLE); 
    usart_dma_transmit_config(UART3, USART_DENT_ENABLE);
    usart_flag_clear(UART3, USART_FLAG_TC);

    usart_interrupt_enable(UART3, USART_INT_IDLE);
    usart_interrupt_enable(UART3, USART_INT_RBNE);

    nvic_irq_enable(UART3_IRQn, 2, 4); 
                    
                     
}

void gd32f4x_usart0_enable(void)
{
    usart_enable(USART0);
}

void gd32f4x_usart0_disable(void)
{
    usart_disable(USART0);
}

void gd32f4x_usart2_enable(void)
{
    usart_enable(USART2);
}

void gd32f4x_usart2_disable(void)
{
    usart_disable(USART2);
}

void gd32f4x_uart3_enable(void)
{
    usart_enable(UART3);
}

void gd32f4x_uart3_disable(void)
{
    usart_disable(UART3);
}

/* 串口调试************************************************************************** */
/**
 * @函数名称: usart_init()
 * @功能描述: usart初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_usart_init(void) 
{
    gd32f4x_usart0_init(115200); 
	//gd32f4x_usart2_init(9600); 
	//gd32f4x_uart3_init(115200); 
}

/* 将gcc的C库printf函数重定向到USART */
int _write(int file, char* data, int len)
{
    if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
        errno = EBADF;
        return -1;
    }
    for (int i = 0; i < len; i++) {
        usart_data_transmit(USART2, (uint8_t)data[i]);
        while (RESET == usart_flag_get(USART2, USART_FLAG_TBE))
            ;
    }
    return len;
}


/* ************************************************************************** */
/**
 * @函数名称: usart0_send_data()
 * @功能描述: usart0发送一字节
 * @输入: ucch 要发送的数据
 * @输出: 无
 * @返回值: 无
 */
void usart0_send_data(uint8_t ucch)
{
	usart_data_transmit(USART0, (uint8_t)ucch);							 
	while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));  
}

/* ************************************************************************** */
/**
 * @函数名称: usart0_send_string()
 * @功能描述: usart0发送字符串
 * @输入: data 要发送的数据的地址 len 要发送的数据长度
 * @输出: 无
 * @返回值: 0 - 成功, 1 - 失败
 */
uint8_t usart0_send_string(uint8_t *data, uint32_t len)     
{  
    usart_flag_clear(USART0, USART_FLAG_TC);

    dma_channel_disable(DMA1, DMA_CH7);
    dma_flag_clear(DMA1, DMA_CH7, DMA_FLAG_FTF);
    dma_memory_address_config(DMA1, DMA_CH7, DMA_MEMORY_0, (uint32_t)data);
    dma_transfer_number_config(DMA1, DMA_CH7, len);
    dma_channel_enable(DMA1, DMA_CH7);
    
    while (usart_flag_get(USART0, USART_FLAG_TC) != RESET);
    return 0; // 成功
}

/* ************************************************************************** */
/**
 * @函数名称: usart2_send_string()
 * @功能描述: usart2发送字节
 * @输入: ucch 要发送的数据
 * @输出: 无
 * @返回值: 无
 */
void usart2_send_data(uint8_t ucch)
{
	usart_data_transmit(USART2, (uint8_t)ucch);							 
	while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));  
}

/* ************************************************************************** */
/**
 * @函数名称: usart2_send_string()
 * @功能描述: usart2发送字符串
 * @输入: data 要发送的数据的地址 len 要发送的数据长度
 * @输出: 无
 * @返回值: 0 - 成功, 1 - 失败
 */
uint8_t usart2_send_string(uint8_t *data, uint32_t len) 
{
    usart_flag_clear(USART2, USART_FLAG_TC);

    dma_channel_disable(DMA0, DMA_CH3);
    dma_flag_clear(DMA0, DMA_CH3, DMA_FLAG_FTF);
    dma_memory_address_config(DMA0, DMA_CH3, DMA_MEMORY_0, (uint32_t)data);
    dma_transfer_number_config(DMA0, DMA_CH3, len);
    dma_channel_enable(DMA0, DMA_CH3);
    
    while (usart_flag_get(USART2, USART_FLAG_TC) != RESET);
    return 0; // 成功
}

/* ************************************************************************** */
/**
 * @函数名称: usart3_send_string()
 * @功能描述: usart3发送字节
 * @输入: ucch 要发送的数据
 * @输出: 无
 * @返回值: 无
 */
void uart3_send_data(uint8_t ucch)    
{
	usart_data_transmit(UART3, (uint8_t)ucch);							 
	while(RESET == usart_flag_get(UART3, USART_FLAG_TBE));  
}

/* ************************************************************************** */
/**
 * @函数名称: usart3_send_string()
 * @功能描述: 串口3发送字符串数据函数
 * @输入: str 要发送的数据的地址 
 * @输出: 无
 * @返回值: 无
 */
void uart3_send_string(char *str) 
{
	while (*str) {
        while (usart_flag_get(UART3, USART_FLAG_TBE) == RESET); 
        usart_data_transmit(UART3, *str++);
    }
}

/* ************************************************************************** */
/**
 * @函数名称: gd32f4x_pc_communicate_rs485rxbuffadd()
 * @功能描述: 获取pc RS485接收缓冲区内的数据地址
 * @输入: 
 * @输出: 无
 * @返回值: 无
 */
uint8_t *gd32f4x_pc_communicate_rs485rxbuffadd(void)
{
	return Usart2RxBuff;
}

/* ************************************************************************** */
/**
 * @函数名称: gd32f4x_dcdc_communicate_rs485rxbuffadd()
 * @功能描述: 获取dcdc RS485接收缓冲区内的数据地址
 * @输入:  
 * @输出: 无
 * @返回值: 无
 */
uint8_t *gd32f4x_dcdc_communicate_rs485rxbuffadd(void)
{
	return Usart0RxBuff;
}

/* ************************************************************************** */
/**
 * @函数名称: gd32f4x_dcdc_communicate_rxbuffnumber()
 * @功能描述: 获取串口0接收的到的数据个数
 * @输入: 
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_dcdc_communicate_rxbuffnumber(uint16_t BuffNum)
{
	Usart0RxNum = BuffNum;
}

/* ************************************************************************** */
/**
 * @函数名称: gd32f4x_pc_communicate_rxbuffnumber()
 * @功能描述: 获取串口2接收的到的数据个数
 * @输入: 
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_pc_communicate_rxbuffnumber(uint16_t BuffNum)
{
	Usart2RxNum = BuffNum;
}

void gd32f4x_dcdc_communicate_txbuff(uint8_t *TxBuff,uint16_t BuffNum)
{
	uint16_t i = 0;
	for(i = 0;(i < BuffNum)&&(i < USART0_TXBUFF_COUNT_MAX);i++)
	{
		Usart0TxBuff[i] = TxBuff[i];
	}
	
	dma_transfer_number_config(DMA1, DMA_CH7, BuffNum);           


	dma_channel_enable(DMA1, DMA_CH7);							

}

void gd32f4x_pc_communicate_txbuff(uint8_t *TxBuff,uint16_t BuffNum)
{
	uint16_t i = 0;
	for(i = 0;(i < BuffNum)&&(i < USART2_TXBUFF_COUNT_MAX);i++)
	{
		Usart2TxBuff[i] = TxBuff[i];
	}
	
	dma_transfer_number_config(DMA0, DMA_CH1, BuffNum);          


	dma_channel_enable(DMA0, DMA_CH1);							

}


/* ************************************************************************** */
fun_usart_recive_callback uart0_receive_callback = NULL;
uint8_t gd32f4x_usart0_set_receive_callback(fun_usart_recive_callback receive_callback)
{
    if (NULL == receive_callback) {
        return 1;
    }

    uart0_receive_callback = receive_callback;
    return 0;
}
/**
 * @函数名称: USART0_IRQHandler()
 * @功能描述: 串口0中断函数
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void USART0_IRQHandler(void)
{
	/* BaseType_t RS485ARx_L;*/
    uint32_t data_len = 0;
    uint32_t ulReturn;   
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
    /* 如果是空闲中断 */
	if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) != RESET) {
		usart_interrupt_flag_clear(USART0,USART_INT_FLAG_IDLE);	
		
		(void)usart_data_receive(USART0);
		
		dma_channel_disable(DMA1, DMA_CH5);						
		dma_flag_clear(DMA1, DMA_CH5, DMA_INTF_FTFIF);		

		data_len = USART0_RXBUFF_COUNT_MAX-dma_transfer_number_get(DMA1, DMA_CH5);
        
	 	//gd32f4x_dcdc_communicate_rxbuffnumber(USART0_RXBUFF_COUNT_MAX-dma_transfer_number_get(DMA1, DMA_CH5));
        // printf(" USART0_IRQHandler read_len= %ld \r\n", data_len);
        // for (uint16_t i = 0; i < data_len; i++) {
        //     printf(" %x \r\n", Usart0RxBuff[i]);
        // }
        if (uart0_receive_callback) {
            /*回调进入组包函数*/
            uart0_receive_callback(Usart0RxBuff, data_len);
        }
        dma_interrupt_flag_clear(DMA1,DMA_CH5,DMA_INT_FLAG_FTF);
		dma_channel_enable(DMA1, DMA_CH5); 
		      
	}

    /* 如果是发送完成中断 */
	if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE) != RESET) {
        DRIVER_LOG(" USART0 USART_INT_FLAG_RBNE IRQ \r\n");
		usart_interrupt_flag_clear(USART0,USART_INT_FLAG_RBNE);	
	} 	
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}

/* ************************************************************************** */
fun_usart_recive_callback usart2_receive_callback = NULL;
uint8_t gd32f4x_usart2_set_receive_callback(fun_usart_recive_callback receive_callback)
{
    if (NULL == receive_callback) {
        return 1;
    }

    usart2_receive_callback = receive_callback;
    return 0;
}
/**
 * @函数名称: USART2_IRQHandler()
 * @功能描述: 串口2中断函数
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
/*static u16 data[10] = {0};*/
void USART2_IRQHandler(void)
{
    	uint32_t ulReturn;
        ulReturn = taskENTER_CRITICAL_FROM_ISR();
        if (usart_interrupt_flag_get(USART2, USART_INT_FLAG_IDLE) != RESET) {
        usart_interrupt_flag_clear(USART2, USART_INT_FLAG_IDLE);
        (void)usart_data_receive(USART2);

        dma_channel_disable(DMA0, DMA_CH1);
        data_len = USART2_RXBUFF_COUNT_MAX - dma_transfer_number_get(DMA0, DMA_CH1);

        for (i = 0; i < data_len; i++) {
            ring_buffer[ring_head] = Usart2RxBuff[i];
            ring_head = (ring_head + 1) % RING_BUFFER_SIZE;
        }

        dma_flag_clear(DMA0, DMA_CH1, DMA_INTF_FTFIF);
        dma_transfer_number_config(DMA0, DMA_CH1, USART2_RXBUFF_COUNT_MAX);
        dma_channel_enable(DMA0, DMA_CH1);
    }
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}
/* 回调函数处理 */
fun_usart_recive_callback usart2_data_process(uint8_t* data, uint32_t length) {
    while (length > 0) {
        switch (ota_state) {
            case OTA_STATE_IDLE:
            case OTA_STATE_HEADER:
                while (length > 0 && header_received < 4) {
                    header_buf[header_received++] = *data++;
                    length--;
                }
                if (header_received == 4) {
                    memcpy(&expected_len, header_buf, 4);
                    printf("[OTA] Expected firmware length: %u\r\n", expected_len);
										time_flag = 1;
                    if (expected_len == 0 || expected_len > 0x3C000) {
                        ota_state = OTA_STATE_ERROR;
                        return;
                    }

                    flash_erase(BACKUP_ADDR, expected_len);
                    ota_state = OTA_STATE_RECEIVING;
                    total_received = 0;
                    block_idx = 0;
                    ota_data_ptr = ota_block;
                    time_flag = 1;
                    ota_last_tick = xTaskGetTickCount();
                }
                break;

            case OTA_STATE_RECEIVING: {
                uint32_t remain = expected_len - total_received;
                uint32_t to_copy = (length < remain) ? length : remain;

                while (to_copy-- > 0) {
                    *ota_data_ptr++ = *data++;
                    block_idx++;
                    total_received++;
                    length--;
                    if (block_idx >= OTA_BLOCK_SIZE || total_received == expected_len) {
                        flash_write(BACKUP_ADDR + total_received - block_idx, ota_block, block_idx);
                        block_idx = 0;
                        ota_data_ptr = ota_block;
                    }
                }
                if (total_received == expected_len) {
                    ota_state = OTA_STATE_CRC;
                    crc_received = 0;
                }
                break;
            }

            case OTA_STATE_CRC:
							printf("[OTA] Enter CRC state, crc_received=%d, length=%lu\r\n", crc_received, length);
                while (length > 0 && crc_received < 4) {
                    crc_buf[crc_received++] = *data++;
                    length--;
                }
                if (crc_received == 4) {
                    expected_crc = *(uint32_t*)crc_buf;
                    flash_write(BACKUP_ADDR + expected_len, crc_buf, 4);
                    ota_state = OTA_STATE_DONE;
                    ota_done = true;
                    printf("[OTA] OTA done. CRC = 0x%08X\r\n", expected_crc);
                }
                break;

            case OTA_STATE_DONE:
            case OTA_STATE_ERROR:
                return;
        }
    }
	printf("[OTA DEBUG] State: %d, total_received: %u, expected: %u\r\n", ota_state, total_received, expected_len);
}










/* ************************************************************************** */
fun_usart_recive_callback uart3_receive_callback = NULL;
uint8_t gd32f4x_uart3_set_receive_callback(fun_usart_recive_callback receive_callback)
{
    if (NULL == receive_callback) {
        return 1;
    }

    uart3_receive_callback = receive_callback;
    return 0;
}
/**
 * @函数名称: UART3_IRQHandler()
 * @功能描述: 串口3中断函数
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
//static u16 data[10] = {0};
void UART3_IRQHandler(void)
{
    uint32_t data_len = 0;
    uint32_t ulReturn;
	ulReturn = taskENTER_CRITICAL_FROM_ISR();

	if(usart_interrupt_flag_get(UART3, USART_INT_FLAG_IDLE) != RESET)
	{
		usart_interrupt_flag_clear(UART3,USART_INT_FLAG_IDLE);	
		(void)usart_data_receive(UART3);

		dma_channel_disable(DMA0, DMA_CH2);					
		dma_flag_clear(DMA0, DMA_CH2, DMA_INTF_FTFIF);			
        
        data_len = (UART3_RXBUFF_COUNT_MAX-dma_transfer_number_get(DMA0, DMA_CH2));
        if (uart3_receive_callback) {
            uart3_receive_callback(Uart3RxBuff, data_len);
        }
        /*  ATcmd_server();
            printf("cmd run over over over\r\n");
		    MCU_Drive_External_RS485RxBuffNum(USART2_RXBUFF_COUNT_MAX-dma_transfer_number_get(DMA0, DMA_CH2));获取接收到的字节数 485任务才会用到*/
		dma_interrupt_flag_clear(DMA0,DMA_CH2,DMA_INT_FLAG_FTF);
		dma_channel_enable(DMA0, DMA_CH2); 

	}
	
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}

/**
 * @函数名称: usart0_send_msg()
 * @功能描述: usart0设备发送消息功能
 * @输入: 数据：data 长度：len
 * @输出: 无
 * @返回值: 0 - 成功, 1 - 失败
 */
uint8_t usart0_send_msg(uint8_t* data, uint16_t len)
{
    /* 先做长度判断以提前返回错误 */
    if (len > USART0_TXBUFF_COUNT_MAX || len == 0) {
        /* 长度错误 */
        return 1; // 失败
    }
    uint16_t count = 0;
    for (count = 0; count < len; count++) {
        Usart0TxBuff[count] = data[count];
    }

    dma_transfer_number_config(DMA1, DMA_CH7, len);
    dma_channel_enable(DMA1, DMA_CH7);

    return 0; // 成功
}

/**
 * @函数名称: usart2_send_msg()
 * @功能描述: usart2设备发送消息功能
 * @输入: 数据：data 长度：len
 * @输出: 无
 * @返回值: 0 - 成功, 1 - 失败
 */
uint8_t usart2_send_msg(uint8_t* data, uint16_t len)
{
    /* 先做长度判断以提前返回错误 */
    if (len > USART2_TXBUFF_COUNT_MAX || len == 0) {
        /* 长度错误 */
        return 1; // 失败
    }
    uint16_t count = 0;
    for (count = 0; count < len; count++) {
        Usart2TxBuff[count] = data[count];
    }

    dma_transfer_number_config(DMA0, DMA_CH3, len);
    dma_channel_enable(DMA0, DMA_CH3);

    return 0; // 成功
}


/* ************************************************************************** */
/**
 * @函数名称: gd32f4x_dma_init()
 * @功能描述: DMA初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_dma_init(void)
{
	dma_single_data_parameter_struct dma_single_data_parameter;
	
	rcu_periph_clock_enable(RCU_DMA0);
	rcu_periph_clock_enable(RCU_DMA1);

	
	/* 串口0 DMA codes------------------------------------------------------------------*/
	/*串口0 数据发送使用DMA1的通道7，外设4    数据接收使用DAM1的通道2，外设4*/
	dma_deinit(DMA1, DMA_CH5);/*复位DMA1的通道5,DMA1的通道2用于串口0的接收 RX*/
	dma_deinit(DMA1, DMA_CH7);/*复位DMA1的通道7,DMA1的通道7用于串口0的发送 TX*/
	
	usart_interrupt_flag_clear(USART0,USART_INT_FLAG_TC);										/*清除串口0的发送中断完成标志位*/
	
	/*串口0 数据发送使用DAM1的通道7，外设4*/
	dma_single_data_parameter.periph_addr 					    = (uint32_t)(&USART_DATA(USART0));			
	dma_single_data_parameter.periph_inc 						= DMA_PERIPH_INCREASE_DISABLE;		
	dma_single_data_parameter.memory0_addr 					    = (uint32_t)&Usart0TxBuff[0];				
	dma_single_data_parameter.memory_inc 						= DMA_MEMORY_INCREASE_ENABLE;			
	dma_single_data_parameter.periph_memory_width 	            = DMA_PERIPH_WIDTH_8BIT;			
    dma_single_data_parameter.direction 						= DMA_MEMORY_TO_PERIPH;					
	dma_single_data_parameter.number 							= USART0_TXBUFF_COUNT_MAX;			
	dma_single_data_parameter.priority 							= DMA_PRIORITY_ULTRA_HIGH;					
	dma_single_data_mode_init(DMA1, DMA_CH7, &dma_single_data_parameter);

	dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI4);	

    dma_interrupt_enable(DMA1,DMA_CH7,DMA_CHXCTL_FTFIE);

    nvic_irq_enable(DMA1_Channel7_IRQn, 2, 0);	

    // dma_circulation_enable(DMA1, DMA_CH7);	
													
	/*串口0 数据接收使用DAM1的通道5，外设4*/
	dma_single_data_parameter.periph_addr 					    = (uint32_t)(&USART_DATA(USART0));			
	dma_single_data_parameter.periph_inc 						= DMA_PERIPH_INCREASE_DISABLE;		
	dma_single_data_parameter.memory0_addr 					    = (uint32_t)&Usart0RxBuff[0];					
	dma_single_data_parameter.memory_inc 						= DMA_MEMORY_INCREASE_ENABLE;			
	dma_single_data_parameter.periph_memory_width 	            = DMA_PERIPH_WIDTH_8BIT;					
    dma_single_data_parameter.direction 						= DMA_PERIPH_TO_MEMORY;						
	dma_single_data_parameter.number 							= USART0_RXBUFF_COUNT_MAX;				
	dma_single_data_parameter.priority 							= DMA_PRIORITY_HIGH;					   
	dma_single_data_mode_init(DMA1, DMA_CH5, &dma_single_data_parameter);
	
	dma_circulation_enable(DMA1, DMA_CH5);																						

	dma_channel_subperipheral_select(DMA1, DMA_CH5, DMA_SUBPERI4);										
	
	dma_transfer_number_config(DMA1, DMA_CH5, USART0_RXBUFF_COUNT_MAX);     					
	
	// dma_circulation_disable(DMA1, DMA_CH5);		
	
	

	/* 串口2 DMA codes------------------------------------------------------------------*/
	dma_deinit(DMA0, DMA_CH1);
	dma_deinit(DMA0, DMA_CH3);
	
	/*串口2 TX 数据发送使用DMA0的通道3，外设4 数据接收使用DAM0的通道1，外设4*/
	usart_interrupt_flag_clear(USART2,USART_INT_FLAG_TC);										
	
	dma_single_data_parameter.periph_addr 					    = (uint32_t)(&USART_DATA(USART2));		
	dma_single_data_parameter.periph_inc 						= DMA_PERIPH_INCREASE_DISABLE;		
	dma_single_data_parameter.memory0_addr 					    = (uint32_t)&Usart2TxBuff[0];					
	dma_single_data_parameter.memory_inc 						= DMA_MEMORY_INCREASE_ENABLE;			
	dma_single_data_parameter.periph_memory_width 	            = DMA_PERIPH_WIDTH_8BIT;				
    dma_single_data_parameter.direction 						= DMA_MEMORY_TO_PERIPH;					
	dma_single_data_parameter.number 							= USART2_TXBUFF_COUNT_MAX;				
	dma_single_data_parameter.priority 							= DMA_PRIORITY_MEDIUM;						
	dma_single_data_mode_init(DMA0, DMA_CH3, &dma_single_data_parameter);

	dma_channel_subperipheral_select(DMA0, DMA_CH3, DMA_SUBPERI4);										
	dma_interrupt_enable(DMA0,DMA_CH3,DMA_CHXCTL_FTFIE);															
	nvic_irq_enable(DMA0_Channel3_IRQn, 1, 0);																		
	
	/*串口2 RX 数据接收使用DAM0的通道1，外设4*/
	dma_single_data_parameter.periph_addr 					    = (uint32_t)(&USART_DATA(USART2));		
	dma_single_data_parameter.periph_inc 						= DMA_PERIPH_INCREASE_DISABLE;		
	dma_single_data_parameter.memory0_addr 					    = (uint32_t)&Usart2RxBuff[0];				
	dma_single_data_parameter.memory_inc 						= DMA_MEMORY_INCREASE_ENABLE;			
	dma_single_data_parameter.periph_memory_width 	            = DMA_PERIPH_WIDTH_8BIT;				
    dma_single_data_parameter.direction 						= DMA_PERIPH_TO_MEMORY;					
	dma_single_data_parameter.number 							= USART2_RXBUFF_COUNT_MAX;				
	dma_single_data_parameter.priority 							= DMA_PRIORITY_HIGH;					
	dma_single_data_mode_init(DMA0, DMA_CH1, &dma_single_data_parameter);
	
	dma_circulation_enable(DMA0, DMA_CH1);																					

	dma_channel_subperipheral_select(DMA0, DMA_CH1, DMA_SUBPERI4);										
	
	dma_transfer_number_config(DMA0, DMA_CH1, USART2_RXBUFF_COUNT_MAX);     					
	
	dma_channel_enable(DMA0, DMA_CH1);																								

    /* 串口3 DMA codes------------------------------------------------------------------*/
    /*串口3 RX 数据接收使用DAM0的通道2，外设4*/
	dma_single_data_parameter.periph_addr 					    = (uint32_t)(&USART_DATA(UART3));			
	dma_single_data_parameter.periph_inc 						= DMA_PERIPH_INCREASE_DISABLE;		
	dma_single_data_parameter.memory0_addr 					    = (uint32_t)&Uart3RxBuff[0];				
	dma_single_data_parameter.memory_inc 						= DMA_MEMORY_INCREASE_ENABLE;		
	dma_single_data_parameter.periph_memory_width 	            = DMA_PERIPH_WIDTH_8BIT;					
    dma_single_data_parameter.direction 						= DMA_PERIPH_TO_MEMORY;						
	dma_single_data_parameter.number 							= UART3_RXBUFF_COUNT_MAX;				
	dma_single_data_parameter.priority 							= DMA_PRIORITY_HIGH;					  
	dma_single_data_mode_init(DMA0, DMA_CH2, &dma_single_data_parameter);
	
	dma_circulation_enable(DMA0, DMA_CH2);																						

	dma_channel_subperipheral_select(DMA0, DMA_CH2, DMA_SUBPERI4);									
	
	dma_transfer_number_config(DMA0, DMA_CH2, UART3_RXBUFF_COUNT_MAX);     				
	
	dma_channel_enable(DMA0, DMA_CH2);		

}




/* ************************************************************************** */
/**
 * @函数名称: gd32f4x_dma1ch0_init()
 * @功能描述: 针对ADC通道的DMA初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_dma1ch0_init(void)
{
    rcu_periph_clock_enable(RCU_DMA1);
	dma_single_data_parameter_struct dma_single_data_parameter;
	dma_deinit(DMA1,DMA_CH0);
	
	dma_single_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));	
	dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_single_data_parameter.memory0_addr = (uint32_t)(&uiDMAData); 
	dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
	dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY; 
	dma_single_data_parameter.number = (uint32_t)DMA1_CH0_BUFF_MAX;
	dma_single_data_parameter.circular_mode = DMA_CIRCULAR_MODE_ENABLE;
	dma_single_data_parameter.priority = DMA_PRIORITY_HIGH;
	dma_single_data_mode_init(DMA1,DMA_CH0,&dma_single_data_parameter);
	
	dma_channel_subperipheral_select(DMA1, DMA_CH0,DMA_SUBPERI0);

	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
	nvic_irq_enable(DMA1_Channel0_IRQn, 1, 1);
	
	dma_circulation_enable(DMA1, DMA_CH0);
	dma_interrupt_enable(DMA1, DMA_CH0,DMA_CHXCTL_FTFIE);

	 
	dma_channel_enable(DMA1, DMA_CH0);
}

/* ************************************************************************** */
/**
 * @函数名称: DMA0_Channel3_IRQHandler()
 * @功能描述:  MCU的DMA0的通道3中断函数   USART2  对外采集器、上位机等
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void DMA0_Channel3_IRQHandler(void)
{
	if(dma_interrupt_flag_get(DMA0, DMA_CH3, DMA_INT_FLAG_FTF))
	{
		dma_channel_disable(DMA0, DMA_CH3);								
		
		dma_interrupt_flag_clear(DMA0, DMA_CH3, DMA_INT_FLAG_FTF);     
	}
}

/* ************************************************************************** */
/**
 * @函数名称: DMA1_Channel7_IRQHandler()
 * @功能描述: MCU的DMA4的通道7中断函数   USART0  对内底板
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */  
void DMA1_Channel7_IRQHandler(void)
{
	if(dma_flag_get(DMA1, DMA_CH7, DMA_INT_FLAG_FTF) == SET)
	{
		dma_channel_disable(DMA1, DMA_CH7);								
		dma_flag_clear(DMA1, DMA_CH7, DMA_INT_FLAG_FTF);    
	}
}

/* ************************************************************************** */
/**
 * @函数名称: DMA1_Channel0_IRQHandler()
 * @功能描述: MCU的DMA1的通道0中断函数  ADC中断在10S中是用于切换片选开关的
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */  
void DMA1_Channel0_IRQHandler(void)
{
    // uint8_t i = 0;
	if(dma_flag_get(DMA1, DMA_CH0, DMA_INT_FLAG_FTF) == SET)
	{
        // printf("adc[%d]=%ld adc[%d]=%ld adc[%d]=%ld adc[%d]=%ld adc[%d]=%ld \r\n",0,uiDMAData[0],1,uiDMAData[1],2,uiDMAData[2],3,uiDMAData[3],4,uiDMAData[4]);
        // for(int i=0;i<5;i++){
        //     printf("adc[%d] = %ld\r\n",i,uiDMAData[i]);
        // }
		dma_flag_clear(DMA1, DMA_CH0, DMA_INT_FLAG_FTF);     
		
	}
} 


/* ************************************************************************** */
/**
 * @函数名称: gd32f4x_adc_init()
 * @功能描述: ADC初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_adc_init(void)
{
    rcu_periph_clock_enable(ADC3_GPIO_CLOCK);
    rcu_periph_clock_enable(ADC4_GPIO_CLOCK);
    rcu_periph_clock_enable(ADC9_GPIO_CLOCK);
    rcu_periph_clock_enable(ADC11_GPIO_CLOCK);
    rcu_periph_clock_enable(ADC12_GPIO_CLOCK);
	adc_deinit();
	/*ADC的GPIO管脚复用功能配置*/
/*	gpio_mode_set(ADC1_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC1_PIN);
	gpio_mode_set(ADC2_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC2_PIN);*/
    /*J4的SENSE+上，采集电池放电时电流值，正向参考，mV值*/
	gpio_mode_set(ADC3_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC3_PIN);
    /*CAN，485通讯芯片供电电源的电压采集，V=Vadc*（338.2/8.2）*/
	gpio_mode_set(ADC4_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC4_PIN);
/*	gpio_mode_set(ADC5_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC5_PIN);
	gpio_mode_set(ADC6_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC6_PIN);
	gpio_mode_set(ADC7_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC7_PIN);
	gpio_mode_set(ADC8_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC8_PIN);*/
    /*NTC4*/
	gpio_mode_set(ADC9_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC9_PIN);
/*	gpio_mode_set(ADC10_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC10_PIN);*/
    /*BOARD_TEMP*/
	gpio_mode_set(ADC11_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC11_PIN);
    /*输入，ADC12_IN12，采集电池总电压（BAT+)*/
	gpio_mode_set(ADC12_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC12_PIN);
/*	gpio_mode_set(ADC13_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC13_PIN);
	gpio_mode_set(ADC14_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC14_PIN);
	gpio_mode_set(ADC15_GPIO,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC15_PIN);*/


	rcu_periph_clock_enable(RCU_ADC0);	

	adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
	
	adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
	adc_special_function_config(ADC0,ADC_SCAN_MODE,ENABLE);
	adc_special_function_config(ADC0,ADC_CONTINUOUS_MODE,ENABLE);
	
	adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
	adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL,ADC0_CHANNEL_NUMBER);
	adc_resolution_config(ADC0,ADC_RESOLUTION_12B);
	adc_external_trigger_config(ADC0,ADC_REGULAR_CHANNEL,EXTERNAL_TRIGGER_DISABLE);
	
/*	adc_oversample_mode_disable(ADC0);//禁能ADC过采样
	adc_software_trigger_enable(ADC1,ADC_ROUTINE_CHANNEL);
	adc_end_of_conversion_config(ADC0,ADC_EOC_SET_SEQUENCE);
	adc_routine_channel_config(ADC1,0,ADC_CHANNEL_9,ADC_SAMPLETIME_56);
    adc_routine_channel_config(ADC1,1,ADC_CHANNEL_8,ADC_SAMPLETIME_56);
	adc_routine_channel_config(ADC1,2,ADC_CHANNEL_15,ADC_SAMPLETIME_56);
	adc_routine_channel_config(ADC1,3,ADC_CHANNEL_14,ADC_SAMPLETIME_56);
	adc_routine_channel_config(ADC1,4,ADC_CHANNEL_7,ADC_SAMPLETIME_56);
	adc_routine_channel_config(ADC1,5,ADC_CHANNEL_6,ADC_SAMPLETIME_56);
	adc_routine_channel_config(ADC1,6,ADC_CHANNEL_5,ADC_SAMPLETIME_56);*/
    adc_regular_channel_config(ADC0,0,ADC_CHANNEL_3,ADC_SAMPLETIME_56);
	adc_regular_channel_config(ADC0,1,ADC_CHANNEL_4,ADC_SAMPLETIME_56);
	adc_regular_channel_config(ADC0,2,ADC_CHANNEL_9,ADC_SAMPLETIME_56);
/*  adc_routine_channel_config(ADC1,10,ADC_CHANNEL_1,ADC_SAMPLETIME_56);          
	adc_routine_channel_config(ADC1,11,ADC_CHANNEL_13,ADC_SAMPLETIME_56);
	adc_routine_channel_config(ADC0,0,ADC_CHANNEL_10,ADC_SAMPLETIME_56);*/
    adc_regular_channel_config(ADC0,3,ADC_CHANNEL_11,ADC_SAMPLETIME_56);                                
    adc_regular_channel_config(ADC0,4,ADC_CHANNEL_12,ADC_SAMPLETIME_56);   
    /************* */
	gd32f4x_dma1ch0_init();	
	
	adc_dma_mode_enable(ADC0);
	adc_dma_request_after_last_enable(ADC0);
	
	adc_enable(ADC0);
	gd32f4x_delay_ms(1);
	adc_calibration_enable(ADC0);
	adc_software_trigger_enable(ADC0,ADC_REGULAR_CHANNEL);
}

/* ************************************************************************** */
/**
 * @函数名称: void gd32f4x_timer2_init(void)
 * @功能描述: TIMER2初始化 1个LED初始化 
 * @输入: usr_hz
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_timer2_init(uint32_t usr_hz)
{

	rcu_periph_clock_enable(LED_J49_6_RCU_CLOCK);

    gpio_mode_set(LED_J49_6_GPIO, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_J49_6_PIN);
	gpio_af_set(LED_J49_6_GPIO, LED_J49_6_AF, LED_J49_6_PIN);
    gpio_output_options_set(LED_J49_6_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_J49_6_PIN);

    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);   

	timer_parameter_struct timer_initpara;

	rcu_periph_clock_enable(RCU_TIMER2);
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);
	timer_struct_para_init(&timer_initpara);
	timer_deinit(TIMER2);
	
	timer_initpara.prescaler            = 100-1;
	timer_initpara.alignedmode          = TIMER_COUNTER_EDGE; 
	timer_initpara.counterdirection     = TIMER_COUNTER_UP; 
	timer_initpara.period               = 1000-1;
	timer_initpara.clockdivision        = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter    = 0; 
	timer_init(TIMER2,&timer_initpara);

    timer_oc_parameter_struct timer_ocintpara_d;
    timer_ocintpara_d.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara_d.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara_d.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara_d.outputnstate = TIMER_CCXN_ENABLE;
    timer_ocintpara_d.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara_d.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER2, TIMER_CH_3, &timer_ocintpara_d);
    timer_channel_output_mode_config(TIMER2, TIMER_CH_3, TIMER_OC_MODE_PWM0);
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_3, (20 * (SystemCoreClock / 2 / usr_hz) / 100));
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);

    /* 激活输出通道 */
    timer_channel_output_state_config(TIMER2, TIMER_CH_3, TIMER_CCX_ENABLE);
    timer_channel_complementary_output_state_config(TIMER2, TIMER_CH_3, TIMER_CCXN_DISABLE);
    timer_channel_output_polarity_config(TIMER2, TIMER_CH_3, TIMER_OC_POLARITY_HIGH);

    /* 启用TIMER2主输出 */
    timer_primary_output_config(TIMER2, ENABLE);

    /* 开启TIMER2计数 */
    timer_enable(TIMER2);
    gd32f4x_timer2_led_j49_6_set_duty(0);
}

/**
 * @函数名称: gd32f4x_timer2_channel_set_duty()
 * @功能描述: 呼吸灯初始化
 * @输入: LED_J49_6_CHANNEL
 *        占空比：0-100
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_timer2_channel_set_duty(uint16_t channel, uint16_t duty)
{
    uint32_t period = TIMER_CAR(TIMER2);
    uint32_t pulse = (duty * period) / 100;
    /* 更新占空比 */
    timer_channel_output_pulse_value_config(TIMER2, channel, pulse);
}


/**
 * @函数名称: gd32f4x_timer2_led_j49_6_set_duty()
 * @功能描述: LED(J49-6)占空比设置
 * @输入: duty 占空比：0-100
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_timer2_led_j49_6_set_duty(uint8_t duty)
{
    gd32f4x_timer2_channel_set_duty(LED_J49_6_CHANNEL, duty);
}


 
/* ************************************************************************** */
/**
 * @函数名称: void gd32f4x_timer2_init(void)
 * @功能描述: TIMER4初始化 100mS定时中断回调函数	 
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_timer4_init(void)
{
	timer_parameter_struct timer_initpara;
	
	rcu_periph_clock_enable(RCU_TIMER4);
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);
/*	timer_struct_para_init(&timer_initpara);将定时器结构体内参数配置成默认参数*/
	timer_deinit(TIMER4);//复位外设TIMER4
	
	timer_initpara.prescaler = 1000-1;
	timer_initpara.alignedmode = TIMER_COUNTER_EDGE; 
	timer_initpara.counterdirection  = TIMER_COUNTER_UP; 
	timer_initpara.period = 10000-1; 
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0; 
	timer_init(TIMER4,&timer_initpara);
	
	timer_auto_reload_shadow_enable(TIMER4);
	
	timer_interrupt_enable(TIMER4,TIMER_INT_UP);
	
	
	nvic_irq_enable(TIMER4_IRQn, 0, 5);
	
	timer_enable(TIMER4);
} 

/* ************************************************************************** */
/**
 * @函数名称: gd32f4x_timer0_init(uint16_t Prescaler, uint32_t Period, uint32_t Duty)
 * @功能描述: TIMER0初始化 蜂鸣器
 * @输入: Prescaler 预分频值 Period 计数值 Duty 占空比
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_timer0_init(uint16_t Prescaler, uint32_t Period, uint32_t BUZZER_Duty, uint32_t LED_Duty)
{
	rcu_periph_clock_enable(BUZZER_RCU_CLOCK);
	rcu_periph_clock_enable(LED_J49_7_RCU_CLOCK);

	/* LED(J49-7)，电量指示LED */
	gpio_mode_set(LED_J49_7_GPIO,GPIO_MODE_AF,GPIO_PUPD_NONE,LED_J49_7_PIN);
	gpio_output_options_set(LED_J49_7_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,LED_J49_7_PIN);
	gpio_af_set(LED_J49_7_GPIO, LED_J49_7_AF, LED_J49_7_PIN);
    gpio_bit_reset(LED_J49_7_GPIO,LED_J49_7_PIN);

    //蜂鸣器 
	 gpio_mode_set(BUZZER_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, BUZZER_PIN);
     gpio_output_options_set(BUZZER_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BUZZER_PIN);
     gpio_af_set(BUZZER_PORT, BUZZER_AF, BUZZER_PIN);
     gpio_bit_reset(BUZZER_PORT,BUZZER_PIN);


	timer_parameter_struct timer_initpara;
	timer_oc_parameter_struct timer_ocinitstructure;
	
	rcu_periph_clock_enable(RCU_TIMER0);									
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);	
	timer_struct_para_init(&timer_initpara);							
	timer_deinit(TIMER0);																
	
	timer_initpara.prescaler = Prescaler;										
	timer_initpara.alignedmode = TIMER_COUNTER_EDGE; 		
	timer_initpara.counterdirection  = TIMER_COUNTER_UP; 	    
	timer_initpara.period = (uint32_t)Period; 									
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0; 							
	timer_init(TIMER0,&timer_initpara);
	
	timer_auto_reload_shadow_enable(TIMER0);							
	
	/* -----------------------------------------------------------------------
    TIMER0 configuration to:
    generate 3 complementary PWM signals with 3 different duty cycles:
    TIMER0CLK is fixed to systemcoreclock, the TIMER0 prescaler is equal to 5400 so the 
    TIMER0 counter clock used is 20KHz.
    the three duty cycles are computed as the following description: 
    the channel 0 duty cycle is set to 25% so channel 1N is set to 75%.
    the channel 1 duty cycle is set to 50% so channel 2N is set to 50%.
    the channel 2 duty cycle is set to 75% so channel 3N is set to 25%.
  ----------------------------------------------------------------------- */
	timer_ocinitstructure.outputstate           = TIMER_CCX_ENABLE;					
	timer_ocinitstructure.outputnstate 			= TIMER_CCXN_ENABLE;			
	timer_ocinitstructure.ocpolarity   			= TIMER_OC_POLARITY_HIGH;		
	timer_ocinitstructure.ocnpolarity  			= TIMER_OCN_POLARITY_HIGH;	
	timer_ocinitstructure.ocidlestate  			= TIMER_OC_IDLE_STATE_LOW;	
	timer_ocinitstructure.ocnidlestate 			= TIMER_OCN_IDLE_STATE_LOW;	

	timer_channel_output_config(TIMER0,TIMER_CH_0,&timer_ocinitstructure);
	timer_channel_output_config(TIMER0,TIMER_CH_1,&timer_ocinitstructure);
	/*LED1*/
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,LED_Duty);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_0,TIMER_OC_SHADOW_ENABLE);
	
	/*蜂鸣器*/
	timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,BUZZER_Duty);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_1,TIMER_OC_SHADOW_ENABLE);

    timer_primary_output_config(TIMER0,ENABLE);

	timer_enable(TIMER0);
}

/**
 * @函数名称: gd32f4x_time0_channel_set_duty()
 * @功能描述: 呼吸灯初始化
 * @输入: 通道：LED_J49_7_CHANNEL
 *        占空比：0-100
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_timer0_channel_set_duty(uint16_t channel, uint16_t duty)
{
    uint32_t period = TIMER_CAR(TIMER0);
    uint32_t pulse = (duty * period) / 100;
    /* 更新占空比 */
    timer_channel_output_pulse_value_config(TIMER0, channel, pulse);
}


/**
 * @函数名称: gd32f4x_time0_led_j49_7_set_duty()
 * @功能描述: LED_J49_7占空比设置
 * @输入: duty 占空比：0-100
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_timer0_led_j49_7_set_duty(uint8_t duty)
{
    gd32f4x_timer0_channel_set_duty(LED_J49_7_CHANNEL, duty);
}


/**
 * @函数名称: gd32f4x_timer0ch1_set_hz()
 * @功能描述: 蜂鸣器频率更改
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_timer0ch1_set_hz(uint32_t new_hz)
{
    uint32_t old_arr = TIMER_CAR(TIMER0);
    uint32_t old_ccr = TIMER_CH1CV(TIMER0);
    /* 计算更改前的占空比 */
    float duty_cycle = (float)old_ccr / (old_arr + 1);
    uint32_t new_arr = (SystemCoreClock / 2 / new_hz) - 1;
    uint32_t new_ccr = (uint32_t)(duty_cycle * (new_arr + 1));
    /* 更新ARR来更新频率 */
    TIMER_CAR(TIMER0) = new_arr;
    /* 更新ARR会带来占空比的改变，更新crr来保持原有的占空比 */
    TIMER_CH1CV(TIMER0) = new_ccr;
}

/**
 * @函数名称: gd32f4x_timer0ch1_set_pwm_duty()
 * @功能描述: 蜂鸣器占空比更改
 * @输入: duty 占空比
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_timer0ch1_set_pwm_duty(uint8_t duty)
{
    uint32_t period = TIMER_CAR(TIMER0);
    uint32_t pulse = ((100 - duty) * period) / 100;
    /* 更新占空比 */
    timer_channel_output_pulse_value_config(TIMER0, BUZZER_CHANNEL, pulse);
}

/* ************************************************************************** */
/**
 * @函数名称: gd32f4x_timer8_init(uint16_t Prescaler, uint32_t Period, uint32_t Duty)
 * @功能描述: TIMER8初始化 风扇
 * @输入: Prescaler 预分频值 Period 计数值 Duty 占空比
 * @输出: 无
 * @返回值: 无
 */ 
void gd32f4x_timer8_init(uint16_t Prescaler, uint32_t Period, uint32_t Duty)
{
	rcu_periph_clock_enable(FAN_RCU_CLOCK);		
	gpio_mode_set(FAN_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, FAN_PIN);
    gpio_output_options_set(FAN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FAN_PIN);
    gpio_af_set(FAN_PORT, FAN_AF, FAN_PIN);
	
	timer_parameter_struct timer_initpara;
	timer_oc_parameter_struct timer_ocinitstructure;
	
	rcu_periph_clock_enable(RCU_TIMER8);									
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);	
	timer_struct_para_init(&timer_initpara);							
	timer_deinit(TIMER8);																	
	
	timer_initpara.prescaler = Prescaler;								
	timer_initpara.alignedmode = TIMER_COUNTER_EDGE; 			
	timer_initpara.counterdirection  = TIMER_COUNTER_UP; 	
	timer_initpara.period = (uint32_t)Period; 									
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0; 								
	timer_init(TIMER8,&timer_initpara);
	
	timer_auto_reload_shadow_enable(TIMER8);						
	
	/* -----------------------------------------------------------------------
    TIMER0 configuration to:
    generate 3 complementary PWM signals with 3 different duty cycles:
    TIMER0CLK is fixed to systemcoreclock, the TIMER0 prescaler is equal to 5400 so the 
    TIMER0 counter clock used is 20KHz.
    the three duty cycles are computed as the following description: 
    the channel 0 duty cycle is set to 25% so channel 1N is set to 75%.
    the channel 1 duty cycle is set to 50% so channel 2N is set to 50%.
    the channel 2 duty cycle is set to 75% so channel 3N is set to 25%.
  ----------------------------------------------------------------------- */
	timer_ocinitstructure.outputstate           = TIMER_CCX_ENABLE;					
	timer_ocinitstructure.outputnstate 			= TIMER_CCXN_ENABLE;				
	timer_ocinitstructure.ocpolarity   			= TIMER_OC_POLARITY_HIGH;		
	timer_ocinitstructure.ocnpolarity  			= TIMER_OCN_POLARITY_HIGH;	
	timer_ocinitstructure.ocidlestate  			= TIMER_OC_IDLE_STATE_LOW;	
	timer_ocinitstructure.ocnidlestate 			= TIMER_OCN_IDLE_STATE_LOW;	

	timer_channel_output_config(TIMER8,TIMER_CH_0,&timer_ocinitstructure);

    timer_channel_output_pulse_value_config(TIMER8,TIMER_CH_0,Duty);
    timer_channel_output_mode_config(TIMER8,TIMER_CH_0,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER8,TIMER_CH_0,TIMER_OC_SHADOW_ENABLE);
	
	timer_enable(TIMER8);

}

/**
 * @函数名称: gd32f4x_timer8ch0_set_hz()
 * @功能描述: 风扇频率更改
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_timer8ch0_set_hz(uint32_t new_hz)
{
    uint32_t old_arr = TIMER_CAR(TIMER8);
    uint32_t old_ccr = TIMER_CH0CV(TIMER8);
    /* 计算更改前的占空比 */
    float duty_cycle = (float)old_ccr / (old_arr + 1);
    uint32_t new_arr = (SystemCoreClock / 2 / new_hz) - 1;
    uint32_t new_ccr = (uint32_t)(duty_cycle * (new_arr + 1));
    /* 更新ARR来更新频率 */
    TIMER_CAR(TIMER8) = new_arr;
    /* 更新ARR会带来占空比的改变，更新crr来保持原有的占空比 */
    TIMER_CH0CV(TIMER8) = new_ccr;
}

/**
 * @函数名称: gd32f4x_timer8ch0_set_pwm_duty()
 * @功能描述: 风扇占空比更改
 * @输入: duty 占空比
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_timer8ch0_set_pwm_duty(uint8_t duty)
{
    uint32_t period = TIMER_CAR(TIMER8);
    uint32_t pulse = (duty * period) / 100;
    /* 更新占空比 */
    timer_channel_output_pulse_value_config(TIMER8, FAN_CHANNEL, pulse);
}

/* ************************************************************************** */
/**
 * @函数名称: gd32f4x_timer_init()
 * @功能描述: TIMER初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_timer_init(void)
{
    /*这三个TIMER是用于任务中断的*/
    gd32f4x_timer2_init(50);/*定时器2初始化,1ms定时中断一次 led6*/

	gd32f4x_timer4_init();/*定时器4初始化,100ms定时中断一次*/
	
	/*下面的两个TIMER是用于硬件中输出的分别控制蜂鸣器以及风扇的 led7 beep*/
	gd32f4x_timer0_init(400,100,100,50);/*定时器0初始化*/

	gd32f4x_timer8_init(100,1000,50);/*定时器8初始化*/
}
uint32_t timer4irq_flag = 0;
uint32_t timer4irq_count = 0;
uint32_t timer4irq_count_flag = 0;
// uint32_t duty_data = 0;
/* ************************************************************************** */
/**
 * @函数名称: TIMER4_IRQHandler()
 * @功能描述: TIMER4 100mS中断
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void TIMER4_IRQHandler(void)
{
    uint32_t ulReturn;
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
    if(SET == timer_interrupt_flag_get(TIMER4,TIMER_INT_UP))
	{
        /* 配合定时调试使用的 6.6调试 */
        // timer4irq_flag = 1;
        /* 呼吸灯测试 */        
        // if(duty_data == 100) {
        //     duty_data = 0;
        // }       
        // duty_data++;
        // printf("duty = %d\r\n",duty_data);
		timer_interrupt_flag_clear(TIMER4,TIMER_INT_UP);
    }   
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}



/* CAN************************************************************************** */
/* 初始化指针为空，防止野指针 */
static can0_receive_call_back_func can0_receive_call_back = NULL;
can0_error_handle_call_back_func can0_error_handle = NULL;

/* ************************************************************************** */
/**
 * @函数名称: gd32f4x_can0_init()
 * @功能描述: CAN0初始化
 * @输入: 波特率 ：baud
 *        500k  ：BAUD_500K
 *        250k  ：BAUD_250K
 *        100K  ：BAUD_100K
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_can0_init(uint32_t baud)
{
    /* can0配置结构体 */
    can_parameter_struct can_parameter;
    /* can0时钟初始化 */
    rcu_periph_clock_enable(RCU_CAN0);
    /* can0引脚初始化 */
    rcu_periph_clock_enable(CAN0_RCU_CLOCK);
    /* CAN_TX */
    gpio_mode_set(CAN0_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, CAN0_TX_PIN);
    gpio_output_options_set(CAN0_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, CAN0_TX_PIN);
    gpio_af_set(CAN0_TX_PORT, CAN0_TX_AF, CAN0_TX_PIN);
    /* CAN_RX */
    gpio_mode_set(CAN0_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, CAN0_RX_PIN);
    gpio_af_set(CAN0_RX_PORT, CAN0_RX_AF, CAN0_RX_PIN);

    can_deinit(CAN0);
    /* can配置 */
    can_parameter.working_mode = CAN_NORMAL_MODE;
    can_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
    can_parameter.time_segment_1 = CAN_BT_BS1_8TQ;
    can_parameter.time_segment_2 = CAN_BT_BS2_1TQ;
    can_parameter.time_triggered = DISABLE;
    can_parameter.auto_bus_off_recovery = ENABLE;
    can_parameter.auto_wake_up = ENABLE;
    can_parameter.rec_fifo_overwrite = ENABLE;
    can_parameter.trans_fifo_order = DISABLE;
    switch (baud) {
    case BAUD_100K:
        can_parameter.prescaler = 50;
        break;
    case BAUD_250K:
        can_parameter.prescaler = 20;
        break;
    case BAUD_500K:
        can_parameter.prescaler = 10;
        break;
     case BAUD_1000K:
        can_parameter.prescaler = 5;
        break;
    default:
        break;
    }
    can_init(CAN0, &can_parameter);

    can_filter_parameter_struct can_filter;

    /* 配置滤波器以接收所有消息 */
    can_filter.filter_number = 0;
    can_filter.filter_mode = CAN_FILTERMODE_MASK;
    can_filter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter.filter_list_high = 0x0000;
    can_filter.filter_list_low = 0x0000;
    can_filter.filter_mask_high = 0x0000;
    can_filter.filter_mask_low = 0x0000;
    can_filter.filter_fifo_number = CAN_FIFO0;
    can_filter.filter_enable = ENABLE;
    can_filter_init(&can_filter);

    nvic_irq_enable(CAN0_RX0_IRQn, 0, 4);
    can_interrupt_enable(CAN0, CAN_INT_RFNE0);

    // can_interrupt_disable(CAN0, CAN_INT_RFNE0);			/*失能CAN0接收非空中断*/
    // can_interrupt_disable(CAN0, CAN_INT_TME);			/*失能CAN0发送中断*/
}

void gd32f4x_can0_rx_irqn_enable(void)
{
    nvic_irq_enable(CAN0_RX0_IRQn, 1, 2);
}

void gd32f4x_can0_rx_irqn_disable(void)
{
    nvic_irq_disable(CAN0_RX0_IRQn);
}


/**
 * @函数名称: can0_check_whitelist_ef()
 * @功能描述: 允许通过can0的白名单扩展帧
 * @输入: 输入ID数组：filter_ids 输入id数量：num_filters
 * @输出: 无
 * @返回值: 0 - 成功, 1 - 失败
 */
uint8_t can0_check_whitelist_ef(uint32_t *filter_ids, uint8_t num_filters)
{
    /* 最大14个滤波器 */
    if (num_filters > MAX_FILTERS) {
        return 1; // 失败
    }
    can_filter_parameter_struct can_filter;
    uint8_t filter_number = 0;
    for (filter_number = 0; filter_number < num_filters; filter_number++) {
        can_filter.filter_number = filter_number;
        can_filter.filter_mode = CAN_FILTERMODE_MASK;
        can_filter.filter_bits = CAN_FILTERBITS_32BIT;
        can_filter.filter_list_high = (filter_ids[filter_number] >> 16) & 0xFFFF;
        can_filter.filter_list_low = filter_ids[filter_number] & 0xFFFF;
        can_filter.filter_mask_high = 0xFFFF;
        can_filter.filter_mask_low = 0x0000;
        can_filter.filter_fifo_number = CAN_FIFO0;
        can_filter.filter_enable = ENABLE;
        can_filter_init(&can_filter);
    }
    return 0; // 成功
}

/**
 * @函数名称: can0_receive_register()
 * @功能描述: can0接收数据注册函数
 * @输入: 输入一个函数
 * @输出: 无
 * @返回值: 0 - 成功, 1 - 失败
 */
uint8_t can0_receive_register(can0_receive_call_back_func call_back)
{
    if (call_back == NULL) {
        return 1; // 失败
    }
    if (can0_receive_call_back == NULL) {
        can0_receive_call_back = call_back;
    }
    return 0; // 成功
}

/**
 * @函数名称: can0_send_msg()
 * @功能描述: can0设备发送消息功能
 * @输入: canid:id 数据：data 长度：len
 * @输出: 无
 * @返回值: 0 - 成功, 1 - 失败
 */
uint8_t can0_send_msg(uint32_t id, uint8_t* data, uint8_t len)
{
    /* 先做长度判断以提前返回错误 */
    if (len > 8 || len == 0) {
        /* 长度错误 */
        return 1; // 失败
    }
    uint32_t error_count = 0;
    can_trasnmit_message_struct message;
    message.tx_sfid = 0;
    message.tx_efid = id;
    message.tx_ff = CAN_FF_EXTENDED;
    message.tx_ft = CAN_FT_DATA;
    message.tx_dlen = len;
    memcpy(message.tx_data, data, len);

    can_message_transmit(CAN0, &message);
    while (can_transmit_states(CAN0, 0) != CAN_TRANSMIT_OK) {
        error_count++;
        if (error_count >= 50000) {
            return 1; // 失败
        }
    }
    return 0; // 成功
}

/**
 * @函数名称: can0_error_handle_register()
 * @功能描述: can0错误处理注册函数
 * @输入: call_back - 错误处理回调函数
 * @输出: 无
 * @返回值: 0 - 注册成功, 1 - 注册失败
 */
uint8_t can0_error_handle_register(can0_error_handle_call_back_func call_back)
{
    if (call_back == NULL) {
        return 1; // 失败
    }
    if (can0_error_handle == NULL) {
        can0_error_handle = call_back;
    }
    return 0; // 成功
}

/**
 * @函数名称: can0_check_whitelist_sf()
 * @功能描述: can0白名单标准帧
 * @输入: filter_ids - 输入ID数组, num_filters - 输入ID数量
 * @输出: 无
 * @返回值: 0 - 注册成功, 1 - 注册失败
 */
uint8_t can0_check_whitelist_sf(uint32_t *filter_ids, uint8_t num_filters)
{
    /* 最大14个滤波器 */
    if (num_filters > MAX_FILTERS || num_filters <= MIN_FILTERS) {
        return 1; // 失败
    }
    can_filter_parameter_struct can_filter;
    uint8_t filter_number = 0;
    for (filter_number = 0; filter_number < num_filters; filter_number++) {
        can_filter.filter_number = filter_number;
        can_filter.filter_mode = CAN_FILTERMODE_MASK;
        /* 适配于标准帧处理 */
        can_filter.filter_bits = CAN_FILTERBITS_32BIT;
        /* 匹配高8位 */
        can_filter.filter_list_high = (filter_ids[filter_number] & 0x700) << 5;
        /* 不使用 */
        can_filter.filter_list_low = 0x0000;
        /* 掩码高3位有效 */
        can_filter.filter_mask_high = 0x0700 << 5;
        /* 不影响低位 */
        can_filter.filter_mask_low = 0x0000;
        can_filter.filter_fifo_number = CAN_FIFO0;
        can_filter.filter_enable = ENABLE;
        can_filter_init(&can_filter);
    }
    return 0; // 成功
}

T_CAN0_ERR_TYPDEF can0_err_param = {
    .init_flag      = CAN0_INIT_FLAG_NONE,          /* 初始化标志位 0未初始化 1初始化成功 2初始化失败 */
    .err_flag       = CAN0_ERR_NONE,                /* 发送错误标志位 0无 1错误（触发发送错误数阈值） */
    .no_communication_time_count = 0,               /* 无通信计时 */
    .no_communication_time_err_threshold = CAN0_ERR_NO_COMMUNICATION_TIME_THRESHOLD_DEFAULT, /* 无通信异常时间阈值s */
    .send_err_count = 0,                            /* 发送错误计数，成功一次清零，失败一次计数 */
    .send_err_num_threshold = CAN0_ERR_NUM_THRESHOLD_DEFAULT,/* 发送错误计数阈值，超过阈值置位发送错误标志位 */
    .err_callback = NULL,                           /* 异常回调 */
};


/**
 * @函数名称: can0_process_cycle()
 * @功能描述: can0周期处理函数，用于处理错误标志位，无通信时间计时，发送错误计数错误标志位
 * @输入: cycle_time:周期时间 单位s
 * @输出: 无
 * @返回值: 无
 */
 void can0_process_cycle(uint32_t cycle_time)
 {
    /* 计时器到达阈值，清除错误标志位 */
    if (can0_err_param.no_communication_time_count >= can0_err_param.no_communication_time_err_threshold) {
        can0_err_param.err_flag |= CAN0_ERR_NO_COMMUNICATION;
    } 
    else {
        can0_err_param.err_flag &= ~CAN0_ERR_NO_COMMUNICATION;
        can0_err_param.no_communication_time_count += cycle_time;
    }

    /* 发送错误计数超过阈值，置位错误标志位 */
    if (can0_err_param.send_err_count >= can0_err_param.send_err_num_threshold) {
        can0_err_param.err_flag |= CAN0_ERR_SEND;
    }
    else {
        can0_err_param.err_flag &= ~CAN0_ERR_SEND;
    }

    /* 错误回调处理 */
    if (can0_err_param.err_callback != NULL) {
        can0_err_param.err_callback(can0_err_param.err_flag);
    }
 }


/* 初始化指针为空，防止野指针 */
static can1_receive_call_back_func can1_receive_call_back = NULL;
can1_error_handle_call_back_func can1_error_handle = NULL;

/* ************************************************************************** */
/**
 * @函数名称: gd32f4x_can1_init()
 * @功能描述: CAN1初始化
 * @输入: 波特率 ：baud
 *        500k  ：BAUD_500K
 *        250k  ：BAUD_250K
 *        100K  ：BAUD_100K
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_can1_init(uint32_t baud)
{/* can1配置结构体 */
    can_parameter_struct can_parameter;
    /* can1时钟初始化 */
    rcu_periph_clock_enable(RCU_CAN1);
    /* can1引脚初始化 */
    rcu_periph_clock_enable(CAN1_RCU_CLOCK);
    /* CAN_TX */
    gpio_mode_set(CAN1_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, CAN1_TX_PIN);
    gpio_output_options_set(CAN1_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, CAN1_TX_PIN);
    gpio_af_set(CAN1_TX_PORT, CAN1_TX_AF, CAN1_TX_PIN);
    /* CAN_RX */
    gpio_mode_set(CAN1_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, CAN1_RX_PIN);
    gpio_af_set(CAN1_RX_PORT, CAN1_RX_AF, CAN1_RX_PIN);
    can_deinit(CAN1);
    /* can配置 */
    can_parameter.working_mode = CAN_NORMAL_MODE;
    can_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
    can_parameter.time_segment_1 = CAN_BT_BS1_8TQ;
    can_parameter.time_segment_2 = CAN_BT_BS2_1TQ;
    can_parameter.time_triggered = DISABLE;
    can_parameter.auto_bus_off_recovery = ENABLE;
    can_parameter.auto_wake_up = ENABLE;
    can_parameter.rec_fifo_overwrite = ENABLE;
    can_parameter.trans_fifo_order = DISABLE;
    switch (baud) {
    case BAUD_100K:
        can_parameter.prescaler = 50;
        break;
    case BAUD_250K:
        can_parameter.prescaler = 20;
        break;
    case BAUD_500K:
        can_parameter.prescaler = 10;
        break;
    case BAUD_1000K:
        can_parameter.prescaler = 5;
        break;
    default:
        break;
    }
    can_init(CAN1, &can_parameter);

    can_filter_parameter_struct can_filter;

    /* 配置滤波器以接收所有消息 */
    can_filter.filter_number = 15;
    can_filter.filter_mode = CAN_FILTERMODE_MASK;
    can_filter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter.filter_list_high = 0x0000;
    can_filter.filter_list_low = 0x0000;
    can_filter.filter_mask_high = 0x0000;
    can_filter.filter_mask_low = 0x0000;
    can_filter.filter_fifo_number = CAN_FIFO1;
    can_filter.filter_enable = ENABLE;
    can_filter_init(&can_filter);

    
    can_interrupt_enable(CAN1, CAN_INT_RFNE1);
    can_interrupt_enable(CAN1, CAN_INT_TME);
    nvic_irq_enable(CAN1_RX1_IRQn, 1, 2);
}

void gd32f4x_can1_rx_irqn_enable(void)
{
    nvic_irq_enable(CAN1_RX1_IRQn, 1, 2);
}

void gd32f4x_can1_rx_irqn_disable(void)
{
    nvic_irq_disable(CAN1_RX1_IRQn);
}

/**
 * @函数名称: CAN1_RX1_IRQHandler()
 * @功能描述: CAN1_RX1中断函数
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void CAN1_RX1_IRQHandler(void)
{
    /* 定义结构体 */
    can_receive_message_struct RxMessage = {0};
    uint32_t ulreturn;
    /* 临界保护 */
    ulreturn = taskENTER_CRITICAL_FROM_ISR(); 
    /* 接收数据到结构体 */
    can_message_receive(CAN1, CAN_FIFO1, &RxMessage);

    if (can1_receive_call_back != NULL) {
 
        if(atcommand_can_flag == 1)
        {
            can1_send_msg(0x0C000000, RxMessage.rx_data, sizeof(RxMessage.rx_data)); 
        }
        /* can0_receive_call_back(&RxMessage); */

        can1_receive_call_back(&RxMessage);
    }
    taskEXIT_CRITICAL_FROM_ISR(ulreturn); 
}


/**
 * @函数名称: can1_check_whitelist_ef()
 * @功能描述: 允许通过can1的白名单扩展帧
 * @输入: filter_ids - 输入ID数组, num_filters - 输入ID数量
 * @输出: 无
 * @返回值: 0 - 成功, 1 - 失败
 */
uint8_t can1_check_whitelist_ef(uint32_t *filter_ids, uint8_t num_filters)
{
    /* 最大14个滤波器 */
    if (num_filters > MAX_FILTERS) {
        return 1; // 失败
    }
    can_filter_parameter_struct can_filter;
    uint8_t filter_number = 0;
    for (filter_number = 0; filter_number < num_filters; filter_number++) {
        can_filter.filter_number = filter_number;
        can_filter.filter_mode = CAN_FILTERMODE_MASK;
        can_filter.filter_bits = CAN_FILTERBITS_32BIT;
        can_filter.filter_list_high = (filter_ids[filter_number] >> 16) & 0xFFFF;
        can_filter.filter_list_low = filter_ids[filter_number] & 0xFFFF;
        can_filter.filter_mask_high = 0xFFFF;
        can_filter.filter_mask_low = 0x0000;
        can_filter.filter_fifo_number = CAN_FIFO1;
        can_filter.filter_enable = ENABLE;
        can_filter_init(&can_filter);
    }
    return 0; // 成功
}

/**
 * @函数名称: can1_receive_register()
 * @功能描述: can1接收数据注册函数
 * @输入: call_back - 接收回调函数
 * @输出: 无
 * @返回值: 0 - 注册成功, 1 - 注册失败
 */
uint8_t can1_receive_register(can1_receive_call_back_func call_back)
{
    if (call_back == NULL) {
        return 1; // 失败
    }
    if (can1_receive_call_back == NULL) {
        can1_receive_call_back = call_back;
    }
    return 0; // 成功
}

/**
 * @函数名称: can1_send_msg()
 * @功能描述: can1设备发送消息功能
 * @输入: id - CAN ID, data - 数据, len - 数据长度
 * @输出: 无
 * @返回值: 0 - 成功, 1 - 长度超长, 2 - 发送失败
 */
uint8_t can1_send_msg(uint32_t id, uint8_t* data, uint8_t len)
{
    /* 先做长度判断以提前返回错误 */
    if (len > 8 || len == 0) {
        /* 长度错误 */
        return 1; // 长度超长
    }
    uint32_t error_count = 0;
    can_trasnmit_message_struct message;
    /* 设置消息结构体参数 */
    message.tx_sfid = 0;
    /* 通常扩展ID设为0 */
    message.tx_efid = id;
    /* 标准帧 */
    message.tx_ff = CAN_FF_EXTENDED;
    /* 数据帧 */
    message.tx_ft = CAN_FT_DATA;
    message.tx_dlen = len;
    memcpy(message.tx_data, data, len);
    /* 发送消息 */
    can_message_transmit(CAN1, &message);
    /* 轮询等待发送完成，带有限制的重试机制 */
    while (can_transmit_states(CAN1, 0) != CAN_TRANSMIT_OK) {
        error_count++;
        if (error_count >= 50000) {
            return 2; // 发送失败
        }
    }
    return 0; // 成功
}

/**
 * @函数名称: can1_error_handle_register()
 * @功能描述: can1错误处理注册函数
 * @输入: call_back - 错误处理回调函数
 * @输出: 无
 * @返回值: 0 - 注册成功, 1 - 注册失败
 */
uint8_t can1_error_handle_register(can1_error_handle_call_back_func call_back)
{
    if (call_back == NULL) {
        return 1; // 失败
    }
    if (can1_error_handle == NULL) {
        can1_error_handle = call_back;
    }
    return 0; // 成功
}

/**
 * @函数名称: can1_check_whitelist_sf()
 * @功能描述: can1白名单标准帧
 * @输入: filter_ids - 输入ID数组, num_filters - 输入ID数量
 * @输出: 无
 * @返回值: 0 - 注册成功, 1 - 注册失败
 */
uint8_t can1_check_whitelist_sf(uint32_t *filter_ids, uint8_t num_filters)
{
    /* 最大14个滤波器 */
    if (num_filters > MAX_FILTERS || num_filters <= MIN_FILTERS) {
        return 1; // 失败
    }
    can_filter_parameter_struct can_filter;
    uint8_t filter_number = 0;
    for (filter_number = 0; filter_number < num_filters; filter_number++) {
        can_filter.filter_number = filter_number;
        can_filter.filter_mode = CAN_FILTERMODE_MASK;
        /* 适配于标准帧处理 */
        can_filter.filter_bits = CAN_FILTERBITS_32BIT;
        /* 匹配高8位 */
        can_filter.filter_list_high = (filter_ids[filter_number] & 0x700) << 5;
        /* 不使用 */
        can_filter.filter_list_low = 0x0000;
        /* 掩码高3位有效 */
        can_filter.filter_mask_high = 0x0700 << 5;
        /* 不影响低位 */
        can_filter.filter_mask_low = 0x0000;
        can_filter.filter_fifo_number = CAN_FIFO1;
        can_filter.filter_enable = ENABLE;
        can_filter_init(&can_filter);
    }
    return 0; // 成功
}


/* 看门狗************************************************************************** */
/**
 * @函数名称: gd32f4x_fwdgt_init()
 * @功能描述: 独立看门狗初始化
 * @输入: prer:分频数:FWDGT_PSC_DIV4~FWDGT_PSC_DIV256
 *        分频因子 = 4 * 2^prer. 但最大值只能是256!
 *        rlr:自动重装载值,0~0XFFF.
 *        时间计算(大概):Tout=((4 * 2^) * rlr) / 32 (ms).
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_fwdgt_init(uint8_t prer, uint16_t rlr)
{
    /* 配置独立看门狗定时器 */
    fwdgt_write_enable();
    fwdgt_config(rlr, prer);
    fwdgt_counter_reload();
    fwdgt_enable();
}

/**
 * @函数名称: gd32f4x_fwdgt_feed()
 * @功能描述: 独立看门狗喂狗
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_fwdgt_feed(void)
{
    fwdgt_counter_reload();
}

void gd32f4x_fwdgt_disable(void)
{

}

/**
 * @函数名称: gd32f4x_wwdgt_init()
 * @功能描述: 窗口看门狗初始化
 * @输入: tr: T[6:0],计数器值 wr: W[6:0],窗口值
 *            fprer:分频系数,范围:WWDGT_CFG_PSC_DIV1 ~WWDGT_CFG_PSC_DIV8
 *            Fwwdg = PCLK1 / (4096 * 2^fprer). 一般PCLK1=50Mhz
 *            溢出时间 = (4096 * 2^fprer) * (tr - 0X3F) / PCLK1
 *            假设fprer = WWDGT_CFG_PSC_DIV8, tr = 7f, PCLK1 = 50Mhz
 *            则溢出时间 = 4096 * 8 * 64 / 50Mhz = 41.94ms
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_wwdgt_init(uint16_t tr, uint8_t wr, uint32_t fprer)
{
    /* 使能WWDGT时钟 */
    rcu_periph_clock_enable(RCU_WWDGT);
    /* 设置WWDGT计数器值、窗口值和预分频值 */
    wwdgt_config(tr, wr, fprer);
    /* 使能窗口看门狗定时器 */
    wwdgt_enable();
    /* 清除WWDGT提前唤醒中断标志位状态 */
    wwdgt_flag_clear();
    /* 使能窗口看门狗提前唤醒中断 */
    wwdgt_interrupt_enable();
    /* 抢占优先级2，响应优先级为3 */
    nvic_irq_enable(WWDGT_IRQn, 2, 3);
}

/**
 * @函数名称: gd32f4x_fwdgt_init_ms()
 * @功能描述: 独立看门狗溢出时间设置(最大32秒),设置独立看门狗后
 *           系统进入睡眠状态，长时间不喂狗会导致系统复位，这个
 *           问题在此处无法解决，后续采用RTC中断喂狗的方式，就不
 *           会产生这种问题。
 * @输入: tout_ms：毫秒
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_fwdgt_init_ms(uint32_t tout_ms)
{
    if (tout_ms == 0) {
        tout_ms = 7.8125;
    }
    uint16_t rlr = (uint16_t)(tout_ms / 7.8125);
    if (rlr > 0xFFF) {
        rlr = 0xFFF;
    }
    gd32f4x_fwdgt_init(FWDGT_PSC_DIV256, rlr);
}

/**
 * @函数名称: gd32f4x_wwdgt_init_ms()
 * @功能描述: 窗口看门狗溢出时间设置
 * @输入: tout_ms：毫秒
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_wwdgt_init_ms(uint32_t tout_ms)
{
    if(tout_ms>=41) {
        tout_ms=41;
    }
    if(tout_ms<=0) {
        tout_ms=1;
    }
    uint32_t temp=50000*tout_ms;

    uint16_t tr = (temp/32768)+63;
    
    gd32f4x_wwdgt_init(tr, 0X5F, WWDGT_CFG_PSC_DIV8);
}

/**
 * @函数名称: WWDGT_IRQHandler()
 * @功能描述: 窗口看门狗中断喂狗
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void WWDGT_IRQHandler(void)
{
    if (wwdgt_flag_get() != RESET) {
       wwdgt_counter_update(0x7F);
    }
    wwdgt_flag_clear();
}

/* RTC************************************************************************** */
rtc_parameter_struct rtc_time_structure;

/**
 * @函数名称: gd32f4x_rtc_init()
 * @功能描述: RTC时钟外设初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 0: 成功 -1：外部晶振未就绪 -2：同步失败
 */
int gd32f4x_rtc_init(void) 
{
    /**** 使能时钟单元 *****/
    /* 使能PMU电源管理单元时钟 */
    rcu_periph_clock_enable(RCU_PMU);
    /* 使能电源备份源，置位BKPWEN位 */
    pmu_backup_write_enable();

    /***配置外部时钟***/
    /* 使用外部晶振32.768K */
    rcu_osci_on(RCU_LXTAL); 
    /* 等待振荡器稳定标志位置位或振荡器起振超时 */
    if (rcu_osci_stab_wait(RCU_LXTAL) != SUCCESS) {
        /* 外部晶振未就绪 */
        return -1; 
    }
    /* 配置RTC时钟源为外部晶振32.768K */
    rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
    /* 设置分频 */
    rtc_time_structure.factor_asyn = 0X7F;
    rtc_time_structure.factor_syn = 0XFF;
    /* 使能RTC外设时钟 */
    rcu_periph_clock_enable(RCU_RTC);
    /* 等待RTC寄存器(RTC_TIME、RTC_DATE)与RTC的APB时钟同步 */
    if (rtc_register_sync_wait() != SUCCESS) {
        /* 同步失败 */
        return -2; 
    }
    #ifdef RTC_ALARM
    /* rtc闹钟中断初始化 */
    gd32f4x_rtc_init_alarm_interrupt();
    #endif
    #ifdef RTC_WEAKUP
    /* rtc自动唤醒定时器初始化 */
    gd32f4x_rtc_wakeup_init(0x0010);
    #endif
    
    return 0;
}

/**
 * @函数名称: gd32f4x_rtc_init_alarm_interrupt()
 * @功能描述: RTC闹钟中断配置
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_rtc_init_alarm_interrupt(void)
{    
    /* 使能中断 */
    rtc_interrupt_enable(RTC_INT_ALARM0);
    rtc_flag_clear(RTC_FLAG_ALRM0);
    exti_flag_clear(EXTI_17);
    exti_init(EXTI_17,EXTI_INTERRUPT,EXTI_TRIG_RISING);
    nvic_irq_enable(RTC_Alarm_IRQn,0,0); 
}

/**
 * @函数名称: gd32f4x_rtc_wakeup_init()
 * @功能描述: RTC自动唤醒定时器初始化
 * @输入: wakup_time_s 唤醒时间 单位s
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_rtc_wakeup_init(uint16_t wakup_time_s)
{
    rtc_wakeup_disable();
    /* 清空中断标志 */
    rtc_flag_clear(RTC_FLAG_WT);
    exti_flag_clear(EXTI_22);
    
    exti_interrupt_flag_clear(EXTI_22);    
    nvic_irq_enable(RTC_WKUP_IRQn, 3U, 0U);
    /* 上升沿 */
    exti_init(EXTI_22, EXTI_INTERRUPT, EXTI_TRIG_RISING); 
    /* 设置RTC自动唤醒定时器时钟 */
    rtc_wakeup_clock_set(WAKEUP_CKSPRE); 
    /* 自动唤醒定时器重载值 */
    rtc_wakeup_timer_set(wakup_time_s); 
    rtc_wakeup_enable();
    /* 使能中断 */
    rtc_interrupt_enable(RTC_INT_WAKEUP);
}

/**
 * @函数名称: RTC_Alarm_IRQHandler()
 * @功能描述: RTC闹钟中断服务函数
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void RTC_Alarm_IRQHandler(void)
{
    if (rtc_flag_get(RTC_FLAG_ALRM0) != RESET) {
        #ifdef RTC_PRINTF
        printf("alarm interrupt triggered!!!!\r\n");
        #endif
        exti_interrupt_flag_clear(EXTI_17);        
        rtc_flag_clear(RTC_FLAG_ALRM0);
    }
}

/**
 * @函数名称: RTC_WKUP_IRQHandler()
 * @功能描述: RTC自动定时唤醒中断服务函数
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void RTC_WKUP_IRQHandler(void)
{
    if (rtc_flag_get(RTC_FLAG_WT) != RESET) {
        #ifdef RTC_PRINTF
        printf("Wakeup timer interrupt triggered!\r\n");
        #endif
        rtc_flag_clear(RTC_FLAG_WT);
        exti_flag_clear(EXTI_22);
    }
}

/**
 * @函数名称: gd32f4x_mcu_sleep_enter()
 * @功能描述: mcu低功耗模式使能
 * @输入: 枚举变量三种模式：睡眠模式、深度睡眠模式、待机模式
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_mcu_sleep_enter(MCU_SLEEP_MODE_TYPEDEF mode)
{
    rcu_periph_clock_enable(RCU_PMU);
    switch (mode) {
    case MCU_MODE_SLEEP:
        pmu_to_sleepmode(WFI_CMD);
        break;
    case MCU_MODE_DEEP_SLEEP:
        pmu_to_deepsleepmode(PMU_LDO_LOWPOWER, WFI_CMD);
        break;
    case MCU_MODE_STANDBY:
        pmu_to_standbymode(WFI_CMD);
        break;
    default:
        break;
    }
}

/**
 * @函数名称: gd32f4x_after_sleep_init_func()
 * @功能描述: 深度睡眠后要恢复时钟频率（只有深度睡眠的时候才需要使用）
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_after_sleep_init_func(void)
{
    gd32f4x_system_clock_init();
}

gd32f4x_after_sleep_init_call_back gd32f4x_after_sleep_init;

/**
 * @函数名称: gd32f4x_after_sleep_init_register()
 * @功能描述: 深度睡眠后要恢复时钟频率注册
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
uint8_t gd32f4x_after_sleep_init_register(gd32f4x_after_sleep_init_call_back call_back)
{
    if (call_back == NULL) {
        return -1;
    }
    if (gd32f4x_after_sleep_init == NULL) {
        gd32f4x_after_sleep_init = call_back;
    }
    return 1;
}

/**
 * @函数名称: gd32f4x_key_wakeup_init()
 * @功能描述: 按键唤醒初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_key_wakeup_init(void)
{
    rcu_periph_clock_enable(RCU_SYSCFG);
    rcu_periph_clock_enable(KEY_WAKEUP_PIN_PERIPH);
    gpio_mode_set(KEY_WAKEUP_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, KEY_WAKEUP_PIN);
    syscfg_exti_line_config(KEY_WAKEUP_EXTI_PORT, KEY_WAKEUP_EXTI_PIN);
    exti_init(KEY_WAKEUP_EXTI, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_flag_clear(KEY_WAKEUP_EXTI);
    exti_interrupt_enable(EXTI_2);
    /* 按键中断的引脚和CAN引脚接在一起，CAN电信号进入后
        反转按键的引脚去复位，这个基于目前的硬件环境，CAN唤醒没有测
        使用的时候不要发送也不要接收，否则会频繁唤醒MCU，无法进入睡眠 */
    nvic_irq_enable(EXTI2_IRQn, 2, 3);
    pmu_wakeup_pin_enable();
}

/**
 * @函数名称: EXTI2_IRQHandler()
 * @功能描述: 按键中断
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void EXTI2_IRQHandler(void)
{
	uint32_t ulReturn;
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(exti_interrupt_flag_get(EN_SKY_EXTI_X) == SET)
	{
        DRIVER_LOG(" KEY INPUT \r\n");
        if (gd32f4x_after_sleep_init != NULL) {
            gd32f4x_after_sleep_init();
        }
		exti_interrupt_flag_clear(EN_SKY_EXTI_X);
	}
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}

/**
 * @函数名称: gd32f4x_key_wakeup_enable()
 * @功能描述: 按键唤醒使能
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_key_wakeup_enable(void)
{
    exti_interrupt_enable(EXTI_2);
    pmu_wakeup_pin_enable();
}

/**
 * @函数名称: gd32f4x_key_wakeup_disable()
 * @功能描述: 按键唤醒失能
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_key_wakeup_disable(void)
{
    pmu_wakeup_pin_disable();
    exti_interrupt_disable(EXTI_2);
}

/**
 * @函数名称: gd32f4x_can0_wakeup_init()
 * @功能描述: can0设备唤醒初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_can0_wakeup_init(void)
{
    // gd32f4x_can0_init(BAUD_500K);
    can_interrupt_enable(CAN0, CAN_INT_WAKEUP);
}

/**
 * @函数名称: gd32f4x_can0_wakeup_enable()
 * @功能描述: can0设备唤醒使能
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_can0_wakeup_enable(void)
{
    can_interrupt_enable(CAN0, CAN_INT_WAKEUP);
}

/**
 * @函数名称: gd32f4x_can0_wakeup_disable()
 * @功能描述: can0设备唤醒失能
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_can0_wakeup_disable(void)
{
    can_interrupt_disable(CAN0, CAN_INT_WAKEUP);
}


/**
 * @函数名称: gd32f4x_can1_wakeup_init()
 * @功能描述: can1设备唤醒初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_can1_wakeup_init(void)
{
    // gd32f4x_can1_init(BAUD_500K);
    can_interrupt_enable(CAN1, CAN_INT_WAKEUP);
}

/**
 * @函数名称: gd32f4x_can1_wakeup_enable()
 * @功能描述: can1设备唤醒使能
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_can1_wakeup_enable(void)
{
    can_interrupt_enable(CAN1, CAN_INT_WAKEUP);
}

/**
 * @函数名称: gd32f4x_can1_wakeup_disable()
 * @功能描述: can1设备唤醒失能
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void gd32f4x_can1_wakeup_disable(void)
{
    can_interrupt_disable(CAN1, CAN_INT_WAKEUP);
}

/*IIC************************************************************/
uint8_t iic1_delay_rate = 0;
 /**
 * @函数名称: gd32f4x_i2c1_init()
 * @功能描述: 设置IIC1口初始化 AFE
 * @输入: clock 是频率（单位：Hz）
 * @输出: 无
 * @返回值: 0:初始化成功；1：初始化失败
 */
uint8_t gd32f4x_i2c1_init(uint32_t clock)
{    
	uint8_t retval = 0;
	uint32_t delay_us = 0;

    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);

    gpio_mode_set(IIC1_PORT_SCL, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, IIC1_PIN_SCL);
    gpio_output_options_set(IIC1_PORT_SCL, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, IIC1_PIN_SCL);
     
    gpio_mode_set(IIC1_PORT_SDA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, IIC1_PIN_SDA);
    gpio_output_options_set(IIC1_PORT_SDA, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, IIC1_PIN_SDA);
    
	if (clock > 0) {
		/* 计算每个周期的延时， clock 是频率（单位：Hz）*/
		delay_us = 2000000 / clock; 
	}
	iic1_delay_rate = delay_us;
    // printf(" system_clock  %ld delay_us = %ld \r\n",rcu_clock_freq_get(CK_APB1),delay_us);  /* 打印系统时钟*/
	return retval;

}
 /**
 * @函数名称: gd32f4x_spi_init()
 * @功能描述: 设置SPI口初始化 
 * @输入: 无
 * @输出: 无
 * @返回值: 0:初始化成功；1：初始化失败
 */
uint8_t gd32f4x_spi_init(void)
{
	uint8_t retval = 0;
     
	/* 启用GPIO时钟*/
    rcu_periph_clock_enable(SPI_RCU_CLOCK);

	/* 配置SCLK引脚 (SPI时钟线)*/
	gpio_mode_set(SPI_PORT_SCLK, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SPI_PIN_SCLK);  /* 配置为输出*/
	gpio_output_options_set(SPI_PORT_SCLK, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI_PIN_SCLK);  /* 配置为推挽输出，50MHz速度*/
	
	/* 配置MOSI引脚 (SPI主输出从输入)*/
	gpio_mode_set(SPI_PORT_MOSI, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SPI_PIN_MOSI);  /*配置为输出*/
	gpio_output_options_set(SPI_PORT_MOSI, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI_PIN_MOSI);  /* 配置为推挽输出，50MHz速度*/
	
	/* 配置MISO引脚 (SPI主输入从输出)*/
	gpio_mode_set(SPI_PORT_MISO, GPIO_MODE_INPUT, GPIO_PUPD_NONE, SPI_PIN_MISO);  /*配置为输入*/
	/* 如果需要带上上拉或下拉电阻，可以修改 `GPIO_PUPD_NONE` 为 `GPIO_PUPD_PULLUP` 或 `GPIO_PUPD_PULLDOWN`
	 例如：gpio_mode_set(SPI_PORT_MISO, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, SPI_PIN_MISO);*/
	
	/* 配置CS引脚 (SPI片选线)*/
	gpio_mode_set(SPI_PORT_CS, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SPI_PIN_CS);  /* 配置为输出*/
	gpio_output_options_set(SPI_PORT_CS, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI_PIN_CS);  /*配置为推挽输出，50MHz速度*/
	
    gpio_bit_set(SPI_PORT_CS,SPI_PIN_CS); /*拉高片选，空闲状态	*/

	return retval;
}


/* 发送单个比特*/
uint8_t spi_send_bit(uint8_t data)
{
    uint8_t retval = 0;

    /* MSB，最高有效位发送数据*/
    if (data == 1)
        FLSH_SPI_MOSI(1); /* 输出高电平*/
    else
        FLSH_SPI_MOSI(0); /* 输出低电平*/

    /* 设置SCLK引脚为低电平，这个时候，从机开始读取MOSI引脚电平*/
    FLSH_SPI_SCLK(0); /* 输出低电平*/
    FLSH_SPI_US_DELAY();

    /* 设置SCLK引脚为高电平，告诉从机，主机准备采集MISO电平*/
    FLSH_SPI_SCLK(1); /* 输出高电平*/
    FLSH_SPI_US_DELAY();

    /* 主机采集MISO引脚电平*/
    if (FLSH_SPI_READ_SDA())
        retval = 1;

    return retval;
}

/* 发送一个字节*/
uint8_t spi_send_byte(uint8_t data)
{
    int32_t i = 0;
    uint8_t retval = 0;

    /* 发送字节，每次发送一个比特*/
    for (i = 7; i >= 0; i--)
    {
        retval |= (spi_send_bit((data >> i) & 1) << i); /* 发送bit并更新返回值*/
    }

    return retval;
}

/* 发送一个半字（16位）*/
uint16_t spi_send_halfword(uint16_t data)
{
    int32_t i = 0;
    uint16_t retval = 0;

    /* 发送半字，每次发送一个比特*/
    for (i = 15; i >= 0; i--)
    {
        retval |= (spi_send_bit((data >> i) & 1) << i); /* 发送bit并更新返回值*/
    }

    return retval;
}

/* 接收单个比特*/
uint8_t spi_receive_bit(void)
{
    uint8_t retval = 0;

    /*设置SCLK引脚为低电平，等待从机准备数据*/
    FLSH_SPI_SCLK(0); /* 输出低电平*/
    FLSH_SPI_US_DELAY();

    /* 设置SCLK引脚为高电平，准备采集数据*/
    FLSH_SPI_SCLK(1); /* 输出高电平*/
    FLSH_SPI_US_DELAY();

    /* 采集MISO引脚电平*/
    if (FLSH_SPI_READ_SDA())
        retval = 1; /* 从MISO读取到1*/
    else
        retval = 0; /* 从MISO读取到0*/

    return retval;
}

/* 接收一个字节*/
uint8_t spi_receive_byte(void)
{
    int32_t i = 0;
    uint8_t retval = 0;

    /* 接收字节，每次接收一个比特*/
    for (i = 7; i >= 0; i--)
    {
        retval |= (spi_receive_bit() << i); /* 接收bit并更新返回值*/
    }

    return retval;
}



/* 接收一个半字（16位）*/
uint16_t spi_receive_halfword(void)
{
    int32_t i = 0;
    uint16_t retval = 0;

    /* 接收半字，每次接收一个比特*/
    for (i = 15; i >= 0; i--)
    {
        retval |= (spi_receive_bit() << i); /* 接收bit并更新返回值*/
    }

    return retval;
}


struct McuInfo_t
{
	union
	{
		uint32_t Size;
		struct
		{
			uint16_t FlashSize;
			uint16_t SramSize;
		};
	}Storage;
	uint8_t UniqueId[12];
}McuInfo;

void gd32f4x_read_id(void)
{
    uint8_t i = 0;
    McuInfo.Storage.Size=*(uint32_t*)STORAGE_INFO;
    printf("sram size=%dk\r\n\r\n",McuInfo.Storage.SramSize);
    printf("flash size=%dk\r\n\r\n",McuInfo.Storage.FlashSize);
    printf("unique id:[");
    for(i=0;i<12;i++)
    {
        McuInfo.UniqueId[i]=*(uint8_t*)(UNIQUE_ID+i);
        printf(" %02x",McuInfo.UniqueId[i]);
    }
    printf("]\r\n");
}


/**
 * @brief   通过软件 SPI 同时发送并接收一个字节
 * @param   dat  要发送的字节
 * @return  接收到的字节
 */
uint8_t spi_read_write_byte(uint8_t data)
{
    uint8_t recv = 0;
    /* 从最高位到最低位 */
    for (int8_t i = 7; i >= 0; i--) {
        /* spi_send_bit 会：
         *   1) 把 (dat>>i)&1 写到 MOSI
         *   2) 拉低——延时——拉高产生时钟
         *   3) 读取 MISO 并返回这一位
         */
        recv |= (spi_send_bit((data >> i) & 1) << i);
    }
    return recv;
}

uint16_t rs485_datacrc16(uint8_t *puchMsg, uint16_t usDataLen)
{
    uint16_t CRC_high = 0xFF;    
    uint16_t CRC_low = 0xFF;    
    uint32_t Index;             

    /* 传输消息缓冲区*/
    while (usDataLen--)
    {
		if(puchMsg != NULL)
		{
			Index = CRC_high^*puchMsg++;   /*计算CRC*/
			CRC_high = CRC_low ^ ExternalCommRxA_CRCTable_high[Index];
			CRC_low = ExternalCommRxA_CRCTable_low[Index];
		}
		else
		{
			CRC_high = 0xFF;
			CRC_low = 0xFF;
			break;
		}
    }
    /*返回CRC校验值*/
    return (CRC_high  | (CRC_low<<8));/*return ((CRC_high << 8) | CRC_low);*/
}




