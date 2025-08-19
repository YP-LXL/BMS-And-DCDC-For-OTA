#include "dcdc_communicate.h"

#define DCDC_COMMUNICATE_DEBUG_EN   0 /* 调试打印 1使能 0关闭 */
#if DCDC_COMMUNICATE_DEBUG_EN
    #define DCDC_LOG(fmt, ...)   printf(fmt, ##__VA_ARGS__)
#else
    #define DCDC_LOG(fmt, ...)   ((void)0)
#endif


/* dcdc-变量 环形接收队列  */
BLOCK_QUEUE_T t_dcdc_recive_queue = {0};
/* dcdc-变量 环形发送队列 */
BLOCK_QUEUE_T t_dcdc_send_queue = {0};

/* DCDC-变量 通信接口参数结构体变量 */
COMMUNICATE_PARAM_TYPEDEF dcdc_communication_param = {
    .com_type = COM_USART0, // 默认使用串口0
    .uart_handle.int_handle = DCDC_COM_UART0_INIT_HANDLE,
    .uart_handle.send_handle = DCDC_COM_UART0_SEND_HANDLE,
    .uart_handle.set_recive_handle = DCDC_COM_UART0_SET_RECIVE_HANDLE,
};


/* ***********************环形队列************************* */
/**
 * @函数名称: block_queue_init
 * @功能描述: 初始化块队列
 * @输入: q - 指向块队列的指针
 * @输出: 无
 * @返回值: 无
 */
void block_queue_init(BLOCK_QUEUE_T *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    q->lock_flag = DCDC_BLOCK_QUEUE_UNLOCK;
}

/**
 * @函数名称: block_queue_lock
 * @功能描述: 上锁块队列
 * @输入: q - 指向块队列的指针
 * @输出: 无
 * @返回值: 无
 */
void block_queue_lock(BLOCK_QUEUE_T *q) 
{
    CRITICAL_SECTION_ENTER(); // 进入临界区
    q->lock_flag = DCDC_BLOCK_QUEUE_LOCK; // 上锁
    CRITICAL_SECTION_EXIT(); // 退出临界区
}

/**
 * @函数名称: block_queue_unlock
 * @功能描述: 解锁块队列
 * @输入: q - 指向块队列的指针
 * @输出: 无
 * @返回值: 无
 */
void block_queue_unlock(BLOCK_QUEUE_T *q) 
{
    CRITICAL_SECTION_ENTER(); // 进入临界区
    q->lock_flag = DCDC_BLOCK_QUEUE_UNLOCK; // 解锁
    CRITICAL_SECTION_EXIT(); // 退出临界区
}

/**
 * @函数名称: block_queue_is_full
 * @功能描述: 判断块队列是否已满
 * @输入: q - 指向块队列的指针
 * @输出: 无
 * @返回值: 1 - 队列已满, 0 - 队列未满
 */
uint8_t block_queue_is_full(BLOCK_QUEUE_T *q) {
    return q->count == DCDC_BLOCK_QUEUE_SIZE ? 1 : 0;
}

/**
 * @函数名称: block_queue_is_empty
 * @功能描述: 判断块队列是否为空
 * @输入: q - 指向块队列的指针
 * @输出: 无
 * @返回值: 1 - 队列为空, 0 - 队列不为空
 */
uint8_t block_queue_is_empty(BLOCK_QUEUE_T *q) {
    return q->count == 0 ? 1 : 0;
}

/**
 * @函数名称: block_queue_enqueue
 * @功能描述: 将数据块入队（复制数据）
 * @输入: 
 *      q - 指向块队列的指针
 *      data - 指向待入队数据的指针
 *      len - 数据长度
 * @输出: 无
 * @返回值: 0 - 入队成功, 1 - 入队失败
 */
uint8_t block_queue_enqueue(BLOCK_QUEUE_T *q, const uint8_t *data, uint16_t len) 
{
    
    if (q->lock_flag == DCDC_BLOCK_QUEUE_LOCK || block_queue_is_full(q) || len > DCDC_BLOCK_MAX_SIZE) {
        return 1; // 失败
    }
    block_queue_lock(q);  // 加锁

    DATA_BLOCK_T *block = &q->blocks[q->head];
    memcpy(block->data, data, len);
    block->length = len;

    q->head = (q->head + 1) % DCDC_BLOCK_QUEUE_SIZE;
    q->count++;

    block_queue_unlock(q);  // 解锁
    return 0; // 成功
}

/**
 * @函数名称: block_queue_dequeue
 * @功能描述: 将数据块出队（返回指针，不拷贝数据）
 * @输入: q - 指向块队列的指针
 * @输出: block_out - 指向出队数据块指针的指针
 * @返回值: 0 - 出队成功, 1 - 出队失败
 */
uint8_t block_queue_dequeue(BLOCK_QUEUE_T *q, DATA_BLOCK_T **block_out) 
{
    if (q->lock_flag == DCDC_BLOCK_QUEUE_LOCK || block_queue_is_empty(q)) {
        return 1; // 失败
    }

    block_queue_lock(q);  // 加锁

    *block_out = &q->blocks[q->tail];
    q->tail = (q->tail + 1) % DCDC_BLOCK_QUEUE_SIZE;
    q->count--;

    block_queue_unlock(q);  // 解锁
    return 0; // 成功
}

/* ***********************DCDC-通信接口*********************** */
/**
 * @函数名称: communicate_usart0_store_to_buffer()
 * @功能描述: 将USART0接收的消息写入环形缓冲区
 * @输入: 
 *      p_data 接收数据地址
 *      len 接收数据长度
 * @输出: 无
 * @返回值: 1：写入失败 0：写入成功
 */
static void communicate_usart0_recive(uint8_t* p_data, uint32_t len) 
{
    {
        /* 测试打印 */
        DCDC_LOG("USART0 receive data: num = %ld, data = ", len);
        for (uint32_t i = 0; i < len; i++) {
            DCDC_LOG("0x%02X ", p_data[i]);
        }
        DCDC_LOG("\r\n");
    }
    
    if(len<1) {
        return;
    }

    if (block_queue_is_full(&t_dcdc_recive_queue)) {
        DCDC_LOG("Warning: dcdc receive queue is full. Data will be discarded!\n");
        return; // 队列已满，丢弃数据
    }

    if (block_queue_enqueue(&t_dcdc_recive_queue, p_data, len)) {
        DCDC_LOG("Error: Failed to enqueue data to dcdc receive queue!\n");
        return; // 入队失败
    }

}



