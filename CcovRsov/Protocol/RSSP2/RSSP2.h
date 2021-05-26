/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  RSSP2.h
  * @brief	        :  RSSP-2 protocol
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _RSSP2_H
#define _RSSP2_H

#include "RSSP2Define.h"


class CAleCtrl
{
public:
	CAleCtrl();
	~CAleCtrl();

	/* ��ȡ�����ڵ�״̬ */
	TCP_CTRL GetRedTcpCtrl(unsigned char ctrl_code);

	/* ��ȡ�����ڵ�״̬ */
	TCP_CTRL GetBlueTcpCtrl(unsigned char ctrl_code);

	/* ��ʼ��ALE�ڵ� */
	unsigned char ALE_InitNode(TCP_ALE_O *alenode, int arc_id, unsigned int local_etcsId, unsigned int remote_etcsId, unsigned char local_ip[4], unsigned int ccov_atp_type);
	
	/* ɾ��ALE�ڵ� */
	unsigned char ALE_DelNode(TCP_ALE_O *alenode);

	/* ��ȡ����ALE�ڵ� */
	TCP_ALE_O *ALE_GetFreeNode();

	/* ��ѯALE�ڵ� */
	TCP_ALE_O *ALE_SearchNode(unsigned int remote_etcsId);

	/* ��ȡALE�ڵ�״̬ */
	TCP_STATE Ale_GetTcpState(TCP_ALE_O *alenode);

public:
	TCP_ALE_O _AleNode[MAX_ALE_OBJ_NUM];
};


#endif  /*_RSSP2_H*/