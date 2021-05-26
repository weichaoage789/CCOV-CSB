#include "AppDefine.h"
#include "AppConfig.h"

#include "vobcInternalTelegramDef.h"
#include "Protocol/FFFE/FFFE.h"
#include "USBPlugin/USBCtrl.h"
#include "Driver/LED/Led.h"
#include "Driver/COM20020/COM20020.h"

#include "Utilities/Convert/ConvertWork.h"
using namespace Convert;

#include "Utilities/FileWork/FileWork.h"
#include "Utilities/FileWork/LogFileWork.h"
using namespace FileWork;

#include "RsovDefine.h"
#include "RsovConfig.h"
#include "RsovHandler.h"


CRsovHandler::CRsovHandler() : _FileCtrl(nullptr),  _ParseCnt(0), _RdCnt(0), _TIMER_(0)
{
	_FileCtrl = new CRsovFileCtrl();
	_ArcnetCtrl = new CArcnet(&_Que_Rx_Arcnet3, &_Que_Rx_Arcnet4);

	/** @timer thread */
	std::thread([this]() {
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			OnTimer(nullptr);
		}
	}).detach();

	if ((0 > _ComCtrl.Open(1)) || (0 != _ComCtrl.SetOptions(230400, 8, 'N', 1)))
	{
		LOG(ERROR) << "RSOV open SerialComm error";
	}
	_Ccov.init(ADDR_ANY, PORT_50004);
	LOG(INFO) << "CCOV UDP--RSOV initialize success, port = " << PORT_50004;

	memset(_Verbuf, 0, sizeof(_Verbuf));
	InitRedund();
}

CRsovHandler::~CRsovHandler()
{
	_ComCtrl.Close();

	if (_FileCtrl)
	{
		delete _FileCtrl;
		_FileCtrl = nullptr;
	}
}


