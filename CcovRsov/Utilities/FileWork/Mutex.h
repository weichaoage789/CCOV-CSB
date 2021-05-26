/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  Mutex.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _MUTEX_H
#define _MUTEX_H

#include <pthread.h>
#include <stdlib.h>


namespace FileWork
{
	typedef pthread_mutex_t MutexType;

	class Mutex
	{
	public:
		inline Mutex();
		inline ~Mutex();

		inline void Lock();			// Block if needed until free then acquire exclusively
		inline void Unlock();		// Release a lock acquired via Lock()
		inline bool TryLock();	// If free, Lock() and return true, else return false

		// Note that on systems that don't support read-write locks, these may be implemented as synonyms to Lock() and Unlock().  
		// So you can use these for efficiency, but don't use them anyplace where being able to do shared reads is necessary to avoid deadlock.
		inline void ReaderLock() { Lock(); }				// Block until free or shared then acquire a share
		inline void ReaderUnlock() { Unlock(); }		// Release a read share of this Mutex
		inline void WriterLock() { Lock(); }					// Acquire an exclusive lock
		inline void WriterUnlock() { Unlock(); }		// Release a lock from WriterLock()

	private:
		MutexType mutex_;
		// We want to make sure that the compiler sets is_safe_ to true only when we tell it to, and never makes assumptions is_safe_ is always true.
		// volatile is the most reliable way to do that.
		volatile bool is_safe_;

		inline void SetIsSafe() { is_safe_ = true; }

		Mutex(Mutex*) {}
		Mutex(const Mutex&) = delete;
		Mutex &operator=(const Mutex&) = delete;
	};

#define SAFE_PTHREAD(fncall)  do {   /* run fncall if is_safe_ is true */  \
    if (is_safe_ && fncall(&mutex_) != 0) abort();                           \
    } while (0)

	Mutex::Mutex()
	{
		SetIsSafe();
		if (is_safe_ && pthread_mutex_init(&mutex_, nullptr) != 0) abort();
	}

	Mutex::~Mutex() { SAFE_PTHREAD(pthread_mutex_destroy); }
	void Mutex::Lock() { SAFE_PTHREAD(pthread_mutex_lock); }
	void Mutex::Unlock() { SAFE_PTHREAD(pthread_mutex_unlock); }
	bool Mutex::TryLock() { return is_safe_ ? pthread_mutex_trylock(&mutex_) == 0 : true; }


	class MutexLock
	{
	public:
		explicit MutexLock(Mutex *mu) : mu_(mu) { mu_->Lock(); }
		~MutexLock() { mu_->Unlock(); }

	private:
		Mutex * const mu_;
		MutexLock(const MutexLock&) = delete;
		MutexLock &operator=(const MutexLock&) = delete;
	};


	class ReaderMutexLock
	{
	public:
		explicit ReaderMutexLock(Mutex *mu) : mu_(mu) { mu_->ReaderLock(); }
		~ReaderMutexLock() { mu_->ReaderUnlock(); }

	private:
		Mutex * const mu_;
		ReaderMutexLock(const ReaderMutexLock&) = delete;
		ReaderMutexLock &operator=(const ReaderMutexLock&) = delete;
	};


	class WriterMutexLock
	{
	public:
		explicit WriterMutexLock(Mutex *mu) : mu_(mu) { mu_->WriterLock(); }
		~WriterMutexLock() { mu_->WriterUnlock(); }

	private:
		Mutex * const mu_;
		WriterMutexLock(const WriterMutexLock&) = delete;
		WriterMutexLock &operator=(const WriterMutexLock&) = delete;
	};
}


#endif  /*_MUTEX_H*/
