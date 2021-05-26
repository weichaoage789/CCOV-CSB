#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>

#include <thread>
using namespace std;

#include "Utilities/CRC.h"
#include "Utilities/Convert/ConvertWork.h"
using namespace Convert;

#include "Utilities/FileWork/LogFileWork.h"
using namespace FileWork;

#include "COM20020Define.h"
#include "COM20020.h"


CArcnet::CArcnet(CMesgQue *arc1, CMesgQue *arc2)
{
	_Rxcnt[ARCNET1] = 0;
	_Rxcnt[ARCNET2] = 0;
	_Arcnet[ARCNET1].rx = arc1;
	_Arcnet[ARCNET2].rx = arc2;

	for (int i = ARCNET1; i <= ARCNET2; i++)
	{
		_Arcnet[i].baseaddr = 0;
		_Arcnet[i].nodeaddr = 0;
		_Arcnet[i].totalresend = 0;
		_Arcnet[i].totaldiscard = 0;
		_Arcnet[i].totalreconn = 0;
		_Arcnet[i].curresend = 0;
		_Arcnet[i].pagein = 0;
		_Arcnet[i].pageout = 2;
	}
	ARCNET_Init();
}

CArcnet::~CArcnet()
{
	_RunningFlag = false;
	close(_Fd);
}


void CArcnet::ARCNET_Init()
{
	LOG(INFO) << "ARCNET initialization start";

	/** @ARCNET ����Ϊ Linux �ַ��豸 */
	_Fd = open("/dev/arcnet", O_RDWR);
	if (_Fd < 0)
	{
		LOG(ERROR) << "ARCENT open error, open code = " << _Fd;
		exit(EXIT_FAILURE);
	}
	usleep(100000);

	/** @get ARCNET base address */
	struct cs_base_addr cs_addr;
	ioctl(_Fd, GET_CS0_ADDR, &cs_addr);
	ioctl(_Fd, GET_CS2_ADDR, &cs_addr);
	_Arcnet[ARCNET1].baseaddr = cs_addr.cs0_addr;
	_Arcnet[ARCNET2].baseaddr = cs_addr.cs2_addr;

	/** @ARCNET driver channel node initialization */
	int cnt = 0;
	while (cnt < 5)
	{
		if (!ARCNET_NodeInit(ARCNET1, DCS_LOGGER_ARCNET_NODE_ADDR, ARCNET_BAUDRATE_5000K)
			|| !ARCNET_NodeInit(ARCNET2, DCS_LOGGER_ARCNET_NODE_ADDR, ARCNET_BAUDRATE_5000K))
		{
			cnt++;
			sleep(1);
			continue;
		}
		else
		{
			LOG(INFO) << "ARCNET initialize successfully, FD = " << _Fd << ". ARCENT1 address = " << cs_addr.cs0_addr << "; ARCNET2 address = " << cs_addr.cs2_addr;
			_RunningFlag = true;

			for (int i = ARCNET1; i <= ARCNET2; i++)
			{
				/** @create ARCNET IRQ_CS thread */
				std::pair<CArcnet *, int > *info = new std::pair<CArcnet *, int>(this, i);
				std::thread irq_cs(Thread_Arcnet_IRQ_CS, info);
				irq_cs.detach();
			}

			return;
		}
	}

	LOG(ERROR) << "ARCNET initialization error after trying 5 times.";
	exit(EXIT_FAILURE);
}

void CArcnet::Thread_Arcnet_IRQ_CS(void *arg)
{
	if (!arg)
	{
		return;
	}

	std::pair<CArcnet *, int > *info = (std::pair<CArcnet *, int> *)arg;
	CArcnet *pThisObject = info->first;
	int arcid = info->second;
	delete info;
	info = nullptr;

	LOG(INFO) << "ARCENT" << arcid << " IRQ_CS thread start, thread id = " << syscall(SYS_gettid);

	while (pThisObject->_RunningFlag)
	{
		if (ARCNET1 == arcid)
		{
			ioctl(pThisObject->_Fd, ARCNET_WAITE_IRQ_CS0, NULL);
			pThisObject->ARCNET_IntHandler(ARCNET1);
		}
		else if (ARCNET2 == arcid)
		{
			ioctl(pThisObject->_Fd, ARCNET_WAITE_IRQ_CS2, NULL);
			pThisObject->ARCNET_IntHandler(ARCNET2);
		}
		//LOG(INFO) << "ARCNET IOCTL execute successfully, ID = " << arcid;
	}

	LOG(INFO) << "ARCENT" << arcid << " IRQ_CS thread quit";
	return;
}