/**
* @函数名称: dcdc_communicate_init()
* @功能描述: 与dcdc板通信的初始化任务,注册回调函数
* @输入: 无
* @输出: 无
* @返回值: 正确：0 失败：1
*/
uint8_t dcdc_communicate_init(void)
{
    /* 队列初始化 */
    block_queue_init(&t_dcdc_recive_queue);
    block_queue_init(&t_dcdc_send_queue);


    /* 通信接口初始化 */
    switch (dcdc_communication_param.com_type)
    {
        case COM_USART0: {
            /* 检查接口函数是否有效 */
            if (!dcdc_communication_param.uart_handle.int_handle || 
            !dcdc_communication_param.uart_handle.send_handle || 
            !dcdc_communication_param.uart_handle.set_recive_handle) {
                return 1; // 初始化失败
            }

            /* 初始化通信接口并设置波特率 */
            dcdc_communication_param.uart_handle.int_handle(DCDC_COM_UART_BPS_DEFAULT);

            /* 注册接收回调函数 */
            if (dcdc_communication_param.uart_handle.set_recive_handle(communicate_usart0_recive)) {
                return 1; // 注册回调函数失败
            }
        }
        
        break;
        case COM_USART1:break;
        case COM_USART2:break;
        case COM_CAN0:break;
        case COM_CAN1:break;
    default:
        break;
    }
    return 0;
}

/**
* @函数名称: dcdc_communicate_com_set()
* @功能描述: 设置dcdc通信接口,设置完后会自动初始化 重新初始化通信接口
* @输入: 无
* @输出: 无
* @返回值: 正确：0 失败：1
*/
uint8_t dcdc_communicate_com_set(DCDC_COM_TYPE com_type)
{
    dcdc_communication_param.com_type = com_type;
    switch (dcdc_communication_param.com_type)
    {
        case COM_USART0: {
            /* 检查接口函数是否有效 */
            if (!DCDC_COM_UART0_INIT_HANDLE || !DCDC_COM_UART0_SEND_HANDLE || !DCDC_COM_UART0_SET_RECIVE_HANDLE) {
                return 1; // 初始化失败
            }

            /* 注册dcdc通信接口 */
            dcdc_communication_param.uart_handle.int_handle = DCDC_COM_UART0_INIT_HANDLE;
            dcdc_communication_param.uart_handle.send_handle = DCDC_COM_UART0_SEND_HANDLE;
            dcdc_communication_param.uart_handle.set_recive_handle = DCDC_COM_UART0_SET_RECIVE_HANDLE;
        }
        
        break;
        case COM_USART1:break;
        case COM_USART2:break;
        case COM_CAN0:break;
        case COM_CAN1:break;
    default:
        break;
    }

    dcdc_communicate_init(); // 重新初始化通信接口
    return 0; // 成功
} 

/**
 * @函数名称: dcdc_communicate_send()
 * @功能描述: 发送数据到DCDC通信接口
 * @输入: p_data - 指向要发送的数据的指针
 *        len - 数据长度
 * @输出: 无
 * @返回值: 0 - 成功, 1 - 失败
 */
uint8_t dcdc_communicate_send(uint8_t* p_data, uint32_t len) 
{
    if (dcdc_communication_param.uart_handle.send_handle(p_data, len)) {
        return 1; // 发送失败
    }
    /* 对dcdc通信下发的指令进行计数 */
    rom_parameter.dcdc_comm_send_num++;
    return 0; // 成功
}


/* ***********************DCDC-MODBUS协议*********************** */
T_DCDC_DATA_TYPDEF t_dcdc_data = {0};
const T_DCDC_MODBUS_INPUT_REG_ADDRESS_TYPDEF t_dcdc_input_reg[INPUT_REG_INDEX_MAX] = {
    [INPUT_REG_INDEX_DEVICE_ADDRESS   ]={.p_u16_data = &t_dcdc_data.dcdc_device_address     ,},          /* 输入寄存器地址下标-设备地址 */
    [INPUT_REG_INDEX_SOFTWARE_VERSION ]={.p_u16_data = &t_dcdc_data.dcdc_sw_version   ,},          /* 输入寄存器地址下标-软件版本号（示例v1） */
    [INPUT_REG_INDEX_STATUS           ]={.p_u16_data = &t_dcdc_data.status             ,},          /* 输入寄存器地址下标-状态 */
    [INPUT_REG_INDEX_U_ALARM          ]={.p_u16_data = &t_dcdc_data.u_alarm.value      ,},          /* 输入寄存器地址下标-报警（共用体 位操作） */
    [INPUT_REG_INDEX_U_PROTECTION     ]={.p_u16_data = &t_dcdc_data.u_protection.value ,},          /* 输入寄存器地址下标-保护（共用体 位操作） */
    [INPUT_REG_INDEX_BUS_VOLTAGE      ]={.p_u16_data = &t_dcdc_data.dcdc_bus_voltage        ,},          /* 输入寄存器地址下标-母线电压（0.1V） */
    [INPUT_REG_INDEX_BUS_CURRENT      ]={.p_i16_data = &t_dcdc_data.dcdc_bus_current        ,},          /* 输入寄存器地址下标-母线电流（mA） */
    [INPUT_REG_INDEX_BATTERY_VOLTAGE  ]={.p_u16_data = &t_dcdc_data.dcdc_lv_voltage    ,},          /* 输入寄存器地址下标-低压电压（电池端）（0.01V） */
    [INPUT_REG_INDEX_BATTERY_CURRENT  ]={.p_i16_data = &t_dcdc_data.dcdc_lv_current    ,},          /* 输入寄存器地址下标-低压电流（电池端）（mA） */
    [INPUT_REG_INDEX_TEMPERATURE      ]={.p_i16_data = &t_dcdc_data.dcdc_temperature        ,},          /* 输入寄存器地址下标-温度（0.01℃） */
    [INPUT_REG_INDEX_CURRENT_POWER    ]={.p_u16_data = &t_dcdc_data.dcdc_power      ,},          /* 输入寄存器地址下标-当前功率值（W） */
};

