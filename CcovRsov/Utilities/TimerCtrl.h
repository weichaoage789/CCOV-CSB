/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  TimerCtrl.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _TIMER_CONTROL_H
#define _TIMER_CONTROL_H

#include <csignal>
#include <sys/time.h>
using namespace std;


typedef struct TimerEvent
{
	unsigned long long periodic_time;
	void(*ExecuFunc)(void *);
	void *arg;
}TimerEvent;

static TimerEvent _TEvnet;
static unsigned long long _TExpires;


class CMyTimer
{
private:
	CMyTimer() { InitTimer(); }
	CMyTimer(const CMyTimer &) = delete;
	CMyTimer & operator = (const CMyTimer &) = delete;

public:
	static CMyTimer &GetInstance()
	{
		static CMyTimer instance;
		return instance;
	}

	void AddTimer(int sec, void(*ExecuFunc)(void *), void *arg)
	{
		_TEvnet.periodic_time = sec;
		_TEvnet.arg = arg;
		_TEvnet.ExecuFunc = ExecuFunc;
		_TExpires = 0;
	}

	void ResetTimer(int sec)
	{
		_TEvnet.periodic_time = sec;
		_TExpires = 0;
	}

protected:
	int InitTimer()
	{
		struct itimerval value_new;
		signal(SIGALRM, TimerSchedule);
		value_new.it_value.tv_sec = 1;
		value_new.it_value.tv_usec = 0;
		value_new.it_interval.tv_sec =	1;
		value_new.it_interval.tv_usec = 0;
		setitimer(ITIMER_REAL, &value_new, nullptr);
	}

private:
	static void TimerSchedule(int signo)
	{
		_TExpires++;
		if ((nullptr != _TEvnet.ExecuFunc) && (0 == (_TExpires % _TEvnet.periodic_time)))
		{
			_TEvnet.ExecuFunc(_TEvnet.arg);
		}
	}
};


#endif  /*_TIMER_CONTROL_H*/