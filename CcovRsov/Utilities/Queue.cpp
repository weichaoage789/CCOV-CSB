#include "AppDefine.h"
#include "Queue.h"


CMesgQue::CMesgQue() : _Pushfailcnt(0), _Totalpopcnt(0), _Totalpushcnt(0)
{
	sem_init(&_MesgQueIsUsed, 0, 1);
	sem_init(&_MesgQueIsRead, 0, 0);
	sem_init(&_MesgQueIsWrite, 0, 10000);
}

CMesgQue::~CMesgQue()
{
	sem_wait(&_MesgQueIsUsed);
	while (!_MesgQue.empty())
	{
		if (_MesgQue.front().first)
		{
			delete[] _MesgQue.front().first;
			_MesgQue.front().first = nullptr;
		}
	}
	sem_post(&_MesgQueIsUsed);

	sem_destroy(&_MesgQueIsWrite);
	sem_destroy(&_MesgQueIsRead);
	sem_destroy(&_MesgQueIsUsed);
}


bool CMesgQue::PopMesg(unsigned char ** dest, unsigned int & len)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += 1;

	if (sem_timedwait(&_MesgQueIsRead, &ts) == 0)
	{
		sem_wait(&_MesgQueIsUsed);

		std::pair<unsigned char *, unsigned int> RxMesgsPair;
		RxMesgsPair = _MesgQue.front();
		(*dest) = RxMesgsPair.first;
		len = RxMesgsPair.second;
		_MesgQue.pop();

		sem_post(&_MesgQueIsUsed);
		sem_post(&_MesgQueIsWrite);

		_Totalpopcnt++;
		return true;
	}

	return false;
}

bool CMesgQue::PushMesg(unsigned char *src, unsigned int len)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += 1;

	if (sem_timedwait(&_MesgQueIsWrite, &ts) == 0)
	{
		sem_wait(&_MesgQueIsUsed);

		std::pair<unsigned char *, unsigned int> RxMesgsPair(src, len);
		_MesgQue.push(RxMesgsPair);

		sem_post(&_MesgQueIsUsed);
		sem_post(&_MesgQueIsRead);

		_Totalpushcnt++;
		return true;
	}

	if (src)
	{
		_Pushfailcnt++;
		delete[] src;
		src = nullptr;
	}
	return false;
}