const T_DCDC_MODBUS_HOLD_REG_ADDRESS_TYPDEF t_dcdc_hold_reg[HOLD_REG_INDEX_MAX] = {
    [HOLD_REG_INDEX_SLEEP_STATUS                            ]={.p_u16_data = &t_dcdc_data.sleep_status},                /* 保持寄存器地址下标-睡眠 */
    [HOLD_REG_INDEX_POWER_STATUS                            ]={.p_u16_data = &t_dcdc_data.power_status},                /* 保持寄存器地址下标-开/关机 */
    [HOLD_REG_INDEX_CHARGE_DISCHARGE_CONTROL                ]={.p_u16_data = &t_dcdc_data.charge_discharge_control},    /* 保持寄存器地址下标-充、放电控制 */
    [HOLD_REG_INDEX_CURRENT_CALIBRATION_PREPARATION         ]={.p_u16_data = &t_dcdc_data.current_calibration_preparation},
    [HOLD_REG_INDEX_CURRENT_CALIBRATION_VALUE               ]={.p_u16_data = &t_dcdc_data.current_calibration_value},
    [HOLD_REG_INDEX_VOLTAGE_CALIBRATION_PREPARATION         ]={.p_u16_data = &t_dcdc_data.voltage_calibration_preparation},
    [HOLD_REG_INDEX_VOLTAGE_CALIBRATION_VALUE               ]={.p_u16_data = &t_dcdc_data.voltage_calibration_value},
    [HOLD_REG_INDEX_TEMP_CALIBRATION_PREPARATION            ]={.p_u16_data = &t_dcdc_data.temp_calibration_preparation},
    [HOLD_REG_INDEX_TEMP_CALIBRATION_VALUE                  ]={.p_u16_data = &t_dcdc_data.temp_calibration_value},
    [HOLD_REG_INDEX_FREQ_CALIBRATION_PREPARATION            ]={.p_u16_data = &t_dcdc_data.freq_calibration_preparation},
    [HOLD_REG_INDEX_FREQ_CALIBRATION_VALUE                  ]={.p_u16_data = &t_dcdc_data.freq_calibration_value},
    [HOLD_REG_INDEX_RECEIVE_NUM                             ]={.p_u16_data = &t_dcdc_data.usart_receive_num},
    [HOLD_REG_INDEX_SEND_NUM                                ]={.p_u16_data = &t_dcdc_data.usart_send_num},
    [HOLD_REG_INDEX_ERASE_USART_NUM_CLT                     ]={.p_u16_data = &t_dcdc_data.erase_usart_num_clt},
};


/**
 * @函数名称: g_modbus_master
 * @功能描述: Modbus主机状态机实例
 */
T_DCDC_MODBUS_MASTER_TYPDEF g_modbus_master = {0};


/**
 * @函数名称: modbus_crc16
 * @功能描述: 计算 Modbus CRC16 校验
 * 名称：CRC16-ANSI
 *      多项式：0xA001
 *      初始值：0xFFFF
 *      每个字节与CRC的低8位进行异或腐蚀。
 *      根据最低位是1还是0，决定0xA001（0x8005 高位在前）进行
 *      最终生成的CRC为低位在前，高位在后（小端格式）。
 * @输入: buffer - 数据缓冲区指针
 * @输入: length - 数据长度
 * @输出: 无
 * @返回值: CRC16 校验值
 */
// static uint16_t modbus_crc16(uint8_t *data, uint16_t length) {
//     uint16_t crc = 0xFFFF; // 初始值为 0xFFFF
//     for (uint16_t i = 0; i < length; i++) {
//         crc ^= data[i]; // 将当前字节与 CRC 的低 8 位进行异或
//         for (uint8_t j = 0; j < 8; j++) {
//             if (crc & 0x0001) {
//                 crc = (crc >> 1) ^ 0xA001; // 右移并与多项式 0xA001 异或
//             } else {
//                 crc = crc >> 1; // 直接右移
//             }
//         }
//     }
//     return crc; // 返回最终 CRC 值
// }

/**
 * @函数名称: dcdc_modbus_master_init
 * @功能描述: 初始化 Modbus 主机
 * @输入: 无
 * @输出: 无
 * @返回值: 0 - 初始化成功，1 - 初始化失败
 */
uint8_t dcdc_modbus_master_init(void)
{
    memset(&g_modbus_master, 0, sizeof(g_modbus_master));
    g_modbus_master.state = DCDC_MODBUS_STATE_IDLE;

    return 0; // 成功
}

/**
 * @函数名称: send_modbus_error()
 * @功能描述: 发送 Modbus 错误响应
 * @输入: func_code - 功能码
 * @输入: error_code - 错误码
 * @输出: 无
 * @返回值: 无
 */
void send_modbus_error(uint8_t func_code, uint8_t error_code)
{
    uint8_t resp[5];
    resp[0] = DCDC_MODBUS_DEVICE_ADDR;
    resp[1] = func_code | 0x80;
    resp[2] = error_code;
    uint16_t crc = modbus_crc16(resp, 3);
    resp[3] = crc & 0xFF;
    resp[4] = crc >> 8;
    dcdc_communicate_send(resp, 5);
}

/**
 * @函数名称: dcdc_modbus_read_holding_registers
 * @功能描述: 发起读保持寄存器请求（功能码0x03）
 * @输入: slave_addr - 从机地址
 * @输入: start_addr - 起始寄存器地址
 * @输入: quantity - 读取寄存器数量
 * @输出: 无
 * @返回值: 0 - 请求成功，1 - 请求失败（主机忙）
 */
uint8_t dcdc_modbus_read_holding_registers(uint8_t slave_addr, uint16_t start_addr, uint16_t quantity)
{
    if (g_modbus_master.state != DCDC_MODBUS_STATE_IDLE) {
        return 1; /* 主机忙 */
    }

    g_modbus_master.request_buffer[0] = slave_addr;
    g_modbus_master.request_buffer[1] = DCDC_MODBUS_CODE_03H; /* 功能码 */
    g_modbus_master.request_buffer[2] = (start_addr >> 8) & 0xFF;
    g_modbus_master.request_buffer[3] = start_addr & 0xFF;
    g_modbus_master.request_buffer[4] = (quantity >> 8) & 0xFF;
    g_modbus_master.request_buffer[5] = quantity & 0xFF;

    uint16_t crc = modbus_crc16(g_modbus_master.request_buffer, 6);
    g_modbus_master.request_buffer[6] = crc & 0xFF;
    g_modbus_master.request_buffer[7] = (crc >> 8) & 0xFF;

    g_modbus_master.request_length = 8;
    g_modbus_master.state = DCDC_MODBUS_STATE_SEND_REQUEST;

    return 0; // 成功
}

