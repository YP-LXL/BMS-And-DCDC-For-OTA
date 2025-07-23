
#include "atcommand.h"
char*                     p_parse_data_buff  = NULL;
ATCMD_HANDLER_LIST_TYPE * atcmd_handler_list = NULL;
// ATCMD_HANDLER_LIST_TYPE atcmd_handler_list = {0};

/*AT指令和对应处理函数*/
ATCMD_HANDLER_TYPE ATcmd_handler[] = {
	{"AT+LEDOn" , 		&at_ledon_func},
	{"AT+LEDOff", 		&at_ledoff_func},
	{"AT+GPIOOUTOn", 	&at_gpioouton_func},
	{"AT+GPIOOUTOff", 	&at_gpiooutoff_func},
	{"AT+GPIOINOn", 	&at_gpioinon_func},
	{"AT+GPIOINOff", 	&at_gpioinoff_func},
	{"AT+ADC", 			&at_adc_func},
	{"AT+BEEPWM", 		&at_beepwm_func},
	{"AT+FANPWM", 		&at_fanpwm_func},
	{"AT+IICOn", 		&at_iic_func},
	{"AT+SPIOn", 		&at_spi_func},
	{"AT+CANOn", 		&at_canon_func},
	{"AT+CANOff", 		&at_canoff_func},
	{"AT+USARTOn", 		&at_usarton_func},
	{"AT+USARTOff", 	&at_usartoff_func},
	{"AT+SLEEP", 		&at_sleep_func},
	{"AT+WDGTOn", 		&at_wdgton_func},
	{"AT+SOFTRESET", 	&at_softreset_func},
	{"AT+GDID", 		&at_gdid_func},
	{NULL, NULL}
};

/*检查AT指令的前三个字符是否正确*/
uint8_t AT_check_command_valid(void)
{
	char *cmd = (char *)p_parse_data_buff;
	
	char A = *cmd;               //the first charactor is should be 'A'
	char T = *(cmd + 1U);        //the second charactor is should be 'T'
	char P = *(cmd + 2U);        //the third charactor is should be '+'
	
	if (A == 'A' && T == 'T' && P == '+')
		return 1U;
	else
		return 0U;
}

/*获取AT指令的长度*/
uint8_t AT_get_cmd_len(void)
{
	uint8_t i  = 0U;
	char *prev = (char *)p_parse_data_buff;
	char *next = (char *)p_parse_data_buff;
	
	if (AT_check_command_valid()){
		for(; i < TRANSFER_NUM ; i++){   //try to scan all the p_parse_data_buff
			prev = next;
			next = next + i + 1U;
			
			if (*prev != '\r' && *next != '\n'){   //every AT cmd finished with "\r\n"
				continue;
			}
			else
				break;                               //after find finish flags, break, exit loop
		}
		return i;
	}	else
		return 0U; //no command, return 0
}

/*获取AT指令的name  AT+LEDOn,1   name = AT+LEDOn*/
void ATcmd_get_name(char name[])
{
	uint8_t name_len = 0U;
	
	char *p = (char *)p_parse_data_buff;
	char *tmp = (char *)p_parse_data_buff;

	for (; name_len < TRANSFER_NUM ; name_len++){ // try to scan all the p_parse_data_buff     
		p = tmp + name_len;
		if (*p == ',')                               // 如果找到第一个逗号
			break;
		else if ( *p == 92)                          //如果到达结尾标志符（ASCII 码 92 表示 '\\'）
			break;
	}
	
	memcpy(name, (void *)p_parse_data_buff, name_len);/*把接收到的AT指令赋到name上*/
	*(name + name_len) = '\0';
}

/*从 `p_parse_data_buff` 中提取 AT 指令的参数部分（逗号 `,` 之后至 `\r\n` 之前的内容）。*/
void ATcmd_get_params(char params[])
{
    uint8_t i        = 0U;
    uint8_t name_len = 0U;

    char* p_name   = (char*)p_parse_data_buff;
    char* p_params = (char*)p_parse_data_buff;
    char* tmp      = (char*)p_parse_data_buff;  // protect thr original pointer

    for (; i < TRANSFER_NUM; i++) {  // try to scan all the p_parse_data_buff
        p_params = tmp + i;

        if (*p_name != ',') {  // after find the first ',', parama begin
            p_name = p_params;
            name_len++;
        }

        if (*p_params == '\r' && *(p_params + 1) == '\n' ) {  
            // after find "\r\n", AT command
            // finished,break, exit loop
            break;
        }

        // if (*p_params == 92 && *(p_params + 1) == 'r' &&
        //     *(p_params + 2) == 92 &&
        //     *(p_params + 3) == 'n') {  // after find "\r\n", AT command
        //                                // finished,break, exit loop
        //     break;
        // }
    }
    // if p_name == '\', it means there hasn't parameters,
    // eg, AT+TEST\r\n
    // if (*p_name == 92) {
    if (*p_name == '\r') {
        // In this case, params array is NULL, let the first
        // item is '\0' 
        params[0] = '\0';  
    }                      
    else {
        // p_name pointer the first ','. total count remove name lenght, keep
        // "\r\n"
        memcpy(params, (void*)(p_name + 1), (i - name_len + 4U));
        *(params + (i - name_len + 4U + 1U)) = '\0';  // 4U is "\r\n" , 1U used for '\0'
    }
}

