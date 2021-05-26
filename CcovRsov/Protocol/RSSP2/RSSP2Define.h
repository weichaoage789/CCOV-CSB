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

/* TCP�����ṹ�� */
typedef struct
{
	unsigned int delaytime;							/* ��������ʱ�� */
	unsigned int Class_D_Connections;		/* D�����ͨ���� */
	IP_CFG addr_local[MAX_CHANNEL_NUM];
	IP_CFG addr_remote[MAX_CHANNEL_NUM];
}TCP_CFG;

/* TCP ����ṹ�� */
typedef struct
{
	CTCP *tcp;									/* tcp��Դ */
	TCP_STATE mngt_state;

	bool flag;										/* ����ʹ�ñ�ʶ */
	unsigned int maintainTaskDog; /* �������� */
	unsigned int recvTaskDog;			/* �������� */
}TCP_MNGT;

/* ALE����ṹ�� */
typedef struct
{
	bool flag;											/* ����ʹ�ñ�ʶ */
	unsigned int etcsId_remote;			/* Զ��ETCSID */

	unsigned char arc_id;
	unsigned char arc_data_type;
	unsigned int	arc_send;

	TCP_STATE state;							/* ���ڷ���Э����ALE�����TCP״̬ */
	TCP_CTRL ctrl;									/* ����Э��������ALE����� */
	TCP_CFG cfg;									/* ALE�������ýṹ�� */
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
	unsigned char AskCtrlTag;			/*������������*/
	unsigned char MinConnect;		/*����ϵ���ӱ�־0x55;��ϵ;0xaa��ϵ;0xffδ֪*/
}TCP_CONNECT_STRUCT;




#endif  /*_RSSP2_DEFINE_H*/