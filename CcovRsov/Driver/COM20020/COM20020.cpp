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

	/** @ARCNET 驱动为 Linux 字符设备 */
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

	/*获取COM20020的版本号，暂时没有对版本不同做处理*/
	WRITE_20020(_Arcnet[id].baseaddr + SUB_ADDR, 0x0);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_ADDR);

	/* 0x80为REVD,0xC0为REVE  */
	WRITE_20020(_Arcnet[id].baseaddr + SUB_ADDR, 0xc0);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_ADDR);

	/*先将芯片复位，并且不允许发送(TXEN=0)
		1-产生软复位，允许
		1-指令链模式，允许
		0-发送使能，不允许发送
		11-收发周期设置，此处为：37.4us回应时间，41us最大线路闲置，420ms重构周期
		1-发送方式选择，1为漏极开路输出
		11-子地址选择，作用同子地址寄存器
	正常使用的时候是0x7F（发送使能）*/
	WRITE_20020(_Arcnet[id].baseaddr + CONFIG, 0xdf);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + CONFIG);

	/** @清空复位比特 */
	WRITE_20020(_Arcnet[id].baseaddr + CONFIG, 0x5f);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + CONFIG);

	/*写入控制字00H, 其每一位表示：
		1-设置P1 MODE，20020的PULSE1脚采用“1“为推挽输出(0=漏极开路输出)
		0-设置当重复发送128次均返回“拒绝请求”信号时认为发送超时，(“1“为4次)
		0-无效位
		0-设置20020是否进入监听模式，此处为禁止
		000-设置晶振脉冲分频数，此处为8分频（默认为2.5MBPS）
		0-是否采内存读写周期2分频降速模式，此处为禁止*/
	WRITE_20020(_Arcnet[id].baseaddr + SUB_ADDR, SETUP1);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_ADDR);

	WRITE_20020(_Arcnet[id].baseaddr + SUB_DATA, 0x80);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_DATA);

	/*	1-是否进入高速CPU总线模式，此处为是
		0-无效位
		00-晶振频率选择，此处为20MHz
		1-是否使用20020 D版本的增强功能，此处为打开
		1-非同步模式位，此处定义20020不用在网路空闲情况下才能进行内部RAM初始化写入
		11-计时器周期，定义重构超时时间为26.25ms，网络最大节点数16*/
	WRITE_20020(_Arcnet[id].baseaddr + SUB_ADDR, SETUP2);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_ADDR);

	WRITE_20020(_Arcnet[id].baseaddr + SUB_DATA, 0x8f + boudrate);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_DATA);

	/** @改变系统时钟频率ckup1,0后，重新启动20020核心 */
	usleep(100000);

	WRITE_20020(_Arcnet[id].baseaddr + COMMAND, 0x18);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + COMMAND);

	/** @清除POR and RECON */
	WRITE_20020(_Arcnet[id].baseaddr + COMMAND, CLEARFLAGS);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + COMMAND);
	/** @设置为NODE ID寄存器 */
	WRITE_20020(_Arcnet[id].baseaddr + SUB_ADDR, NODE_ID);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_ADDR);
	/** @设置本路节点地址 */
	WRITE_20020(_Arcnet[id].baseaddr + SUB_DATA, _Arcnet[id].nodeaddr);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + SUB_DATA);

	/** @复位20020 */
	temp1 = READ_20020(_Arcnet[id].baseaddr + CONFIG);
	WRITE_20020(_Arcnet[id].baseaddr + CONFIG, temp1 | RESET);
	WRITE_20020(_Arcnet[id].baseaddr + CONFIG, temp1 & ~RESET);
	drvnouse = READ_20020(_Arcnet[id].baseaddr + CONFIG);

	/** @复位后0地址必须为0xd1，下一个数据是本机的节点地址 */
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
		tempbuf[2] = READ_20020(_Arcnet[id].baseaddr + DATA);     /** <帧长度 */
		tempbuf[3] = READ_20020(_Arcnet[id].baseaddr + DATA);     /** <帧长度 */

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
		/* 网络重构，删除过去可能存在的发送指令 */
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, CLEARRECON);
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, DISABLETX);
		//LOG(WARN) << "ARCNET" << id << " reconnect and delete the last sending command";
	}

	if ((ArcDiag_int & EXC_NAK) == EXC_NAK)
	{
		/* 重发处理 */
		WRITE_20020(_Arcnet[id].baseaddr + COMMAND, CLEARPOR);  /* clear flags POR AND RECON */
		_Arcnet[id].totalresend++;
		_Arcnet[id].curresend++;

		if (_Arcnet[id].curresend >= ARCNET_RESEND_MAXCNT)
		{
			/*重发次数到，删除上次的发送，此后TA将置1，进入发送中断进行屏蔽中断或清除中断的处理*/
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
		/*添写的数据长度为：有效数据长度len+1字节报文类型+4字节序列号+4字节CRC*/
		short_to_char(len + 9, buf);

		/*添加数据类型*/
		buf[2] = type;

		/*添加字节序号*/
		long_to_char(sn, buf + 3);

		/*添加发送数据*/
		memcpy(buf + 7, data, len);

		/*计算数据CRC，长度开始一直到应用数据结束：有效数据长度len+2+1+4字节*/
		Crc_32(buf, len + 7, 0, &crc_rt);

		/*添加Crc结果*/
		long_to_char(crc_rt, buf + len + 7);

		//if ((length == 254) || (length == 255) || (length == 256))
		//{
		//	/*长度为限制字节时，统一将长度改为257字节，在发送时会自动将数据后面的无效字节发送出去而不需改动原数据*/
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
		/*添写的数据长度为：有效数据长度len+1字节报文类型+4字节序列号+4字节CRC+1字节设备类型+1字节设备ID+2字节逻辑ID*/
		short_to_char(len + 13, buf);

		/*添加数据类型*/
		buf[2] = type;

		/*添加字节序号*/
		long_to_char(sn, buf + 3);

		/*设备类型*/
		buf[7] = device_type;

		/*设备ID*/
		buf[8] = device_id;

		/*逻辑ID*/
		short_to_char(log_id, buf + 9);

		/*添加发送数据*/
		memcpy(buf + 11, data, len);

		/*计算数据CRC，长度开始一直到应用数据结束：有效数据长度len+2+1+4字节*/
		Crc_32(buf, len + 11, 0, &crc_rt);

		/*添加Crc结果*/
		long_to_char(crc_rt, buf + len + 11);

		return ArcSend(id, destaddr, (char *)buf, length);
	}
	return false;
}