/*
它用于将 AT 指令的参数部分进行分割，并存储到二维字符数组 `param` 中，同时返回参数个数。
`params[]`:
提取的 AT 指令参数部分（例如 `"param1,param2,param3\r\n"`）。
`param[][PER_PARAM_MAX_LEN]`:
用于存储每个拆分出的子参数，二维数组，每行存储一个参数。
`uint8_t *param_num`:
输出参数，存储解析后得出的参数个数。

*/
void ATcmd_split_params(char params[], char param[][PER_PARAM_MAX_LEN], uint8_t *param_num) 
{
	char *p = params;
	char *tmp = params;
	uint8_t sub_param_len = 0U;
	uint8_t i = 0U, j = 0U;
	
	if (params[0] == '\0'){              //first to check the params is null or not. if the first item is '\0',
		*param_num = 0u;                   //it means param number is 0.
		return;
	}

	for (; i < ATCMD_PARAMS_MAX_LEN; ){                  
		p = tmp + i;// 更新当前字符指针

		// printf("Debug: i = %u, j = %u, sub_param_len = %u, current char = 0x%02X ('%c')\r\n",
		// 	i, j, sub_param_len, *p, ((*p) >= 32 && (*p) <= 126) ? *p : '.');

		if (*p != ','){
			// 如果不是逗号，累计当前参数的长度
			sub_param_len++;
			i++;
		}else{
			// 如果是逗号，表示一个子参数结束
            // printf("Parameter %u extracted: ", j);
            for (uint8_t k = 0; k < sub_param_len; k++){
                printf("%c", tmp[k]);
            }
            printf("\r\n");

			memcpy(param[j], tmp, (sub_param_len)); // 拷贝当前子参数到 param[j]
			tmp = tmp + sub_param_len + 1; // 更新起始指针到下一个子参数
			sub_param_len = 0U; // 重置当前参数的长度计数
			i = 0U;// 重置当前子参数的扫描索引
			j++; // 指向下一个子参数存储位置
		}

		//92 is the ASCII of '\'根据 `\r\n` 结束标志退出循环，表明 AT 指令解析完成。
		if (*p == '\r' && *(p + 1U) == '\n'){
			// 说明遇到了 "\r\n" 结束标志
			// printf("End flag detected. Final parameter %u: ", j);
			// 假设需要移除最后一个字符（比如换行符），调整复制长度
			memcpy(param[j], tmp, (sub_param_len));
			param[j][sub_param_len] = '\0';
			// printf("%s\r\n", param[j]);
			break;
		}
	}
	
	*param_num = (j + 1);   // j is begin from 0, so the number of param is j + 1;
}


/*AT指令接收核验*/
void ATcmd_init_cmdData(ATCMD_TYPE *cmd, char name[], char params[])
{
	ATcmd_get_name(name);
	ATcmd_get_params(params);
	cmd->name = name;
	cmd->params = params;
	/* printf("Get AT command \r\n name:%s\r\n params:%s\r\n", name, params);*/
}

/*遍历链表寻找匹配的AT指令*/
ATCMD_HANDLER_TYPE * ATcmd_match_handler(ATCMD_TYPE *cmd)
{
	
	ATCMD_HANDLER_LIST_TYPE *p = atcmd_handler_list;

	/* printf("atcmd_handler_list->ATcmd_handler->name= %s\r\n",p->ATcmd_handler->name);*/
	
	for(; p->next != NULL; p = p->next){
		if (p->ATcmd_handler != NULL){
			
		  if (!strcmp(p->ATcmd_handler->name, cmd->name)){
			   return p->ATcmd_handler;
		  }
		}
	}
	/*check last node*/
	if (p->ATcmd_handler != NULL){
	  if (!strcmp(p->ATcmd_handler->name, cmd->name)){
		  return p->ATcmd_handler;
	  }else{
		printf("p->ATcmd_handler->name = %s cmd->name = %s\r\n", p->ATcmd_handler->name, cmd->name);
		  return NULL;
	  }
  }

	return NULL;
}

/*匹配并执行对应的AT指令处理器函数*/
void ATcmd_execute(ATCMD_TYPE *cmd)
{
	ATCMD_HANDLER_TYPE *handler = NULL;
	handler = malloc(sizeof(ATCMD_HANDLER_TYPE));
	handler = ATcmd_match_handler(cmd);

	if (handler){
	  handler->ATfunc(cmd->params);		//传入的是AT指令中的','后面的参数，可能是1,2,3,4等多个
	}
}

