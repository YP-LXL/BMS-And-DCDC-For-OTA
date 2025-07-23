#ifndef GD32F4X_H
#define GD32F4X_H

/* gd32f4x驱动-记录 */
/* 系统时钟************************************************************************** */
/* USART调试************************************************************************** */
/* IO-输入/输出************************************************************************** */
/* 定时器************************************************************************** */
/* ADC************************************************************************** */
/* CAN************************************************************************** */
/* DMA************************************************************************** */
/* 低功耗睡眠************************************************************************** */
/* 看门狗************************************************************************** */
/* RTC************************************************************************** */

#include "common.h"
#include "ota.h"
// #include "parameter.h"
/* 系统时钟************************************************************************** */
void gd32f4x_delay_us(uint32_t nus);
void gd32f4x_delay_ms(uint16_t nms);
void gd32f4x_system_clock_init(void);


// extern uint32_t duty_data;

#define  MCU_GPIO_PIN(x,y)         x##y
#define  MCU_GPIO_(x,y)            x##y
#define  MCU_GPIO_EXTI_(x,y)       x##y

/* IO-输入/输出************************************************************************** */
/*输出,EN_SW1置高,Vbat开始给板卡通电,辅电工作*/
#define EN_SW1_APS_VBT_PIN        			MCU_GPIO_PIN(GPIO_PIN_,1)
#define EN_SW1_APS_VBT_GPIO       			MCU_GPIO_(GPIO,A)

/*输出,EN_SW2置高,Vbat开始防反接电路工作*/
#define EN_SW2_ARC_VBT_PIN        			MCU_GPIO_PIN(GPIO_PIN_,2)
#define EN_SW2_ARC_VBT_GPIO       			MCU_GPIO_(GPIO,A)

/*输出,485/CAN用的隔离芯片供电使能,常低,拉高一次即可正常供电,休眠或不使用时拉低一次才能断电(转换芯片供电)*/
#define PERIPHERAL_POWER_VDD_PIN        	MCU_GPIO_PIN(GPIO_PIN_,7)
#define PERIPHERAL_POWER_VDD_GPIO       	MCU_GPIO_(GPIO,A)

/*输出,J4的EH_DCH,EN_DCH置高,低压Vbat驱动端开始工作,升压输出*/
#define LOW_VOLTAGE_DRIVE_EN_PIN        	MCU_GPIO_PIN(GPIO_PIN_,10)
#define LOW_VOLTAGE_DRIVE_EN_GPIO       	MCU_GPIO_(GPIO,B)

/*输出,RS485(/RE)收发器的接收使能引脚*/
#define PERIPHERAL_RS485_EN_PIN        		MCU_GPIO_PIN(GPIO_PIN_,15)
#define PERIPHERAL_RS485_EN_GPIO       		MCU_GPIO_(GPIO,B)

/*输出,12V转8V电源VPRO的供电使能*/
#define VPRO12V8V_CONTROL_PIN           	MCU_GPIO_PIN(GPIO_PIN_,0)
#define VPRO12V8V_CONTROL_GPIO          	MCU_GPIO_(GPIO,C)

/*输入,J4的S_HV,高压存在后,S_HV=0,母线没高压S_HV=1*/
#define BUS_HIGH_VOLTAGE_STATE_PIN          MCU_GPIO_PIN(GPIO_PIN_,5)
#define BUS_HIGH_VOLTAGE_STATE_GPIO         MCU_GPIO_(GPIO,C)
#define BUS_HIGH_VOLTAGE_EXTI_X			    MCU_GPIO_EXTI_(EXTI_,5)

/*输入,插入检测*/
#define MS0_DET_STATE_PIN                   MCU_GPIO_PIN(GPIO_PIN_,7)
#define MS0_DET_STATE_GPIO                  MCU_GPIO_(GPIO,C)
#define MS0_DET_EXTI_X			            MCU_GPIO_EXTI_(EXTI_,7)

/*输出,发出插入信号*/
#define MS0_OUT_EN_PIN                      MCU_GPIO_PIN(GPIO_PIN_,8)
#define MS0_OUT_EN_GPIO                     MCU_GPIO_(GPIO,C)

/*输出,控制干接点继电器供电电源,高电平继电器动作,低电平继电器复位
#define DRY_CONTACT_RELAY_EN_PIN			MCU_GPIO_PIN(GPIO_PIN_,11)
#define DRY_CONTACT_RELAY_EN_GPIO  			MCU_GPIO_(GPIO,D)*/

/*AFESH367309的MOS强制控制引脚CTL,当该引脚输出低电平强制关闭充放电和预充MOS,
当该引脚输出高电平,MOS由AFE内部逻辑控制*/
#define	AFESH367309MOS_CONTR_PIN            MCU_GPIO_PIN(GPIO_PIN_,12)
#define	AFESH367309MOS_CONTR_GPIO           MCU_GPIO_(GPIO,D)