/**
 * @函数名称: dcdc_modbus_read_input_registers
 * @功能描述: 发起读输入寄存器请求（功能码0x04）
 * @输入: slave_addr - 从机地址
 * @输入: start_addr - 起始寄存器地址
 * @输入: quantity - 读取寄存器数量
 * @输出: 无
 * @返回值: 0 - 请求成功，1 - 请求失败（主机忙）
 */
uint8_t dcdc_modbus_read_input_registers(uint8_t slave_addr, uint16_t start_addr, uint16_t quantity)
{
    if (g_modbus_master.state != DCDC_MODBUS_STATE_IDLE) {
        return 1; /* 主机忙 */
    }

    g_modbus_master.request_buffer[0] = slave_addr;
    g_modbus_master.request_buffer[1] = DCDC_MODBUS_CODE_04H; /* 功能码 */
    g_modbus_master.request_buffer[2] = (start_addr >> 8) & 0xFF;
    g_modbus_master.request_buffer[3] = start_addr & 0xFF;
    g_modbus_master.request_buffer[4] = (quantity >> 8) & 0xFF;
    g_modbus_master.request_buffer[5] = quantity & 0xFF;

    uint16_t crc = modbus_crc16(g_modbus_master.request_buffer, 6);
    g_modbus_master.request_buffer[6] = crc & 0xFF;
    g_modbus_master.request_buffer[7] = (crc >> 8) & 0xFF;

    g_modbus_master.request_length = 8;
    g_modbus_master.state = DCDC_MODBUS_STATE_SEND_REQUEST;

    return 0;
}

/**
 * @函数名称: dcdc_modbus_write_single_register
 * @功能描述: 发起写单个保持寄存器请求（功能码0x06）
 * @输入: slave_addr - 从机地址
 * @输入: reg_addr - 要写的寄存器地址
 * @输入: value - 要写入的值
 * @输出: 无
 * @返回值: 0 - 请求成功，1 - 请求失败（主机忙）
 */
uint8_t dcdc_modbus_write_single_register(uint8_t slave_addr, uint16_t reg_addr, uint16_t value)
{
    if (g_modbus_master.state != DCDC_MODBUS_STATE_IDLE) {
        return 1; /* 主机忙 */
    }

    g_modbus_master.request_buffer[0] = slave_addr;
    g_modbus_master.request_buffer[1] = DCDC_MODBUS_CODE_06H; /* 功能码 */
    g_modbus_master.request_buffer[2] = (reg_addr >> 8) & 0xFF;
    g_modbus_master.request_buffer[3] = reg_addr & 0xFF;
    g_modbus_master.request_buffer[4] = (value >> 8) & 0xFF;
    g_modbus_master.request_buffer[5] = value & 0xFF;

    uint16_t crc = modbus_crc16(g_modbus_master.request_buffer, 6);
    g_modbus_master.request_buffer[6] = crc & 0xFF;
    g_modbus_master.request_buffer[7] = (crc >> 8) & 0xFF;

    g_modbus_master.request_length = 8;
    g_modbus_master.state = DCDC_MODBUS_STATE_SEND_REQUEST;

    return 0;
}

/**
 * @函数名称: dcdc_modbus_write_multiple_registers
 * @功能描述: 发起写多个连续保持寄存器请求（功能码0x10）
 * @输入: slave_addr - 从机地址
 * @输入: start_addr - 起始寄存器地址
 * @输入: quantity - 连续寄存器数量
 * @输入: p_data - 指向待写入的数据数组（每个元素16位）
 * @输出: 无
 * @返回值: 0 - 请求成功，1 - 请求失败（主机忙）
 */
uint8_t dcdc_modbus_write_multiple_registers(uint8_t slave_addr, uint16_t start_addr, uint16_t quantity, uint16_t *p_data)
{
    if ((g_modbus_master.state != DCDC_MODBUS_STATE_IDLE) || (p_data == NULL)) {
        return 1; /* 主机忙 或 空指针 */
    }

    g_modbus_master.request_buffer[0] = slave_addr;
    g_modbus_master.request_buffer[1] = DCDC_MODBUS_CODE_10H; /* 功能码 */
    g_modbus_master.request_buffer[2] = (start_addr >> 8) & 0xFF;
    g_modbus_master.request_buffer[3] = start_addr & 0xFF;
    g_modbus_master.request_buffer[4] = (quantity >> 8) & 0xFF;
    g_modbus_master.request_buffer[5] = quantity & 0xFF;
    g_modbus_master.request_buffer[6] = quantity * 2; /* 后面字节数 = 寄存器数 × 2 */

    /* 填写寄存器数据 */
    for (uint16_t i = 0; i < quantity; i++)
    {
        g_modbus_master.request_buffer[7 + i * 2] = (p_data[i] >> 8) & 0xFF;
        g_modbus_master.request_buffer[8 + i * 2] = p_data[i] & 0xFF;
    }

    uint16_t frame_len = 7 + quantity * 2;
    uint16_t crc = modbus_crc16(g_modbus_master.request_buffer, frame_len);
    g_modbus_master.request_buffer[frame_len] = crc & 0xFF;
    g_modbus_master.request_buffer[frame_len + 1] = (crc >> 8) & 0xFF;

    g_modbus_master.request_length = frame_len + 2;
    g_modbus_master.state = DCDC_MODBUS_STATE_SEND_REQUEST;

    return 0;
}


/**
 * @函数名称: modbus_receive_data()
 * @功能描述: 接收 Modbus 数据并处理
 * @输入: data - 接收到的数据指针
 * @输入: len - 数据长度
 * @输出: 无
 * @返回值: 接收字节长度
 */