/*检测是否接收到AT指令*/
void ATcmd_server(void)
{
	ATCMD_TYPE cmd;
	char name[ATCMD_NAME_MAX_LEN]   = {'\0'};
	char params[ATCMD_PARAMS_MAX_LEN] = {'\0'};
	
    if (NULL == p_parse_data_buff) {
        /* 解析数地址为空 不解析，避免出错 */
        return;
    }
	//check rx has AT command or not
	if (!AT_check_command_valid())
		return;
	ATcmd_init_cmdData(&cmd, name, params);

	ATcmd_execute(&cmd);
}

uint8_t ATcmd_set_parse_data_address(char* p_data) 
{
    if (NULL == p_data) {
        return 1;
    }
    p_parse_data_buff = p_data;
    return 0;
}

/*
	用于将新的AT指令处理器 注册到全局的双向链表 的末尾
自动将新注册的处理函数插入在ATCMD_HANDLER_LIST_TYPE * atcmd_handler_list的尾节点
*/
void ATcmd_register_handler(ATCMD_HANDLER_TYPE *ATcmd_handler)
{
	ATCMD_HANDLER_LIST_TYPE *p = atcmd_handler_list;
	ATCMD_HANDLER_LIST_TYPE *newItem = (ATCMD_HANDLER_LIST_TYPE *)malloc(sizeof(ATCMD_HANDLER_LIST_TYPE));
	printf("ATcmd_handler->name = %s \r\n", ATcmd_handler->name);
	//insert newItem to ATcmd_handler_list last node
	for(; p->next != NULL; p = p->next){}
		
	p->next = newItem;
	newItem->prev = p;
	newItem->next =NULL;
	newItem->ATcmd_handler = ATcmd_handler;
}

/*AT指令注册函数*/
void ATcmd_register(char *ATcmd_name, uint8_t (*ATfunc)(char params[]))
{
	ATCMD_HANDLER_TYPE *ATcmd_handler_register = NULL;

	ATcmd_handler_register = malloc(sizeof(ATCMD_HANDLER_TYPE));
	
	/*将全局结构体里的函数和AT名称一个一个一个承接下来 */
	ATcmd_handler_register->name = malloc(16); //max name size is 16
	memset(ATcmd_handler_register->name, 0, 16); // 确保内存初始化

	memcpy(ATcmd_handler_register->name, ATcmd_name, strlen(ATcmd_name));
	ATcmd_handler_register->ATfunc = ATfunc;

	/*现在ATcmd_handler这个结构体的内容和全局的一致*/
	ATcmd_register_handler(ATcmd_handler_register);	

}

/*AT 指令初始化，由main函数调用*/
void ATcmd_init(void)
{
	uint8_t i = 0U;
	uint8_t ATcmd_num = sizeof(ATcmd_handler)/sizeof(ATCMD_HANDLER_TYPE);
	
	atcmd_handler_list = (ATCMD_HANDLER_LIST_TYPE *)malloc(sizeof(ATCMD_HANDLER_LIST_TYPE)); 	//给全局开辟了空间
	atcmd_handler_list->ATcmd_handler = NULL;
	atcmd_handler_list->prev = NULL;
	atcmd_handler_list->next = NULL;

	for (; i < ATcmd_num; i++){
		if (ATcmd_handler[i].name && ATcmd_handler[i].ATfunc)
			ATcmd_register(ATcmd_handler[i].name, ATcmd_handler[i].ATfunc);		//注册处理函数
	}
}



/********************************utils**************************************/
/*!
    \brief      string switch to int
    \param[in]  the string of number list, eg "123"
    \param[out] none
    \retval     int
*/
int String2Int(char *str) 
{
    char flag = '+';
    long res = 0;
    
    if(*str=='-') 
    {
        ++str; 
        flag = '-'; 
    } 

    while(*str>=48 && *str<=57)
    {
        res = 10*res + *str++-48;
    } 
 
    if(flag == '-')
    {
        res = -res;
    }
 
    return (int)res;
}