/*输入,ALARM AFE SH367309与MCU之间的通信引脚,AFE输出,MCU输入 ALARM*/
#define AFE_SH367309_ALARM_PIN		    	MCU_GPIO_PIN(GPIO_PIN_,13)
#define AFE_SH367309_ALARM_GPIO				MCU_GPIO_(GPIO,D)
#define ALARM_GPIO_EXTI_X					MCU_GPIO_EXTI_(EXTI_,13)

/*输出,SHIP 仓运状态控制*/
#define	WAREHOUSE_STAT_CONTR_PIN			MCU_GPIO_PIN(GPIO_PIN_,14)
#define	WAREHOUSE_STAT_CONTR_GPIO			MCU_GPIO_(GPIO,D)

/*输入,EN_SKY:开机按钮按下检测功能,开机按钮按下EN_SKY=0,没按下EN_SKY=1*/
#define EN_SKY_PIN          				MCU_GPIO_PIN(GPIO_PIN_,2)
#define EN_SKY_GPIO         				MCU_GPIO_(GPIO,E) 
#define EN_SKY_EXTI_X					    MCU_GPIO_EXTI_(EXTI_,2)

/*输出,EN_PWR:当开机按钮按下后辅电供电,系统自检OK后EN_PWRs输出1高,保持辅电一直提供*/
#define EN_PWR_PIN          				MCU_GPIO_PIN(GPIO_PIN_,3)
#define EN_PWR_GPIO         				MCU_GPIO_(GPIO,E) 

/*输出,J4的EH_CH,EN_CH置高,高压驱动端开始工作,降压输出*/
#define HIGH_VOLTAGE_DRIVE_EN_PIN           MCU_GPIO_PIN(GPIO_PIN_,15)
#define HIGH_VOLTAGE_DRIVE_EN_GPIO          MCU_GPIO_(GPIO,E)

/*PA0复位*/
#define PA0_OFF()                           gpio_bit_reset(MCU_GPIO_(GPIO,A),MCU_GPIO_PIN(GPIO_PIN_,0))

/*板卡通电,辅电控制开关*/
#define EN_SW1_APS_VBT_ON()                 gpio_bit_set(EN_SW1_APS_VBT_GPIO,EN_SW1_APS_VBT_PIN)
#define EN_SW1_APS_VBT_OFF()                gpio_bit_reset(EN_SW1_APS_VBT_GPIO,EN_SW1_APS_VBT_PIN)

/*防反接电路工作控制开关*/
#define EN_SW2_ARC_VBT_ON()                 gpio_bit_set(EN_SW2_ARC_VBT_GPIO,EN_SW2_ARC_VBT_PIN)
#define EN_SW2_ARC_VBT_OFF()                gpio_bit_reset(EN_SW2_ARC_VBT_GPIO,EN_SW2_ARC_VBT_PIN)

/*485/CAN用的隔离芯片供电使能控制开关*/
#define PERIPHERAL_POWER_VDD_ON()           gpio_bit_set(PERIPHERAL_POWER_VDD_GPIO,PERIPHERAL_POWER_VDD_PIN)
#define PERIPHERAL_POWER_VDD_OFF()          gpio_bit_reset(PERIPHERAL_POWER_VDD_GPIO,PERIPHERAL_POWER_VDD_PIN)

/*蜂鸣器控制开关*/
#define BUZZER_POWER_ON()                   gpio_bit_set(BUZZER_PORT,BUZZER_PIN)
#define BUZZER_POWER_OFF()                  gpio_bit_reset(BUZZER_PORT,BUZZER_PIN)

/*低压Vbat驱动端工作控制开关*/
#define LOW_VOLTAGE_DRIVE_EN_ON()           gpio_bit_set(LOW_VOLTAGE_DRIVE_EN_GPIO,LOW_VOLTAGE_DRIVE_EN_PIN)
#define LOW_VOLTAGE_DRIVE_EN_OFF()          gpio_bit_reset(LOW_VOLTAGE_DRIVE_EN_GPIO,LOW_VOLTAGE_DRIVE_EN_PIN)

/*RS485(/RE)收发器的接收使能控制开关*/
#define PERIPHERAL_RS485_EN_ON()            gpio_bit_set(PERIPHERAL_RS485_EN_GPIO,PERIPHERAL_RS485_EN_PIN)
#define PERIPHERAL_RS485_EN_OFF()           gpio_bit_reset(PERIPHERAL_RS485_EN_GPIO,PERIPHERAL_RS485_EN_PIN)

/*12V转8V电源VPRO的供电使能,这个功能是用于开启写AFE的EEPROM功能用的
开启这个功能才能写AFE的配置,写完要关闭*/
#define VPRO12V8V_CONTROL_ON()             	gpio_bit_set(VPRO12V8V_CONTROL_GPIO,VPRO12V8V_CONTROL_PIN)
#define VPRO12V8V_CONTROL_OFF()            	gpio_bit_reset(VPRO12V8V_CONTROL_GPIO,VPRO12V8V_CONTROL_PIN)

