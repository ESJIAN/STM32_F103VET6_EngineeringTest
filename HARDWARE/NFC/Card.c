/******************** (C) COPYRIGHT 2015 ********************
* �ļ���          : Card.c
* ����            : ����Ƽ������Ŷ�
* �汾            : V3.1.0.0
* ����            : 2015-5-10
* ����            : ������ģ��Կ�Ƭ�����з����������
										����ѡ�����õĵ��ã����õ�ɾ��
										��������Ľ������������еĿ��ź����ݵȴ浽Card�ṹ����
* ����֧��        : QQ: 3419865840
********************************************************************************/
/* �����ļ� -------------------------------------------------------------------*/
#include"Card.H"

/* �궨�� ---------------------------------------------------------------------*/
/* ���ļ�ʹ�õı��� -----------------------------------------------------------*/
CARD Card;
CMD Cmd;
/* ���ļ�ʹ�õĺ������� -------------------------------------------------------*/
unsigned char CheckSum(unsigned char *dat, unsigned char num);
/* ���ļ������� ---------------------------------------------------------------*/
void UartSendCommand(u8 *buff, u8 cnt);
u8 UartReceiveCommand(void);
/*******************************************************************************
* ������         : ReadCard
* ����           : ������
* ����           : ��
* ���           : ��
* ����           : ��
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
* ������         : ReadBlock
* ����           : ��������
* ����           : block: ��Ҫ�����Ŀ��ַ��
* ���           : ��
* ����           : ��
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
* ������         : WriteBlock
* ����           : д������
* ����           : block: ��Ҫ�����Ŀ��ַ��
									 blockData: ��Ҫд������ݡ�
* ���           : ��
* ����           : ��
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
* ������         : MakeCard
* ����           : �쿨��
* ����           : block: ��Ҫ�����Ŀ��ַ��
                   value: ��Ҫ������ֵ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void MakeCard(unsigned char block, unsigned long value)
{
	unsigned char len = 9;
	
	Cmd.SendBuffer[0] = len;
	Cmd.SendBuffer[1] = 0;
	Cmd.SendBuffer[2] = 0x13;
	Cmd.SendBuffer[3] = block;
	Cmd.SendBuffer[4] = 0;	//�̶�Ϊ0
	Cmd.SendBuffer[5] = (unsigned char)(value >> 24);
	Cmd.SendBuffer[6] = (unsigned char)(value >> 16);
	Cmd.SendBuffer[7] = (unsigned char)(value >> 8);
	Cmd.SendBuffer[8] = (unsigned char)(value);
	
	Cmd.SendBuffer[9] = CheckSum(Cmd.SendBuffer, len);
	UartSendCommand(Cmd.SendBuffer, len);
}

/*******************************************************************************
* ������         : Inc
* ����           : ��ֵ��
* ����           : block: ��Ҫ�����Ŀ��ַ��
                   value: ��Ҫ������ֵ��
* ���           : ��
* ����           : ��
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
* ������         : Dec
* ����           : �ۿ
* ����           : block: ��Ҫ�����Ŀ��ַ��
                   value: ��Ҫ������ֵ��
* ���           : ��
* ����           : ��
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
* ������         : ClearCard
* ����           : �����
* ����           : block: ��Ҫ�����Ŀ��ַ��
* ���           : ��
* ����           : ��
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
* ������         : CommandProcess
* ����           : �����
* ����           : ��
* ���           : ��
* ����           : ִ�н����
                   -0   : ��ȷ��
                   -0xFF: �޿���
                   -0xFE: ����������
                   -0xFD: û���յ����
                   -0xFC: ���㡣
                   -0xFB: У�����
*******************************************************************************/
unsigned char CommandProcess(void)
{
	unsigned char i, status, bTemp;
	
	//���յ�����
	bTemp = UartReceiveCommand();
	if(bTemp == 1)
	{
		//У������
		if(Cmd.ReceiveBuffer[Cmd.ReceiveBuffer[0]] == CheckSum(Cmd.ReceiveBuffer, Cmd.ReceiveBuffer[0]))
		{
			//��ȡ���ţ�����UartReceiveCommnad������������¹���
			//Cmd.ReceiveBuffer[0] 		-> �����
			//Cmd.ReceiveBuffer[1] 		-> ģ���ַ
			//Cmd.ReceiveBuffer[2] 		-> ������
			//Cmd.ReceiveBuffer[3] 		-> ״̬��
			//Cmd.ReceiveBuffer[4~5] 	-> ������
			//Cmd.ReceiveBuffer[6~9] 	-> ����
			
			Card.UID = Cmd.ReceiveBuffer[9]; Card.UID <<= 8;
			Card.UID |= Cmd.ReceiveBuffer[8]; Card.UID <<= 8;
			Card.UID |= Cmd.ReceiveBuffer[7]; Card.UID <<= 8;
			Card.UID |= Cmd.ReceiveBuffer[6];
			Cmd.Code = Cmd.ReceiveBuffer[2];
			switch(Cmd.Code)
			{
				//�����ţ����ﲻ��ɾ��
				case 0x90:break;
				//��������
				case 0x91:
					for(i = 0; i < 16; i++){Card.BlockData[i] = Cmd.ReceiveBuffer[10 + i];}
					break;
				//д�����ݣ����ﲻ��ɾ��
				case 0x92: break;
				//�쿨�����ﲻ��ɾ��
				case 0x93: break;
				//�ָ��������ﲻ��ɾ��
				case 0x94: break;
				//��ֵ
				case 0x95:
					Card.Value = Cmd.ReceiveBuffer[10];
					Card.Value <<= 8;
					Card.Value |= Cmd.ReceiveBuffer[11];
					Card.Value <<= 8;
					Card.Value |= Cmd.ReceiveBuffer[12];
					Card.Value <<= 8;
					Card.Value |= Cmd.ReceiveBuffer[13];
					break;
				//�ۿ�
				case 0x96:
					Card.Value = Cmd.ReceiveBuffer[10];
					Card.Value <<= 8;
					Card.Value |= Cmd.ReceiveBuffer[11];
					Card.Value <<= 8;
					Card.Value |= Cmd.ReceiveBuffer[12];
					Card.Value <<= 8;
					Card.Value |= Cmd.ReceiveBuffer[13];
					break;
				//�����֤�����ﲻ��ɾ��
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
* ������         : CheckSum
* ����           : ����У�顣
* ����           : 	dat : ҪУ������顣
										num : У����ֽ�����
* ���           : ��
* ����           : У��ֵ��
*******************************************************************************/
unsigned char CheckSum(unsigned char *dat, unsigned char num)
{
  unsigned char bTemp = 0, i;
  
  for(i = 0; i < num; i ++){bTemp ^= dat[i];}
  return bTemp;
}

/*******************************************************************************
* ������         : UartSendCommand
* ����           : ���ڷ�������
* ����           : 	buff : ���͵����顣
										cnt  : ���͵ĳ���
* ���           : ��
* ����           : У��ֵ��
*******************************************************************************/
void UartSendCommand(u8 *buff, u8 cnt)
{
  u8 i;
	
	//��������ͷ
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, 0x7F);
	
  for(i = 0; i < cnt; i++)
  {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, *(buff + i));
		//��������7F���Ͷ෢һ��
		if(*(buff + i) == 0x7F)
		{
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, 0x7F);
		}
  }
}

/*******************************************************************************
* ������         : UartReceiveCommand
* ����           : ������պ�����
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
u8 UartReceiveCommand(void)
{
	unsigned char i, j;
	
	//�п�ʼ����������
	if(Cmd.ReceivePoint > 0)
	{
		//������ʣ�µ�����
		Delay(4);
		//ȥ������ͷ
		Cmd.ReceiveBuffer[0] = Cmd.ReceiveBuffer[1];
		//ȥ�������ж����һ��7F
		for(i = 2, j = 1; i < Cmd.ReceivePoint; i++)
		{
			//���ж�һ�����Ϊ7Fʱ����һ��Ҳ��7F�Ļ�������˫��7F���Ͷ�����
			if(Cmd.ReceiveBuffer[i] == 0x7F && Cmd.ReceiveBuffer[i - 1] == 0x7F){continue;}
			
			Cmd.ReceiveBuffer[j++] = Cmd.ReceiveBuffer[i];
		}
		Cmd.ReceivePoint = 0;
		return 1;
	}
	return 0;
}