/*!
    \brief      string switch to Hex
								ASCII 48->0, 57->9, 65->A, 70->F, 97->a, 102->f
    \param[in]  the string of number list, eg "0x123", must begin with "0x" or "0X"
    \param[out] none
    \retval     int
*/
uint32_t String2Hex(const char *str)
{
    uint32_t res = 0u;
    const char *p = str;
		uint8_t i = 8u; //only deal 8 byte

    if (!str)
			return 0u;
		
//    if (*str != '0' && *(str + 1) != 'x' && *(str + 1) != 'X')
//			p = str + 2;
        
    while((*p>=48u && *p<=57u) || (*p>=65u && *p<=70u) || (*p>=97u && *p<=102u))
    {
			if (i == 0u)
				break;
			else
				i--;
			
       if(*p>=48u && *p<=57u)
				 res = 16u*res + ((*p++)-48u);
       if(*p>=65u && *p<=70u)
         res = 16u*res + (((*p++)-65u) + 10u);
       if(*p>=97 && *p<=102)
         res = 16u*res + (((*p++)-97u) + 10u);
    }

    return res;
}
/*!
    \brief      string switch to float
								ASCII 48->0, 57->9
    \param[in]  the string of number list, eg "8.123"
    \param[out] none
    \retval     int
*/
float String2float(char *str)
{
    char flag = '+';
    char dot_flag = 0;
    float integer = 0.0;
    float fractional = 0.0;
    float res = 0.0;
    float dot_base = 0.1; //fractional increase

    if (!str)
			return 0;

    if(*str=='-')
    {
			++str;
      flag = '-';
    }

    while((*str>=48 && *str<=57) || (*str == '.'))
    {
       if (dot_flag == 0){
				 if (*str != '.')
					 integer = 10*integer + ((*str++)-48);
         else{
           str++;
           dot_flag = 1;
         }
       }

       if (dot_flag == 1){
          fractional = fractional + dot_base*((*str++)-48);
          dot_base *= 0.1;
       
			 }
		 }

    res = integer + fractional;

    if(flag == '-')
    {
        res = -res;
    }

    return res;
}

void String2ASCII(char *str, uint32_t data[])
{
	
}




uint8_t at_ledon_func(char params[])
{
  	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
  	uint8_t param_num = 0;

	ATcmd_split_params(params, param, &param_num);		/*在这个函数中将多个参数进行拆分*/

	if (param_num != 1U){  /*LED的AT指令只有1个参数*/
		printf("AT+RES,ACK\r\n");
		printf("AT+RES,Err,wrong num.[1:LED(J49-2), 2:LED(J49-3), 3:LED(J49-4), 4:LED(J49-5), 5:LED(J49-6), 6:LED(J49-7)]\r\n");
		printf("AT+RES,end\r\n");
		return 0U; 
	}
	
	uint32_t num = String2Int(param[0]);

	if ((num < 1U) || (num > 6u)) {
		printf("AT+RES,ACK\r\n");
		printf("AT+RES,Err,wrong num.[1:LED1(red), 2:LED2]\r\n");
		printf("AT+RES,end\r\n");
		return 0U;
	}
	
	switch (num){
      case 1:/*LED1(red)*/
	  			gd32f4x_timer2_led_j49_6_set_duty(101);
				printf("AT+RES,ACK\r\n");
				printf("AT+RES,LED1(red) on\r\n");
		    printf("AT+RES,end\r\n");			
				break;
      case 2:/*LED2*/
	 		 	gd32f4x_timer0_led_j49_7_set_duty(101);
				printf("AT+RES,ACK\r\n");
				printf("AT+RES,LED2 on\r\n");
		    printf("AT+RES,end\r\n");			
				break;
			default:
				break;
		}

    return 1U;		
}

uint8_t at_ledoff_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status. [1:red, 2:green, 3:blue ~9]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 6u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:LED1(red), 2:LED2]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  switch (num){	
		case 1:/*LED1(red)*/
			  gd32f4x_timer2_led_j49_6_set_duty(0);
			  printf("AT+RES,ACK\r\n");
			  printf("AT+RES,LED1(red) off\r\n");
		  printf("AT+RES,end\r\n");			
			  break;
		case 2:/*LED2*/
				gd32f4x_timer0_led_j49_7_set_duty(0);
			  printf("AT+RES,ACK\r\n");
			  printf("AT+RES,LED2 off\r\n");
		  printf("AT+RES,end\r\n");			
			  break;
		  default:
			  break;
	  }

  return 1U;		
}

