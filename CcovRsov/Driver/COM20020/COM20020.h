/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  COM20020.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _COM_20020_H
#define _COM_20020_H

#include "Utilities/Queue.h"

#define		ARCNET1		0
#define 		ARCNET2		1

/** @通信控制器和记录系统两路ARCNET节点地址 */
#define 		DCS_LOGGER_ARCNET_NODE_ADDR			'R'   

/** @通信控制器或记录系统向通信板发送数据时的目标地址 */
#define 		COMAB26_ARCNET_NODE_ADDR				0xFF 

/** @波特率为5M */
#define		ARCNET_BAUDRATE_5000K			0x10

/** @ARCNET resend max count */
#define		ARCNET_RESEND_MAXCNT			0x02


struct arcnet_info
{
	unsigned int addr;
	char data;
};

struct cs_base_addr {
	unsigned int cs0_addr;
	unsigned int cs2_addr;
};


/** <ARCNET information */
typedef struct
{
	/** @ARCNET address */
	unsigned int baseaddr;

	/** @ARCNET node address */
	unsigned int nodeaddr;

	/** @resend count of current frame */
	unsigned int curresend;

	/** @resend total count */
	unsigned short  totalresend;

	/** @discard total count */
	unsigned short totaldiscard;

	/** @reconnect total count */
	unsigned short totalreconn;

	/** @reconnect total count caused by application */
	unsigned int totalmyreconn;

	/** @ARCNET receive page */
	unsigned int pagein;

	/** @ARCNET send page */
	unsigned int pageout;

	/** @receive queue */
	CMesgQue *rx;
} ARCNET_INFO;

class CArcnet
{
public:
	CArcnet(CMesgQue *arc1, CMesgQue *arc2);
	~CArcnet();
	
	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  ARCNET data send function.
	  * @param	 :  [in] const int id, ARCNET id number.
	  * @param	 :  [in] const char destaddr, remote dest address.
	  * @param	 :  [in] const char *data, source data to be send.
	  * @param	 :  [in] const int len, source data length.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool ArcSend(const int id, const char destaddr, const char *data, const int len);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  ARCNET receive total count.
	  * @param	 :  [in] const int id, ARCNET id number.
	  * @return	 :  [type] unsigned int, total receive count.
	  *-----------------------------------------------------------------------------
	  */
	unsigned int GetRxcnt(const int id);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  ARCNET data extra-net sending function.
	  * @param	 :  [in] const int id, ARCNET id number.
	  * @param	 :  [in] const char, remote dest address.
	  * @param	 :  [in] const char *data, source data to be send.
	  * @param	 :  [in] const int len, source data length.
	  * @param	 :  [in] const unsigned char type, data type.
	  * @param	 :  [in] const unsigned int sn, send serial number.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool OutDataSend(const int id, const char destaddr, const char *data, const int len, const unsigned char type, const unsigned int sn);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  ARCNET data extranet sending function.
	  * @param	 :  [in] const int id, ARCNET id number.
	  * @param	 :  [in] const char, remote dest address.
	  * @param	 :  [in] const char *data, source data to be send.
	  * @param	 :  [in] const int len, source data length.
	  * @param	 :  [in] const unsigned char type, data type.
	  * @param	 :  [in] const unsigned int sn, send serial number.
	  * @param	 :  [in] const unsigned char device_type, device type.
	  * @param	 :  [in] const unsigned char device_id, device id number.
	  * @param	 :  [in] const unsigned short log_id, logic id number.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool OutDataSend_Prtcl(const int id, const char destaddr, const char *data, const int len, const unsigned char type, const unsigned int sn
		, const unsigned char device_type, const unsigned char device_id, const unsigned short log_id);

	/** @ARCNET sub node info */
	ARCNET_INFO _Arcnet[2];
protected:
	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  ARCNET initialize.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void ARCNET_Init();

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  ARCNET node initialize.
	  * @param	 :  [in] int id, ARCNET channel ID.
	  * @param	 :  [in] unsigned int nodeaddr, node address of ARCNET channel ID.
	  * @param	 :  [in] unsigned int boudrate, ARCNET boudrate.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	bool ARCNET_NodeInit(int id, unsigned int nodeaddr, unsigned int boudrate);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  ARCNET interrupt handler function.
	  * @param	 :  [in] int id, ARCNET channel ID.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void ARCNET_IntHandler(int id);

private:

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  ARCNET COM20020 read function.
	  * @param	 :  [in] unsigned int addr, input register address to be read.
	  * @return	 :  [type] char, data read.
	  *-----------------------------------------------------------------------------
	  */
	char READ_20020(unsigned int addr);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  ARCNET COM20020 write function.
	  * @param	 :  [in] unsigned int addr, input register address to be write.
	  * @param	 :  [in] char data, input data to be write.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void WRITE_20020(unsigned int addr, char data);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  ARCNET IRQ_CS thread function.
	  * @param	 :  [in] void *arg, input param.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	static void Thread_Arcnet_IRQ_CS(void *arg);

private:
	/** @ARCNET operator */
	int _Fd;

	/** @running flag */
	bool _RunningFlag;

	/** @ARCNET receive count */
	unsigned int _Rxcnt[2];
};


#endif  /*_COM_20020_H*/
