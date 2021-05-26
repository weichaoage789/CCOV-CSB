#include "AppDefine.h"
#include "AppConfig.h"

#include "CbtcVersion.h"
#include "dfsDataRead.h"
#include "dsuRssp2Func.h"
#include "dsuCcovFunc.h"
#include "dquDataSize.h"
#include "dquIdmapInit.h"
#include "dquIdmapQuery.h"
#include "vobcInternalTelegramDef.h"

#include "Protocol/RSSP2/RSSP2Define.h"
#include "Driver/LED/Led.h"

#include "Utilities/Convert/ConvertWork.h"
#include "Utilities/FileWork/LogFileWork.h"
using namespace Convert;
using namespace FileWork;

#include "CcovDefine.h"
#include "CcovConfig.h"
#include "CcovHandler.h"


unsigned int CCcovHandler::_TIMER_ = 0;

CCcovHandler::CCcovHandler() : _ReportToAtpIndex1(0), _ReportToAtpIndex2(0), _VerSn(0)
{
	memset(_WgbIp, 0, sizeof(_WgbIp));
	
	_Attri = &CAppCfg::GetInstance()._Attri;
	_ArcnetCtrl = new CArcnet(&_Que_Rx_Arcnet1, &_Que_Rx_Arcnet2);

	/** @timer thread */
	std::thread([this]() {
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			OnTimer(nullptr);
		}
	}).detach();

	Init();
	Init_Dsu();
}

CCcovHandler::~CCcovHandler()
{
	if (nullptr != _ArcnetCtrl)
	{
		delete _ArcnetCtrl;
		_ArcnetCtrl = nullptr;
	}

	for (auto iter = _Outer.begin(); iter != _Outer.end(); )
	{
		delete iter->second.first;
		iter = _Outer.erase(iter);
	}
}

void CCcovHandler::Init()
{
	/** @CCOV software version */
	_Attri->CcovSwVer = VerToU32SoftwareInt(CCOV_VER_PROJECT_NO, CCOV_VER_PRODUCT_NO, CCOV_VER_OC_NO, CCOV_VER_INTEG_VER);
	LOG(INFO) << "CCOV software version = " << _Attri->CcovSwVer;

	CUDP *udp123 = new CUDP;
	if (nullptr != udp123)
	{
		udp123->init(ADDR_ANY, PORT_123);
		_Outer[PORT_123].first = udp123;
		LOG(INFO) << "CCOV UDP123 initialize success, port = " << PORT_123;
	}

	CUDP *udp5001 = new CUDP();
	if (nullptr != udp5001)
	{
		udp5001->init(ADDR_ANY, PORT_50001);
		_Outer[PORT_50001].first = udp5001;
		LOG(INFO) << "CCOV UDP5001 initialize success, port = " << PORT_50001;
	}

	CUDP *udp5002 = new CUDP();
	if (nullptr != udp5002)
	{
		udp5002->init(ADDR_ANY, PORT_50002);
		_Outer[PORT_50002].first = udp5002;
		LOG(INFO) << "CCOV PORT_5002 initialize success, port = " << PORT_50002;
	}

	CUDP *udp5003 = new CUDP();
	if (nullptr != udp5003)
	{
		udp5003->init(ADDR_ANY, PORT_50003);
		_Outer[PORT_50003].first = udp5003;
		LOG(INFO) << "CCOV PORT_5003 initialize success, port = " << PORT_50003;
	}
	
	_Wgb.init(ADDR_ANY, PORT_WGB_RX);
	LOG(INFO) << "CCOV UDP--WGB initialize success, port = " << PORT_WGB_RX;

	_Rsov.init(ADDR_ANY, PORT_50004);
	LOG(INFO) << "CCOV UDP--RSOV initialize success, port = " << PORT_50004;

	memset(&_MoAtp, 0xFF, sizeof(CCOV_TO_ATP_STRU));
	_MoAtp.StateRecv = 0xAA;
	_MoAtp.UpdateRecv = 0xAA;
	_MoAtp.MMI_Show_Restart = 0xAA;
}

