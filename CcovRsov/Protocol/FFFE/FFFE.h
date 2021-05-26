/************************************************************************
*@ 文件名	    ： FFFE_CFG.h
*@ 版权说明	：
*@ 版本号	    ： 1.0
*@ 创建时间	： 2020.4
*@ 作者		    ： HHY
*@ 功能描述	：FFFE_CFG类
*@ 使用注意	：
*@ 修改记录	：

************************************************************************/

#ifndef _FFFE_H
#define _FFFE_H


/*FFFE_CFG解转义状态*/
#define WAIT_FF_C1 0x00
#define WAIT_FF_C2 0x01
#define WAIT_NO_FF 0x02
#define WAIT_FF_POS 0x03
#define ABNORMAL_STATE 0x04

/*FFFE_CFG数据接收状态*/
#define COM_WAIT_START_FF 0x00
#define COM_WAIT_START_FE 0x01
#define COM_WAIT_STOP_FF 0x02
#define COM_WAIT_STOP_FD 0x03


typedef struct
{
	unsigned char *pRawDataRcvBuf;/*未解转义数据接收缓冲区*/
	unsigned short RawDataRcvBufferSize;/*未解转义数据接收缓冲区大小*/
	unsigned char RcvState;/*FFFE_CFG数据接收状态*/
	unsigned char GotFrame;/*FFFE_CFG是否收到了完整帧*/
	unsigned short DataCount;/* 收到完整帧之前，接收到的FFFE_CFG数据总长度，每次收到完整帧后清零*/
} FFFE_CFG;


class CFFFE
{
public:
	static CFFFE &GetInstance()
	{
		static CFFFE instance;
		return instance;
	}

	/*
	* 功能描述：  FFFE_CFG结构体初始化函数
	* 输入参数：  unsigned short FFFEBufferSize, 用于接收一帧数据的缓冲区长度
	*             PORT_STATE_STRU *pFFFEStatStru, FFFE_CFG状态结构体
	* 返回值：    0,函数执行失败
	*             1,函数执行成功
	*/
	unsigned char FFFEInit(FFFE_CFG *pFFFE, unsigned short BufSize);

	/*
	* 功能描述： 将数据进行FFFE_CFG转义处理（不加头FFFE_CFG及尾FFFD）。
	* 输入参数： const unsigned char *pSrc    数据(无FFFE_CFG转义)
	*            unsigned short SrcLen        数据(无FFFE_CFG转义)的长度
	* 输出参数： unsigned char *pTgt          数据(经过FFFE_CFG转义)
	* 返回值：   转义后的长度
	*/
	unsigned short Encode(const unsigned char *pSrc, unsigned char *pTgt, unsigned short Len);

	/*
	* 功能描述：  将经过FFFE_CFG转义处理后的数据（已经去掉了头FFFE_CFG及尾FFFD）还原为原始数据
	* 输入参数：  const unsigned char *pSrc,    数据(FFFE_CFG转义)
	*             unsigned short SrcLen,        数据(FFFE_CFG转义)的长度
	* 输出参数：  unsigned char *pTgt,          数据(去掉FFFE_CFG转义)
	* 返回值：    大于0,  转义成功,返回转义后的数据长度
	*             0,   有不能转义的字符，转义失败。或者转义后的数据长度为0
	*/
	unsigned short Decode(const unsigned char *pSrc, unsigned char *pTgt, unsigned short Len);

	/*
	* 功能描述：   按字符接收待解转义的数据，获得整帧FFFE_CFG报文（不含FFFE_CFG和FFFD）。
	* 输入参数：   unsigned char RcvChar,      当前接收的字符
	*              PORT_STATE_STRU *pFFFEStatStru,
	* 输出参数：   unsigned char *pRawDataRcv,  数据接收缓冲区指针
	* 返回值：     大于0,  表示收到完整帧的数据长度，并将收到的完整帧拷贝到接收缓冲区；
	*              0,   未收到整帧数据；
	*/

	unsigned short Recv(FFFE_CFG *pFFFE, unsigned char RcvChar, unsigned char *pData);

	/*
	* 功能描述：  FFFE_CFG状态清理函数
	* 输入参数：  PORT_STATE_STRU *pFFFEStatStru, FFFE_CFG状态结构体
	* 返回值：
	*/
	void Free(FFFE_CFG *pFFFE);

	/*
	* 功能描述： 将数据进行FFFE_CFG转义处理,加头FFFE_CFG及尾FFFD
	* 输入参数： const unsigned char *pSrc    数据(无FFFE_CFG转义)
	*            unsigned short SrcLen        数据(无FFFE_CFG转义)的长度
	* 输出参数： unsigned char *pTgt          数据(经过FFFE_CFG转义),并添加FFFE_CFG头及FFFD尾
	* 返回值：   转义后的长度
	*/
	unsigned short Pack(const unsigned char *pSrc, unsigned char *pTgt, unsigned short Len);

private:
	CFFFE() {}
};


#endif