uint16_t modbus_receive_data(uint8_t *buffer, uint16_t buffer_size)
{
    uint16_t len = 0;
    DATA_BLOCK_T *block = NULL;

    if (block_queue_dequeue(&t_dcdc_recive_queue, &block) == 0) {
        len = block->length;
        if (len > buffer_size) {
            len = buffer_size; // 限制长度
        }
        memcpy(buffer, block->data, len);
    } else {
        len = 0; // 队列为空
    }

    return len; // 返回接收字节长度
}

/**
 * @函数名称: modbus_parse_input_registers
 * @功能描述: 解析 Modbus 读输入寄存器响应数据，填充对应结构体
 * @输入: start_addr - 起始寄存器地址（偏移地址）
 * @输入: data - 寄存器数据区（高位在前）
 * @输入: length - 数据长度（单位: 字节）
 * @输入: pt_dcdc_input_reg - 指向输入寄存器结构体的指针
 * @输出: 无
 * @返回值: 0 成功，1 失败
 */
uint8_t modbus_parse_input_registers(uint16_t start_addr, uint8_t *data, uint16_t length, const T_DCDC_MODBUS_INPUT_REG_ADDRESS_TYPDEF *pt_dcdc_input_reg)
{
    if ((data == NULL) || (pt_dcdc_input_reg == NULL)) {
        return 1;
    }

    uint16_t reg_count = length / 2;

    for (uint16_t i = 0; i < reg_count; i++) {
        uint16_t value = (data[i * 2] << 8) | data[i * 2 + 1];
        uint16_t addr = start_addr + i;

        switch (addr) {
            // case INPUT_REG_INDEX_DEVICE_ADDRESS   : *pt_dcdc_input_reg[INPUT_REG_INDEX_DEVICE_ADDRESS   ].p_u16_data = value; printf("INPUT_REG_INDEX_DEVICE_ADDRESS = %d\r\n", value); break;            /* 设备地址 */
            // case INPUT_REG_INDEX_SOFTWARE_VERSION : *pt_dcdc_input_reg[INPUT_REG_INDEX_SOFTWARE_VERSION ].p_u16_data = value; printf("INPUT_REG_INDEX_SOFTWARE_VERSION = %d\r\n", value); break;            /* 软件版本号（示例v1） */
            // case INPUT_REG_INDEX_STATUS           : *pt_dcdc_input_reg[INPUT_REG_INDEX_STATUS           ].p_u16_data = value; printf("INPUT_REG_INDEX_STATUS = %d\r\n", value); break;            /* 状态 */
            // case INPUT_REG_INDEX_U_ALARM          : *pt_dcdc_input_reg[INPUT_REG_INDEX_U_ALARM          ].p_u16_data = value; printf("INPUT_REG_INDEX_U_ALARM = %d\r\n", value); break;            /* 报警（共用体 位操作） */
            // case INPUT_REG_INDEX_U_PROTECTION     : *pt_dcdc_input_reg[INPUT_REG_INDEX_U_PROTECTION     ].p_u16_data = value; printf("INPUT_REG_INDEX_U_PROTECTION = %d\r\n", value); break;            /* 保护（共用体 位操作） */
            // case INPUT_REG_INDEX_BUS_VOLTAGE      : *pt_dcdc_input_reg[INPUT_REG_INDEX_BUS_VOLTAGE      ].p_u16_data = value; printf("INPUT_REG_INDEX_BUS_VOLTAGE = %d\r\n", value); break;            /* 母线电压（0.1V） */
            // case INPUT_REG_INDEX_BUS_CURRENT      : *pt_dcdc_input_reg[INPUT_REG_INDEX_BUS_CURRENT      ].p_i16_data = (int16_t)value; printf("INPUT_REG_INDEX_BUS_CURRENT = %d\r\n", value); break;   /* 母线电流（mA） */
            // case INPUT_REG_INDEX_BATTERY_VOLTAGE  : *pt_dcdc_input_reg[INPUT_REG_INDEX_BATTERY_VOLTAGE  ].p_u16_data = value; printf("INPUT_REG_INDEX_BATTERY_VOLTAGE = %d\r\n", value); break;            /* 低压电压（电池端）（0.01V） */
            // case INPUT_REG_INDEX_BATTERY_CURRENT  : *pt_dcdc_input_reg[INPUT_REG_INDEX_BATTERY_CURRENT  ].p_i16_data = (int16_t)value; printf("INPUT_REG_INDEX_BATTERY_CURRENT = %d\r\n", value); break;   /* 低压电流（电池端）（mA） */
            // case INPUT_REG_INDEX_TEMPERATURE      : *pt_dcdc_input_reg[INPUT_REG_INDEX_TEMPERATURE      ].p_i16_data = (int16_t)value; printf("INPUT_REG_INDEX_TEMPERATURE = %d\r\n", value); break;   /* 温度（0.01℃） */
            // case INPUT_REG_INDEX_CURRENT_POWER    : *pt_dcdc_input_reg[INPUT_REG_INDEX_CURRENT_POWER    ].p_u16_data = value; printf("INPUT_REG_INDEX_CURRENT_POWER = %d\r\n", value); break;            /* 当前功率值（W） */
 
            case INPUT_REG_INDEX_DEVICE_ADDRESS   : *pt_dcdc_input_reg[INPUT_REG_INDEX_DEVICE_ADDRESS   ].p_u16_data = value; break;            /* 设备地址 */
            case INPUT_REG_INDEX_SOFTWARE_VERSION : *pt_dcdc_input_reg[INPUT_REG_INDEX_SOFTWARE_VERSION ].p_u16_data = value; break;            /* 软件版本号（示例v1） */
            case INPUT_REG_INDEX_STATUS           : *pt_dcdc_input_reg[INPUT_REG_INDEX_STATUS           ].p_u16_data = value; break;            /* 状态 */
            case INPUT_REG_INDEX_U_ALARM          : *pt_dcdc_input_reg[INPUT_REG_INDEX_U_ALARM          ].p_u16_data = value; break;            /* 报警（共用体 位操作） */
            case INPUT_REG_INDEX_U_PROTECTION     : *pt_dcdc_input_reg[INPUT_REG_INDEX_U_PROTECTION     ].p_u16_data = value; break;            /* 保护（共用体 位操作） */
            case INPUT_REG_INDEX_BUS_VOLTAGE      : *pt_dcdc_input_reg[INPUT_REG_INDEX_BUS_VOLTAGE      ].p_u16_data = value; break;            /* 母线电压（0.1V） */
            case INPUT_REG_INDEX_BUS_CURRENT      : *pt_dcdc_input_reg[INPUT_REG_INDEX_BUS_CURRENT      ].p_i16_data = (int16_t)value; break;   /* 母线电流（mA） */
            case INPUT_REG_INDEX_BATTERY_VOLTAGE  : *pt_dcdc_input_reg[INPUT_REG_INDEX_BATTERY_VOLTAGE  ].p_u16_data = value; break;            /* 低压电压（电池端）（0.01V） */
            case INPUT_REG_INDEX_BATTERY_CURRENT  : *pt_dcdc_input_reg[INPUT_REG_INDEX_BATTERY_CURRENT  ].p_i16_data = (int16_t)value; break;   /* 低压电流（电池端）（mA） */
            case INPUT_REG_INDEX_TEMPERATURE      : *pt_dcdc_input_reg[INPUT_REG_INDEX_TEMPERATURE      ].p_i16_data = (int16_t)value; break;   /* 温度（0.01℃） */
            case INPUT_REG_INDEX_CURRENT_POWER    : *pt_dcdc_input_reg[INPUT_REG_INDEX_CURRENT_POWER    ].p_u16_data = value; break;            /* 当前功率值（W） */
            
            default: break; // 非法地址忽略
        }
    }

    return 0;
}