void CRsovHandler::InitRedund()
{
	memset(_TypeSn, 0, sizeof(_TypeSn));
	_TypeSn[0].Type = VOBC_TMR_A_TO_LOGGER;
	_TypeSn[1].Type = VOBC_TMR_B_TO_LOGGER;
	_TypeSn[2].Type = VOBC_TMR_C_TO_LOGGER;
	_TypeSn[3].Type = VOBC_1_ATP_TO_LOGGER;
	_TypeSn[4].Type = VOBC_2_ATP_TO_LOGGER;
	_TypeSn[5].Type = VOBC_ATP_ERR_TO_LOGGER;
	_TypeSn[6].Type = VOBC_ATO_TO_LOGGER;
	_TypeSn[7].Type = VOBC_ATO_ERR_TO_LOGGER;
	_TypeSn[8].Type = VOBC_MMI_TO_LOGGER;
	_TypeSn[9].Type = VOBC_COM_1_TO_LOGGER1;
	_TypeSn[10].Type = VOBC_COM_2_TO_LOGGER1;
	_TypeSn[11].Type = VOBC_RED_CCOV_TO_LOGGER;
	_TypeSn[12].Type = VOBC_BLUE_CCOV_TO_LOGGER;
	_TypeSn[13].Type = LOGGER_TO_TESTER;
	_TypeSn[14].Type = ATP_ATO;
	_TypeSn[15].Type = ATP_MMI;
	_TypeSn[16].Type = VOBC_ANOTHER_VOBC;
	_TypeSn[17].Type = ATP_ATO2;
	_TypeSn[18].Type = ATP_RED_CCOV;
	_TypeSn[19].Type = ATP_BLUE_CCOV;
	_TypeSn[20].Type = ATP_LOGGER;
	_TypeSn[21].Type = ATP_RED_RADIO_TEST;
	_TypeSn[22].Type = ATP_BLUE_RADIO_TEST;
	_TypeSn[23].Type = VOBC_ANOTHER_VOBC_SDU;
	_TypeSn[24].Type = TMR_CMD_TO_COMAB_START_SEND;
	_TypeSn[25].Type = TMR_CMD_TO_COMAB_STOP_SEND;
	_TypeSn[26].Type = VOBC_ATP_PARAMETER_UPDATE;
	_TypeSn[27].Type = VOBC_FROM_OUT_NET;
	_TypeSn[28].Type = VOBC_FROM_CI;
	_TypeSn[29].Type = VOBC_FROM_ATS_NTP;
	_TypeSn[30].Type = VOBC_FROM_ATS;
	_TypeSn[31].Type = VOBC_TO_CI1;
	_TypeSn[32].Type = VOBC_TO_CI2;
	_TypeSn[33].Type = VOBC_TO_CI3;
	_TypeSn[34].Type = VOBC_TO_CI4;
	_TypeSn[35].Type = VOBC_TO_ATS_NTP;
	_TypeSn[36].Type = VOBC_TO_ATS1;
	_TypeSn[37].Type = VOBC_TO_ATS2;
	_TypeSn[38].Type = VOBC_TO_ATS3;
	_TypeSn[39].Type = VOBC_TO_ATS4;
	_TypeSn[40].Type = VOBC_TO_ATS5;
	_TypeSn[41].Type = VOBC_1_TO_OUT_NET;
	_TypeSn[42].Type = VOBC_2_TO_OUT_NET;
	_TypeSn[43].Type = VOBC_3_TO_OUT_NET;
	_TypeSn[44].Type = VOBC_ATP_A_SW_UPDATE;
	_TypeSn[45].Type = VOBC_ATP_B_SW_UPDATE;
	_TypeSn[46].Type = VOBC_ATP_C_SW_UPDATE;
	_TypeSn[47].Type = VOBC_COMA_SW_UPDATE;
	_TypeSn[48].Type = VOBC_COMB_SW_UPDATE;
	_TypeSn[49].Type = VOBC_CCOV_SW_UPDATE;
	_TypeSn[50].Type = VOBC_LOGGER_SW_UPDATE;
	_TypeSn[51].Type = VOBC_MMI_SW_UPDATE;
	_TypeSn[52].Type = VOBC_ATO_SW_UPDATE;
	_TypeSn[53].Type = VOBC_TO_MSS1;
	_TypeSn[54].Type = VOBC_TO_MSS2;
	_TypeSn[55].Type = VOBC_SDU_A_TO_LOGGER;
	_TypeSn[56].Type = VOBC_SDU_B_TO_LOGGER;
	_TypeSn[57].Type = VOBC_SDU_C_TO_LOGGER;
	_TypeSn[58].Type = VOBC_ATO2_TO_LOGGER;
	_TypeSn[59].Type = VOBC_ATO2_ERR_TO_LOGGER;
	_TypeSn[60].Type = VOBC_ATORPM1_TO_LOGGER;
	_TypeSn[61].Type = VOBC_ATORPM2_TO_LOGGER;
	_TypeSn[62].Type = VOBC_3_ATP_TO_LOGGER;
	_TypeSn[63].Type = VOBC_ATO_MA_TO_LOGGER;
	_TypeSn[64].Type = VOBC_COM_1_TO_LOGGER2;
	_TypeSn[65].Type = VOBC_COM_2_TO_LOGGER2;
	_TypeSn[66].Type = VOBC_ATO_TMS_TO_LOGGER;
	_TypeSn[67].Type = VOBC_TO_PSC;
	_TypeSn[68].Type = VOBC_TO_PSC2;
	_TypeSn[69].Type = VOBC_FROM_PSC;
}

void CRsovHandler::DataRecordPrepare(char *typebuf, unsigned char *data, unsigned int len)
{
	/** @接收数据长度,不包括自身,加2以便保存含长度的报文 */
	len += 2;
	unsigned int total = len + USUAL_HEAD_LEN;
	/** @记录数据的总长度为:2字节长度信息+时间和红蓝网信息(以及有效性信息)+待保存的数据长度信息(dataLen+2,因为长度两个字节不包含在内)*/

	unsigned char *tmp = nullptr;
	tmp = new unsigned char[total + 2];
	if (nullptr != tmp)
	{
		short_to_char(total, tmp);
		memcpy(tmp + 2, typebuf, USUAL_HEAD_LEN);
		memcpy(tmp + 2 + USUAL_HEAD_LEN, data + 4, len);

		_Que_Record.PushMesg(tmp, total + 2);
	}
}

