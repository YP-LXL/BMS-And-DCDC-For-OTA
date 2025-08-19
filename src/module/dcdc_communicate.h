#ifndef DCDC_COMMUNICATE_H
#define DCDC_COMMUNICATE_H
/* 代码块 描述 */
    /* ***********************环形队列************************* */
    /* ***********************DCDC-通信接口*********************** */
    /* ***********************DCDC-MODBUS协议*********************** */
/* 使用说明 */
    /* 
        调用 dcdc_communicate_com_set(),初始化dcdc通信接口 并初始化接口；
        调用 dcdc_communicate_send（），发送数据
    */
#include "common.h"
#include "gd32f4x.h"
#include "parameter.h"
#include "ota.h"

/* ***********************环形队列************************* */
#define DCDC_BLOCK_QUEUE_SIZE  2         /* 队列最多存储的块数 */
#define DCDC_BLOCK_MAX_SIZE    256       /* 每块数据最大字节数 */

#define DCDC_BLOCK_QUEUE_LOCK    1  /* 队列上锁 */
#define DCDC_BLOCK_QUEUE_UNLOCK  0  /* 队列解锁 */

#if 1 /* 使用临界资源保护 */
#define CRITICAL_SECTION_ENTER()  __disable_irq()  /* 临界资源保护-进入 */
#define CRITICAL_SECTION_EXIT()   __enable_irq()   /* 临界资源保护-退出 */
#else /* 不使用临界资源保护 */
#define CRITICAL_SECTION_ENTER()  
#define CRITICAL_SECTION_EXIT()   
#endif

typedef struct {
    uint8_t data[DCDC_BLOCK_MAX_SIZE];
    uint16_t length;
} DATA_BLOCK_T;

