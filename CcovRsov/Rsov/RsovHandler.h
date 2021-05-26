/**
  ******************************************************************************
  * @copyright	:
  * @file		        : RsovHandler.h
  * @brief	        :  
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _RSOV_MESSAGE_HANDLE_H
#define _RSOV_MESSAGE_HANDLE_H

#include "RsovFileCtrl.h"
#include "Utilities/Queue.h"
#include "Utilities/TimerCtrl.h"
#include "Utilities/NetWork/SerialCom.h"
#include "Utilities/NetWork/NetWork.h"
using namespace NetWork;

#include "RsovDefine.h"
#include "RsovConfig.h"


class CRsovHandler
{
public:
	CRsovHandler();
	~CRsovHandler();

	friend class CRsovCtrl;
public:
	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  data handle function.
	  * @param	 :  [in] int arcid, ARCNET id number.
	  * @param	 :  [in] unsigned char *data, input source data.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void HandleFunc(int arcid, unsigned char *data);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  redundant processing initialization.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void InitRedund();

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  data preprocessing, combine the data and save it to the record queue.
	  * @param	 :  [in] char *typebuf, input typebuf.
	  * @param	 :  [in] unsigned char *data, input source data tobe recorded.
	  * @param	 :  [in] unsigned int len, input source data length.
	  * @return	 :  [type] 
	  *-----------------------------------------------------------------------------
	  */
	void DataRecordPrepare(char *typebuf, unsigned char *data, unsigned int len);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  redundant processing of data to be saved.
	  * @param	 :  [in] int arcid, ARCNET id number.
	  * @param	 :  [in] unsigned char *redunbuf, input source redundant processing data.
	  * @return	 :  [type] 
	  *-----------------------------------------------------------------------------
	  */
	void DataRedundProc(int arcid, unsigned char *redunbuf);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  FFFE data send.
	  * @param	 :  [in] unsigned char *data, data to be send.
	  * @param	 :  [in] unsigned int len, data length.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	bool FFFEsend(unsigned char *data, unsigned int len);

private:
	/** @timer count */
	unsigned int _TIMER_;

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  timer	 function.
	  * @param	 :  [in] void *arg, input param.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void OnTimer(void *arg);

private:
	/** @file operator */
	CRsovFileCtrl *_FileCtrl;

	/** @ARCNET operator */
	CArcnet *_ArcnetCtrl;

	/** @COM operator */
	CCOMCtrl _ComCtrl;

	/** @ARCNET receive message queue3 */
	CMesgQue _Que_Rx_Arcnet3;

	/** @ARCNET receive message queue4 */
	CMesgQue _Que_Rx_Arcnet4;

	/** @data record queue */
	CMesgQue _Que_Record;

	/** @parse total count */
	unsigned int _ParseCnt;

	/** @record total count */
	unsigned int _RdCnt;

	/** @To MMI version data */
	unsigned char _Verbuf[128];

	/** @type and serial number array */
	TYPE_SN_CFG _TypeSn[TYPE_SN_NUM];

	/** @ _Rsov network operator */
	CUDP _Ccov;
};


#endif  /*_RSOV_MESSAGE_HANDLE_H*/