uint8_t at_gpioouton_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status. [1:EN_SW1, 2:EN_SW2, 3:CAN,485-POWER, 4:BUZZER, 5:EH_DCH, 6:RS485(/RE), 7:AFE_VPRO, 8:MS0_OUT, 9:AFE-CTL, 10:AFE-SHIP, 11:EN_PWR, 12:EH_CH]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 12u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:EN_SW1, 2:EN_SW2, 3:CAN,485-POWER, 4:BUZZER, 5:EH_DCH, 6:RS485(/RE), 7:AFE_VPRO, 8:MS0_OUT, 9:AFE-CTL, 10:AFE-SHIP, 11:EN_PWR, 12:EH_CH]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  switch (num){
	  	case 1:/*EN_SW1*/
		  	EN_SW1_APS_VBT_ON();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,EN_SW1 on\r\n");
		  	printf("AT+RES,end\r\n");
			break;
	  	case 2:/*EN_SW2*/
		  	EN_SW2_ARC_VBT_ON();
			printf("AT+RES,ACK\r\n");
			printf("EN_SW2 on\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
	  	case 3:/*CAN,485-POWER*/
		  	PERIPHERAL_POWER_VDD_ON();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,CAN,485-POWER on\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
		case 4:/*BUZZER*/
			BUZZER_POWER_ON();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,BUZZER on\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
		case 5:/*EH_DCH*/
			LOW_VOLTAGE_DRIVE_EN_ON();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,EH_DCH on\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
		case 6:/*RS485(/RE)*/
			PERIPHERAL_RS485_EN_ON();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,RS485(/RE) on\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
		case 7:/*AFE_VPRO*/
			VPRO12V8V_CONTROL_ON();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,AFE_VPRO on\r\n");
			printf("AT+RES,end\r\n");			
			break;
		case 8:/*MS0_OUT*/
			MS0_OUT_EN_ON();
		  	printf("AT+RES,ACK\r\n");
		  	printf("AT+RES,MS0_OUT on\r\n");
	  		printf("AT+RES,end\r\n");			
		  	break;
		case 9:/*AFE-CTL*/
			AFESH367309MOS_ON();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,AFE-CTL on\r\n");
			printf("AT+RES,end\r\n");			
			break;
		case 10:/*AFE-SHIP*/
			WAREHOUSE_STAT_CONTR_IN();
	  		printf("AT+RES,ACK\r\n");
	 		printf("AT+RES,AFE-SHIP on\r\n");
  			printf("AT+RES,end\r\n");			
	  		break;
	  	case 11:/*EN_PWR*/
		  	EN_PWR_ON();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,EN_PWR on\r\n");
			printf("AT+RES,end\r\n");			
	  		break;
		case 12:/*EH_CH*/
			HIGH_VOLTAGE_DRIVE_EN_ON();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,EH_CH on\r\n");
			printf("AT+RES,end\r\n");			
			break;					  				
		  	default:
			  break;
	  }

  return 1U;		
}

uint8_t at_gpiooutoff_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  //motor must has two paramters color and status
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status. [1:EN_SW1, 2:EN_SW2, 3:CAN,485-POWER, 4:BUZZER, 5:EH_DCH, 6:RS485(/RE), 7:AFE_VPRO, 8:MS0_OUT, 9:AFE-CTL, 10:AFE-SHIP, 11:EN_PWR, 12:EH_CH]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 12u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:EN_SW1, 2:EN_SW2, 3:CAN,485-POWER, 4:BUZZER, 5:EH_DCH, 6:RS485(/RE), 7:AFE_VPRO, 8:MS0_OUT, 9:AFE-CTL, 10:AFE-SHIP, 11:EN_PWR, 12:EH_CH]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
    switch (num){
	  	case 1:/*EN_SW1*/
		  	EN_SW1_APS_VBT_OFF();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,EN_SW1 off\r\n");
		  	printf("AT+RES,end\r\n");
			break;
	  	case 2:/*EN_SW2*/
		  	EN_SW2_ARC_VBT_OFF();
			printf("AT+RES,ACK\r\n");
			printf("EN_SW2 off\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
	  	case 3:/*CAN,485-POWER*/
		  	PERIPHERAL_POWER_VDD_OFF();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,CAN,485-POWER off\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
		case 4:/*BUZZER*/
			BUZZER_POWER_OFF();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,BUZZER off\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
		case 5:/*EH_DCH*/
			LOW_VOLTAGE_DRIVE_EN_OFF();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,EH_DCH off\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
		case 6:/*RS485(/RE)*/
			PERIPHERAL_RS485_EN_OFF();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,RS485(/RE) off\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
		case 7:/*AFE_VPRO*/
			VPRO12V8V_CONTROL_OFF();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,AFE_VPRO off\r\n");
			printf("AT+RES,end\r\n");			
			break;
		case 8:/*MS0_OUT*/
			MS0_OUT_EN_OFF();
		  	printf("AT+RES,ACK\r\n");
		  	printf("AT+RES,MS0_OUT off\r\n");
	  		printf("AT+RES,end\r\n");			
		  	break;
		case 9:/*AFE-CTL*/
			AFESH367309MOS_OFF();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,AFE-CTL off\r\n");
			printf("AT+RES,end\r\n");			
			break;
		case 10:/*AFE-SHIP*/
			WAREHOUSE_STAT_CONTR_OUT();
	  		printf("AT+RES,ACK\r\n");
	 		printf("AT+RES,AFE-SHIP off\r\n");
  			printf("AT+RES,end\r\n");			
	  		break;
	  	case 11:/*EN_PWR*/
		  	EN_PWR_OFF();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,EN_PWR off\r\n");
			printf("AT+RES,end\r\n");			
	  		break;
		case 12:/*EH_CH*/
			HIGH_VOLTAGE_DRIVE_EN_OFF();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,EH_CH off\r\n");
			printf("AT+RES,end\r\n");			
			break;					  				
		  	default:
				break;
	  }

  return 1U;		
}