void CRsovHandler::DataRedundProc(int arcid, unsigned char *redunbuf)
{
	char headbuf[16] = { 0 };
	GetSysDateAndTime(nullptr, headbuf);

	if (ARCNET1 == arcid)
		strcat(headbuf, NET_RED);
	else if (ARCNET2 == arcid)
		strcat(headbuf, NET_BLUE);

	unsigned short datalen = char_to_short(redunbuf + 4);
	unsigned int recvsn = char_to_long(redunbuf + 7);

	for (int i = 0; i < TYPE_SN_NUM; i++)
	{
		if (_TypeSn[i].Type == redunbuf[6])
		{
			/** @去除冗余帧数据 */
			if (_TypeSn[i].Sn < recvsn)
			{
				/** @添加记录数据长帧信息 */
				*(headbuf + USUAL_HEAD_LEN - 1) = 0x55;
				DataRecordPrepare(headbuf, redunbuf, datalen);
				_TypeSn[i].Sn = recvsn;
			}
			else
			{
				/** @添加记录数据短帧信息 */
				*(headbuf + USUAL_HEAD_LEN - 1) = 0xAA;
				DataRecordPrepare(headbuf, redunbuf, REDUND_DATA_LEN);
			}
		}
	}
}

bool CRsovHandler::FFFEsend(unsigned char *data, unsigned int len)
{
	if (nullptr != data)
	{
		unsigned char combuf[1024];
		memset(combuf, 0, sizeof(combuf));

		int translted = CFFFE::GetInstance().Encode(data, combuf + 2, len);
		combuf[0] = 0xFF;
		combuf[1] = 0xFE;
		combuf[translted + 2] = 0xFF;
		combuf[translted + 3] = 0xFD;

		if ( (translted + 4) == _ComCtrl.Send((char *)combuf, translted + 4)) 
			return true;
	}
	return false;
}