/**
 * @函数名称: modbus_parse_holding_registers
 * @功能描述: 解析读保持寄存器 (0x04) 的响应数据
 * @输入: start_addr - 读取的起始寄存器地址（如40001为0）
 * @输入: data - Modbus响应数据区（去掉地址、功能码、CRC）
 * @输入: length - 数据长度
 * @输入: pt_dcdc_hold_reg - 指向保持寄存器结构体的指针
 * @输出: 无
 * @返回值: 0 - 成功，1 - 失败
 */
uint8_t modbus_parse_holding_registers(uint16_t start_addr, uint8_t *data, uint16_t length, const T_DCDC_MODBUS_HOLD_REG_ADDRESS_TYPDEF* pt_dcdc_hold_reg)
{
    if ((data == NULL) || (pt_dcdc_hold_reg == NULL)) {
        return 1;
    }

    uint16_t reg_count = length / 2;

    for (uint16_t i = 0; i < reg_count; i++) {
        uint16_t value = (data[i * 2] << 8) | data[i * 2 + 1];
        uint16_t addr = start_addr + i;

        switch (addr) {
            case HOLD_REG_INDEX_SLEEP_STATUS                                        : *pt_dcdc_hold_reg[HOLD_REG_INDEX_SLEEP_STATUS                         ].p_u16_data = value; break;  /* 保持寄存器地址下标-睡眠 */
            case HOLD_REG_INDEX_POWER_STATUS                                        : *pt_dcdc_hold_reg[HOLD_REG_INDEX_POWER_STATUS                         ].p_u16_data = value; break;  /* 保持寄存器地址下标-开/关机 */
            case HOLD_REG_INDEX_CHARGE_DISCHARGE_CONTROL                            : *pt_dcdc_hold_reg[HOLD_REG_INDEX_CHARGE_DISCHARGE_CONTROL             ].p_u16_data = value; break;  /* 保持寄存器地址下标-充、放电控制 */
            case HOLD_REG_INDEX_CURRENT_CALIBRATION_PREPARATION                     : *pt_dcdc_hold_reg[HOLD_REG_INDEX_CURRENT_CALIBRATION_PREPARATION      ].p_u16_data = value; break;  
            case HOLD_REG_INDEX_CURRENT_CALIBRATION_VALUE                           : *pt_dcdc_hold_reg[HOLD_REG_INDEX_CURRENT_CALIBRATION_VALUE            ].p_u16_data = value; break;  
            case HOLD_REG_INDEX_VOLTAGE_CALIBRATION_PREPARATION                     : *pt_dcdc_hold_reg[HOLD_REG_INDEX_VOLTAGE_CALIBRATION_PREPARATION      ].p_u16_data = value; break;  
            case HOLD_REG_INDEX_VOLTAGE_CALIBRATION_VALUE                           : *pt_dcdc_hold_reg[HOLD_REG_INDEX_VOLTAGE_CALIBRATION_VALUE            ].p_u16_data = value; break;  
            case HOLD_REG_INDEX_TEMP_CALIBRATION_PREPARATION                        : *pt_dcdc_hold_reg[HOLD_REG_INDEX_TEMP_CALIBRATION_PREPARATION         ].p_u16_data = value; break;  
            case HOLD_REG_INDEX_TEMP_CALIBRATION_VALUE                              : *pt_dcdc_hold_reg[HOLD_REG_INDEX_TEMP_CALIBRATION_VALUE               ].p_u16_data = value; break;  
            case HOLD_REG_INDEX_FREQ_CALIBRATION_PREPARATION                        : *pt_dcdc_hold_reg[HOLD_REG_INDEX_FREQ_CALIBRATION_PREPARATION         ].p_u16_data = value; break;  
            case HOLD_REG_INDEX_FREQ_CALIBRATION_VALUE                              : *pt_dcdc_hold_reg[HOLD_REG_INDEX_FREQ_CALIBRATION_VALUE               ].p_u16_data = value; break;  
            case HOLD_REG_INDEX_RECEIVE_NUM                                         : *pt_dcdc_hold_reg[HOLD_REG_INDEX_RECEIVE_NUM                          ].p_u16_data = value; break;  
            case HOLD_REG_INDEX_SEND_NUM                                            : *pt_dcdc_hold_reg[HOLD_REG_INDEX_SEND_NUM                             ].p_u16_data = value; break;  
            case HOLD_REG_INDEX_ERASE_USART_NUM_CLT                                 : *pt_dcdc_hold_reg[HOLD_REG_INDEX_ERASE_USART_NUM_CLT                  ].p_u16_data = value; break;  
            default: break; // 非法地址忽略
        }
    }

    return 0;
}