uint8_t at_gpioinon_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status. [1:S_HV, 2:MS0_DET, 3:AFE-ALARM, 4:EN_SKY]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 4u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:S_HV, 2:MS0_DET, 3:AFE-ALARM, 4:EN_SKY]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  switch (num){
	  	case 1:/*S_HV*/
		  	gd32f4x_bus_high_voltage_irq_enable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,S_HV on\r\n");
		  	printf("AT+RES,end\r\n");
			break;
	  	case 2:/*MS0_DET*/
		  	gd32f4x_input_irq_enable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,MS0_DET on\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
	  	case 3:/*ALARM*/
		  	gd32f4x_afe_alarm_irq_enable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,ALARM on\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
		case 4:/*EN_SKY*/
			gd32f4x_sky_irq_enable();
		  	printf("AT+RES,ACK\r\n");
		  	printf("AT+RES,EN_SKY on\r\n");
			printf("AT+RES,end\r\n");			
		  	break;
		  	default:
				break;
	  }
  return 1U;		
}

uint8_t at_gpioinoff_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status. [1:S_HV, 2:MS0_DET, 3:AFE-ALARM, 4:EN_SKY]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 4u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:S_HV, 2:MS0_DET, 3:AFE-ALARM, 4:EN_SKY]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  switch (num){
	  	case 1:/*S_HV*/
		  	gd32f4x_bus_high_voltage_irq_disable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,S_HV off\r\n");
		  	printf("AT+RES,end\r\n");
			break;
	  	case 2:/*MS0_DET*/
		  	gd32f4x_input_irq_disable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,MS0_DET off\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
	  	case 3:/*ALARM*/
		  	gd32f4x_afe_alarm_irq_disable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,ALARM off\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
		case 4:/*EN_SKY*/
			gd32f4x_sky_irq_disable();
		  	printf("AT+RES,ACK\r\n");
		  	printf("AT+RES,EN_SKY off\r\n");
			printf("AT+RES,end\r\n");			
		  	break;
		  	default:
				break;
	  }
  return 1U;		
}

uint8_t at_adc_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  //motor must has two paramters color and status
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status.[1:I_SENSE+, 2:V_CAN_485=Vadc*(338.2/8.2), 3:NTC4, 4:BOARD_TEMP, 5:V_BAT+]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 5u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:I_SENSE+, 2:V_CAN_485=Vadc*(338.2/8.2), 3:NTC4, 4:BOARD_TEMP, 5:V_BAT+]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }

  switch (num){
	  	case 1:/*I_SENSE+*/
			printf("AT+RES,ACK\r\n");
			// printf("AT+RES,I_SENSE= %d\r\n",uiDMAData[0]);
		  	printf("AT+RES,end\r\n");
			  break;
	  	case 2:/*V_CAN_485=Vadc*(338.2/8.2)*/
			printf("AT+RES,ACK\r\n");
			// printf("V_CAN_485 = %d\r\n",uiDMAData[1]);
		  printf("AT+RES,end\r\n");			
			break;
	  	case 3:/*NTC4*/
			printf("AT+RES,ACK\r\n");
			// printf("NTC4 = %d\r\n",uiDMAData[2]);
		  printf("AT+RES,end\r\n");			
			break;
		case 4:/*BOARD_TEMP*/
			printf("AT+RES,ACK\r\n");
			//printf("BOARD_TEMP = %d\r\n",uiDMAData[3]);
		  printf("AT+RES,end\r\n");			
			break;
		case 5:/*V_BAT+*/
			printf("AT+RES,ACK\r\n");
			// printf("V_BAT+ = %d\r\n",uiDMAData[4]);
		  	printf("AT+RES,end\r\n");			
			break;
		  	default:
			break;
	  }
  return 1U;		
}

uint8_t at_beepwm_func(char params[])
{
    char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
    uint8_t param_num = 0;
  
    ATcmd_split_params(params, param, &param_num);
  
    if (param_num != 2U) {
        printf("AT+RES,ACK\r\n");
        printf("AT+RES,Err,wrong param num, need <freq>,<duty>\r\n");
        printf("AT+RES,end\r\n");
        return 0U; 
    }
  
    // 直接读取分割后的参数
    uint32_t freq = String2Int(param[0]);    // 频率（Hz）
    uint32_t duty = String2Int(param[1]);    // 占空比（0-100）
  
    // 设置频率和占空比
    gd32f4x_timer0ch1_set_hz(freq);
    gd32f4x_timer0ch1_set_pwm_duty((uint8_t)duty);
    
    // 打印执行结果
    printf("AT+RES,ACK\r\n");
    printf("AT+RES,BEEPWM set to %u Hz, duty %u%%\r\n", freq, duty);
    printf("AT+RES,end\r\n");
  
    return 1U;		
}

