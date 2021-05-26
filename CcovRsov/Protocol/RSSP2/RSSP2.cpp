#include <string.h>
#include "dsuCcovFunc.h"
#include "dsuRssp2DataStru.h"
#include "dsuRssp2Func.h"
#include "vobcInternalTelegramDef.h"

#include "Utilities/Convert/ConvertWork.h"
using namespace Convert;

#include "RSSP2.h"


CAleCtrl::CAleCtrl()
{
	memset(_AleNode, 0, sizeof(TCP_ALE_O) * MAX_ALE_OBJ_NUM);
}

CAleCtrl::~CAleCtrl()
{

}

TCP_CTRL CAleCtrl::GetRedTcpCtrl(unsigned char ctrl_code)
{
	TCP_CTRL ctrl = CTRL_IDLE;
	ctrl_code = (ctrl_code >> 4) & 0x0f;
	if ((0 <= ctrl_code) && (4 >= ctrl_code))
	{
		ctrl = (TCP_CTRL)ctrl_code;
	}
	return ctrl;
}

TCP_CTRL CAleCtrl::GetBlueTcpCtrl(unsigned char ctrl_code)
{
	TCP_CTRL ctrl = CTRL_IDLE;
	ctrl_code = ctrl_code & 0x0f;
	if ((0 <= ctrl_code) && (4 >= ctrl_code))
	{
		ctrl = (TCP_CTRL)ctrl_code;
	}
	return ctrl;
}


unsigned char CAleCtrl::ALE_InitNode(TCP_ALE_O *alenode, int arc_id, unsigned int local_etcsId, unsigned int remote_etcsId, unsigned char local_ip[4], unsigned int net_type)
{
	unsigned int i = 0;
	unsigned char linkNum = 0;

	DSU_IP_PORT LocalIpPort, *pIpPort = nullptr;
	DSU_IP_PORT RedIpPorts[MAX_CHANNEL_NUM];
	DSU_IP_PORT BlueIpPorts[MAX_CHANNEL_NUM];

	if (nullptr != alenode)
	{
		memset(alenode, 0, sizeof(TCP_ALE_O));
		alenode->etcsId_remote = remote_etcsId;
		alenode->arc_data_type = VOBC_FROM_OUT_NET;
		alenode->arc_send = 0;
		alenode->arc_id = arc_id;
		alenode->state = STATE_IDLE;
		alenode->ctrl = CTRL_IDLE;

		alenode->cfg.delaytime = 10000;
		alenode->cfg.Class_D_Connections = 0;
		memset(alenode->cfg.addr_local, 0, sizeof(IP_CFG) * MAX_CHANNEL_NUM);
		memset(alenode->cfg.addr_remote, 0, sizeof(IP_CFG) * MAX_CHANNEL_NUM);

		if (CCOV_RED_IP == net_type)
		{
			pIpPort = RedIpPorts;
		}
		else if (CCOV_BLUE_IP == net_type)
		{
			pIpPort = BlueIpPorts;
		}

		/* 查找本地IP端口 */
		if (1 == dquGetIpPortByEtcsId(local_etcsId, &linkNum, RedIpPorts, BlueIpPorts))
		{
			for (i = 0; i < linkNum; i++)
			{
				if (0 == memcmp(local_ip, pIpPort[i].Ip, 4))
				{
					LocalIpPort = pIpPort[i];
					break;
				}
			}

			if (i >= linkNum)	{	return 0;	}
		}

		/* 查找远端IP端口 */
		if (1 == dquGetIpPortByEtcsId(remote_etcsId, &linkNum, RedIpPorts, BlueIpPorts))
		{
			if (linkNum > MAX_CHANNEL_NUM || linkNum == 0)	{	return 0;	}

			for (i = 0; i < linkNum; i++)
			{
				alenode->cfg.addr_local[i].ip = char_to_long_LE(LocalIpPort.Ip);
				alenode->cfg.addr_local[i].port = LocalIpPort.Port;
				alenode->cfg.addr_remote[i].ip = char_to_long_LE(pIpPort[i].Ip);
				alenode->cfg.addr_remote[i].port = pIpPort[i].Port;
				alenode->mngt[i].flag = false;
				alenode->mngt[i].tcp = nullptr;
				alenode->mngt[i].mngt_state = STATE_DISCONNECTED;
			}
			alenode->cfg.Class_D_Connections = linkNum;
			alenode->flag = true;
		}

		return 1;
	}
	return 0;
}

unsigned char CAleCtrl::ALE_DelNode(TCP_ALE_O *alenode)
{
	for (int i = 0; i < alenode->cfg.Class_D_Connections; i++)
	{
		alenode->mngt[i].mngt_state = STATE_IDLE;
		alenode->mngt[i].maintainTaskDog = 0;
		alenode->mngt[i].recvTaskDog = 0;
		if (nullptr != alenode->mngt[i].tcp)
		{
			delete alenode->mngt[i].tcp;
			alenode->mngt[i].tcp = nullptr;
		}
	}
	alenode->flag = false;
	alenode->arc_id = 0;
	alenode->arc_data_type = 0;
	alenode->arc_send = 0;
	alenode->etcsId_remote = 0;
	alenode->state = STATE_IDLE;
	alenode->ctrl = CTRL_IDLE;
	memset(&alenode->cfg, 0, sizeof(alenode->cfg));
}

TCP_ALE_O *CAleCtrl::ALE_SearchNode(unsigned int remote_etcsId)
{
	TCP_ALE_O *node = nullptr;
	for (int i = 0; i < MAX_ALE_OBJ_NUM; i++)
	{
		if ((true == _AleNode[i].flag) && (remote_etcsId == _AleNode[i].etcsId_remote))
		{
			node = &_AleNode[i];
			break;
		}
	}
	return node;
}

TCP_ALE_O *CAleCtrl::ALE_GetFreeNode()
{
	TCP_ALE_O *node = nullptr;
	for (int i = 0; i < MAX_ALE_OBJ_NUM; i++)
	{
		if (false == _AleNode[i].flag)
		{
			node = &_AleNode[i];
			break;
		}
	}
	return node;
}

TCP_STATE CAleCtrl::Ale_GetTcpState(TCP_ALE_O *node)
{
	TCP_STATE state = STATE_DISCONNECTED;
	for (int i = 0; i < node->cfg.Class_D_Connections; i++)
	{
		if (STATE_CONNECTED == node->mngt[i].mngt_state)
		{
			state = STATE_CONNECTED;
			break;
		}
	}
	return state;
}