/*插入信号开关*/
#define	MS0_OUT_EN_ON()		                gpio_bit_set(MS0_OUT_EN_GPIO,MS0_OUT_EN_PIN)   
#define	MS0_OUT_EN_OFF()		            gpio_bit_reset(MS0_OUT_EN_GPIO,MS0_OUT_EN_PIN)  

/*AFESH367309的MOS强制控制引脚CTL*/
#define	AFESH367309MOS_ON()                 gpio_bit_set(AFESH367309MOS_CONTR_GPIO,AFESH367309MOS_CONTR_PIN)    
#define	AFESH367309MOS_OFF() 				gpio_bit_reset(AFESH367309MOS_CONTR_GPIO,AFESH367309MOS_CONTR_PIN)  
	
/*仓运状态控制*/
#define	WAREHOUSE_STAT_CONTR_IN()		    gpio_bit_reset(WAREHOUSE_STAT_CONTR_GPIO,WAREHOUSE_STAT_CONTR_PIN)
#define	WAREHOUSE_STAT_CONTR_OUT()		    gpio_bit_set(WAREHOUSE_STAT_CONTR_GPIO,WAREHOUSE_STAT_CONTR_PIN)  

/*自检状态输出*/
#define EN_PWR_ON()                		    gpio_bit_set(EN_PWR_GPIO,EN_PWR_PIN)
#define EN_PWR_OFF()               			gpio_bit_reset(EN_PWR_GPIO,EN_PWR_PIN)
		
/*高压Vbat驱动端工作控制开关*/
#define HIGH_VOLTAGE_DRIVE_EN_ON()          gpio_bit_set(HIGH_VOLTAGE_DRIVE_EN_GPIO,HIGH_VOLTAGE_DRIVE_EN_PIN)
#define HIGH_VOLTAGE_DRIVE_EN_OFF()         gpio_bit_reset(HIGH_VOLTAGE_DRIVE_EN_GPIO,HIGH_VOLTAGE_DRIVE_EN_PIN)

/******************************************************获取开关量状态*******************************************************************************/

/*板卡通电,辅电使能状态*/
#define EN_SW1_APS_VBT_STATE() 				gpio_input_bit_get(EN_SW1_APS_VBT_GPIO,EN_SW1_APS_VBT_PIN)

/*防反接电路工作状态*/
#define EN_SW2_ARC_VBT_STATE()        		gpio_input_bit_get(EN_SW2_ARC_VBT_GPIO,EN_SW2_ARC_VBT_PIN)

/*485/CAN用的隔离芯片供电使能控制开关*/
#define PERIPHERAL_POWER_VDD_STATE()		gpio_input_bit_get(PERIPHERAL_POWER_VDD_GPIO,PERIPHERAL_POWER_VDD_PIN)

/*低压Vbat驱动端工作使能状态*/
#define LOW_VOLTAGE_DRIVE_EN_STATE()     	gpio_input_bit_get(LOW_VOLTAGE_DRIVE_EN_GPIO,LOW_VOLTAGE_DRIVE_EN_PIN)

/*RS485(/RE)收发器的接收使能状态*/
#define PERIPHERAL_RS485_EN_STATE()			gpio_input_bit_get(PERIPHERAL_RS485_EN_GPIO,PERIPHERAL_RS485_EN_PIN)

/*12V转8V电源VPRO的供电使能*/
#define VPRO12V8V_CONTROL_STATE() 			gpio_input_bit_get(VPRO12V8V_CONTROL_GPIO,VPRO12V8V_CONTROL_PIN)

/*读取母线高压是否存在*/
#define BUS_HIGH_VOLTAGE_STATE() 			gpio_input_bit_get(BUS_HIGH_VOLTAGE_STATE_GPIO,BUS_HIGH_VOLTAGE_STATE_PIN)

/*控制干接点继电器供电电源使能状态*/
#define DRY_CONTACT_RELAY_EN_STATE() 		gpio_input_bit_get(DRY_CONTACT_RELAY_EN_GPIO,DRY_CONTACT_RELAY_EN_PIN)

/*AFESH367309的MOS强制控制引脚CTL使能状态*/
#define AFESH367309MOS_CONTR_STATE() 		gpio_input_bit_get(AFESH367309MOS_CONTR_GPIO,AFESH367309MOS_CONTR_PIN)

/*仓运状态控制状态*/
#define	WAREHOUSE_STAT_CONTR_STATE()		gpio_input_bit_get(WAREHOUSE_STAT_CONTR_GPIO,WAREHOUSE_STAT_CONTR_PIN)

/*读取按键工作状态*/
#define EN_SKY_STATE()		                gpio_input_bit_get(EN_SKY_GPIO,EN_SKY_PIN)

/*高压Vbat驱动端工作状态*/
#define HIGH_VOLTAGE_DRIVE_ENSTATE()		gpio_input_bit_get(HIGH_VOLTAGE_DRIVE_EN_GPIO,HIGH_VOLTAGE_DRIVE_EN_PIN)

/*PWR工作状态*/
#define EN_PWR_DRIVE_STATE()		gpio_input_bit_get(EN_PWR_GPIO,EN_PWR_PIN)
     

