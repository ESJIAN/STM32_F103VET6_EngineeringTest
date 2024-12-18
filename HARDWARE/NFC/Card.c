/******************** (C) COPYRIGHT 2015 ********************
* 文件名          : Card.c
* 作者            : 优灵科技开发团队
* 版本            : V3.1.0.0
* 日期            : 2015-5-10
* 描述            : 集合了模块对卡片的所有发送命令操作
										可以选择有用的调用，无用的删除
										还有命令的解析，将命令中的卡号和数据等存到Card结构体中
* 技术支持        : QQ: 3419865840
********************************************************************************/
/* 包含文件 -------------------------------------------------------------------*/
#include"Card.H"

/* 宏定义 ---------------------------------------------------------------------*/
/* 本文件使用的变量 -----------------------------------------------------------*/
CARD Card;
CMD Cmd;
/* 本文件使用的函数声明 -------------------------------------------------------*/
unsigned char CheckSum(unsigned char *dat, unsigned char num);
void Delay(__IO uint32_t nCount);
/* 本文件函数体 ---------------------------------------------------------------*/
void UartSendCommand(u8 *buff, u8 cnt);
u8 UartReceiveCommand(void);
/*******************************************************************************
* 函数名         : ReadCard
* 描述           : 读卡号
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void ReadCard(void)
{
	unsigned char len = 3;
	
	Cmd.SendBuffer[0] = len;
	Cmd.SendBuffer[1] = 0;
	Cmd.SendBuffer[2] = 0x10;
	Cmd.SendBuffer[3] = CheckSum(Cmd.SendBuffer, len);
	UartSendCommand(Cmd.SendBuffer, len);
}

/*******************************************************************************
* 函数名         : ReadBlock
* 描述           : 读块数据
* 输入           : block: 将要操作的块地址。
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void ReadBlock(unsigned char block)
{
	unsigned char len = 4;
	
	Cmd.SendBuffer[0] = len;
	Cmd.SendBuffer[1] = 0;
	Cmd.SendBuffer[2] = 0x11;
	Cmd.SendBuffer[3] = block;
	Cmd.SendBuffer[4] = CheckSum(Cmd.SendBuffer, len);
	UartSendCommand(Cmd.SendBuffer, len);
}

/*******************************************************************************
* 函数名         : WriteBlock
* 描述           : 写块数据
* 输入           : block: 将要操作的块地址。
									 blockData: 将要写入的数据。
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void WriteBlock(unsigned char block, unsigned char *blockData)
{
	unsigned char i;
	unsigned char len = 20;
	
	Cmd.SendBuffer[0] = len;
	Cmd.SendBuffer[1] = 0;
	Cmd.SendBuffer[2] = 0x12;
	Cmd.SendBuffer[3] = block;

	for(i = 0; i < 16; i ++)
	{
		Cmd.SendBuffer[4 + i] = blockData[i];
	}
	Cmd.SendBuffer[20] = CheckSum(Cmd.SendBuffer, len);
	UartSendCommand(Cmd.SendBuffer, len);
}

/*******************************************************************************
* 函数名         : MakeCard
* 描述           : 办卡。
* 输入           : block: 将要操作的块地址。
                   value: 将要操作的值。
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void MakeCard(unsigned char block, unsigned long value)
{
	unsigned char len = 9;
	
	Cmd.SendBuffer[0] = len;
	Cmd.SendBuffer[1] = 0;
	Cmd.SendBuffer[2] = 0x13;
	Cmd.SendBuffer[3] = block;
	Cmd.SendBuffer[4] = 0;	//固定为0
	Cmd.SendBuffer[5] = (unsigned char)(value >> 24);
	Cmd.SendBuffer[6] = (unsigned char)(value >> 16);
	Cmd.SendBuffer[7] = (unsigned char)(value >> 8);
	Cmd.SendBuffer[8] = (unsigned char)(value);
	
	Cmd.SendBuffer[9] = CheckSum(Cmd.SendBuffer, len);
	UartSendCommand(Cmd.SendBuffer, len);
}

/*******************************************************************************
* 函数名         : Inc
* 描述           : 充值。
* 输入           : block: 将要操作的块地址。
                   value: 将要操作的值。
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void Inc(unsigned char block, unsigned long value)
{
	unsigned char len = 8;
	
	Cmd.SendBuffer[0] = len;
	Cmd.SendBuffer[1] = 0;
	Cmd.SendBuffer[2] = 0x15;
	Cmd.SendBuffer[3] = block;
	Cmd.SendBuffer[4] = (unsigned char)(value >> 24);
	Cmd.SendBuffer[5] = (unsigned char)(value >> 16);
	Cmd.SendBuffer[6] = (unsigned char)(value >> 8);
	Cmd.SendBuffer[7] = (unsigned char)(value);
	
	Cmd.SendBuffer[8] = CheckSum(Cmd.SendBuffer, len);
	UartSendCommand(Cmd.SendBuffer, len);
}

/*******************************************************************************
* 函数名         : Dec
* 描述           : 扣款。
* 输入           : block: 将要操作的块地址。
                   value: 将要操作的值。
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void Dec(unsigned char block, unsigned long value)
{
	unsigned char len = 8;
	
	Cmd.SendBuffer[0] = len;
	Cmd.SendBuffer[1] = 0;
	Cmd.SendBuffer[2] = 0x16;
	Cmd.SendBuffer[3] = block;
	Cmd.SendBuffer[4] = (unsigned char)(value >> 24);
	Cmd.SendBuffer[5] = (unsigned char)(value >> 16);
	Cmd.SendBuffer[6] = (unsigned char)(value >> 8);
	Cmd.SendBuffer[7] = (unsigned char)(value);
	
	Cmd.SendBuffer[8] = CheckSum(Cmd.SendBuffer, len);
	UartSendCommand(Cmd.SendBuffer, len);
}

/*******************************************************************************
* 函数名         : ClearCard
* 描述           : 清除卡
* 输入           : block: 将要操作的块地址。
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void ClearCard(unsigned char block)
{
	unsigned char len = 7;

	Cmd.SendBuffer[0] = len;
	Cmd.SendBuffer[1] = 0;
	Cmd.SendBuffer[2] = 0x14;
	Cmd.SendBuffer[3] = block;
	Cmd.SendBuffer[4] = 0x38;	
	Cmd.SendBuffer[5] = 0x52;
	Cmd.SendBuffer[6] = 0x7A;
	
	Cmd.SendBuffer[7] = CheckSum(Cmd.SendBuffer, len);	
	UartSendCommand(Cmd.SendBuffer, len);
}

/*******************************************************************************
* 函数名         : CommandProcess
* 描述           : 命令处理。
* 输入           : 无
* 输出           : 无
* 返回           : 执行结果。
                   -0   : 正确。
                   -0xFF: 无卡。
                   -0xFE: 卡操作错误。
                   -0xFD: 没有收到命令。
                   -0xFC: 余额不足。
                   -0xFB: 校验错误。
*******************************************************************************/
unsigned char CommandProcess(void)
{
	unsigned char i, status, bTemp;
	
	//接收到命令
	bTemp = UartReceiveCommand();
	if(bTemp == 1)
	{
		//校验命令
		if(Cmd.ReceiveBuffer[Cmd.ReceiveBuffer[0]] == CheckSum(Cmd.ReceiveBuffer, Cmd.ReceiveBuffer[0]))
		{
			//获取卡号，经过UartReceiveCommnad处理过后，有以下规律
			//Cmd.ReceiveBuffer[0] 		-> 命令长度
			//Cmd.ReceiveBuffer[1] 		-> 模块地址
			//Cmd.ReceiveBuffer[2] 		-> 命令码
			//Cmd.ReceiveBuffer[3] 		-> 状态码
			//Cmd.ReceiveBuffer[4~5] 	-> 卡类型
			//Cmd.ReceiveBuffer[6~9] 	-> 卡号
			//Cmd.ReceiveBuffer[10~25] 	-> 卡数据()
			
			Card.UID = Cmd.ReceiveBuffer[9]; Card.UID <<= 8;
			Card.UID |= Cmd.ReceiveBuffer[8]; Card.UID <<= 8;
			Card.UID |= Cmd.ReceiveBuffer[7]; Card.UID <<= 8;
			Card.UID |= Cmd.ReceiveBuffer[6];
			Cmd.Code = Cmd.ReceiveBuffer[2];
			switch(Cmd.Code)
			{
				//读卡号，这里不能删除
				case 0x90:break;
				//读块数据
				case 0x91:
					for(i = 0; i < 16; i++){Card.BlockData[i] = Cmd.ReceiveBuffer[10 + i];}
					break;
				//写块数据，这里不能删除
				case 0x92: break;
				//办卡，这里不能删除
				case 0x93: break;
				//恢复卡，这里不能删除
				case 0x94: break;
				//充值
				case 0x95:
					Card.Value = Cmd.ReceiveBuffer[10];
					Card.Value <<= 8;
					Card.Value |= Cmd.ReceiveBuffer[11];
					Card.Value <<= 8;
					Card.Value |= Cmd.ReceiveBuffer[12];
					Card.Value <<= 8;
					Card.Value |= Cmd.ReceiveBuffer[13];
					break;
				//扣款
				case 0x96:
					Card.Value = Cmd.ReceiveBuffer[10];
					Card.Value <<= 8;
					Card.Value |= Cmd.ReceiveBuffer[11];
					Card.Value <<= 8;
					Card.Value |= Cmd.ReceiveBuffer[12];
					Card.Value <<= 8;
					Card.Value |= Cmd.ReceiveBuffer[13];
					break;
				//读身份证，这里不能删除
				case 0xA0:break;
				default: status = 0xFB; break;
			}
			status = Cmd.ReceiveBuffer[3];
		}
		else {status = 0xFB;}
	}
	else{status = 0xFD;}
	return status;
}