void CRsovHandler::HandleFunc(int arcid, unsigned char *data)
{
	unsigned short buflen = char_to_short(data + 2);
	unsigned short datalen = char_to_short(data + 4);
	unsigned int crc_rt = 0;
	
	switch (data[6])
	{
	case VOBC_ATO_ERR_TO_LOGGER:
		/** @ATO集成软件版本号与数据版本号 */
		memcpy(_Verbuf, data + 11, 4);
		memcpy(_Verbuf + 4, data + buflen - 4, 4);
		/** @ATO输入板FPGA版本号*/
		if (0 == data[15])
		{
			memset(_Verbuf + 8, 0, 4);
		}
		else
		{
			_Verbuf[8] = 88;
			_Verbuf[9] = 21;
			_Verbuf[10] = 12;
			_Verbuf[11] = *(data + 15);
		}
		/** @ATO输出板FPGA版本号*/
		if (0 == data[16])
		{
			memset(_Verbuf + 12, 0, 4);
		}
		else
		{
			_Verbuf[12] = 88;
			_Verbuf[13] = 21;
			_Verbuf[14] = 11;
			_Verbuf[15] = *(data + 16);
		}
		/** @TRDP 软件版本号*/
		memcpy(_Verbuf + 88, data + 17, 4);
		/** @TRDP 数据版本号*/
		memcpy(_Verbuf + 92, data + datalen - 2, 4);
		break;

	case VOBC_ATP_ERR_TO_LOGGER:
		/** @ATP集成软件版本号与数据版本号 */
		memcpy(_Verbuf + 16, data + 11, 4);
		memcpy(_Verbuf + 20, data + buflen - 4, 4);
		/** @AOM集成软件版本号与数据版本号 */
		memcpy(_Verbuf + 24, data + 49, 4);
		memcpy(_Verbuf + 28, data + 53, 4);
		/** @CSB集成软件版本号与数据版本号 */
		memcpy(_Verbuf + 32, data + 67, 4);
		memcpy(_Verbuf + 36, data + 71, 4);
		/** @CSBFPGA版本号*/
		if (0 == data[79])
		{
			memset(_Verbuf + 40, 0, 4);
		}
		else
		{
			_Verbuf[40] = 88;
			_Verbuf[41] = 28;
			_Verbuf[42] = 9;
			_Verbuf[43] = *(data + 78);
		}
		/** @AOM输入板FPGA版本号 */
		if (0 == data[82])
		{
			memset(_Verbuf + 44, 0, 4);
		}
		else
		{
			_Verbuf[44] = 88;
			_Verbuf[45] = 28;
			_Verbuf[46] = 10;
			_Verbuf[47] = *(data + 82);
		}
		/** @AOM输出板FPGA版本号 */
		if (0 == data[86])
		{
			memset(_Verbuf + 48, 0, 4);
		}
		else
		{
			_Verbuf[48] = 88;
			_Verbuf[49] = 28;
			_Verbuf[50] = 8;
			_Verbuf[51] = *(data + 86);
		}
		break;

	case VOBC_TMR_A_TO_LOGGER:
		/** @TRM_A软件版本号 */
		memcpy(_Verbuf + 52, data + 11, 4);
		break;

	case VOBC_TMR_B_TO_LOGGER:
		/** @TRM_B软件版本号 */
		memcpy(_Verbuf + 56, data + 11, 4);
		break;

	case VOBC_TMR_C_TO_LOGGER:
		/** @TRM_C软件版本号 */
		memcpy(_Verbuf + 60, data + 11, 4);
		break;

	case VOBC_RED_CCOV_TO_LOGGER:
		memcpy(_Verbuf + 64, data + 11, 4);
		memcpy(_Verbuf + 68, data + 26, 4);
		break;
	case VOBC_BLUE_CCOV_TO_LOGGER:
		/** @红蓝网CCOV集成软件与数据版本号 */
		memcpy(_Verbuf + 72, data + 11, 4);
		memcpy(_Verbuf + 76, data + 26, 4);
		break;

	case VOBC_COM_1_TO_LOGGER1:
		/** @通信板A集成软件与数据版本号 */
		memcpy(_Verbuf + 80, data + 11, 4);
		break;

	case VOBC_COM_2_TO_LOGGER1:
		/** @通信板B集成软件与数据版本号 */
		memcpy(_Verbuf + 84, data + 11, 4);
		break;

	default:
		break;
	}

	/** @sending version message to MMI per second */
	if (_TIMER_ >= 5)
	{
		Crc_32(_Verbuf, 88, 0, &crc_rt);
		long_to_char(crc_rt, _Verbuf + 88);
		_Ccov.sendmesg((char*)_Verbuf, sizeof(_Verbuf), Ccov_IP, PORT_50004);
	}

	/** @save data to file */
	DataRedundProc(arcid, data);

	if (ARCNET1 == arcid)
	{
#if VOBC_DEBUG
		if ((VOBC_RED_CCOV_TO_LOGGER == data[6]) || (VOBC_COM_1_TO_LOGGER1 == data[6]) || (VOBC_COM_1_TO_LOGGER2 == data[6])
			|| (VOBC_TMR_A_TO_LOGGER == data[6]) || (VOBC_TMR_B_TO_LOGGER == data[6]) || (VOBC_TMR_C_TO_LOGGER == data[6])
			|| (VOBC_ATO_ERR_TO_LOGGER == data[6]) || (VOBC_ATO2_ERR_TO_LOGGER == data[6]) || (VOBC_ATORPM1_TO_LOGGER == data[6])
			|| (VOBC_ATORPM2_TO_LOGGER == data[6]))
		{
			/* 将待发送的报文(报文含内网层,去掉了ArcLen,SID,DID)转义 */
			if (!FFFEsend(data + 4, buflen))
			{
				LOG(ERROR) << "FFFE send fail";
			}
		}
#endif
	}
	else if (ARCNET2 == arcid)
	{
		/*从蓝网更新RSOV时间信息,同时为了平衡红蓝网的处理,故在这里处理,从应用记录数据中提取时间信息VOBC_1_ATP_TO_LOGGER */
		if (VOBC_1_ATP_TO_LOGGER == data[6])
		{
			/*ATP记录数据的接口描述为版本号(4)+时间信息(4);SID(1)+DID(1)+ArcLen(2)+Len(2)+Type(1)+Sn(4)*/
			unsigned int atp_time = char_to_long(data + 15);
			if (0xFFFFFFFF != atp_time)
			{
				_FileCtrl->RectifyTime(atp_time);
			}
		}

#if VOBC_DEBUG
		if ((VOBC_COM_2_TO_LOGGER1 == data[6]) || (VOBC_COM_2_TO_LOGGER2 == data[6]) || (VOBC_BLUE_CCOV_TO_LOGGER == data[6])
			|| (VOBC_MMI_TO_LOGGER == data[6]) || (VOBC_ATP_ERR_TO_LOGGER == data[6]) || (VOBC_1_ATP_TO_LOGGER == data[6])
			|| (VOBC_ATO_ERR_TO_LOGGER == data[6]) || (VOBC_ATO2_ERR_TO_LOGGER == data[6]) || (VOBC_ATORPM1_TO_LOGGER == data[6])
			|| (VOBC_ATORPM2_TO_LOGGER == data[6]))
		{
			/* 将待发送的报文(报文含内网层,去掉了ArcLen,SID,DID)转义 */
			if (!FFFEsend(data + 4, buflen))
			{
				LOG(ERROR) << "FFFE send fail";
			}
		}
#endif
	}
}