/*插入信号检测*/
#define MS0_DET_READ_STATE()		        gpio_input_bit_get(MS0_DET_STATE_GPIO,MS0_DET_STATE_PIN)


void gd32f4x_gpio_init(void);


typedef void (*afe_alarm_interrupt_callback)(void);
void gd32f4x_gpio_set_afe_alarm_callback(afe_alarm_interrupt_callback alarm_calback);
void gd32f4x_afe_alarm_irq_enable(void);
void gd32f4x_afe_alarm_irq_disable(void);
void gd32f4x_bus_high_voltage_irq_enable(void);
void gd32f4x_bus_high_voltage_irq_disable(void);
void gd32f4x_sky_irq_enable(void);
void gd32f4x_sky_irq_disable(void);
void gd32f4x_input_irq_enable(void);
void gd32f4x_input_irq_disable(void);
uint8_t gd32f4x_key_read(void);

extern uint32_t exitirq_count;

/* 串口调试************************************************************************** */

#define HARDWARE_DEBUG_USART3

#define DRIVER_DEBUG_EN   0 /* 调试打印 1使能 0关闭 */
#if DRIVER_DEBUG_EN
    #define DRIVER_LOG(fmt, ...)   printf(fmt, ##__VA_ARGS__)
#else
    #define DRIVER_LOG(fmt, ...)   ((void)0)
#endif

/* 用于USART （UASRT）和引脚的设置, TX = P, RX = P */
/*与底板的通信口*/
#define USART0_RX_PIN          		MCU_GPIO_PIN(GPIO_PIN_,11) 
#define USART0_RX_GPIO     	   		MCU_GPIO_(GPIO,C)
#define USART0_TX_PIN          		MCU_GPIO_PIN(GPIO_PIN_,10) 
#define USART0_TX_GPIO     	    	MCU_GPIO_(GPIO,C)

#define USART0_TXBUFF_COUNT_MAX			(1024u)	/*串口0的数据发送缓冲区大小*/

/*对外RS485*/
// #define USART2_RX_PIN          		MCU_GPIO_PIN(GPIO_PIN_,9) 
// #define USART2_RX_GPIO     	   		MCU_GPIO_(GPIO,D)
// #define USART2_TX_PIN          		MCU_GPIO_PIN(GPIO_PIN_,8) 
// #define USART2_TX_GPIO     	    	MCU_GPIO_(GPIO,D)
#define USART2_RX_PIN          		MCU_GPIO_PIN(GPIO_PIN_,11) 
#define USART2_RX_GPIO     	   		MCU_GPIO_(GPIO,C)
#define USART2_TX_PIN          		MCU_GPIO_PIN(GPIO_PIN_,10) 
#define USART2_TX_GPIO     	    	MCU_GPIO_(GPIO,C)
void gd_eval_com_init(void);

#define USART2_TXBUFF_COUNT_MAX			(1024u)	/*串口2的数据发送缓冲区大小*/

/*串口屏*/
#define UART3_RX_PIN          		MCU_GPIO_PIN(GPIO_PIN_,11) 
#define UART3_RX_GPIO     	   		MCU_GPIO_(GPIO,C)
#define UART3_TX_PIN          		MCU_GPIO_PIN(GPIO_PIN_,10) 
#define UART3_TX_GPIO     	    	MCU_GPIO_(GPIO,C)

#define RX_DATA_BUFF_QUANTITY         							(1024u)
/*#define TX_DATA_BUFF_QUANTITY         							(500u)*/
#define BMS_MODBUSS_ADD                							(1u)

/* DCDC通信口 */
void gd32f4x_usart0_init(uint32_t baudrate);
void gd32f4x_usart0_enable(void);
void gd32f4x_usart0_disable(void);
uint8_t usart0_send_string(uint8_t *data,uint32_t len);
void usart0_send_data(uint8_t ucch);
uint8_t usart0_send_msg(uint8_t* data, uint16_t len);
void gd32f4x_dcdc_communicate_rxbuffnumber(uint16_t BuffNum);

void gd32f4x_dcdc_communicate_txbuff(uint8_t *TxBuff,uint16_t BuffNum);
uint8_t *gd32f4x_dcdc_communicate_rs485rxbuffadd(void);

typedef void (*fun_usart_recive_callback)(uint8_t* data, uint32_t length);
uint8_t gd32f4x_usart0_set_receive_callback(fun_usart_recive_callback receive_callback);
fun_usart_recive_callback usart2_data_process(uint8_t* data, uint32_t length);

/* RS485通信口 */
void gd32f4x_usart2_init(uint32_t baudrate);
void gd32f4x_usart2_enable(void);
void gd32f4x_usart2_disable(void);
uint8_t usart2_send_string(uint8_t *data,uint32_t len);
void usart2_send_data(uint8_t ucch);
uint8_t usart2_send_msg(uint8_t* data, uint16_t len);