void CCcovHandler::Init_Dsu()
{
	if (RSSP2_PROTOCOL == _Attri->ProtclType)
	{
		if (1 != vDquIdmapStructInit(&dsuIdmap1) || 1 != vDquIdmapStructInit(&dsuIdmap2))
		{
			LOG(ERROR) << "vDquIdmapStructInit() error";
			exit(EXIT_FAILURE);
		}

		/* RSSP-2Э���¶�ȡ��Ŷ��ձ��Э���������ݱ� */
		if (1 != dquSetCurIdMapQueryDataSource(&dsuIdmap1))
		{
			LOG(ERROR) << "dquSetCurIdMapQueryDataSource() error";
			exit(EXIT_FAILURE);
		}

		if (1 != dfsIdmapInitExp(CAppCfg::GetInstance()._FsStorCfg.DataBuf, &dsuIdmap1, CAppCfg::GetInstance()._FsStorCfg.ReadMode, 0))
		{
			LOG(ERROR) << "dfsIdmapInitExp() error";
			exit(EXIT_FAILURE);
		}
		
		dfsIdmapInitFinally();
		if (1 != dsuRssp2Init((unsigned char *)APP_CONFIG_FILE))
		{
			LOG(ERROR) << "dsuRssp2Init() error";
			exit(EXIT_FAILURE);
		}
	}

	if ( (NET_TYPE_RED == _Attri->NetType || NET_TYPE_BLUE == _Attri->NetType)
		&& (CCOV_RED_IP == CAppCfg::GetInstance()._IpType || CCOV_BLUE_IP == CAppCfg::GetInstance()._IpType) 
		&& DEVICE_CCOV == ((CAppCfg::GetInstance()._DevName >> 8) & 0xFF) )
	{
		/*����CCOV��IP��ַ��ѯ������WGB��IP��ַ,��ʹ��ѯʧ��Ҳ��Ӱ��WGB״̬��ѯ�ͽ�������,ATP���������п��ܲ���Ӱ��*/
		if (1 == dsuWgbIpGetByCcovIp(_Attri->Ip, _Attri->WgbIpBuff, 4))
		{
			sprintf((char *)_WgbIp, "%u.%u.%u.%u", _Attri->WgbIpBuff[0][0], _Attri->WgbIpBuff[0][1], _Attri->WgbIpBuff[0][2], _Attri->WgbIpBuff[0][3]);
		}

		/*��ѯNTP��������NTp��IP��ַ,��ѯʧ�����޷��շ����������͵�����,��Ӱ����������*/
		_Attri->NtpServerCount = dsuNtpServerIpGetByCcovIp(_Attri->Ip, _Attri->NtpIpBuff, 4);

		/** @��ѯWGB�Զ�IP */
		unsigned char wgbremoteip[16] = { 0 };
		if (0 != dsuGetDevIPInfoByName(&_LteIp, 1, 0x6901))
		{
			if (NET_TYPE_RED == _Attri->NetType)
			{
				sprintf((char *)wgbremoteip, "%u.%u.%u.%u", _LteIp.IpAddrRed[0], _LteIp.IpAddrRed[1], _LteIp.IpAddrRed[2], _LteIp.IpAddrRed[3]);
			}
			else if (NET_TYPE_BLUE == _Attri->NetType)
			{
				sprintf((char *)wgbremoteip, "%u.%u.%u.%u", _LteIp.IpAddrBlue[0], _LteIp.IpAddrBlue[1], _LteIp.IpAddrBlue[2], _LteIp.IpAddrBlue[3]);
			}
		}
		else
		{
			if (0 != dsuGetGateWayByName(wgbremoteip, 0x1e01, _Attri->NetType))
			{
				sprintf((char *)wgbremoteip, "%u.%u.%u.%u", wgbremoteip[0], wgbremoteip[1], wgbremoteip[2], wgbremoteip[3]);
			}
		}
		_WgbCtrl.SetApIp(wgbremoteip);
		LOG(INFO) << "WGB remote ip = " << wgbremoteip;

		/*��ѯNms��������NMS��IP��ַ,��ѯʧ�����޷��շ����������͵�����,��Ӱ����������*/
		if (1 == dsuNmsIpGetByCcovIp(_Attri->Ip, _Attri->NmsIpBuff, &_Attri->NmsPort))
		{
			CUDP *udpNms = new CUDP();
			if (nullptr != udpNms)
			{
				udpNms->init(ADDR_ANY, _Attri->NmsPort);
				_Outer[_Attri->NmsPort].first = udpNms;
				LOG(INFO) << "CCOV UDP--NMS initialize success, port = " << _Attri->NmsPort;
			}
		}

		/*��ѯFS�����ļ��汾�ź��豸IP�汾����Ϣ*/
		if (1 == dquGetFSVersion(APP_CONFIG_FILE, _Attri->CcovFsVer))
		{
			_MoAtp.CurlineNum = _Attri->CcovFsVer[0];/* TO BE DONE DH�е���·�Ų��� */
		}

		dquGetDataVersion(APP_CONFIG_FILE, IP_VERSION_ID, _Attri->CcovIpVer);
		return;
	}

	LOG(ERROR) << "Init_Dsu() error, exit EXIT_FAILURE";
	exit(EXIT_FAILURE);
}


unsigned char CCcovHandler::GetNetType()
{
	if (ATP_RED_CCOV == _Attri->CcovToAtpType)
	{
		return CCOV_RED_IP;
	}
	else if (ATP_BLUE_CCOV == _Attri->CcovToAtpType)
	{
		return CCOV_BLUE_IP;
	}
	return CCOV_UNKNOWN_IP;
}