uint8_t at_fanpwm_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 2U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status. [1000:timer8_hz, 50:timer8_duty]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
  if (num >= 1000U){  /*motor must has two paramters color and status*/
	printf("AT+RES,ACK\r\n");
	printf("AT+RES,Err,wrong param num, color,status. [1000:timer8_hz, 100:timer8_duty]\r\n");
	printf("AT+RES,end\r\n");
	return 0U; 
	}
  gd32f4x_timer8ch0_set_hz((uint32_t)num);

  char *comma = strchr(param[0], ',');

  if (comma != NULL) {
    num = String2Int(comma + 1);
	if (num >= 100U){  /*motor must has two paramters color and status*/
		printf("AT+RES,ACK\r\n");
		printf("AT+RES,Err,wrong param num, color,status. [1000:timer8_hz, 100:timer8_duty]\r\n");
		printf("AT+RES,end\r\n");
		return 0U; 
		}
    gd32f4x_timer8ch0_set_pwm_duty((uint8_t)num); 
  }
  return 1U;		
}

uint8_t at_iic_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
	uint8_t readbuff[8] = {0};
	uint8_t lenghth = 1;
	uint8_t result_num = 0;
  	ATcmd_split_params(params, param, &param_num);
  
  	if (param_num != 1U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status. [1:RTC时钟, 2:AFE]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  	}
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 2u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:RTC时钟, 2:AFE]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  switch (num){
	  	case 1:/*RTC时钟*/
		   	/* result_num = at8563t_read(lenghth,readbuff[0]); */
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,RTC IIC = NULL\r\n");
		  	printf("AT+RES,end\r\n");
			break;
	  	case 2:/*AFE*/
		  	result_num = sh367309_read(0x19,lenghth,readbuff);
			printf("AT+RES,ACK\r\n");
			printf("AFE IIC = %d\r\n",result_num);
		  	printf("AT+RES,end\r\n");			
			break;
		  	default:
			break;
	  }
  return 1U;		
}

uint8_t at_spi_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
	uint16_t result_num = 0;
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status. [1:SPI flash]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 1u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:SPI flash]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  switch (num){
	  	case 1:/*SPI flash*/
		  	result_num = gd25a32_readid();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,SPI flash id = %x\r\n", result_num);
		  	printf("AT+RES,end\r\n");
			break;
		  	default: 
			break;
	  }
  return 1U;		
}

uint16_t atcommand_can_flag = 0;
uint8_t at_canon_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status. [1:CAN0, 2:CAN1]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 2u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:CAN0, 2:CAN1]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  /*接收中断使能后未实现上位机自发自收的功能,关于自发自收可能会影响后续通信，后期可以考虑添加标志位来区分工作场景*/
  switch (num){
	  	case 1:/*CAN0*/
		  	gd32f4x_can0_rx_irqn_enable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,CAN0 on\r\n");
		  	printf("AT+RES,end\r\n");
			atcommand_can_flag = 1;
			break;
	  	case 2:/*CAN1*/
		  	gd32f4x_can1_rx_irqn_enable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,CAN1 on\r\n");
		  	printf("AT+RES,end\r\n");	
			atcommand_can_flag = 1;		
			break;
		  	default:
			break;
	  }
  return 1U;		
}

uint8_t at_canoff_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status. [1:CAN0, 2:CAN1]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 2u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:CAN0, 2:CAN1]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  switch (num){
	  	case 1:/*CAN0*/
		  	gd32f4x_can0_rx_irqn_disable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,CAN0 off\r\n");
		  	printf("AT+RES,end\r\n");
			atcommand_can_flag = 0;
			break;
	  	case 2:/*CAN1*/
		  	gd32f4x_can1_rx_irqn_disable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,CAN1 off\r\n");
		  	printf("AT+RES,end\r\n");		
			atcommand_can_flag = 0;	
			break;
		  	default:
			break;
	  }
  return 1U;		
}

uint8_t at_usarton_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status. [1:USART0(DCDC通信总线), 2:USART2(485通信总线), 3:USART3(调试总线、串口屏总线)]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 3u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:USART0(DCDC通信总线), 2:USART2(485通信总线), 3:USART3(调试总线、串口屏总线)]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  /*接收中断使能后未实现上位机自发自收的功能,关于自发自收可能会影响后续通信，后期可以考虑添加标志位来区分工作场景*/
  switch (num){
	  	case 1:/*USART0(DCDC通信总线)*/
		  	gd32f4x_usart0_enable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,USART0(DCDC通信总线) on\r\n");
		  	printf("AT+RES,end\r\n");
			break;
	  	case 2:/*USART2(485通信总线)*/
		  	gd32f4x_usart2_enable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,USART2(485通信总线) on\r\n");
		  	printf("AT+RES,end\r\n");			
			  break;
		case 3:/*UART3(调试总线、串口屏总线)*/
		  	gd32f4x_uart3_enable();
		 	printf("AT+RES,ACK\r\n");
			printf("AT+RES,USART3(调试总线、串口屏总线) on\r\n");
			printf("AT+RES,end\r\n");			
			break;
		  	default:
			  break;
	  }
  return 1U;		
}

