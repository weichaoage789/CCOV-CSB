#include "AppDefine.h"
#include "AppConfig.h"

#include "dfsDataRead.h"
#include "dsuRssp2Func.h"
#include "dsuCcovFunc.h"
#include "dquIdmapInit.h"
#include "dquIdmapQuery.h"
#include "vobcInternalTelegramDef.h"
#include "SNMP/CcovSnmpConfig.h"
#include "SNMP/CcovSnmpMdl.h"

#include "Utilities/CRC.h"
#include "Driver/LED/Led.h"
#include "Driver/COM20020/COM20020.h"

#include "Utilities/FileWork/LogFileWork.h"
using namespace FileWork;
#include "Utilities/Convert/ConvertWork.h"
using namespace Convert;

#include "CcovCtrl.h"


void CCcovCtrl::Start()
{
	if (!_RunningFlag)
	{
		_RunningFlag = true;

		for (int arc = ARCNET1; arc <= ARCNET2; arc++)
		{
			std::pair<CCcovCtrl *, int> *info = new std::pair<CCcovCtrl *, int>(this, arc);
			std::thread t_parse(Thread_Parse_Arcnet, info);
			t_parse.detach();
		}

		for (auto it = _Handle._Outer.begin(); it != _Handle._Outer.end(); it++)
		{
			std::pair<CCcovCtrl *, int> *thispair = new std::pair<CCcovCtrl *, int>(this, it->first);
			std::thread rx_outer(Thread_Rx_Outnet, thispair);
			rx_outer.detach();
		}

		std::thread rx_wgb(Thread_Rx_WGB, this);
		rx_wgb.detach();

		std::thread rx_rsov(Thread_Rx_Rsov, this);
		rx_rsov.detach();
	}
}

void CCcovCtrl::Stop()
{
	_RunningFlag = false;
}


void CCcovCtrl::Thread_Parse_Arcnet(void *arg)
{
	if (nullptr == arg)
	{
		return;
	}
	std::pair<CCcovCtrl *, int> *thispair = (std::pair<CCcovCtrl *, int> *)arg;
	CCcovCtrl *pThisObject = thispair->first;
	int arc_id = thispair->second;
	delete thispair;
	thispair = nullptr;
	LOG(INFO) << "CCOV ARCNET" << arc_id << " parse thread begin, thread id = " << syscall(SYS_gettid);

	unsigned int len = 0;
	unsigned char *data = nullptr;
	unsigned char buf[1024] = { 0 };

	while (pThisObject->_RunningFlag)
	{
		if (ARCNET1 == arc_id)
		{
			if (!pThisObject->_Handle._Que_Rx_Arcnet1.PopMesg(&data, len))
			{
				continue;
			}
		}
		else if (ARCNET2 == arc_id)
		{
			if (!pThisObject->_Handle._Que_Rx_Arcnet2.PopMesg(&data, len))
			{
				continue;
			}
		}

		if (nullptr != data && len != 0 && len < 1024)
		{
			memset(buf, 0, sizeof(buf));
			memcpy(buf, data, len);
			delete[] data;
			data = nullptr;

			/** @获取接收到的数据区长度与实际有效数据的长度 */
			unsigned short buflen = char_to_short(buf + 2);
			unsigned short datalen = char_to_short(buf + 4);

			if (datalen > 9 && datalen < 508)
			{
				/** @计算CRC，数据最后四个字节即数据CRC校验值 */
				unsigned int crc_rt = 0, crc_calc = 0;
				Crc_32(buf + 4, datalen - 2, 0, &crc_rt);
				crc_calc = char_to_long(buf + datalen + 2);
				if (crc_calc != crc_rt) continue; 

				/** @turn on A2/A3 according to ID */
				if (ARCNET1 == arc_id)
				{
					CLEDCtrl::GetInstance().LED_A2(0);
				}
				else if (ARCNET2 == arc_id)
				{
					CLEDCtrl::GetInstance().LED_A3(0);
				}

				/** @Data handle */
				pThisObject->_Handle.HandleFunc(arc_id, buf);
			}
		}

		usleep(1000);
	}

	LOG(INFO) << "CCOV ARCNET" << arc_id << " parse thread quit";
	return;
}