int CCcovHandler::CalcPacketCnt(const unsigned char *data, unsigned short len)
{
	int cnt = 0;
	unsigned short single_len = 0, total_len = 0;

	while (total_len != len)
	{
		single_len = char_to_short(data + total_len) + 2;
		total_len += single_len;
		cnt++;

		/* ����ĳ��ȳ�����ָ������Ŀ�����ش�����0xFF */
		if (total_len > len) { return 0xFF; }
	}

	return cnt;
}

int CCcovHandler::FindIpAndPort(unsigned short devName, unsigned char type, unsigned char ip[][4], unsigned short port[])
{
	int ipcnt = 0;

	if (nullptr != ip && nullptr != port)
	{
		/* IP��ַ��Ϣ */
		DSU_IP_INFO_STRU ipInfo[MAX_IP_INFO_NUM];

		/* �����豸���Ʋ����豸IP��ַ��Ϣ */
		ipcnt = (int)dsuGetDevIPInfoByName(ipInfo, MAX_IP_INFO_NUM, devName);

		/* �������е�IP��ַ��Ϣ */
		for (int i = 0; i < ipcnt; i++)
		{
			/* ��ȡIP��ַ */
			if (CCOV_RED_IP == GetNetType())
			{
				memcpy(ip[i], ipInfo[i].IpAddrRed, DSURP_LEN_IP_ADDR);
			}
			else if (CCOV_BLUE_IP == GetNetType())
			{
				memcpy(ip[i], ipInfo[i].IpAddrBlue, DSURP_LEN_IP_ADDR);
			}

			/* ����Э������ѡ��˿ں� */
			switch (type)
			{
			case 0x01: /* SFPЭ�� */
				port[i] = ipInfo[i].SfpRedPort;
				break;

			case 0x02: /* RPЭ�� */
				port[i] = ipInfo[i].RpRedPort;
				break;

			case 0x03: /* RSRЭ�� */
				port[i] = ipInfo[i].RsspRedPort;
				break;

			case 0x04: /* RSSPЭ�� */
				port[i] = ipInfo[i].RsspRedPort;
				break;

			default:
				break;
			}
		}
	}

	return ipcnt;
}

int CCcovHandler::FindDestNetAddr(const unsigned char *data, unsigned char ip[][4], unsigned short port[])
{
	unsigned char  protocolType = 0xFF;		/* Э������ */
	unsigned char  destDevId = 0xFF;				/* Ŀ���豸��� */
	unsigned char  destDevType = 0xFF;			/* Ŀ���豸���� */
	unsigned short devName = 0;					/* �豸���� */

	/* ��ȡЭ������ */
	protocolType = data[2];

	/* ��ȡĿ���豸���� */
	destDevType = data[3];

	/* ��ȡĿ���豸��� */
	destDevId = data[4];

	/* �����豸��ź���������豸���� */
	devName = (destDevId) | (destDevType << 8);

	/* ����IP��ַ�Ͷ˿ں� */
	return FindIpAndPort(devName, protocolType, ip, port);
}

unsigned int CCcovHandler::FindDestEtcsId(const unsigned char *data)
{
	unsigned int etcs_id = 0;
	unsigned short dev_name = char_to_short(data + 3);
	unsigned short logi_name = char_to_short(data + 5);
	dquDevName2EtcsId(dev_name, logi_name, &etcs_id);
	return etcs_id;
}


void CCcovHandler::Decode_AtpMesg(int arcid, unsigned char *data, unsigned short len)
{
	if (nullptr != data)
	{
		_MiAtp.CurLineNum = data[0];									/* ��ǰ��·�� */
		_MiAtp.NextLineNum = data[1];								/* ��һ��·�� */
		memcpy(_MiAtp.NextLineVer, data + 2, 4);				/* ��һ��·���ݰ汾��*/
		memcpy(_MiAtp.NextLineMd5, data + 6, 16);			/* ��һ��·���ݰ汾��*/
		_MiAtp.VobcProperties = data[22];							/* ����VOBC�����ԡ�55�����ƶˣ� AA���ȴ��� */
		_MiAtp.LocalDevName = char_to_short(data +23);				/* �����豸���� */

		if (0 == arcid)
		{
			_MiAtp.IsZeroSpeed = data[25];			/* �Ƿ����� */
			_MiAtp.IsLocation = data[26];				/* �Ƿ���λ�� */
			_MiAtp.PreviewResults = data[27];		/* Ԥ���� */
		}

		/* TCP���Ʊ��ĳ��� */
		_MiAtp.TcpCtrlMsgLen = 2 + char_to_short(data + 30);
		if ((_MiAtp.TcpCtrlMsgLen >= 2) && (_MiAtp.TcpCtrlMsgLen < 200))
		{
			/* TCP���Ʊ��� */
			memcpy(_MiAtp.TcpCtrlMsg, data + 30, _MiAtp.TcpCtrlMsgLen);
		}

		if (RSSP2_PROTOCOL == _Attri->ProtclType)
		{
			/* ֻ���ڱ����ǿ��ƶ˵�ʱ���������ת������������ݣ��ȴ��˲�ת�� */
			if (0x55 == _MiAtp.VobcProperties)
			{
				/* TCP���Ʊ��� */
				Decode_TcpCtrlMsg(arcid, _MiAtp.LocalDevName, _MiAtp.TcpCtrlMsg, _MiAtp.TcpCtrlMsgLen);
				/* ��¼���ƶ˵�ArcnetID,������ת������ʱֻ����IDת�� */
				_MiAtp.arc_id = arcid;
			}
		}
	}
}