uint8_t *gd32f4x_pc_communicate_rs485rxbuffadd(void);
void gd32f4x_pc_communicate_rxbuffnumber(uint16_t BuffNum);
void gd32f4x_pc_communicate_txbuff(uint8_t *TxBuff,uint16_t BuffNum);

typedef void (*fun_usart_recive_callback)(uint8_t* data, uint32_t length);
uint8_t gd32f4x_usart2_set_receive_callback(fun_usart_recive_callback receive_callback);

/* 调试口或串口屏 */
void gd32f4x_uart3_init(uint32_t baudrate);
void gd32f4x_uart3_enable(void);
void gd32f4x_uart3_disable(void);
void uart3_send_data(uint8_t ucch);
void uart3_send_string(char *str);

// typedef void (*fun_usart_recive_callback)(uint8_t* data, uint32_t length);
uint8_t gd32f4x_uart3_set_receive_callback(fun_usart_recive_callback receive_callback);


/* 串口所有初始化 */
void gd32f4x_usart_init(void);


/* DMA************************************************************************** */

#define DMA1_CH0_BUFF_MAX 				(5u)/*DMA0_CH1传输通道最大数据的个数*/
#define USART0_TXBUFF_COUNT_MAX			(1024u)	/*串口0的数据发送缓冲区大小*/
#define USART0_RXBUFF_COUNT_MAX			(1024u)	/*串口0的数据接收缓冲区大小*/

#define USART2_TXBUFF_COUNT_MAX			(1024u)	/*串口2的数据发送缓冲区大小*/
#define USART2_RXBUFF_COUNT_MAX			(4096u)	/*串口2的数据接收缓冲区大小*/

#define UART3_TXBUFF_COUNT_MAX			(1024u)	/*串口3的数据发送缓冲区大小*/
#define UART3_RXBUFF_COUNT_MAX			(1024u)	/*串口3的数据接收缓冲区大小*/

extern uint8_t  Uart3RxBuff[UART3_RXBUFF_COUNT_MAX];			    /*串口3的数据接收缓冲区*/
extern uint8_t  Usart2RxBuff[USART2_RXBUFF_COUNT_MAX];
void gd32f4x_dma_init(void);
void gd32f4x_dma1ch0_init(void);

/* 定时器************************************************************************** */

/*输出 TIMER0_CH1_ON(TIMER0_CH1的互补通道) 蜂鸣器SP1(PWM)，高电平蜂鸣器响，低电平蜂鸣器关。*/

/* 风扇 */
#define FAN_RCU_CLOCK                       RCU_GPIOE
#define FAN_PIN        		                MCU_GPIO_PIN(GPIO_PIN_,5)
#define FAN_PORT       		                MCU_GPIO_(GPIO,E)
#define FAN_AF                              GPIO_AF_3
#define FAN_CHANNEL                         TIMER_CH_0

/* 蜂鸣器 */
#define BUZZER_RCU_CLOCK                    RCU_GPIOB
#define BUZZER_PIN        		            MCU_GPIO_PIN(GPIO_PIN_,0)
#define BUZZER_PORT       		            MCU_GPIO_(GPIO,B)
#define BUZZER_AF                           GPIO_AF_1
#define BUZZER_CHANNEL                      TIMER_CH_1


/* LED(J49-6) RED */
#define LED_J49_6_RCU_CLOCK                 RCU_GPIOC
#define LED_J49_6_PIN          				MCU_GPIO_PIN(GPIO_PIN_,9)
#define LED_J49_6_GPIO         				MCU_GPIO_(GPIO,C)
#define LED_J49_6_AF                        GPIO_AF_2
#define LED_J49_6_CHANNEL                   TIMER_CH_3

/* LED(J49-7) */
#define LED_J49_7_RCU_CLOCK                 RCU_GPIOA
#define LED_J49_7_PIN          			    MCU_GPIO_PIN(GPIO_PIN_,8)
#define LED_J49_7_GPIO         			    MCU_GPIO_(GPIO,A)
#define LED_J49_7_AF                        GPIO_AF_1
#define LED_J49_7_CHANNEL                   TIMER_CH_0


void gd32f4x_timer_init(void);

void gd32f4x_timer2_init(uint32_t usr_hz);
void gd32f4x_timer2_channel_set_duty(uint16_t channel, uint16_t duty);
// void gd32f4x_timer2_led_j49_3_set_duty(uint8_t duty);
// void gd32f4x_timer2_led_j49_4_set_duty(uint8_t duty);
// void gd32f4x_timer2_led_j49_5_set_duty(uint8_t duty);
void gd32f4x_timer2_led_j49_6_set_duty(uint8_t duty);


// void gd32f4x_timer3_channel_set_duty(uint16_t channel, uint16_t duty);
// void gd32f4x_timer3_led_j49_2_set_duty(uint8_t duty);

void gd32f4x_timer4_init(void);

void gd32f4x_timer0_init(uint16_t Prescaler, uint32_t Period, uint32_t BUZZER_Duty, uint32_t LED_Duty);
void gd32f4x_timer0_channel_set_duty(uint16_t channel, uint16_t duty);
void gd32f4x_timer0_led_j49_7_set_duty(uint8_t duty);

