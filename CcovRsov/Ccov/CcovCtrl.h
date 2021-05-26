/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  CcovCtrl.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _CCOV_CONTROL_H
#define _CCOV_CONTROL_H

#include "Utilities/Queue.h"
#include "CcovHandler.h"


class CCcovCtrl
{
private:
	CCcovCtrl() { _RunningFlag = false; }
	CCcovCtrl(const CCcovCtrl &) = delete;
	CCcovCtrl & operator = (const CCcovCtrl &) = delete;

public:
	static CCcovCtrl &GetInstance()
	{
		static CCcovCtrl instance;
		return instance;
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
	  * @brief		 :  OUTNET message receive thread function.
	  * @param	 :  [in] void *arg, input param.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	static void Thread_Rx_Outnet(void *arg);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  WGB message receive thread function.
	  * @param	 :  [in] void *arg, input param.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	static void Thread_Rx_WGB(void *arg);

	static void Thread_Rx_Rsov(void *arg);

private:
	/** @running flag */
	bool _RunningFlag;

	/** @CCOV handler */
	CCcovHandler _Handle;
};


#endif  /*_CCOV_CONTROL_H*/