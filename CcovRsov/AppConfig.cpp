#include "DataLoad.h"
#include "dfsDataRead.h"
#include "dsuCcovFunc.h"
#include "vobcInternalTelegramDef.h"

#include "Utilities/FileWork/IniFileWork.h"
#include "Utilities/FileWork/LogFileWork.h"
using namespace FileWork;

#include "Utilities/Convert/ConvertWork.h"
using namespace Convert;

#include "Utilities/NetWork/NetWork.h"
using namespace NetWork;

#include "AppDefine.h"
#include "AppConfig.h"


CAppCfg::CAppCfg() : _AppType(0), _ProductType(510)
{
	memset(_LocalIp, 0, sizeof(_LocalIp));
	memset(_NetMask, 0, sizeof(_NetMask));
	memset(_GateWay, 0, sizeof(_GateWay));
	memset(_OIDInfo, 0, sizeof(_OIDInfo));
	memset(&_Attri, 0, sizeof(CCOV_ATTRI));
	init();
}

void CAppCfg::init()
{
	char *eth2 = "eth2";
	char Str[64] = { 0 };

	ReadStringValue(APP_INTERNET, INTERNET_LOCALIP, _LocalIp, APP_CONFIG_PROFILE);
	ip_string_to_array(_LocalIp, _Attri.Ip);

	_Attri.DataLenNum = ReadIntValue(APP_INFO, FRAME_DATA_LEN, APP_CONFIG_PROFILE);
	_Attri.ProtclType = ReadIntValue(APP_INFO, PROTOCOL_TYPE, APP_CONFIG_PROFILE);
	_ProductType = ReadIntValue(APP_INFO, PRODUCT_TYPE, APP_CONFIG_PROFILE);
	_AppType = ReadIntValue(APP_INFO, APP_TYPE, APP_CONFIG_PROFILE);
	

	if (1 == _AppType)
	{
		if (!SetLocalIp(Ccov_IP, "eth2"))
		{
			LOG(ERROR) << "CCOV set Ccov_to_Rsov local ip error. ip = " << _LocalIp;
		}

		if (!SetLocalNetMask(Net_Mask, "eth2"))
		{
			LOG(ERROR) << "CCOV set Ccov_to_Rsov local net mask error. net mask = " << _NetMask;
		}

		if (!SetLocalGateway(Gateway, eth2))
		{
			LOG(ERROR) << "CCOV set Ccov_to_Rsov local gateway error. gate way = " << _GateWay;
		}



		if (!SetLocalIp(_LocalIp, "eth0"))
		{
			LOG(ERROR) << "CCOV set local ip error. ip = " << _LocalIp;
		}

		ReadStringValue(APP_INTERNET, INTERNET_NETMASK, _NetMask, APP_CONFIG_PROFILE);
		if (!SetLocalNetMask(_NetMask, "eth0"))
		{
			LOG(ERROR) << "CCOV set local net mask error. net mask = " << _NetMask;
		}
		char * eth0 = "eth0";
		ReadStringValue(APP_INTERNET, INTERNET_GATEWAY, _GateWay, APP_CONFIG_PROFILE);
		if (!SetLocalGateway(_GateWay, eth0))
		{
			LOG(ERROR) << "CCOV set local gateway error. gate way = " << _GateWay;
		}

		ReadStringValue(APP_WGB, WGB_OID1, (char *)_OIDInfo[0], APP_CONFIG_PROFILE);
		ReadStringValue(APP_WGB, WGB_OID2, (char *)_OIDInfo[1], APP_CONFIG_PROFILE);
		memset(&_ReqArgv, 0, sizeof(REQ_ARGV_STRU));
		_ReqArgv.version = 0x01;
		strcpy((char *)_ReqArgv.communityStr, "public");
		_ReqArgv.poid[0] = _OIDInfo[0];
		_ReqArgv.poid[1] = _OIDInfo[1];

		/* WGB */
		char tmp[16] = { 0 };
		ReadStringValue(APP_WGB, WGB_REQMODE, tmp, APP_CONFIG_PROFILE);
		sscanf(tmp, "%x", &_REQInfo);
	}
	else
	{
		if (!SetLocalIp(Rsov_IP, "eth2"))
		{
			LOG(ERROR) << "RSOV set Ccov_to_Rsov local ip error. ip = " << _LocalIp;
		}

		if (!SetLocalNetMask(Net_Mask, "eth2"))
		{
			LOG(ERROR) << "RSOV set Ccov_to_Rsov local net mask error. net mask = " << _NetMask;
		}

		if (!SetLocalGateway(Gateway, eth2))
		{
			LOG(ERROR) << "RSOV set Ccov_to_Rsov local gateway error. gate way = " << _GateWay;
		}
	}

	/* 根据IP地址获取设备名称和红蓝网信息 */
	if (1 == dataRead(APP_CONFIG_FILE))
	{
		free(g_pDataBuf);
		g_pDataBuf = nullptr;
		free(g_pDFsDH);
		g_pDFsDH = nullptr;

		_FsStorCfg.ReadMode = 0x01;
		_FsStorCfg.DataBuf = (unsigned char *)MyNew(DATA_CONTENT_MALLOC_SIZE);
		if (nullptr != _FsStorCfg.DataBuf)
		{
			memset(_FsStorCfg.DataBuf, 0xFF, DATA_CONTENT_MALLOC_SIZE);
		}

		_FsStorCfg.DfsDH = (dfsDataHead*)MyNew(sizeof(dfsDataHead));
		if (nullptr != _FsStorCfg.DfsDH)
		{
			memset(_FsStorCfg.DfsDH, 0, sizeof(dfsDataHead));
		}

		dquSetFSReadStruParamRef(&_FsStorCfg);
		dataReadTemp(APP_CONFIG_FILE, &_FsStorCfg);

		if (1 == dsuCcovIpInit((UINT8 *)APP_CONFIG_FILE))
		{
			_IpType = dsuGetDevNameByIp(_Attri.Ip, &_DevName);
			if (CCOV_RED_IP == _IpType || CCOV_BLUE_IP == _IpType)
			{
				if (DEVICE_CCOV == ((_DevName >> 8) & 0xFF))
				{
					/* 查询红蓝网信息 */
					_Attri.NetType = dsuGetCcovRedOrBlue(_Attri.Ip);
					if (NET_TYPE_RED == _Attri.NetType)
					{
						/* 红网，设置红网信息 */
						_Attri.CcovToAtpType = ATP_RED_CCOV;
						_Attri.CcovToRsovType = VOBC_RED_CCOV_TO_LOGGER;
						_Attri.RadioTestType = ATP_RED_RADIO_TEST;
						_Attri.CcovToMmiType = VOBC_RED_CCOV_TO_MMI;
					}
					else if (NET_TYPE_BLUE == _Attri.NetType)
					{
						/* 蓝网，设置蓝网信息 */
						_Attri.CcovToAtpType = ATP_BLUE_CCOV;
						_Attri.CcovToRsovType = VOBC_BLUE_CCOV_TO_LOGGER;
						_Attri.RadioTestType = ATP_BLUE_RADIO_TEST;
						_Attri.CcovToMmiType = VOBC_BLUE_CCOV_TO_MMI;
					}
				}
			}
		}
	}
	else
	{
		LOG(ERROR) << "AppConfig dataRead error. exit";
		exit(EXIT_FAILURE);
	}
}