void CCcovHandler::Decode_TcpCtrlMsg(int arc_id, unsigned short dev_name, unsigned char *data, unsigned short len)
{
	unsigned int local_etcsid = 0, remote_etcsid = 0;
	TCP_CTRL tcp_ctrl = CTRL_IDLE;
	TCP_ALE_O *ale_node = nullptr;

	if (nullptr != data && CCOV_UNKNOWN_IP != GetNetType())
	{
		unsigned short data_len = char_to_short(data);
		if (data_len + 2 == len)
		{
			/* ������ALE����Ŀ����ָ�λ */
			for (int i = 0; i < MAX_ALE_OBJ_NUM; i++)
			{
				_AleCtrl._AleNode[i].ctrl = CTRL_IDLE;
			}

			dquDevName2EtcsId(dev_name, 0, &local_etcsid);

			for (int i = 2; i < len; i += 5)
			{
				remote_etcsid = char_to_long(data + i);

				unsigned char ctrl_code = data[i + 4];
				if (CCOV_RED_IP == data[i + 4])
				{
					tcp_ctrl = _AleCtrl.GetRedTcpCtrl(ctrl_code);
				}
				else
				{
					tcp_ctrl = _AleCtrl.GetBlueTcpCtrl(ctrl_code);
				}

				ale_node = _AleCtrl.ALE_SearchNode(remote_etcsid);
				if (nullptr != ale_node)
				{
					/* ALE�����Ѿ����� */
					ale_node->ctrl = tcp_ctrl;
				}
				else 
				{
					/* ������Ϊ����������ALE������δ���� */
					ale_node = _AleCtrl.ALE_GetFreeNode();
					if (1 == _AleCtrl.ALE_InitNode(ale_node, arc_id, local_etcsid, remote_etcsid, _Attri->Ip, GetNetType()))
					{
						/* ������ALE�ڵ�ɹ� */
						ale_node->ctrl = CTRL_CONNECT;
						ale_node->state = STATE_DISCONNECTED;

						for (int index = 0; index < ale_node->cfg.Class_D_Connections; index++)
						{
							/* �����ල�߳� */
							//std::thread t_maintain;

							/* ���������߳� */
							//std::thread t_rx;
						}
					}
					//else /* ������ALE�ڵ�ʧ�� */
				}
			}

			for (int i = 0; i < MAX_ALE_OBJ_NUM; i++)
			{
				if ((true == _AleCtrl._AleNode[i].flag) && ((CTRL_IDLE == _AleCtrl._AleNode[i].ctrl) || (CTRL_DISCONNECT == _AleCtrl._AleNode[i].ctrl)))
				{
					for (int j = 0; j < _AleCtrl._AleNode[i].cfg.Class_D_Connections; j++)
					{
						_AleCtrl._AleNode[i].mngt[j].flag = false;
						delete _AleCtrl._AleNode[i].mngt[j].tcp;
						_AleCtrl._AleNode[i].mngt[j].tcp = nullptr;
					}

					_AleCtrl.ALE_DelNode(ale_node + i);
				}
			}
		}
	}

	return;
}

void CCcovHandler::Encode_TcpStateMsg(unsigned char *data, unsigned short &len)
{
	unsigned short cnt = 0;
	TCP_STATE aleTcpState = STATE_IDLE;

	if (nullptr != data)
	{
		for (int aleIndex = 0; aleIndex < MAX_ALE_OBJ_NUM; aleIndex++)
		{
			if (true == _AleCtrl._AleNode[aleIndex].flag)
			{
				aleTcpState = _AleCtrl.Ale_GetTcpState(&_AleCtrl._AleNode[aleIndex]);
				if ((aleTcpState == STATE_DISCONNECTED) || (CTRL_IDLE == _AleCtrl._AleNode[aleIndex].ctrl))
				{
					/* ������Ϊ�ա�״̬��Ϊ�ѶϿ�ʱ,ɾ��ALE�ڵ� */
					/*	ALE_DelNode(&_AleNode[aleIndex]); TO BE DONE */
				}
				else
				{
					long_to_char(_AleCtrl._AleNode[aleIndex].etcsId_remote, &data[2 + 5 * cnt]);

					if (ATP_RED_CCOV == _Attri->CcovToAtpType)
					{
						data[2 + 5 * cnt + 4] = aleTcpState * 16;
					}
					else
					{
						data[2 + 5 * cnt + 4] = aleTcpState;
					}
					cnt++;
				}
			}
		}

		short_to_char(cnt * 5, data);
		len = 2 + cnt * 5;
	}

	return;
}