void CRsovHandler::OnTimer(void *arg)
{
	_TIMER_++;

	if (USB_STATUS::STATE_ADD == CUSBCtrl::Getinstance().GetUSBStatus())
	{
		if (COPY_STATUS::STATE_NORMAL == CUSBCtrl::Getinstance().GetCOPYStatus())
		{
			/** @status change every 400ms */
			if ((_TIMER_ % 4) < 2)
				CLEDCtrl::GetInstance().LED_B3(0);
			else
				CLEDCtrl::GetInstance().LED_B3(1);
		}
		else if (COPY_STATUS::STATE_SUCCESS == CUSBCtrl::Getinstance().GetCOPYStatus())
		{
			CLEDCtrl::GetInstance().LED_B3(0);
		}
		else if (COPY_STATUS::STATE_FAIL == CUSBCtrl::Getinstance().GetCOPYStatus())
		{
			CLEDCtrl::GetInstance().LED_B3(1);
		}
	}
	else
	{
		CLEDCtrl::GetInstance().LED_B3(_TIMER_ % 2);
	}

	if (0 == (_TIMER_ % 1500))
	{
		LOG(INFO) << "ARCNET1 rx = " << _ArcnetCtrl->GetRxcnt(ARCNET1)
			<< ", total push count = " << _Que_Rx_Arcnet3.GetPushcnt() << ", total pop count = " << _Que_Rx_Arcnet3.GetPopcnt()
			<< ", push fail count = " << _Que_Rx_Arcnet3.GetPushfailcnt() << ", remain data count = " << _Que_Rx_Arcnet3.GetDatacnts();

		LOG(INFO) << "ARCNET2 rx = " << _ArcnetCtrl->GetRxcnt(ARCNET2)
			<< ", total push count = " << _Que_Rx_Arcnet4.GetPushcnt() << ", total pop count = " << _Que_Rx_Arcnet4.GetPopcnt()
			<< ", push fail count = " << _Que_Rx_Arcnet4.GetPushfailcnt() << ", remain data count = " << _Que_Rx_Arcnet4.GetDatacnts();

		LOG(INFO) << "PARSE = " << _ParseCnt << "; record to file count = " << _RdCnt;
	}
}