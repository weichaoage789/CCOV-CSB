/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  Queue.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _QUEUE_H
#define _QUEUE_H

#include <queue>
#include <semaphore.h>


class CMesgQue
{
public:
	CMesgQue();
	~CMesgQue();

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  push a message into message queue.
	  * @param	 :  [in] unsigned char *src, input source data to be pushed into message queue.
	  * @param	 :  [in] unsigned int &len, input source data length.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool PushMesg(unsigned char *src, unsigned int len);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  pop a message out of message queue.
	  * @param	 :  [out] unsigned char **dest, output dest data pointer popped out of message queue.
	  * @param	 :  [in] unsigned int &len, output dest data length.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool PopMesg(unsigned char **dest, unsigned int &len);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get total push count.
	  * @return	 :  [type] unsigned int, total push count.
	  *-----------------------------------------------------------------------------
	  */
	unsigned int GetPushcnt() {
		return _Totalpushcnt;
	}

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get total pop count.
	  * @return	 :  [type] unsigned int, total pop count.
	  *-----------------------------------------------------------------------------
	  */
	unsigned int GetPopcnt() {
		return _Totalpopcnt;
	}

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get total push failed count.
	  * @return	 :  [type] unsigned int, total push failed count.
	  *-----------------------------------------------------------------------------
	  */
	unsigned int GetPushfailcnt() {
		return _Pushfailcnt;
	}

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get queue data count.
	  * @return	 :  [type] unsigned int, total push count.
	  *-----------------------------------------------------------------------------
	  */
	unsigned int GetDatacnts() {
		return _MesgQue.size();
	}

private:
	/** @push into message queue fail count */
	unsigned int _Pushfailcnt;

	/** @push into message queue total count */
	unsigned int _Totalpushcnt;

	/** @pop out of message queue total count */
	unsigned int _Totalpopcnt;

	/** @semaphore of message queue is usable */
	sem_t _MesgQueIsUsed;

	/** @semaphore of message queue is readable */
	sem_t _MesgQueIsRead;

	/** @semaphore of message queue is writable */
	sem_t _MesgQueIsWrite;

	/** @message queue for pair of <data, len> */
	std::queue<std::pair<unsigned char *, unsigned int>> _MesgQue;
};


#endif  /*_QUEUE_H*/