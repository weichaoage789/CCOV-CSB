/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  CcovHandler.h
  * @brief	        :  
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _CCOV_MESSAGE_HANDLE_H
#define _CCOV_MESSAGE_HANDLE_H

#include <map>

#include "CcovDefine.h"
#include "CcovConfig.h"

#include "dsuCcovFunc.h"
#include "WGB/WgbPlugin.h"

#include "Utilities/TimerCtrl.h"
#include "Utilities/NetWork/NetWork.h"
#include "Protocol/RSSP2/RSSP2.h"
#include "Driver/COM20020/COM20020.h"

using namespace NetWork;


class CCcovHandler
{
public:
	CCcovHandler();
	~CCcovHandler();

	/** @friend class of CCcovCtrl */
	friend class CCcovCtrl;

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  data handle function.
	  * @param	 :  [in] int arcid, ARCNET id number.
	  * @param	 :  [in] unsigned char *data, input source data.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void HandleFunc(int arc_id, unsigned char *data);


protected:
	/* 初始化 */
	void Init();

	/* DSU初始化 */
	void Init_Dsu();

	/* 获得本机CCOV网络类型 */
	unsigned char GetNetType();

	/* 计算内网收到的包实际包含的外网数据包个数 */
	int CalcPacketCnt(const unsigned char *data, unsigned short len);

	/* 根据设备名称和协议类型，查找对应设备和协议的红网IP地址和端口号，返回目标系个数 */
	int FindIpAndPort(unsigned short devName, unsigned char type, unsigned char ip[][4], unsigned short port[]);

	/* 根据数据中本机的红蓝网IP，查询目标对象的IP和端口，返回目标系个数 */
	int FindDestNetAddr(const unsigned char *data, unsigned char ip[][4], unsigned short port[]);

	/* 根据数据中的设备名称和逻辑ID，查询目标对象ETCSID */
	unsigned int FindDestEtcsId(const unsigned char *data);

	/* 外网数据发送函数 */
	void UdpDataSend(int port, unsigned char *data, const unsigned int len, const unsigned char type, int arcId);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  check WGB status, if not associated, notify WGB to restart.
	  * @param	 :  [in] const char *ip, IP address.
	  * @param	 :  [in] const unsigned short port, port number.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	bool WgbRebootCtrl(const char *ip, const unsigned short port);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  status report to ATP.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void StatusReportToATP();

	void StatusToMMI();
protected:
	void Decode_AtpMesg(int arcid, unsigned char *data, unsigned short len);

	void Encode_TcpStateMsg(unsigned char *data, unsigned short &len);

	void Decode_TcpCtrlMsg(int arc_id, unsigned short dev_name, unsigned char *data, unsigned short len);

	void RSSP2_DataSend(int arc_id, unsigned char *data, unsigned short len);

private:
	/** @LED on-off status */
	static unsigned int _TIMER_;

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  timer function.
	  * @param	 :  [in] void *arg, input param.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void OnTimer(void *arg);

private:
	/** @ CCOV information */
	CCOV_ATTRI *_Attri;

	/** @ WGB network operator */
	CUDP _Wgb;

	/** @ _Rsov network operator */
	CUDP _Rsov;

	/** @ 外网UDP通信描述符集合 */
	std::map<int, std::pair<CUDP *, std::queue<int>>> _Outer;

	/** @ ARCNET operator */
	CArcnet *_ArcnetCtrl;

	/** @ ALE operator */
	CAleCtrl _AleCtrl;

	/** @LTE/WGB operator */
	CWgbCtrl _WgbCtrl;

	/** @LTE/WGB IP address */
	unsigned char _WgbIp[16];

	/** @LTE/WGB associate status */
	unsigned char _WgbAsoctStatus;

	/** @ ARCNET receive message queue1 */
	CMesgQue _Que_Rx_Arcnet1;

	/** @ARCNET receive message queue2 */
	CMesgQue _Que_Rx_Arcnet2;

	/** @status of outer/inner net report message queue */
	//CMesgQue _Que_Rpt_Outer;
	//CMesgQue _Que_Rpt_Inner;


	/** @ 编号对照表结构体1	*/
	DSU_IDMAP_STRU dsuIdmap1;

	/** @ 编号对照表结构体2	*/
	DSU_IDMAP_STRU dsuIdmap2;

	/** @ ATP发给CCOV的消息 */
	ATP_TO_CCOV_STRU _MiAtp;

	/** @ CCOV发给ATP的消息 */
	CCOV_TO_ATP_STRU _MoAtp;

	/** @ DSU IP info */
	DSU_IP_INFO_STRU _LteIp;

	/** @ */
	unsigned int _ReportToAtpIndex1;
	unsigned int _ReportToAtpIndex2;

	/** @To MMI version data SN */
	unsigned int _VerSn;
};


#endif  /*_CCOV_MESSAGE_HANDLE_H*/