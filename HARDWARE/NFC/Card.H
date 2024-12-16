/******************** (C) COPYRIGHT 2015 ********************
* 文件名          : CARD.h
* 作者            : 优灵科技开发团队
* 版本            : V3.1.0.0
* 日期            : 2015-5-10
* 描述            : 集合了模块对卡片的所有发送命令操作
										可以选择有用的调用，无用的删除
										还有命令的解析，将命令中的卡号和数据等存到Card结构体中
* 技术支持        : QQ: 3419865840
*********************************************************************************/

#ifndef _CARD_H
#define _CARD_H

/* 包含文件 -------------------------------------------------------------------*/
#include"main.h"
/* 外部类型 -------------------------------------------------------------------*/
typedef struct _CMD
{
  unsigned char ReceiveBuffer[32];
	unsigned char SendBuffer[32];
  unsigned char ReceivePoint;
	unsigned char Code;
}CMD;

typedef struct _CARD
{
	unsigned int Type;
	unsigned long UID;
	unsigned long Value;
	unsigned char KeyA[6];
	unsigned char KeyB[6];
	unsigned char BlockData[16];
}CARD;

/* 外部变量 -------------------------------------------------------------------*/
extern CARD Card;
extern CMD Cmd;
/* 外部函数 ------------------------------------------------------------------ */
void ReadCard(void);
void ReadBlock(unsigned char block);
void WriteBlock(unsigned char block, unsigned char *blockData);
void MakeCard(unsigned char block, unsigned long value);
void Inc(unsigned char block, unsigned long value);
void Dec(unsigned char block, unsigned long value);
void ClearCard(unsigned char block);
unsigned char CommandProcess(void);

#endif /* __Card_H */

/***************** (C) COPYRIGHT 2017 优灵科技***** END OF FILE ****************/

