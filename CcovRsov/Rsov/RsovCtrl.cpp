#include "AppDefine.h"
#include "AppConfig.h"

#include "dsuCcovFunc.h"
#include "USBPlugin/USBCtrl.h"
#include "Utilities/CRC.h"
#include "Driver/COM20020/COM20020Define.h"

#include "Utilities/FileWork/LogFileWork.h"
using namespace FileWork;
#include "Utilities/Convert/ConvertWork.h"
using namespace Convert;

#include "RsovCtrl.h"


void CRsovCtrl::Start()
{
	if (!_RunningFlag)
	{
		_RunningFlag = true;

		for (int i = ARCNET1; i <= ARCNET2; i++)
		{
			std::pair<CRsovCtrl *, int> *info = new std::pair<CRsovCtrl *, int>(this, i);
			std::thread t_parse(Thread_Parse_Arcnet, info);
			t_parse.detach();
		}

		std::thread t_rd1(Thread_Rd_Arcnet, this);
		t_rd1.detach();

		CUSBCtrl::Getinstance().Start();
	}
}

void CRsovCtrl::Stop()
{
	_RunningFlag = false;
	CUSBCtrl::Getinstance().Stop();
}


void CRsovCtrl::Thread_Parse_Arcnet(void *arg)
{
	if (nullptr == arg)
	{
		return;
	}
	std::pair<CRsovCtrl *, int> *thispair = (std::pair<CRsovCtrl *, int> *)arg;
	CRsovCtrl *pThisObject = thispair->first;
	int arc_id = thispair->second;
	LOG(INFO) << "RSOV ARCNET" << arc_id << " parse thread begin, thread id = " << syscall(SYS_gettid);

	unsigned int len = 0;
	unsigned char *data = nullptr;
	unsigned char buf[1024] = { 0 };

	while (pThisObject->_RunningFlag)
	{
		if (ARCNET1 == arc_id)
		{
			if (!pThisObject->_Handle._Que_Rx_Arcnet3.PopMesg(&data, len))
			{
				continue;
			}
		}
		else if (ARCNET2 == arc_id)
		{
			if (!pThisObject->_Handle._Que_Rx_Arcnet4.PopMesg(&data, len))
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

			/* 获取接收到的数据区长度和实际有效数据的长度 */
			unsigned short buflen = char_to_short(buf + 2);
			unsigned short datalen = char_to_short(buf + 4);

			if (datalen > 9 && datalen < 508)
			{
				/** @calculate CRC，compare with the CRC check value of last four bytes of data */
				unsigned int crc_rt = 0, crc_calc = 0;
				Crc_32(buf + 4, datalen - 2, 0, &crc_rt);
				crc_calc = char_to_long(buf + datalen + 2);

				if (crc_calc == crc_rt)	
				{
					/** @parse/handle RSOV receive data */
					pThisObject->_Handle.HandleFunc(arc_id, buf);
					pThisObject->_Handle._ParseCnt++;
				}
			}
		}

		usleep(1000);
	}

	LOG(INFO) << "RSOV ARCNET" << arc_id << " parse thread quit";
	return;
}

void CRsovCtrl::Thread_Rd_Arcnet(void *arg)
{
	if (nullptr == arg)
	{
		return;
	}
	CRsovCtrl *pThisObject = (CRsovCtrl *)arg;

	unsigned int len = 0;
	unsigned char *data = nullptr;

	unsigned int writelen = 0;
	unsigned char writebuf[2560] = { 0 };

	LOG(INFO) << "RSOV ARCNET record thread begin, thread id = " << syscall(SYS_gettid);

	while (pThisObject->_RunningFlag)
	{
		if (pThisObject->_Handle._Que_Record.PopMesg(&data, len))
		{
			if (nullptr != data && len > 0)
			{
				memcpy(writebuf + writelen, data, len);
				writelen += len;

				delete[] data;
				data = nullptr;

				if (writelen >= CAppCfg::GetInstance()._Attri.DataLenNum)
				{
					if (pThisObject->_Handle._FileCtrl->DataRecord((char *)writebuf, writelen))
					{
						writelen = 0;
						memset(writebuf, 0, sizeof(writebuf));
					}
				}
			}

			pThisObject->_Handle._RdCnt++;
		}

		usleep(1000);
	}

	LOG(INFO) << "RSOV ARCNET record thread quit";
	return;
}