void gd32f4x_timer0ch1_set_hz(uint32_t new_hz);
void gd32f4x_timer0ch1_set_pwm_duty(uint8_t duty);

void gd32f4x_timer8_init(uint16_t Prescaler, uint32_t Period, uint32_t Duty);
void gd32f4x_timer8ch0_set_hz(uint32_t new_hz);
void gd32f4x_timer8ch0_set_pwm_duty(uint8_t duty);
extern uint32_t timer4irq_flag;
extern uint32_t timer4irq_count;
extern uint32_t timer4irq_count_flag;
/* ADC采集************************************************************************** */
/*#define	ADC1_PIN								 		MCU_GPIO_PIN(GPIO_PIN_,1)
#define ADC1_GPIO										    MCU_GPIO_(GPIO,B)   

#define	ADC2_PIN										MCU_GPIO_PIN(GPIO_PIN_,0)
#define ADC2_GPIO										    MCU_GPIO_(GPIO,B)*/

#define	ADC3_PIN										MCU_GPIO_PIN(GPIO_PIN_,3)/*I_SENSE+*/
#define ADC3_GPIO            				            MCU_GPIO_(GPIO,A)

#define	ADC4_PIN										MCU_GPIO_PIN(GPIO_PIN_,4)/*CAN,485通讯芯片供电电源的电压采集*/
#define ADC4_GPIO										MCU_GPIO_(GPIO,A)

/*#define	ADC5_PIN										MCU_GPIO_PIN(GPIO_PIN_,7)
#define ADC5_GPIO										    MCU_GPIO_(GPIO,A)

#define	ADC6_PIN										MCU_GPIO_PIN(GPIO_PIN_,6)
#define ADC6_GPIO 									    MCU_GPIO_(GPIO,A)

#define	ADC7_PIN										MCU_GPIO_PIN(GPIO_PIN_,5)							
#define ADC7_GPIO										    MCU_GPIO_(GPIO,A)

#define	ADC8_PIN										MCU_GPIO_PIN(GPIO_PIN_,4)
#define ADC8_GPIO										    MCU_GPIO_(GPIO,A)*/

#define	ADC9_PIN										MCU_GPIO_PIN(GPIO_PIN_,1)/*NTC4*/
#define ADC9_GPIO										 MCU_GPIO_(GPIO,B)

/*#define	ADC10_PIN										MCU_GPIO_PIN(GPIO_PIN_,2)
#define ADC10_GPIO									    MCU_GPIO_(GPIO,A)*/

#define	ADC11_PIN										MCU_GPIO_PIN(GPIO_PIN_,1)/*BOARD_TEMP*/
#define ADC11_GPIO									    MCU_GPIO_(GPIO,C)

#define	ADC12_PIN										MCU_GPIO_PIN(GPIO_PIN_,2)/*BAT.V*/
#define ADC12_GPIO									    MCU_GPIO_(GPIO,C)

/*#define	ADC13_PIN										MCU_GPIO_PIN(GPIO_PIN_,2)
#define ADC13_GPIO									    MCU_GPIO_(GPIO,C)

#define	ADC14_PIN										MCU_GPIO_PIN(GPIO_PIN_,1)
#define ADC14_GPIO									    MCU_GPIO_(GPIO,C)

#define	ADC15_PIN										MCU_GPIO_PIN(GPIO_PIN_,0)
#define ADC15_GPIO									    MCU_GPIO_(GPIO,C)*/

#define	ADC3_GPIO_CLOCK	 					RCU_GPIOA
#define	ADC4_GPIO_CLOCK	 					RCU_GPIOA
#define	ADC9_GPIO_CLOCK	 					RCU_GPIOB
#define	ADC11_GPIO_CLOCK	 			    RCU_GPIOC
#define	ADC12_GPIO_CLOCK	 				RCU_GPIOC

#define ADC_BUFF_COUNT_MAX              (16u)/*存储ADC数据采集数据的个数*/


#define ADC0_CHANNEL_NUMBER             5
void gd32f4x_adc_init(void);
extern uint32_t uiDMAData[DMA1_CH0_BUFF_MAX];


/* CAN0************************************************************************** */
#define MAX_FILTERS 14 
typedef  void(* can0_receive_call_back_func)(can_receive_message_struct* );
typedef  void(* can0_error_handle_call_back_func)();
/* can0的黑白名单功能，最多14个滤波器 */
#define MAX_FILTERS 14 
#define MIN_FILTERS 0
#define BAUD_1000K 5
#define BAUD_500K 10
#define BAUD_250K 20
#define BAUD_100K 50

#define CAN0_RCU_CLOCK RCU_GPIOB
#define CAN0_TX_PORT GPIOB
#define CAN0_RX_PORT GPIOB
#define CAN0_TX_AF GPIO_AF_9
#define CAN0_RX_AF GPIO_AF_9
#define CAN0_TX_PIN GPIO_PIN_9
#define CAN0_RX_PIN GPIO_PIN_8