void CCcovHandler::HandleFunc(int arc_id, unsigned char *data)
{
	unsigned short buflen = char_to_short(data + 2);
	unsigned short datalen = char_to_short(data + 4);
	
	switch (data[6])
	{
		/*���������������ݣ��������İ�����ͨ��socket���з���*/
	case VOBC_1_TO_OUT_NET:
	case VOBC_2_TO_OUT_NET:
	case VOBC_3_TO_OUT_NET:
		if (RSSP2_PROTOCOL == _Attri->ProtclType)
		{
			if (_MiAtp.arc_id == arc_id)
			{
				RSSP2_DataSend(arc_id, data + 11, datalen - 9);
			}
		}
		else if (RSSP1_PROTOCOL == _Attri->ProtclType)
		{
			UdpDataSend(PORT_50002, data + 11, datalen - 9, RSSP_TYPE, arc_id);
		}
		break;

		/*����ATS������,���ܰ���Ӧ�ñ��ĺͱ�������,ͨ��socket���з���*/
	case VOBC_TO_ATS1:
	case VOBC_TO_ATS2:
	case VOBC_TO_ATS3:
	case VOBC_TO_ATS4:
	case VOBC_TO_ATS5:
		if (RSSP2_PROTOCOL == _Attri->ProtclType)
		{
			if (_MiAtp.arc_id == arc_id)
			{
				RSSP2_DataSend(arc_id, data + 11, datalen - 9);
			}
		}
		else if (RSSP1_PROTOCOL == _Attri->ProtclType)
		{
			UdpDataSend(PORT_50002, data + 11, datalen - 9, RSSP_TYPE, arc_id);
		}
		break;

		/*����CI������,ͨ��socket���ͳ�ȥ*/
	case VOBC_TO_CI1:
	case VOBC_TO_CI2:
	case VOBC_TO_CI3:
	case VOBC_TO_CI4:
	case VOBC_TO_PSC:
	case VOBC_TO_PSC2:
		if (RSSP2_PROTOCOL == _Attri->ProtclType)
		{
			if (_MiAtp.arc_id == arc_id)
			{
				RSSP2_DataSend(arc_id, data + 11, datalen - 9);
			}
		}
		else if (RSSP1_PROTOCOL == _Attri->ProtclType)
		{
			UdpDataSend(PORT_50002, data + 11, datalen - 9, RSSP_TYPE, arc_id);
		}
		break;

		/*VOBC����ATSʱ�ӷ�������Уʱ��Ϣ*/
	case VOBC_TO_ATS_NTP:
		UdpDataSend(PORT_123, data + 11, datalen - 9, RSSP_TYPE, arc_id);
		break;

		/*VOBC����NMS�����߲�����Ϣ,��CCOV����������������ͬ�Ĵ���,������Щ��Ϣ��ֻ�����ڿ��ƶ˷���*/
	case ATP_RED_RADIO_TEST:
	case ATP_BLUE_RADIO_TEST:
		if ((NET_TYPE_RED == _Attri->NetType && 0x55 == data[15]) || (NET_TYPE_BLUE == _Attri->NetType && 0x55 == data[16]))
		{
			std::thread([this]()
			{
				/*��¼ping�Ľ����Ϣ*/
				if (_WgbCtrl.ping((char *)_WgbCtrl.GetApIp()))
				{
					_WgbCtrl.SetPingRslt(0x55);
					_WgbCtrl.SetWgbStatus(0x55);
				}
				else
				{
					_WgbCtrl.SetPingRslt(0xAA);
					_WgbCtrl.SetWgbStatus(0xAA);
				}
			}).detach();
		}

		if (0 != data[17])
		{
			UdpDataSend(_Attri->NmsPort, data + 18, data[17], NMS_TYPE, arc_id);
		}
		break;

	case VOBC_TO_AOM1:
	case VOBC_TO_AOM2:
		UdpDataSend(PORT_50003, data + 11, datalen - 9, SFP_TYPE, arc_id);
		break;

	case VOBC_TO_MSS1:
	case VOBC_TO_MSS2:
		UdpDataSend(PORT_50001, data + 11, datalen - 9, RP_TYPE, arc_id);
		break;

		/* ��������һ�˵�����,ͨ������һ·Arcnetת�� */
	case VOBC_ANOTHER_VOBC_SDU:
	case VOBC_ANOTHER_VOBC:
		{
			int id = (arc_id == ARCNET1 ? ARCNET2 : ARCNET1);
			if (_ArcnetCtrl->ArcSend(id, COMAB26_ARCNET_NODE_ADDR, (char *)(data + 4), buflen))
			{
				if (id == ARCNET1)
					CLEDCtrl::GetInstance().LED_A2(1);
				else
					CLEDCtrl::GetInstance().LED_A3(1);
			}
			else
				LOG(ERROR) << "ArcSend fail, ARCNET id = " << id;
		}
		break;

	case CCOV_AND_ATP1_TYPE:
	case CCOV_AND_ATP2_TYPE:
	case CCOV_AND_ATP3_TYPE:
	case CCOV_AND_ATO1_TYPE:
	case CCOV_AND_ATO2_TYPE:
		unsigned char subsysbuf[512];
		memset(subsysbuf, 0, sizeof(subsysbuf));
		short_to_char(datalen - 9, subsysbuf);
		memcpy(subsysbuf + 2, data + 11, datalen - 9);
		//
		break;

	case VOBC_TO_HOST_SW_UPDATE:
		break;

	case NDSU_AND_ATP_TYPE:
		if (RSSP2_PROTOCOL == _Attri->ProtclType)
		{
			RSSP2_DataSend(arc_id, data + 11, datalen - 9);
		}
		/*else  �ݲ�֧��RSSP-1Э���µ�NDSU */
		break;

		/*ATP����CCOV������,�ݶ��졢������������ͳһ�Ĵ���*/
	case ATP_RED_CCOV:
	case ATP_BLUE_CCOV:
		Decode_AtpMesg(arc_id, data + 11, datalen - 9);
		break;

	default:
		break;
	}
}