char CArcnet::READ_20020(unsigned int addr)
{
	struct arcnet_info read_d = { 0 };

	read_d.addr = addr;
	ioctl(_Fd, ARCNET_READ_DATA, &read_d);

	return read_d.data;
}

void CArcnet::WRITE_20020(unsigned int addr, char data)
{
	struct arcnet_info write_d = { 0 };

	write_d.addr = addr;
	write_d.data = data;

	ioctl(_Fd, ARCNET_WRITE_DATA, &write_d);
}


bool CArcnet::ARCNET_NodeInit(int id, unsigned int nodeaddr, unsigned int boudrate)
{
	unsigned int drvnouse;
	unsigned char temp1, temp2;

	/** @set pagein and pageout */
	_Arcnet[id].pagein = 0;
	_Arcnet[id].pageout = 2;
	_Arcnet[id].nodeaddr = nodeaddr;

	/*��ȡCOM20020�İ汾�ţ���ʱû�ж԰汾��ͬ������*/
	WRITE_20020(_Arcnet[id].baseaddr + SUB_ADDR, 0x0);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_ADDR);

	/* 0x80ΪREVD,0xC0ΪREVE  */
	WRITE_20020(_Arcnet[id].baseaddr + SUB_ADDR, 0xc0);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_ADDR);

	/*�Ƚ�оƬ��λ�����Ҳ�������(TXEN=0)
		1-������λ������
		1-ָ����ģʽ������
		0-����ʹ�ܣ���������
		11-�շ��������ã��˴�Ϊ��37.4us��Ӧʱ�䣬41us�����·���ã�420ms�ع�����
		1-���ͷ�ʽѡ��1Ϊ©����·���
		11-�ӵ�ַѡ������ͬ�ӵ�ַ�Ĵ���
	����ʹ�õ�ʱ����0x7F������ʹ�ܣ�*/
	WRITE_20020(_Arcnet[id].baseaddr + CONFIG, 0xdf);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + CONFIG);

	/** @��ո�λ���� */
	WRITE_20020(_Arcnet[id].baseaddr + CONFIG, 0x5f);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + CONFIG);

	/*д�������00H, ��ÿһλ��ʾ��
		1-����P1 MODE��20020��PULSE1�Ų��á�1��Ϊ�������(0=©����·���)
		0-���õ��ظ�����128�ξ����ء��ܾ������ź�ʱ��Ϊ���ͳ�ʱ��(��1��Ϊ4��)
		0-��Чλ
		0-����20020�Ƿ�������ģʽ���˴�Ϊ��ֹ
		000-���þ��������Ƶ�����˴�Ϊ8��Ƶ��Ĭ��Ϊ2.5MBPS��
		0-�Ƿ���ڴ��д����2��Ƶ����ģʽ���˴�Ϊ��ֹ*/
	WRITE_20020(_Arcnet[id].baseaddr + SUB_ADDR, SETUP1);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_ADDR);

	WRITE_20020(_Arcnet[id].baseaddr + SUB_DATA, 0x80);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_DATA);

	/*	1-�Ƿ�������CPU����ģʽ���˴�Ϊ��
		0-��Чλ
		00-����Ƶ��ѡ�񣬴˴�Ϊ20MHz
		1-�Ƿ�ʹ��20020 D�汾����ǿ���ܣ��˴�Ϊ��
		1-��ͬ��ģʽλ���˴�����20020��������·��������²��ܽ����ڲ�RAM��ʼ��д��
		11-��ʱ�����ڣ������ع���ʱʱ��Ϊ26.25ms���������ڵ���16*/
	WRITE_20020(_Arcnet[id].baseaddr + SUB_ADDR, SETUP2);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_ADDR);

	WRITE_20020(_Arcnet[id].baseaddr + SUB_DATA, 0x8f + boudrate);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_DATA);

	/** @�ı�ϵͳʱ��Ƶ��ckup1,0����������20020���� */
	usleep(100000);

	WRITE_20020(_Arcnet[id].baseaddr + COMMAND, 0x18);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + COMMAND);

	/** @���POR and RECON */
	WRITE_20020(_Arcnet[id].baseaddr + COMMAND, CLEARFLAGS);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + COMMAND);
	/** @����ΪNODE ID�Ĵ��� */
	WRITE_20020(_Arcnet[id].baseaddr + SUB_ADDR, NODE_ID);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_ADDR);
	/** @���ñ�·�ڵ��ַ */
	WRITE_20020(_Arcnet[id].baseaddr + SUB_DATA, _Arcnet[id].nodeaddr);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_DATA);

	/** @��λ20020 */
	temp1 = READ_20020(_Arcnet[id].baseaddr + CONFIG);
	WRITE_20020(_Arcnet[id].baseaddr + CONFIG, temp1 | RESET);
	WRITE_20020(_Arcnet[id].baseaddr + CONFIG, temp1 & ~RESET);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + CONFIG);

	/** @��λ��0��ַ����Ϊ0xd1����һ�������Ǳ����Ľڵ��ַ */
	WRITE_20020(_Arcnet[id].baseaddr + ADDR_H_PT, 0xc0);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + ADDR_H_PT);

	WRITE_20020(_Arcnet[id].baseaddr + ADDR_L_PT, 0x0);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + ADDR_L_PT);

	/** @read memory = 0xd1 */
	temp1 = READ_20020(_Arcnet[id].baseaddr + DATA);
	/** @read next memory  ==NODE ID */
	temp2 = READ_20020(_Arcnet[id].baseaddr + DATA);

	if ((temp1 == 0xD1) && (temp2 == _Arcnet[id].nodeaddr))
	{
		/** @mask interrupts */
		WRITE_20020(_Arcnet[id].baseaddr + IMR, 0x0);

		/** @disable tx, rx */
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, DISABLETX);
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, DISABLERX);

		/** @clear the POR and RECON status bits */
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, CLEARFLAGS);

		/** @short or long packets */
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, LONGPACKET);     /*LONGPACKET;*/

	   /** @enable recv, recon, exenak transmit interrupt  = 0x8d */
		WRITE_20020(_Arcnet[id].baseaddr + IMR, 0x8d);

		/** @configuration - network timeout slowest, enable tx */
		/** @output the configuration - join the network */
		WRITE_20020(_Arcnet[id].baseaddr + CONFIG, 0x7F); /* = US78 | TXEN | CCHEN;*/

		/** @enable two pages for data receive */
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, ENABLE_PAGE0_RECV);
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, ENABLE_PAGE1_RECV);

		/** @send a broadcast for enter interrupt */
		char broadcastbuf[12];
		memset(broadcastbuf, 0x55, sizeof(broadcastbuf));
		ArcSend(id, 0, broadcastbuf, sizeof(broadcastbuf));

		LOG(INFO) << "ARCNET" << id << " initialize success.";
		return true;
	}

	LOG(ERROR) << "ARCNET" << id << " initialize failed.";
	return false;
}