#define CAN0_INIT_FLAG_NONE 0               /* 未初始化 */
#define CAN0_INIT_FLAG_OK   1               /* 初始化成功 */
#define CAN0_INIT_FLAG_ERR  2               /* 初始化失败 */

#define CAN0_ERR_NONE               (0)      /* 错误标志位-无 */
#define CAN0_ERR_INIT               (1<<0)   /* 错误标志位-初始化 */
#define CAN0_ERR_NO_COMMUNICATION   (1<<1)   /* 错误标志位-超时无通信 */
#define CAN0_ERR_SEND               (1<<2)   /* 错误标志位-发送错误 */
#define CAN0_ERR_NO_COMMUNICATION_TIME_THRESHOLD_DEFAULT  260  /* 默认无通信异常时间阈值默认值 2分钟=120s 单位s */
#define CAN0_ERR_NUM_THRESHOLD_DEFAULT  10  /* 默认发送错误计数阈值 */
typedef struct 
{
    uint8_t  init_flag;                             /* 初始化标志位 0未初始化 1初始化成功 2初始化失败 */
    uint8_t  err_flag;                              /* 发送错误标志位 0无 1错误（触发发送错误数阈值） */
    uint32_t no_communication_time_count;           /* 无通信计时 */
    uint32_t no_communication_time_err_threshold;   /* 无通信异常时间阈值s */
    uint32_t send_err_count;                        /* 发送错误计数，成功一次清零，失败一次计数 */
    uint32_t send_err_num_threshold;                /* 发送错误计数阈值，超过阈值置位发送错误标志位 */
    can0_error_handle_call_back_func err_callback;  /* 异常回调 */
}T_CAN0_ERR_TYPDEF;


void gd32f4x_can0_init(uint32_t baud);
uint8_t can0_check_whitelist_ef(uint32_t* filter_ids, uint8_t num_filters);
uint8_t can0_receive_register(can0_receive_call_back_func call_back);
extern uint8_t can0_send_msg(uint32_t id, uint8_t* data, uint8_t len);
uint8_t can0_error_handle_register(can0_error_handle_call_back_func call_back);
uint8_t can0_check_whitelist_sf(uint32_t* filter_ids, uint8_t num_filters);
void gd32f4x_can0_rx_irqn_enable(void);
void gd32f4x_can0_rx_irqn_disable(void);
 void can0_process_cycle(uint32_t cycle_time);
/* CAN1************************************************************************** */
typedef  void(* can1_receive_call_back_func)(can_receive_message_struct* );
typedef  void(* can1_error_handle_call_back_func)();
/* can1的黑白名单功能，最多14个滤波器 */

#define CAN1_RCU_CLOCK RCU_GPIOB
#define CAN1_TX_PORT GPIOB
#define CAN1_RX_PORT GPIOB
#define CAN1_TX_AF GPIO_AF_9
#define CAN1_RX_AF GPIO_AF_9
#define CAN1_TX_PIN GPIO_PIN_13
#define CAN1_RX_PIN GPIO_PIN_12

void gd32f4x_can1_init(uint32_t baud);
uint8_t can1_check_whitelist_ef(uint32_t* filter_ids, uint8_t num_filters);
uint8_t can1_receive_register(can1_receive_call_back_func call_back);
extern uint8_t can1_send_msg(uint32_t id, uint8_t* data, uint8_t len);
uint8_t can1_error_handle_register(can1_error_handle_call_back_func call_back);
uint8_t can1_check_whitelist_sf(uint32_t* filter_ids, uint8_t num_filters);
void gd32f4x_can1_rx_irqn_enable(void);
void gd32f4x_can1_rx_irqn_disable(void);

/* 低功耗睡眠************************************************************************** */
#if 1
#include "gd32f4xx_can.h"
typedef enum
{
    /* 睡眠模式 */
    MCU_MODE_SLEEP,
    /* 深度睡眠模式 */
    MCU_MODE_DEEP_SLEEP,
    /* 待机模式 */
    MCU_MODE_STANDBY
} MCU_SLEEP_MODE_TYPEDEF;

typedef  void(* gd32f4x_after_sleep_init_call_back)();
extern gd32f4x_after_sleep_init_call_back gd32f4x_after_sleep_init;
uint8_t gd32f4x_after_sleep_init_register(gd32f4x_after_sleep_init_call_back call_back);
void gd32f4x_after_sleep_init_func(void);
/* PA0按键触发 */
#define KEY_WAKEUP_PORT         GPIOE
#define KEY_WAKEUP_PIN          GPIO_PIN_2
#define KEY_WAKEUP_EXTI         EXTI_2
#define KEY_WAKEUP_EXTI_PORT    EXTI_SOURCE_GPIOE
#define KEY_WAKEUP_EXTI_PIN     EXTI_SOURCE_PIN2
#define KEY_WAKEUP_PIN_PERIPH   RCU_GPIOE