void CCcovHandler::UdpDataSend(int port, unsigned char *data, const unsigned int len, const unsigned char type, int arcId)
{
	int rt = 0;
	char ip[16] = { 0 };
	unsigned char packetcnt = 0, *outdata = nullptr;
	unsigned short offset = 0, singlepacketlen = 0;

	unsigned char	AimIP[SERIES_NUM_MAX][4];		/*Ŀ��ϵIP*/
	unsigned short	AimPort[SERIES_NUM_MAX];		/*Ŀ��˿�*/

	CUDP *udp = _Outer[port].first;
	_Outer[port].second.push(arcId);

	if (NTP_TYPE == type)
	{
		/* �����е�NTP����������Уʱ���� */
		for (int i = 0; i < _Attri->NtpServerCount; i++)
		{
			sprintf(ip, "%u.%u.%u.%u", _Attri->NtpIpBuff[i][0], _Attri->NtpIpBuff[i][1], _Attri->NtpIpBuff[i][2], _Attri->NtpIpBuff[i][3]);
			rt = udp->sendmesg((char *)data, len, ip, PORT_123);
		}
	}
	else if (NMS_TYPE == type)
	{
		sprintf(ip, "%u.%u.%u.%u", _Attri->NmsIpBuff[0], _Attri->NmsIpBuff[1], _Attri->NmsIpBuff[2], _Attri->NmsIpBuff[3]);
		rt = udp->sendmesg((char *)data, len, ip, _Attri->NmsPort);
	}
	else
	{
		packetcnt = CalcPacketCnt(data, (unsigned short)len);
		if (0xFF != packetcnt)
		{
			/*ָ���һ֡���ݵĵ�ַ*/
			outdata = data;

			for (int i = 0; i < packetcnt; i++)
			{
				/*����һ֡�������ݵĳ��ȣ��Ա�Ѱ����һ֡����(�ѿ����˳��ȵ����ֽ�)*/
				singlepacketlen = char_to_short(outdata) + SFP_FRA_LENGTH_VAR_SIZE;

				/*������Ҫ���͵���������,�ҵ�Ŀ��IP�Ͷ˿�*/
				int destseriescnt = FindDestNetAddr(outdata, AimIP, AimPort);
				/*offset = 2 + 1 + DestSeriesNum * ONE_SERIES_LEN + 1 + ONE_SERIES_LEN;*/
				offset = SFP_FRA_LENGTH_VAR_SIZE + SFP_FRA_PROTOCOL_VAR_SIZE + SFP_FRA_DEV_TYPE_VAR_SIZE + SFP_FRA_DEV_ID_VAR_SIZE + SFP_FRA_RESERVE_VAR_SIZE;

				for (int j = 0; j < destseriescnt; j++)
				{
					sprintf(ip, "%u.%u.%u.%u", AimIP[j][0], AimIP[j][1], AimIP[j][2], AimIP[j][3]);
					rt = udp->sendmesg((char *)(outdata + offset), int(singlepacketlen - offset), ip, AimPort[j]);
				}

				outdata += singlepacketlen;
			}
		}
	}
}