/*******************************************************************************
* 函数名         : CheckSum
* 描述           : 命令校验。
* 输入           : 	dat : 要校验的数组。
										num : 校验的字节数。
* 输出           : 无
* 返回           : 校验值。
*******************************************************************************/
unsigned char CheckSum(unsigned char *dat, unsigned char num)
{
  unsigned char bTemp = 0, i;
  
  for(i = 0; i < num; i ++){bTemp ^= dat[i];}
  return bTemp;
}

/*******************************************************************************
* 函数名         : UartSendCommand
* 描述           : 串口发送命令
* 输入           : 	buff : 发送的数组。
										cnt  : 发送的长度
* 输出           : 无
* 返回           : 校验值。
*******************************************************************************/
void UartSendCommand(u8 *buff, u8 cnt)
{
  u8 i;
	
	//发送命令头
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, 0x7F);
	
  for(i = 0; i < cnt; i++)
  {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, *(buff + i));
		//数据中有7F，就多发一个
		if(*(buff + i) == 0x7F)
		{
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    		USART_SendData(USART1, 0x7F);
		}
  }
}

/*******************************************************************************
* 函数名         : UartReceiveCommand
* 描述           : 命令接收函数。
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
u8 UartReceiveCommand(void)
{
	unsigned char i, j;
	
	//有开始接收命令了
	if(Cmd.ReceivePoint > 0)
	{
		//接收完剩下的数据
		// Delay(4);
		//去掉命令头
		Cmd.ReceiveBuffer[0] = Cmd.ReceiveBuffer[1];
		//去掉数据中多出的一个7F
		for(i = 2, j = 1; i < Cmd.ReceivePoint; i++)
		{
			//先判断一下这个为7F时，上一个也是7F的话，就是双个7F，就丢弃。
			if(Cmd.ReceiveBuffer[i] == 0x7F && Cmd.ReceiveBuffer[i - 1] == 0x7F){continue;}
			
			Cmd.ReceiveBuffer[j++] = Cmd.ReceiveBuffer[i];
		}
		Cmd.ReceivePoint = 0;
		return 1;
	}
	return 0;
}



/*******************************************************************************
* 函数名         : NFC_INit
* 描述           : NFC初始化,包括时钟使能,GPIO配置,USART配置,USART中断使能,NVIC配置,使能USART
* 输入           : NFC模块通信波特率
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void NFC_Init(u32 NFC_SerialBaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 开启 USART 和 GPIO 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    // 2. 配置 GPIO
    // TX: PA9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // RX: PA10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. USART 配置
    USART_InitStructure.USART_BaudRate = NFC_SerialBaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);

    // 4. 使能 USART 接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // 5. NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 6. 使能 USART
    USART_Cmd(USART1, ENABLE);



}