void CArcnet::ARCNET_IntHandler(int id)
{
	char ArcDiag_int, ArcStatus_int;
	unsigned char page, tempbuf[512];
	unsigned int 	bytes, offset, temp, temp1;

	ArcStatus_int = READ_20020(_Arcnet[id].baseaddr + STATUS_REG);
	ArcDiag_int = READ_20020(_Arcnet[id].baseaddr + DIAG);

	/** @interrupt processing of receive */
	if ((ArcStatus_int & TRI) == TRI)
	{
		/** @clear the current receive interrupt */
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, CLR_RI);

		/** @get current receive page*/
		page = _Arcnet[id].pagein;
		/** @set page for next receive */
		_Arcnet[id].pagein += 1;
		_Arcnet[id].pagein &= 1;

		if (page == 0)
		{
			WRITE_20020(_Arcnet[id].baseaddr + ADDR_H_PT, 0xc0);
		}
		else
		{
			WRITE_20020(_Arcnet[id].baseaddr + ADDR_H_PT, 0xc2);
		}
		WRITE_20020(_Arcnet[id].baseaddr + ADDR_L_PT, 0x0);

		memset(tempbuf, 0, sizeof(tempbuf));
		tempbuf[0] = READ_20020(_Arcnet[id].baseaddr + DATA);     /** <SID*/
		tempbuf[1] = READ_20020(_Arcnet[id].baseaddr + DATA);     /** <DID*/
		tempbuf[2] = READ_20020(_Arcnet[id].baseaddr + DATA);     /** <֡���� */
		tempbuf[3] = READ_20020(_Arcnet[id].baseaddr + DATA);     /** <֡���� */

		/** @get the actual data length */
		if (tempbuf[2] == 0)
		{
			/** @LONG Packet format */
			offset = tempbuf[3];
			bytes = 512 - tempbuf[3];
			tempbuf[2] = 1;
			tempbuf[3] = bytes - 256;
		}
		else
		{
			/** @SHORT Packet format */
			offset = tempbuf[2];
			bytes = 256 - tempbuf[2];
			tempbuf[2] = 0;
			tempbuf[3] = bytes;
		}

		/** @read data */
		if (page == 0)
		{
			WRITE_20020(_Arcnet[id].baseaddr + ADDR_H_PT, 0xc0);
		}
		else
		{
			WRITE_20020(_Arcnet[id].baseaddr + ADDR_H_PT, 0xc2);
		}
		WRITE_20020(_Arcnet[id].baseaddr + ADDR_L_PT, offset);

		for (int i = 0; i < bytes; i++)
		{
			tempbuf[4 + i] = READ_20020(_Arcnet[id].baseaddr + DATA);
		}

		/** @update the receiving page for next receiving*/
		if (page == 0)
		{
			WRITE_20020(_Arcnet[id].baseaddr + COMMAND, ENABLE_PAGE0_RECV);
		}
		else
		{
			WRITE_20020(_Arcnet[id].baseaddr + COMMAND, ENABLE_PAGE1_RECV);
		}

		/** (0 == tempbuf[1]) represents for broadcast message. */
		if (tempbuf[1] != 0)
		{
			_Rxcnt[id]++;
			unsigned char *data = new unsigned char[bytes + 4];
			if (nullptr != data && nullptr != _Arcnet[id].rx)
			{
				memset(data, 0, sizeof(data));
				memcpy(data, tempbuf, bytes + 4);
				_Arcnet[id].rx->PushMesg(data, bytes + 4);
			}
		}
	}

	if (((ArcStatus_int & RECON) == RECON) || ((ArcDiag_int & MYRECON) == MYRECON))
	{
		_Arcnet[id].totalreconn++;
		/* �����ع���ɾ����ȥ���ܴ��ڵķ���ָ�� */
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, CLEARRECON);
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, DISABLETX);
		//LOG(WARN) << "ARCNET" << id << " reconnect and delete the last sending command";
	}

	if ((ArcDiag_int & EXC_NAK) == EXC_NAK)
	{
		/* �ط����� */
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, CLEARPOR);  /* clear flags POR AND RECON */
		_Arcnet[id].totalresend++;
		_Arcnet[id].curresend++;

		if (_Arcnet[id].curresend >= ARCNET_RESEND_MAXCNT)
		{
			/*�ط���������ɾ���ϴεķ��ͣ��˺�TA����1�����뷢���жϽ��������жϻ�����жϵĴ���*/
			_Arcnet[id].curresend = 0;
			_Arcnet[id].totaldiscard++;
			WRITE_20020(_Arcnet[id].baseaddr + COMMAND, DISABLETX);
		}
		//LOG(WARN) << "ARCNET resend";
	}

	/** @enable all interrupts except transmit*/
	WRITE_20020(_Arcnet[id].baseaddr + IMR, 0x8c);

	return;
}