void CCcovHandler::RSSP2_DataSend(int arc_id, unsigned char *data, unsigned short len)
{
	unsigned char rslt = 0;

	unsigned char  AimIP[SERIES_NUM_MAX][4];		/*Ŀ��ϵIP*/
	unsigned short AimPort[SERIES_NUM_MAX];		/*Ŀ��˿�*/

	TCP_STATE aleTcpState = STATE_IDLE;
	unsigned char sendRslt = 0;

	int packet_num = CalcPacketCnt(data, len);
	if (0 != packet_num)
	{
		unsigned char *outdata = data;

		for (int i = 0; i < packet_num; i++)
		{
			unsigned int etcsid_dest = FindDestEtcsId(outdata);
			unsigned short single_len = char_to_short(outdata) + SFP_FRA_LENGTH_VAR_SIZE;
			unsigned short offset = SFP_FRA_LENGTH_VAR_SIZE + SFP_FRA_PROTOCOL_VAR_SIZE 
				+ SFP_FRA_DEV_TYPE_VAR_SIZE + SFP_FRA_DEV_ID_VAR_SIZE + SFP_FRA_RESERVE_VAR_SIZE;

			TCP_ALE_O *alenode = _AleCtrl.ALE_SearchNode(etcsid_dest);
			if (nullptr != alenode)
			{
				alenode->arc_id = arc_id;

				/* ����(����TCP���Ʊ���)�Ѵ���ALE�ڵ�,��ALE״̬Ϊ������ʱ�ŷ��ͱ��� */
				for (int j = 0; j < alenode->cfg.Class_D_Connections; j++)
				{
					if (STATE_CONNECTED == alenode->mngt[j].mngt_state && nullptr != alenode->mngt[j].tcp)
					{
						int rt = alenode->mngt[j].tcp->sendmesg((char *)(outdata + offset), int(single_len - offset));
						if (0 == rt)
						{
							alenode->mngt[j].mngt_state = STATE_DISCONNECTED;
						}
					}
				}

				//
			}
			outdata += single_len;
		}
	}

	return;
}


void AleNode_Maintain_Task(TCP_ALE_O *node, unsigned int index)
{
	if (nullptr == node || index >= node->cfg.Class_D_Connections)
	{
		return;
	}

	while (node->flag)
	{
		node->mngt[index].maintainTaskDog = 0;

		switch (node->ctrl)
		{
		case CTRL_CONNECT:
			if (STATE_CONNECTED != node->mngt[index].mngt_state)
			{
				/* ����������*/
				node->mngt[index].tcp = new CTCP();
				if (nullptr != node->mngt[index].tcp)
				{
					if (true == node->mngt[index].tcp->init(node->cfg.addr_remote[index].ip, node->cfg.addr_remote[index].port))
					{
						/*���ӳɹ�*/
						node->mngt[index].mngt_state = STATE_CONNECTED;
					}
					else
					{
						delete node->mngt[index].tcp;
						node->mngt[index].tcp = nullptr;
						node->mngt[index].mngt_state = STATE_DISCONNECTED;
					}
				}
			}
			break;

		case CTRL_IDLE:
		case CTRL_DISCONNECT:
			break;

		default:
			break;
		}

		usleep(node->cfg.delaytime * 100);
	}

	return;
}

