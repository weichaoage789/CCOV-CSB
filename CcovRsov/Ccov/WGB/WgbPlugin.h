/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  WgbPlugin.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _WGB_PLUGIN_H
#define _WGB_PLUGIN_H

#include "Utilities/NetWork/Ping.h"
using namespace NetWork;

typedef struct
{
	unsigned char SNR;			/** <SNR(Signal to Noise Ratio) */
	unsigned char ip[16];		/** <AP IP address */
	unsigned char rt;				/** <PING result */
	unsigned char stauts;		/** <WGB associate status */
} WGB_CFG;


class CWgbCtrl
{
public:
	CWgbCtrl() { 
		Reset(); 
	}
	~CWgbCtrl() {}

	void Reset() {
		_Cfg.SNR = 0;
		_Cfg.rt = 0xAA;
		_Cfg.stauts = 0xAA;
		memset(_Cfg.ip, 0, sizeof(_Cfg.ip));
	}

	/** @PING */
	bool ping(const char *hostip) {
		if (nullptr != hostip)
		{
			PingResult rt;
			return _Ping.ping(hostip, rt);
		}
		return false;
	}

	/** @set SNR */
	void SetSNR(unsigned char snr) {
		_Cfg.SNR = snr;
	}

	/** @get SNR */
	unsigned char GetSNR() {
		return _Cfg.SNR;
	}

	/** @set ping result */
	void SetPingRslt(unsigned char value) {
		_Cfg.rt = value;
	}

	/** @get ping result */
	unsigned char GetPingRslt() {
		return _Cfg.rt;
	}

	/** @set WGB status */
	void SetWgbStatus(unsigned char value) {
		_Cfg.stauts = value;
	}

	/** @get WGB status */
	unsigned char GetWgbStatus() {
		return _Cfg.stauts;
	}

	/** @set AP ip */
	void SetApIp(unsigned char *ip) {
		if (nullptr != ip)
		{
			memset(_Cfg.ip, 0, sizeof(_Cfg.ip));
			memcpy(_Cfg.ip, ip, sizeof(_Cfg.ip));
		}
	}

	/** @get AP ip */
	unsigned char * GetApIp() {
		return _Cfg.ip;
	}

private:
	Ping _Ping;

	WGB_CFG _Cfg;
};


#endif  /*_WGB_PLUGIN_H*/