/**
 * @函数名称: dcdc_modbus_master_poll
 * @功能描述: Modbus 主机状态机轮询处理 建议轮询周期DCDC_MODBUS_PERIODIC_TIME_MS （10）,
 *           若轮询周期有变请根据真实轮询周期修改 宏定义DCDC_MODBUS_PERIODIC_TIME_MS
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void dcdc_modbus_master_poll(void)
{
    static uint32_t idle_read_flag = 0;  /* 空闲读总线标志位 */
    static uint32_t time_count = DCDC_MODBUS_BUS_READY_PERIOD_TIME_MS/2;      /* 计时 */
    uint8_t data_len = 0;
    time_count += DCDC_MODBUS_PERIODIC_TIME_MS; /* 计时 */
    if (time_count > DCDC_MODBUS_BUS_READY_PERIOD_TIME_MS) {
        time_count = 0; // 重置计时器
        idle_read_flag |= (DCDC_MODBUS_READ_HOLDING_REGISTER_FLAG | DCDC_MODBUS_READ_INPUT_REGISTER_FLAG); // 设置空闲读总线标志位
    }

    

    switch (g_modbus_master.state) {
        case DCDC_MODBUS_STATE_IDLE: {
            /* 空闲状态，等待新的请求 */
            /* 空闲计时，每5s读取一次总线所有数据 */
            if (idle_read_flag & DCDC_MODBUS_READ_HOLDING_REGISTER_FLAG) {
                /* 读取保持寄存器 */
                dcdc_modbus_read_holding_registers(DCDC_MODBUS_DEVICE_ADDR, HOLD_REG_INDEX_SLEEP_STATUS, HOLD_REG_INDEX_MAX);
                idle_read_flag &= ~DCDC_MODBUS_READ_HOLDING_REGISTER_FLAG; // 清除标志位
            }
            else if (idle_read_flag & DCDC_MODBUS_READ_INPUT_REGISTER_FLAG) {
                /* 读取输入寄存器 */
                dcdc_modbus_read_input_registers(DCDC_MODBUS_DEVICE_ADDR, INPUT_REG_INDEX_DEVICE_ADDRESS, INPUT_REG_INDEX_MAX);
                idle_read_flag &= ~DCDC_MODBUS_READ_INPUT_REGISTER_FLAG; // 清除标志位
            }
        }
        break;

        case DCDC_MODBUS_STATE_SEND_REQUEST: {
            /* 发送请求 */
            modbus_send_data(g_modbus_master.request_buffer, g_modbus_master.request_length);
            g_modbus_master.timeout_tick = 0;
            g_modbus_master.state        = DCDC_MODBUS_STATE_WAIT_RESPONSE;
        } break;

        case DCDC_MODBUS_STATE_WAIT_RESPONSE: {
            /* 等待从机响应 */
            g_modbus_master.response_length = modbus_receive_data(g_modbus_master.response_buffer, DCDC_MODBUS_MAX_FRAME_LEN);

            /* 最小合法长度 设备地址正确*/
            if ((g_modbus_master.response_length >= 5) &&
                (g_modbus_master.response_buffer[0] == DCDC_MODBUS_DEVICE_ADDR)) {
                g_modbus_master.state = DCDC_MODBUS_STATE_PROCESS_RESPONSE;
            }
            else {
                /* 超时计数 */
                g_modbus_master.timeout_tick += DCDC_MODBUS_PERIODIC_TIME_MS; 
                /* 超时处理 */
                if (g_modbus_master.timeout_tick > DCDC_MODBUS_TIMEOUT_MS) {
                    g_modbus_master.state = DCDC_MODBUS_STATE_ERROR;

                    DCDC_LOG("Modbus error: time out \r\n");
                }
            }
        } break;

        case DCDC_MODBUS_STATE_PROCESS_RESPONSE: {
            /* crc校验 */
            uint16_t crc_calc = modbus_crc16(g_modbus_master.response_buffer, g_modbus_master.response_length - 2);
            uint16_t crc_recv = g_modbus_master.response_buffer[g_modbus_master.response_length - 2] | 
                                (g_modbus_master .response_buffer[g_modbus_master.response_length - 1] << 8);
            if (crc_calc == crc_recv) {
                /* CRC校验通过，处理数据 */

                // printf("len = %d  response_data: ",g_modbus_master.response_length);
                // for(data_len = 0; data_len < g_modbus_master.response_length ; data_len++){
                //     printf(" %x ",g_modbus_master.response_buffer[data_len]);
                // }
                // printf("\r\n");
                /* dcdc的接收完整报文计数 */
                rom_parameter.dcdc_comm_receive_num++;
                // TODO: 这里可以添加数据解析
                uint8_t func_code_request = g_modbus_master.request_buffer[1];  /* 请求功能码 */
                uint8_t func_code_respons = g_modbus_master.response_buffer[1]; /* 应答功能码 */
                
                if (func_code_respons & 0x80) {
                    /* 错误响应 */
                    /* TODO: 可以在这里处理异常码，比如打印、上报、记录日志等 */
                    // example_handle_modbus_exception(func_code_response & 0x7F, exception_code);
                    uint8_t error_code = g_modbus_master.response_buffer[2];
                    // send_modbus_error(func_code_request, error_code);
                    DCDC_LOG("Modbus error: func_code = 0x%02X, error_code = 0x%02X\n", func_code_request, error_code);
                    g_modbus_master.state = DCDC_MODBUS_STATE_IDLE;
                    
                    break;
                }
                else {
                    switch (func_code_respons) {
                        /* 读取保持寄存器值（4区）-应答 */
                        case DCDC_MODBUS_CODE_03H: {
                            uint8_t func_code_request = g_modbus_master.request_buffer[1];
                            uint8_t func_code_respons = g_modbus_master.response_buffer[1];
                            uint16_t start_addr_request = (g_modbus_master.request_buffer[2] << 8) | g_modbus_master.request_buffer[3];
                            uint16_t response_length = g_modbus_master.response_length - 5; // 减去地址、功能码、字节数和CRC
                            if(func_code_request != func_code_respons) {
                                break; // 功能码不匹配
                            }
                            
                                /* 解析保持寄存器数据 */
                            modbus_parse_holding_registers(start_addr_request, &g_modbus_master.response_buffer[3], response_length, t_dcdc_hold_reg);
                        }
                        break;

                        /* 读取输入寄存器值（3区）-应答 */
                        case DCDC_MODBUS_CODE_04H: {
                            
                            uint16_t start_addr_request = (g_modbus_master.request_buffer[2] << 8) | g_modbus_master.request_buffer[3];
                            uint16_t response_length = g_modbus_master.response_length - 5; // 减去地址、功能码、字节数和CRC
                            if(func_code_request != func_code_respons) {
                                break; // 功能码不匹配
                            }
                            modbus_parse_input_registers(start_addr_request, &g_modbus_master.response_buffer[3], response_length, t_dcdc_input_reg);
                        }
                        break;
                        
                            /* 写单个保持寄存器(0x06) */
                        case DCDC_MODBUS_CODE_06H: {
                            if(func_code_request != func_code_respons) {
                                break; // 功能码不匹配
                            }

                            /* 验证回写地址和值是否正确 */
                            uint16_t addr_req = (g_modbus_master.request_buffer[2] << 8) | g_modbus_master.request_buffer[3];
                            uint16_t value_req = (g_modbus_master.request_buffer[4] << 8) | g_modbus_master.request_buffer[5];
                            uint16_t addr_resp = (g_modbus_master.response_buffer[2] << 8) | g_modbus_master.response_buffer[3];
                            uint16_t value_resp = (g_modbus_master.response_buffer[4] << 8) | g_modbus_master.response_buffer[5];

                            if ((addr_req == addr_resp) && (value_req == value_resp)) {
                                /* 写单个寄存器成功，可以在这里通知应用层 */
                                // example_handle_single_write_success(addr_req, value_req);
                            }
                        }
                        break;

                        /* 写多个保持寄存器(0x10) */
                        case DCDC_MODBUS_CODE_10H: {
                            if(func_code_request != func_code_respons) {
                                break; // 功能码不匹配
                            }
                            /* 验证回写的起始地址和数量 */
                            uint16_t start_addr_req = (g_modbus_master.request_buffer[2] << 8) | g_modbus_master.request_buffer[3];
                            uint16_t reg_count_req = (g_modbus_master.request_buffer[4] << 8) | g_modbus_master.request_buffer[5];
                            uint16_t start_addr_resp = (g_modbus_master.response_buffer[2] << 8) | g_modbus_master.response_buffer[3];
                            uint16_t reg_count_resp = (g_modbus_master.response_buffer[4] << 8) | g_modbus_master.response_buffer[5];

                            if ((start_addr_req == start_addr_resp) && (reg_count_req == reg_count_resp)){
                                /* 写多个寄存器成功，可以在这里通知应用层 */
                                // example_handle_multi_write_success(start_addr_req, reg_count_req);
                            }
                        }
                        break;
                        default: break;
                    }
                }
                
                g_modbus_master.state = DCDC_MODBUS_STATE_IDLE;
            }
            else {
                /* CRC错误 */
                g_modbus_master.state = DCDC_MODBUS_STATE_ERROR;
            }
        } break;

        case DCDC_MODBUS_STATE_ERROR: {
            /* 错误处理，回到空闲状态 */
            g_modbus_master.state = DCDC_MODBUS_STATE_IDLE;
        } break;

        default: {
            g_modbus_master.state = DCDC_MODBUS_STATE_IDLE;
        } break;
    }
}