void AleNode_Rx_Task(TCP_ALE_O *node, unsigned int index)
{
	if (nullptr == node || index >= node->cfg.Class_D_Connections || false == node->flag)
	{
		return;
	}

	int len = 0;
	char buf[1024] = { 0 };

	unsigned short dev_name = 0, logi_id = 0;
	if (1 != dquEtcsId2DevName(node->etcsId_remote, &dev_name, &logi_id))
	{
		return;
	}

	while (node->mngt[index].flag)
	{
		node->mngt[index].recvTaskDog = 0;

		if (STATE_CONNECTED == node->mngt[index].mngt_state)
		{
			memset(buf, 0, sizeof(buf));
			len = node->mngt[index].tcp->recvmesg(buf, sizeof(buf));
			if (len > 0)
			{
				/* �����ƶ�ArcnetIdת�� */
				//arcSendRslt = PrtclDataPackAndSendViaArcnet(vMIAtpStru.arc_id, buf, len, pAleNode->arc_data_type, pAleNode->pArcSn, (devName >> 8) & 0xFF, devName & 0xFF, logId);
				//if (1 == arcSendRslt)
				{
					node->arc_send++;
				}
			}
			else if (len < 0)
			{
				/* �����ѶϿ� */
				node->mngt[index].mngt_state = STATE_DISCONNECTED;
				if (nullptr != node->mngt[index].tcp)
				{
					delete node->mngt[index].tcp;
					node->mngt[index].tcp = nullptr;
				}
			}
		}

		usleep(node->cfg.delaytime);
	}

	return;
}
void CCcovHandler::StatusToMMI()
{
	int datelen = 0;
	unsigned char StatusBuf[512] = { 0 };
	
	datelen=_Rsov.recvmesg((char*)StatusBuf,sizeof(StatusBuf));

	/** @����CRC����������ĸ��ֽڼ�����CRCУ��ֵ */
	unsigned int crc_rt = 0, crc_calc = 0;
	Crc_32(StatusBuf, 88, 0, &crc_rt);
	crc_calc = char_to_long(StatusBuf + 88);
	if (crc_calc == crc_rt)
	{
		long_to_char(CAppCfg::GetInstance()._Attri.CcovSwVer, StatusBuf+64);
		memcpy(StatusBuf + 68, CAppCfg::GetInstance()._Attri.CcovFsVer, 4);

		if (_ArcnetCtrl->OutDataSend(ARCNET1, COMAB26_ARCNET_NODE_ADDR, (char *)StatusBuf, 96, CAppCfg::GetInstance()._Attri.CcovToMmiType, _VerSn))
			_VerSn++;
	}
}
void CCcovHandler::StatusReportToATP()
{
	unsigned short datelen = 0;
	unsigned char reportbuf[512] = {0};
	/* ��WGB����״̬��ѯ���� */
	long_to_char(_Attri->CcovSwVer, reportbuf);
	reportbuf[4] = _WgbCtrl.GetSNR();
	reportbuf[5] = _WgbCtrl.GetPingRslt();
	ip_string_to_array((char *)_WgbCtrl.GetApIp(), reportbuf + 6);
	reportbuf[10] =  _WgbCtrl.GetWgbStatus();

	reportbuf[11] = _MoAtp.CurlineNum;
	reportbuf[12] = _MoAtp.NextLienNum;
	memcpy(reportbuf + 13, _MoAtp.NextLineVer, 4);
	reportbuf[17] = _MoAtp.IsSwitchAvailable;
	reportbuf[18] = _MoAtp.MMI_Show_Restart;
	reportbuf[19] = _MoAtp.StateRecv;
	reportbuf[20] = _MoAtp.UpdateRecv;

	/* TCP״̬���� */
	if (500 == CAppCfg::GetInstance()._ProductType && 2 == _Attri->ProtclType)
	{
		unsigned char statedata[512] = { 0 };
		Encode_TcpStateMsg(statedata, datelen);
		memcpy(reportbuf + 23, statedata, datelen);
	}

	if (500 == CAppCfg::GetInstance()._ProductType)
	{
		unsigned char data[512] = { 0 };
		memcpy(data, reportbuf, 6);
		ip_string_to_array(CAppCfg::GetInstance()._LocalIp, data + 6);
		memcpy(data + 10, reportbuf + 6, 21);
		memcpy(reportbuf, data, sizeof(data));
	}

	_ArcnetCtrl->OutDataSend(ARCNET1, COMAB26_ARCNET_NODE_ADDR, (char *)reportbuf, datelen + 20, _Attri->CcovToAtpType, _ReportToAtpIndex1++);
	_ArcnetCtrl->OutDataSend(ARCNET2, COMAB26_ARCNET_NODE_ADDR, (char *)reportbuf, datelen + 20, _Attri->CcovToAtpType, _ReportToAtpIndex2++);

}

bool CCcovHandler::WgbRebootCtrl(const char *ip, const unsigned short port)
{
	/** WGB restart datagram, the specific meaning needs to be provided by the DCS department*/
	const unsigned char wgbRebootBuf[46] = {
		0x30,0x2c,																		/*ǰ����*/
		0x02,0x01,0x00,															/*snmpv1*/
		0x04,0x06,0x70,0x75,0x62,0x6c,0x69,0x63,			/*public*/
		0xa3,0x1f,																		/*get next*/
		0x02,0x04,0x74,															/*request id*/
		0xc1,0x01,0x10,															/*error number*/
		0x02,0x01,0x00,															/*error index*/
		0x02,0x01,0x00,0x30,0x11,0x30,								/*none mean ,different */
		0x0f,0x06,0x0a,0x2b,0x06,0x01,0x04,0x01,0x09,0x02,0x09,0x09,0x00,0x02,			/*oid*/
		0x01,0x02																		/*value  0x04 not null 05=num*/
	};

	/** @�ල��WGB�Ĺ���״̬����δ��������WGB������������ */
	if (CBTC_FALSE == _WgbAsoctStatus)
	{
		_Wgb.sendmesg((char *)wgbRebootBuf, sizeof(wgbRebootBuf), ip, port);
		return true;
	}
	return false;
}


void CCcovHandler::OnTimer(void *arg)
{
	_TIMER_++;
	CLEDCtrl::GetInstance().LED_B2(_TIMER_ % 2);
	/*if(_TIMER_ % 150 == 0)
		_WgbCtrl.Reset();*/
	/** @status report to ATP */
	StatusReportToATP();
}