bool CArcnet::ArcSend(const int id, const char destaddr, const char *data, const int len)
{
	if (nullptr != data && len <= 508)
	{
		char status = 0;
		unsigned int temp, temp1, page;

		/** @set the address pointers, auto increment on write */
		page = _Arcnet[id].pageout;
		temp = page * 512;
		temp1 = ((temp >> 8) | AUTOINC) & WRDATA;

		WRITE_20020(_Arcnet[id].baseaddr + ADDR_H_PT, temp1);
		WRITE_20020(_Arcnet[id].baseaddr + ADDR_L_PT, temp);

		/** @write SID */
		WRITE_20020(_Arcnet[id].baseaddr + DATA, _Arcnet[id].nodeaddr);

		/** @write DID. 0 for broadcast */
		WRITE_20020(_Arcnet[id].baseaddr + DATA, destaddr);

		if (len > 253)
		{
			/** @long packet format, maximum 508 bytes */
			temp = 512 - len;
			WRITE_20020(_Arcnet[id].baseaddr + DATA, 0);
			WRITE_20020(_Arcnet[id].baseaddr + DATA, temp);
		}
		else
		{
			/** @short packet format, maximum 253 bytes */
			temp = 256 - len;
			WRITE_20020(_Arcnet[id].baseaddr + DATA, temp);
		}

		/** @set the address pointers, auto increment on write */
		temp = (page * 512) + temp;
		temp1 = ((temp >> 8) | AUTOINC) & WRDATA;

		WRITE_20020(_Arcnet[id].baseaddr + ADDR_H_PT, temp1);
		WRITE_20020(_Arcnet[id].baseaddr + ADDR_L_PT, temp);

		/** @loading data */
		for (int i = 0; i < len; i++)
		{
			WRITE_20020(_Arcnet[id].baseaddr + DATA, data[i]);
		}

		/** @enable tx */
		page <<= 3;
		page |= 3;
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, page);

		_Arcnet[id].pageout = ((++_Arcnet[id].pageout) > 3) ? 2 : _Arcnet[id].pageout;
		return true;
	}

	LOG(ERROR) << "ARCNET" << id << " send data error";
	return false;
}

