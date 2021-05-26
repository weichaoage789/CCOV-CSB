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
	/* ��ʼ�� */
	void Init();

	/* DSU��ʼ�� */
	void Init_Dsu();

	/* ��ñ���CCOV�������� */
	unsigned char GetNetType();

	/* ���������յ��İ�ʵ�ʰ������������ݰ����� */
	int CalcPacketCnt(const unsigned char *data, unsigned short len);

	/* �����豸���ƺ�Э�����ͣ����Ҷ�Ӧ�豸��Э��ĺ���IP��ַ�Ͷ˿ںţ�����Ŀ��ϵ���� */
	int FindIpAndPort(unsigned short devName, unsigned char type, unsigned char ip[][4], unsigned short port[]);

	/* ���������б����ĺ�����IP����ѯĿ������IP�Ͷ˿ڣ�����Ŀ��ϵ���� */
	int FindDestNetAddr(const unsigned char *data, unsigned char ip[][4], unsigned short port[]);

	/* ���������е��豸���ƺ��߼�ID����ѯĿ�����ETCSID */
	unsigned int FindDestEtcsId(const unsigned char *data);

	/* �������ݷ��ͺ��� */
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

	/** @ ����UDPͨ������������ */
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


	/** @ ��Ŷ��ձ�ṹ��1	*/
	DSU_IDMAP_STRU dsuIdmap1;

	/** @ ��Ŷ��ձ�ṹ��2	*/
	DSU_IDMAP_STRU dsuIdmap2;

	/** @ ATP����CCOV����Ϣ */
	ATP_TO_CCOV_STRU _MiAtp;

	/** @ CCOV����ATP����Ϣ */
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