typedef struct {
    DATA_BLOCK_T blocks[DCDC_BLOCK_QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
    uint8_t lock_flag;
} BLOCK_QUEUE_T;

/* 环形队列 函数 */
void block_queue_init(BLOCK_QUEUE_T* q);
void block_queue_lock(BLOCK_QUEUE_T* q);
void block_queue_unlock(BLOCK_QUEUE_T* q);
uint8_t block_queue_is_full(BLOCK_QUEUE_T* q);
uint8_t block_queue_is_empty(BLOCK_QUEUE_T* q);
uint8_t block_queue_enqueue(BLOCK_QUEUE_T *q, const uint8_t *data, uint16_t len);
uint8_t block_queue_dequeue(BLOCK_QUEUE_T* q, DATA_BLOCK_T** block_out);



/* ***********************DCDC-通信接口*********************** */
#define DCDC_COM_DEFAULT            (0)     /* DCDC 默认使用串口0通信 */
#define DCDC_COM_UART_BPS_DEFAULT   (4800U) /* DCDC 串口 默认通信波特率 9600bps */
/* 驱动接口-UART0 */
#define DCDC_COM_UART0_INIT_HANDLE              gd32f4x_usart0_init    /* UART0 初始化 */
#define DCDC_COM_UART0_SEND_HANDLE              usart0_send_msg        /* UART0 发送 */
#define DCDC_COM_UART0_SET_RECIVE_HANDLE        gd32f4x_usart0_set_receive_callback /* UART0 接收注册 */

typedef void (*fun_init)(uint32_t baud);
typedef uint8_t (*fun_send)(uint8_t *p_data, uint16_t len);
// typedef void (*fun_recive_callback)(uint8_t* data, uint32_t length);
typedef uint8_t (*fun_set_recive_callback)(void (*fun_recive_callback)(uint8_t* data, uint32_t length));


/* 枚举接口类型 */
typedef enum
{
    COM_USART0 = 0, /* 串口0 */
    COM_USART1,     /* 串口1 */
    COM_USART2,     /* 串口2 */
    COM_CAN0,       /* CAN0 */
    COM_CAN1        /* CAN1 */
} DCDC_COM_TYPE;

/* 接口结构体-串口 */
typedef struct 
{
    fun_init int_handle; 
    fun_send send_handle;
    fun_set_recive_callback set_recive_handle;
} DCDC_UART_COM_PARAM;

/* DCDC 接口 参数 */
typedef struct
{
    DCDC_COM_TYPE com_type;
    DCDC_UART_COM_PARAM uart_handle;
}COMMUNICATE_PARAM_TYPEDEF;

/* DCDC 结构体变量 */
extern BLOCK_QUEUE_T t_dcdc_recive_queue;
extern BLOCK_QUEUE_T t_dcdc_send_queue;
extern COMMUNICATE_PARAM_TYPEDEF dcdc_communication_param; 

/* DCDC 通信模块 函数 */
uint8_t dcdc_communicate_init(void);
uint8_t dcdc_communicate_com_set(DCDC_COM_TYPE com_type);
uint8_t dcdc_communicate_send(uint8_t* p_data, uint32_t len);


/* ***********************DCDC-MODBUS协议*********************** */
#define DCDC_MODBUS_DEVICE_ADDR       0x01                /* DCDC 地址 固定值1 */
#define DCDC_MODBUS_MAX_FRAME_LEN     DCDC_BLOCK_MAX_SIZE /* 最大数据长度 */

#define DCDC_MODBUS_PERIODIC_TIME_MS  10                  /* 周期时间 ms*/
#define DCDC_MODBUS_TIMEOUT_MS        500                /* 超时值 ms */

/* 功能码 */
#define DCDC_MODBUS_CODE_03H     0X03 /* 读单个或多个保持寄存器值（读4区数据） */
#define DCDC_MODBUS_CODE_04H     0X04 /* 读单个或多个输入寄存器值（读3区数据） */
#define DCDC_MODBUS_CODE_06H     0X06 /* 写单寄存器值 */
#define DCDC_MODBUS_CODE_10H     0X10 /* 写多寄存器值 */

/* 错误码 */
typedef enum 
{
    ERR_DCDC_OK = 0,
    ERR_DCDC_ILLEGAL_FUNCTION = 1,      /* 0x01：非法功能（Illegal Function） */ 
    ERR_DCDC_ILLEGAL_DATA_ADDRESS,      /* 0x02：非法数据地址（Illegal Data Address） */ 
    ERR_DCDC_ILLEGAL_DATA_VALUE,        /* 0x03：非法数据值（Illegal Data Value） */ 
    ERR_DCDC_SLAVE_DEVICE_FAILURE,      /* 0x04：从设备故障（Slave Device Failure） */ 
    ERR_DCDC_ACKNOWLEDGE,               /* 0x05：确认错误（Acknowledge） */ 
    ERR_DCDC_DEVICE_BUSY,               /* 0x06：设备忙（Device Busy） */ 
    ERR_DCDC_NEGATIVE_ACKNOWLEDGE,      /* 0x07：无响应（Negative Acknowledge） */ 
    ERR_DCDC_CRC_ERROR,                 /* 0x08：校验错误（crc Error） */ 
} E_DCDC_MODBUS_ERR_CODE_TYPDEF;

/* MODBUS状态机 */
typedef enum
{
    DCDC_MODBUS_STATE_IDLE = 0,              /* 空闲状态 */
    DCDC_MODBUS_STATE_SEND_REQUEST,          /* 发送请求 */
    DCDC_MODBUS_STATE_WAIT_RESPONSE,         /* 等待响应 */
    DCDC_MODBUS_STATE_PROCESS_RESPONSE,      /* 处理响应 */
    DCDC_MODBUS_STATE_ERROR,                 /* 错误处理 */
} E_DCDC_MODBUS_STATE_TYPDEF;

/* MODBUS 主机对象 */
typedef struct
{
    E_DCDC_MODBUS_STATE_TYPDEF state;                    /* 当前状态 */
    uint8_t  request_buffer[DCDC_MODBUS_MAX_FRAME_LEN];  /* 请求数据缓存 */
    uint16_t request_length;                             /* 请求数据长度 */
    uint8_t  response_buffer[DCDC_MODBUS_MAX_FRAME_LEN]; /* 响应数据缓存 */
    uint16_t response_length;                            /* 响应数据长度 */
    uint32_t timeout_tick;                               /* 超时计数器 */
} T_DCDC_MODBUS_MASTER_TYPDEF;



/* 输入寄存器 3区 */
typedef enum 
{
    INPUT_REG_INDEX_DEVICE_ADDRESS = 0,           /* 输入寄存器地址下标-设备地址 */
    INPUT_REG_INDEX_SOFTWARE_VERSION,             /* 输入寄存器地址下标-软件版本号（示例v1） */
    INPUT_REG_INDEX_STATUS,                       /* 输入寄存器地址下标-状态 */
    INPUT_REG_INDEX_U_ALARM,                      /* 输入寄存器地址下标-报警（共用体 位操作） */
    INPUT_REG_INDEX_U_PROTECTION,                 /* 输入寄存器地址下标-保护（共用体 位操作） */
    INPUT_REG_INDEX_BUS_VOLTAGE,                  /* 输入寄存器地址下标-母线电压（0.1V） */
    INPUT_REG_INDEX_BUS_CURRENT,                  /* 输入寄存器地址下标-母线电流（mA） */
    INPUT_REG_INDEX_BATTERY_VOLTAGE,              /* 输入寄存器地址下标-低压电压（电池端）（0.01V） */
    INPUT_REG_INDEX_BATTERY_CURRENT,              /* 输入寄存器地址下标-低压电流（电池端）（mA） */
    INPUT_REG_INDEX_TEMPERATURE,                  /* 输入寄存器地址下标-温度（0.01℃） */
    INPUT_REG_INDEX_CURRENT_POWER,                /* 输入寄存器地址下标-当前功率值（W） */

    INPUT_REG_INDEX_MAX,
} E_DCDC_MODBUS_INPUT_REG_ADDR_INDEX_TYPDEF;


#define T_DCDC_MODBUS_INPUT_REG_ADDRESS_TYPDEF U_MODBUS_DATA_POINTER_TYPDEF /* 输入寄存器地址指针 宏定义 */


/* 保持寄存器 4区 */
typedef enum 
{
    HOLD_REG_INDEX_SLEEP_STATUS = 0,         /* 保持寄存器地址下标-睡眠 */
    HOLD_REG_INDEX_POWER_STATUS,             /* 保持寄存器地址下标-开/关机 */
    HOLD_REG_INDEX_CHARGE_DISCHARGE_CONTROL, /* 保持寄存器地址下标-充、放电控制 */
    HOLD_REG_INDEX_CURRENT_CALIBRATION_PREPARATION, /* 电流标定-准备 */
    HOLD_REG_INDEX_CURRENT_CALIBRATION_VALUE,/* 电流标定-标定(A) */
    HOLD_REG_INDEX_VOLTAGE_CALIBRATION_PREPARATION,/* 电压标定-准备 */
    HOLD_REG_INDEX_VOLTAGE_CALIBRATION_VALUE,/* 电压标定-标定(V) */
    HOLD_REG_INDEX_TEMP_CALIBRATION_PREPARATION,/* 温度标定-准备 */
    HOLD_REG_INDEX_TEMP_CALIBRATION_VALUE,/* 温度标定-标定(0.01℃) */
    HOLD_REG_INDEX_FREQ_CALIBRATION_PREPARATION,/* 频率标定-准备 */
    HOLD_REG_INDEX_FREQ_CALIBRATION_VALUE,/* 频率标定-标定(hz) */
    HOLD_REG_INDEX_RECEIVE_NUM,/* 串口指令接收计数 */
    HOLD_REG_INDEX_SEND_NUM,/* 串口指令应答计数 */
    HOLD_REG_INDEX_ERASE_USART_NUM_CLT,             /* 清串口计数值 */                                      

    HOLD_REG_INDEX_MAX,
} E_DCDC_MODBUS_HOLD_REG_ADDR_INDEX_TYPDEF;

#define T_DCDC_MODBUS_HOLD_REG_ADDRESS_TYPDEF U_MODBUS_DATA_POINTER_TYPDEF  /* 保持寄存器地址指针 宏定义 */

// typedef struct 
// {
//     uint16_t*   p_sleep_status;             /* 睡眠 */
//     uint16_t*   p_power_status;             /* 开/关机 */
//     uint16_t*   p_charge_discharge_control; /* 充、放电控制 */

//     // uint16_t* p_
// } T_DCDC_MODBUS_HOLD_REG_ADDRESS_TYPDEF;


#define DCDC_MODBUS_INPUT_REG_NUM   INPUT_REG_INDEX_MAX /* 输入寄存器数（对应3区） */
#define DCDC_MODBUS_HOLD_REG_NUM    HOLD_REG_INDEX_MAX  /* 保持寄存器数（对应4区） */

extern const T_DCDC_MODBUS_INPUT_REG_ADDRESS_TYPDEF t_dcdc_input_reg[INPUT_REG_INDEX_MAX];
extern const T_DCDC_MODBUS_HOLD_REG_ADDRESS_TYPDEF  t_dcdc_hold_reg[HOLD_REG_INDEX_MAX];
extern T_DCDC_MODBUS_MASTER_TYPDEF g_modbus_master; /* Modbus主机状态机实例 */

#define modbus_send_data   dcdc_communicate_send /* 发送数据 */

uint8_t dcdc_modbus_master_init(void);
uint8_t dcdc_modbus_read_holding_registers(uint8_t slave_addr, uint16_t start_addr, uint16_t quantity);
uint8_t dcdc_modbus_read_input_registers(uint8_t slave_addr, uint16_t start_addr, uint16_t quantity);
uint8_t dcdc_modbus_write_single_register(uint8_t slave_addr, uint16_t reg_addr, uint16_t value);
uint8_t dcdc_modbus_write_multiple_registers(uint8_t slave_addr, uint16_t start_addr, uint16_t quantity, uint16_t *p_data);

void dcdc_modbus_master_poll(void);



/* MODBUS 优先级操作 写-读保持-读输入 */
#define DCDC_MODBUS_WRITE_HOLDING_REGISTER_FLAG    (1<<0) /* 写保持寄存器(单个)标志位-优先级高 */
#define DCDC_MODBUS_READ_HOLDING_REGISTER_FLAG     (1<<1) /* 读保持寄存器(多个)标志位-优先级中 */
#define DCDC_MODBUS_READ_INPUT_REGISTER_FLAG       (1<<2) /* 读输入寄存器(多个)标志位-优先级低 */

#define DCDC_MODBUS_BUS_READY_PERIOD_TIME_MS       (5000) /* 总线读取周期时间 ms */
typedef struct 
{
    uint8_t  err_code;         /* 错误码 */
    uint16_t register_address; /* 寄存器地址 0~max(对应40001~49999) */
    uint16_t u16_data;          /* 数据 */
}T_DCDC_SET_HOLD_REGISTER_DATA_TYPDEF;

typedef struct 
{
    uint8_t  err_code;         /* 错误码 */
    uint16_t register_address; /* 寄存器地址 0~max(对应40001~49999) */
    uint16_t read_len;         /* 读取长度 */
}T_DCDC_READ_HOLD_REGISTER_DATA_TYPDEF;
typedef struct 
{
    uint8_t  err_code;         /* 错误码 */
    uint16_t register_address; /* 寄存器地址 0~max(对应30001~39999) */
    uint16_t read_len;         /* 读取长度 */
}T_DCDC_READ_INPUT_REGISTER_DATA_TYPDEF;
typedef struct
{
    uint8_t  deal_flag;                                         /* 写-读保持-读输入 */
    T_DCDC_SET_HOLD_REGISTER_DATA_TYPDEF t_hold_data;           /* 写保持寄存器数据 */
    T_DCDC_READ_HOLD_REGISTER_DATA_TYPDEF t_read_hold_data;     /* 读取保持寄存器数据 */
    T_DCDC_READ_INPUT_REGISTER_DATA_TYPDEF t_read_input_data;   /* 读取输入寄存器数据 */
} T_DCDC_MODBUS_DEAL_TYPDEF;

extern T_DCDC_MODBUS_DEAL_TYPDEF t_dcdc_modbus_deal; /* DCDC MODBUS 处理实例 */
uint8_t dcdc_modbus_deal_set_hold_register(uint16_t reg_addr, uint16_t value);
uint8_t dcdc_modbus_deal_set_hold_register_result(uint8_t err_code);
uint8_t dcdc_modbus_deal_read_hold_register(uint16_t start_addr, uint16_t len);
uint8_t dcdc_modbus_deal_read_hold_register_result(uint8_t err_code);
uint8_t dcdc_modbus_deal_read_input_register(uint16_t start_addr, uint16_t len);
uint8_t dcdc_modbus_deal_read_input_register_result(uint8_t err_code);

#endif
