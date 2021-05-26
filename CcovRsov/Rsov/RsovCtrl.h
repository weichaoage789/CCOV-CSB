/**
  ******************************************************************************
  * @copyright	:
  * @file		        : RsovCtrl.h
  * @brief	        :  
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _RSOV_CONTROL_H
#define _RSOV_CONTROL_H

#include "Driver/COM20020/COM20020.h"
#include "Utilities/Queue.h"
#include "RsovHandler.h"


class CRsovCtrl
{
private:
	CRsovCtrl() { _RunningFlag = false; }
	CRsovCtrl(const CRsovCtrl &) = delete;
	CRsovCtrl & operator = (const CRsovCtrl &) = delete;

public:
	static CRsovCtrl &GetInstance()
	{
		static CRsovCtrl ThisObject;
		return ThisObject;
	}

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  start.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void Start();

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  stop.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void Stop();

private:
	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  ARCNET message parse thread function.
	  * @param	 :  [in] void *arg, input param.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	static void Thread_Parse_Arcnet(void *arg);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  ARCNET message record thread function.
	  * @param	 :  [in] void *arg, input param.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	static void  Thread_Rd_Arcnet(void *arg);

private:
	/** @running flag */
	bool _RunningFlag;

	/** @RSOV handler */
	CRsovHandler _Handle;
};


#endif  /*_RSOV_CONTROL_H*/