unsigned int CArcnet::GetRxcnt(const int id)
{
	return _Rxcnt[id];
}

bool CArcnet::OutDataSend(const int id, const char destaddr, const char *data, const int len, const unsigned char type, const unsigned int sn)
{
	unsigned int crc_rt = 0;
	unsigned char buf[600];
	memset(buf, 0, sizeof(buf));

	unsigned short length = len + 11;
	if (length < 508)
	{
		/*��д�����ݳ���Ϊ����Ч���ݳ���len+1�ֽڱ�������+4�ֽ����к�+4�ֽ�CRC*/
		short_to_char(len + 9, buf);

		/*�����������*/
		buf[2] = type;

		/*����ֽ����*/
		long_to_char(sn, buf + 3);

		/*��ӷ�������*/
		memcpy(buf + 7, data, len);

		/*��������CRC�����ȿ�ʼһֱ��Ӧ�����ݽ�������Ч���ݳ���len+2+1+4�ֽ�*/
		Crc_32(buf, len + 7, 0, &crc_rt);

		/*���Crc���*/
		long_to_char(crc_rt, buf + len + 7);

		//if ((length == 254) || (length == 255) || (length == 256))
		//{
		//	/*����Ϊ�����ֽ�ʱ��ͳһ�����ȸ�Ϊ257�ֽڣ��ڷ���ʱ���Զ������ݺ������Ч�ֽڷ��ͳ�ȥ������Ķ�ԭ����*/
		//	length = 257;
		//}

		return ArcSend(id, destaddr, (char *)buf, length);
	}
	return false;
}

bool CArcnet::OutDataSend_Prtcl(const int id, const char destaddr, const char *data, const int len, const unsigned char type, const unsigned int sn
	, const unsigned char device_type, const unsigned char device_id, const unsigned short log_id)
{
	unsigned int crc_rt = 0;
	unsigned char buf[512];
	memset(buf, 0, sizeof(buf));

	unsigned short length = len + 15;
	if (length < 508)
	{
		/*��д�����ݳ���Ϊ����Ч���ݳ���len+1�ֽڱ�������+4�ֽ����к�+4�ֽ�CRC+1�ֽ��豸����+1�ֽ��豸ID+2�ֽ��߼�ID*/
		short_to_char(len + 13, buf);

		/*�����������*/
		buf[2] = type;

		/*����ֽ����*/
		long_to_char(sn, buf + 3);

		/*�豸����*/
		buf[7] = device_type;

		/*�豸ID*/
		buf[8] = device_id;

		/*�߼�ID*/
		short_to_char(log_id, buf + 9);

		/*��ӷ�������*/
		memcpy(buf + 11, data, len);

		/*��������CRC�����ȿ�ʼһֱ��Ӧ�����ݽ�������Ч���ݳ���len+2+1+4�ֽ�*/
		Crc_32(buf, len + 11, 0, &crc_rt);

		/*���Crc���*/
		long_to_char(crc_rt, buf + len + 11);

		return ArcSend(id, destaddr, (char *)buf, length);
	}
	return false;
}