/* modbus总线 流程处理 */
T_DCDC_MODBUS_DEAL_TYPDEF t_dcdc_modbus_deal = {0};

uint8_t dcdc_modbus_deal_set_hold_register(uint16_t reg_addr, uint16_t value)
{
    if (g_modbus_master.state != DCDC_MODBUS_STATE_IDLE) {
        return 1; // 主机忙
    }

    t_dcdc_modbus_deal.deal_flag |= DCDC_MODBUS_WRITE_HOLDING_REGISTER_FLAG;
    t_dcdc_modbus_deal.t_hold_data.err_code = ERR_DCDC_OK;
    t_dcdc_modbus_deal.t_hold_data.register_address = reg_addr;
    t_dcdc_modbus_deal.t_hold_data.u16_data = value;

    return 0; // 成功
}


uint8_t dcdc_modbus_deal_set_hold_register_result(uint8_t err_code)
{

    t_dcdc_modbus_deal.deal_flag &= (~DCDC_MODBUS_WRITE_HOLDING_REGISTER_FLAG);
    /* TODO:实现错误处理 */
    return err_code; // 成功
   
}

uint8_t dcdc_modbus_deal_read_hold_register(uint16_t start_addr, uint16_t len)
{
    if (g_modbus_master.state != DCDC_MODBUS_STATE_IDLE) {
        return 1; // 主机忙
    }

    t_dcdc_modbus_deal.deal_flag |= DCDC_MODBUS_READ_HOLDING_REGISTER_FLAG;
    t_dcdc_modbus_deal.t_read_hold_data.err_code = ERR_DCDC_OK;
    t_dcdc_modbus_deal.t_read_hold_data.register_address = start_addr;
    t_dcdc_modbus_deal.t_read_hold_data.read_len = len;

    return 0; // 成功
}

uint8_t dcdc_modbus_deal_read_hold_register_result(uint8_t err_code)
{
    t_dcdc_modbus_deal.deal_flag &= (~DCDC_MODBUS_READ_HOLDING_REGISTER_FLAG);
    /*  TODO */
    return err_code;
}
uint8_t dcdc_modbus_deal_read_input_register(uint16_t start_addr, uint16_t len)
{
    if (g_modbus_master.state != DCDC_MODBUS_STATE_IDLE) {
        return 1; // 主机忙
    }

    t_dcdc_modbus_deal.deal_flag |= DCDC_MODBUS_READ_INPUT_REGISTER_FLAG;
    t_dcdc_modbus_deal.t_read_input_data.err_code = ERR_DCDC_OK;
    t_dcdc_modbus_deal.t_read_input_data.register_address = start_addr;
    t_dcdc_modbus_deal.t_read_input_data.read_len = len;

    return 0; // 成功
}
uint8_t dcdc_modbus_deal_read_input_register_result(uint8_t err_code)
{

    t_dcdc_modbus_deal.deal_flag &= (~DCDC_MODBUS_READ_INPUT_REGISTER_FLAG);
    /*  TODO */
    return err_code;
}