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

	/* 获取红网节点状态 */
	TCP_CTRL GetRedTcpCtrl(unsigned char ctrl_code);

	/* 获取蓝网节点状态 */
	TCP_CTRL GetBlueTcpCtrl(unsigned char ctrl_code);

	/* 初始化ALE节点 */
	unsigned char ALE_InitNode(TCP_ALE_O *alenode, int arc_id, unsigned int local_etcsId, unsigned int remote_etcsId, unsigned char local_ip[4], unsigned int ccov_atp_type);
	
	/* 删除ALE节点 */
	unsigned char ALE_DelNode(TCP_ALE_O *alenode);

	/* 获取空闲ALE节点 */
	TCP_ALE_O *ALE_GetFreeNode();

	/* 查询ALE节点 */
	TCP_ALE_O *ALE_SearchNode(unsigned int remote_etcsId);

	/* 获取ALE节点状态 */
	TCP_STATE Ale_GetTcpState(TCP_ALE_O *alenode);

public:
	TCP_ALE_O _AleNode[MAX_ALE_OBJ_NUM];
};


#endif  /*_RSSP2_H*/