void CCcovCtrl::Thread_Rx_Outnet(void *arg)
{
	if (nullptr == arg)
	{
		return;
	}
	std::pair<CCcovCtrl *, int> *thispair = (std::pair<CCcovCtrl *, int> *)arg;
	CCcovCtrl *pThisObject = thispair->first;
	int port = thispair->second;
	delete thispair;
	thispair = nullptr;
	LOG(INFO) << "CCOV Outer receive thread start, thread id = " << syscall(SYS_gettid) << ", port = " << port;

	int len = 0;
	char buf[1024];
	char remote_ip[16];
	unsigned short remote_port = 0;

	unsigned int sn_send = 0;
	unsigned char data_type = 0;
	CUDP *udp = pThisObject->_Handle._Outer[port].first;

	while (pThisObject->_RunningFlag)
	{
		memset(buf, 0, sizeof(buf));
		len = udp->recvmesg(buf, sizeof(buf), remote_ip, remote_port);

		if (len > 0)
		{
			unsigned char source_ip[4] = { 0 };
			ip_string_to_array(remote_ip, source_ip);

			int arc_id = 0xFF;
			bool rt_send = false;
			unsigned char recordflag = 0xFF;

			if (!pThisObject->_Handle._Outer[port].second.empty())
			{
				arc_id = pThisObject->_Handle._Outer[port].second.front();
				pThisObject->_Handle._Outer[port].second.pop();
			}

			if (0xFF != arc_id)
			{
				if ((VOBC_FROM_ATS_NTP == data_type) || (pThisObject->_Handle._Attri->RadioTestType == data_type) || (NDSU_AND_ATP_TYPE == data_type))
				{
					/* 非协议数据,直接打包发送 */
					rt_send = pThisObject->_Handle._ArcnetCtrl->OutDataSend(arc_id, COMAB26_ARCNET_NODE_ADDR, buf, len, data_type, sn_send);
				}
				else
				{
					/** @协议数据， 查询deviceInfo */
					unsigned short devinfo = 0;

					if ((NET_TYPE_RED == pThisObject->_Handle._Attri->NetType) && (1 == dsuGetRedDevName(source_ip, remote_port, &devinfo))
						|| (NET_TYPE_BLUE == pThisObject->_Handle._Attri->NetType) && (1 == dsuGetBlueDevName(source_ip, remote_port, &devinfo)))
					{
						if (DEV_AOM == ((devinfo >> 8) & 0xFF))
						{
							rt_send = pThisObject->_Handle._ArcnetCtrl->OutDataSend_Prtcl(0, COMAB26_ARCNET_NODE_ADDR, buf, len, VOBC_FROM_AOM, sn_send, (devinfo >> 8) & 0xFF, devinfo & 0xFF, 0);
						}
						else if (DEV_PSC == ((devinfo >> 8) & 0xFF))
						{
							rt_send = pThisObject->_Handle._ArcnetCtrl->OutDataSend_Prtcl(arc_id, COMAB26_ARCNET_NODE_ADDR, buf, len, VOBC_FROM_PSC, sn_send, (devinfo >> 8) & 0xFF, devinfo & 0xFF, 0);
						}
						else
						{
							rt_send = pThisObject->_Handle._ArcnetCtrl->OutDataSend_Prtcl(arc_id, COMAB26_ARCNET_NODE_ADDR, buf, len, VOBC_FROM_OUT_NET, sn_send, (devinfo >> 8) & 0xFF, devinfo & 0xFF, 0);
						}
					}
				}

				if (true == rt_send)
				{
					sn_send++;
					recordflag = 0x55;
					if (arc_id == ARCNET1)
					{
						CLEDCtrl::GetInstance().LED_A2(1);
					}
					else if (arc_id == ARCNET2)
					{
						CLEDCtrl::GetInstance().LED_A3(1);
					}
				}
				else
				{
					recordflag = 0xAA;
					LOG(ERROR) << "Thread_Rx_Outnet port = " << port << " ARCNET send failed";
				}
			}
		}
		
		usleep(1000);
	}

	return;
}

void CCcovCtrl::Thread_Rx_WGB(void *arg)
{
	if (nullptr == arg)
	{
		return;
	}
	CCcovCtrl *thisobject = (CCcovCtrl *)arg;
	LOG(INFO) << "CCOV WGB receive thread start, thread id = " << syscall(SYS_gettid);

	int len = 0;
	char buf[512] = { 0 };
	RESPONSE_INFO_NEWSTRU rinfo;

	unsigned char req_code[256];
	unsigned short req_code_len = 0;

	while (thisobject->_RunningFlag)
	{
		/* 调用无线测试接口函数,发送获取场强信息和关联的AP的Ip地址信息的报文包 */
		memset(req_code, 0, sizeof(req_code));
		req_code_len = RequestFlameCode(req_code, CAppCfg::GetInstance()._REQInfo, num_Two, &CAppCfg::GetInstance()._ReqArgv);

		/* 向WGB和LTE无线设备发送SNMP请求报文 */
		thisobject->_Handle._Wgb.sendmesg((char *)req_code, req_code_len, (char *)thisobject->_Handle._WgbIp, PORT_WGB_TX);

		memset(buf, 0, sizeof(buf));
		len = thisobject->_Handle._Wgb.recvmesg(buf, sizeof(buf));
		if (len > 0)
		{
			/* 取场强信息和关联AP的Ip地址信息 */
			memset(&rinfo, 0, sizeof(RESPONSE_INFO_NEWSTRU));
			ResponseDecode(&rinfo, (unsigned char *)buf, len);
			thisobject->_Handle._WgbCtrl.SetSNR(rinfo.intTypeStru[0].intVal);
		}

		usleep(200000);
	}

	return;
}

void CCcovCtrl::Thread_Rx_Rsov(void *arg)
{
	if (nullptr == arg)
	{
		return;
	}
	CCcovCtrl *thisobject = (CCcovCtrl *)arg;
	LOG(INFO) << "CCOV Rsov receive thread start, thread id = " << syscall(SYS_gettid);

	int len = 0;
	char buf[512] = { 0 };

	while (thisobject->_RunningFlag)
	{
		thisobject->_Handle.StatusToMMI();
		usleep(200000);
	}

	return;
}