void gd32f4x_mcu_sleep_enter(MCU_SLEEP_MODE_TYPEDEF mode);
void gd32f4x_after_sleep_init_func(void);
uint8_t gd32f4x_after_sleep_init_register(gd32f4x_after_sleep_init_call_back call_back);
void gd32f4x_key_wakeup_init(void);
void gd32f4x_key_wakeup_enable(void);
void gd32f4x_key_wakeup_disable(void);
void gd32f4x_can0_wakeup_init(void);
void gd32f4x_can0_wakeup_enable(void);
void gd32f4x_can0_wakeup_disable(void);
void gd32f4x_can1_wakeup_init(void);
void gd32f4x_can1_wakeup_enable(void);
void gd32f4x_can1_wakeup_disable(void);
#endif

/* 看门狗************************************************************************** */
#include "gd32f4xx_fwdgt.h"
#include "gd32f4xx_wwdgt.h"

void gd32f4x_fwdgt_init(uint8_t prer, uint16_t rlr);
void gd32f4x_fwdgt_feed(void);
void gd32f4x_fwdgt_init_ms(uint32_t tout_ms);

void gd32f4x_wwdgt_init(uint16_t tr, uint8_t wr, uint32_t fprer);
void gd32f4x_wwdgt_init_ms(uint32_t tout_ms);

/* RTC************************************************************************** */
#include <gd32f4xx_rcu.h>
/* #define RTC_PRINTF */  /* 调试信息打印-有宏定义 RTC_PRINTF 打开打印 无则关闭打印  */
#define BKP_VALUE    0x32F1



extern rtc_parameter_struct rtc_time_structure;

int  gd32f4x_rtc_init(void);
void gd32f4x_rtc_init_alarm_interrupt(void);
void gd32f4x_rtc_wakeup_init(uint16_t wakup_time_s);





/* IIC1************************************************************************** */
/*AFE*/
#define IIC1_PORT_SCL GPIOD
#define IIC1_PORT_SDA GPIOC

#define IIC1_PIN_SCL GPIO_PIN_0
#define IIC1_PIN_SDA GPIO_PIN_12

/*AT8563*/
/*#define IIC2_PORT_SCL GPIOD
#define IIC2_PORT_SDA GPIOD

#define IIC2_PIN_SCL GPIO_PIN_1
#define IIC2_PIN_SDA GPIO_PIN_2*/

uint8_t gd32f4x_i2c1_init(uint32_t clock);
/*uint8_t gd32f4x_i2c2_init(uint32_t clock);*/

extern uint8_t iic1_delay_rate;
/*extern uint8_t iic2_delay_rate;*/


/* SPI************************************************************************** */

#define SPI_RCU_CLOCK RCU_GPIOE
#define SPI_PORT_SCLK GPIOE
#define SPI_PORT_MOSI GPIOE
#define SPI_PORT_MISO GPIOE
#define SPI_PORT_CS   GPIOE

#define SPI_PIN_SCLK GPIO_PIN_9
#define SPI_PIN_MOSI GPIO_PIN_10
#define SPI_PIN_MISO GPIO_PIN_11
#define SPI_PIN_CS   GPIO_PIN_12

#define FLSH_SPI_SCLK(x) do{x?gpio_bit_set(SPI_PORT_SCLK,SPI_PIN_SCLK):gpio_bit_reset(SPI_PORT_SCLK,SPI_PIN_SCLK);}while(0)
#define FLSH_SPI_MOSI(x) do{x?gpio_bit_set(SPI_PORT_MOSI,SPI_PIN_MOSI):gpio_bit_reset(SPI_PORT_MOSI,SPI_PIN_MOSI);}while(0)
#define FLSH_SPI_CS(x)   do{x?gpio_bit_set(SPI_PORT_CS,SPI_PIN_CS):gpio_bit_reset(SPI_PORT_CS,SPI_PIN_CS);}while(0)

#define FLSH_SPI_DELAY_US(x) gd32f4x_delay_us(x)
#define FLSH_SPI_US_DELAY() FLSH_SPI_DELAY_US(2) 

#define FLSH_SPI_READ_SDA() gpio_input_bit_get(SPI_PORT_MISO,SPI_PIN_MISO)

uint8_t gd32f4x_spi_init(void);
uint8_t spi_send_bit(uint8_t data);
uint8_t spi_send_byte(uint8_t data);
uint16_t spi_send_halfword(uint16_t data);
uint8_t spi_receive_bit(void);
uint8_t spi_receive_byte(void);
uint16_t spi_receive_halfword(void);
uint8_t spi_read_write_byte(uint8_t data);
/* GDID************************************************************************** */



#define   STORAGE_INFO       0x1FFF7A20

#define   UNIQUE_ID          0x1FFF7A10

#define   UNIQUE_ID_0_31     0x1FFF7A10
#define   UNIQUE_ID_32_63    0x1FFF7A14
#define   UNIQUE_ID_64_95    0x1FFF7A18

void gd32f4x_read_id(void);

uint16_t rs485_datacrc16(uint8_t *puchMsg, uint16_t usDataLen);

#endif