uint8_t at_usartoff_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status. [1:USART0(DCDC通信总线), 2:USART2(485通信总线),3:USART3(调试总线、串口屏总线)]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 3u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:USART0(DCDC通信总线), 2:USART2(485通信总线),3:USART3(调试总线、串口屏总线)]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  /*接收中断使能后未实现上位机自发自收的功能*/
  switch (num){
	  	case 1:/*USART0(DCDC通信总线)*/
		  	gd32f4x_usart0_disable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,USART0(DCDC通信总线) off\r\n");
		  	printf("AT+RES,end\r\n");
			break;
	  	case 2:/*USART2(485通信总线)*/
		  	gd32f4x_usart2_disable();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,USART2(485通信总线) off\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
		case 3:/*UART3(调试总线、串口屏总线)*/
			gd32f4x_uart3_disable();
		 	printf("AT+RES,ACK\r\n");
			printf("AT+RES,USART3(调试总线、串口屏总线) off\r\n");
			printf("AT+RES,end\r\n");			
			break;
		  	default:
			  break;
	  }
  return 1U;		
}

uint8_t at_sleep_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status.[1:SLEEP, 2:DEEP_SLEEP, 3:STANDBY]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 3u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:SLEEP, 2:DEEP_SLEEP, 3:STANDBY]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  switch (num){
	  	case 1:/*SLEEP*/
		  	gd32f4x_delay_ms(5000);
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,SLEEP on\r\n");
		  	printf("AT+RES,end\r\n");
			gd32f4x_mcu_sleep_enter(MCU_MODE_SLEEP);
			break;
	  	case 2:/*DEEP_SLEEP*/
		  	gd32f4x_delay_ms(5000);
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,DEEP_SLEEP on\r\n");
		  	printf("AT+RES,end\r\n");			
			gd32f4x_mcu_sleep_enter(MCU_MODE_DEEP_SLEEP);
			break;
		case 3:/*STANDBY*/
			gd32f4x_delay_ms(5000);
		 	printf("AT+RES,ACK\r\n");
			printf("AT+RES,STANDBY on\r\n");
			printf("AT+RES,end\r\n");			
			gd32f4x_mcu_sleep_enter(MCU_MODE_STANDBY);
			break;
		  	default:
			break;
	  }
  return 1U;		
}

uint8_t at_wdgton_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status.[1:wwdgt, 2:fwdgt]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 2u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:wwdgt, 2:fwdgt]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  switch (num){
	  	case 1:/*wwdgt*/
		  	gd32f4x_fwdgt_init_ms(500);
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,wwdgt on\r\n");
		  	printf("AT+RES,end\r\n");
			break;
	  	case 2:/*fwdgt*/
		  	gd32f4x_wwdgt_init_ms(500);
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,fwdgt on\r\n");
		  	printf("AT+RES,end\r\n");			
			break;
		  	default:
			  break;
	  }
  return 1U;		
}

uint8_t at_softreset_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  /*motor must has two paramters color and status*/
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status.[1:softreset]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 1u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:softreset]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  switch (num){
	  	case 1:/*[1:softreset]*/
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,softreset on\r\n");
		  	printf("AT+RES,end\r\n");
			PA0_OFF();
			break;
		  	default:
			break;
	  }
  return 1U;		
}

uint8_t at_gdid_func(char params[])
{
	char param[PARAM_MAX_NUM][PER_PARAM_MAX_LEN] = {'\0'};
	uint8_t param_num = 0;
  
  ATcmd_split_params(params, param, &param_num);
  
  if (param_num != 1U){  //motor must has two paramters color and status
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong param num, color,status.[1:READ GD32ID]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U; 
  }
  
  uint32_t num = String2Int(param[0]);
	  
  if ((num < 1U) || (num > 1u)) {
	  printf("AT+RES,ACK\r\n");
	  printf("AT+RES,Err,wrong num.[1:READ GD32ID]\r\n");
	  printf("AT+RES,end\r\n");
	  return 0U;
  }
  
  switch (num){
	  	case 1:/*[1:READ GD32ID]*/
		  	gd32f4x_read_id();
			printf("AT+RES,ACK\r\n");
			printf("AT+RES,READ GD32ID on\r\n");
		  	printf("AT+RES,end\r\n");
			break;
		  	default:
			break;
	  }
  return 1U;		
}
