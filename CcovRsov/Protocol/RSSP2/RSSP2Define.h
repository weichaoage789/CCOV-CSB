/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  RSSP2Define.h
  * @brief	        :  RSSP-2 protocol
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _RSSP2_DEFINE_H
#define _RSSP2_DEFINE_H

#include "Utilities/NetWork/NetWork.h"
using namespace NetWork;


#define MAX_ALE_OBJ_NUM				40
#define MAX_CHANNEL_NUM				10


typedef enum
{
	STATE_IDLE,
	STATE_CONNECTED,
	STATE_DISCONNECTED
}TCP_STATE;

typedef enum
{
	CTRL_IDLE,
	CTRL_CONNECT,
	CTRL_DISCONNECT
}TCP_CTRL;

typedef struct
{
	unsigned int ip;
	unsigned short port;
}IP_CFG;

/* TCP参数结构体 */
typedef struct
{
	unsigned int delaytime;							/* 周期休眠时间 */
	unsigned int Class_D_Connections;		/* D类服务通道数 */
	IP_CFG addr_local[MAX_CHANNEL_NUM];
	IP_CFG addr_remote[MAX_CHANNEL_NUM];
}TCP_CFG;

/* TCP 管理结构体 */
typedef struct
{
	CTCP *tcp;									/* tcp资源 */
	TCP_STATE mngt_state;

	bool flag;										/* 对象使用标识 */
	unsigned int maintainTaskDog; /* 管理任务狗 */
	unsigned int recvTaskDog;			/* 接收任务狗 */
}TCP_MNGT;

/* ALE对象结构体 */
typedef struct
{
	bool flag;											/* 对象使用标识 */
	unsigned int etcsId_remote;			/* 远端ETCSID */

	unsigned char arc_id;
	unsigned char arc_data_type;
	unsigned int	arc_send;

	TCP_STATE state;							/* 用于发回协议层的ALE对象的TCP状态 */
	TCP_CTRL ctrl;									/* 来自协议适配层的ALE对象的 */
	TCP_CFG cfg;									/* ALE参数配置结构体 */
	TCP_MNGT mngt[MAX_CHANNEL_NUM];
}TCP_ALE_O;



typedef struct
{
	int Socket;
	unsigned int RecvDataFram;
	unsigned int SendDataFram;
	unsigned short ServerPort;
	unsigned char ServerIp[4];
	unsigned char ConnectState;
	unsigned char AskCtrlTag;			/*数据下载请求*/
	unsigned char MinConnect;		/*主备系连接标志0x55;主系;0xaa备系;0xff未知*/
}TCP_CONNECT_STRUCT;




#endif  /*_RSSP2_DEFINE_H*/