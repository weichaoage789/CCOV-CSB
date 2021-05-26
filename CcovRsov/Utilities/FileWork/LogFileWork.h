/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  LogFileWork.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _LOG_FILE_WORK_H
#define _LOG_FILE_WORK_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "Mutex.h"


namespace FileWork
{
constexpr auto _MAXPATH_ = 256;
constexpr auto _SPLITPATH_ = '/';

	enum LogLevel {
		DEBUG = -1,		/** @debug */
		INFO = 0,			/** @info */
		WARN,				/** @warning */
		ERROR,				/** @error */
		FATAL,					/** @crash */
		OFF						/** @over error level */
	};


	class ILogger
	{
	public:
		virtual ~ILogger() {}

#define ABSTRACT_LOG_FUNC(name)			\
		virtual void name(string msg) = 0;		\
		virtual void name(const char* fmt,...) = 0;

		ABSTRACT_LOG_FUNC(Debug)
		ABSTRACT_LOG_FUNC(Info)
		ABSTRACT_LOG_FUNC(Warn)
		ABSTRACT_LOG_FUNC(Error)
		ABSTRACT_LOG_FUNC(Fatal)

#undef ABSTRACT_LOG_FUNC
#define ABSTRACT_LOG_FUNC_X(name)		\
		virtual void name(LogLevel lv,string msg) = 0;		\
		virtual void name(LogLevel lv,const char* fmt,...) = 0;		\
		virtual void name(const char* file,int line,LogLevel lv,string msg) = 0;		\
		virtual void name(const char* file,int line,LogLevel lv,const char* fmt,...) = 0;
		ABSTRACT_LOG_FUNC_X(Log)

#undef LOG_FUNC_X
	};


	class Logger : public ILogger
	{
	private:
		std::string logPath;
		std::string logPrefix;
		std::fstream logFile;
		LogLevel level;
		Mutex mutex;

		Logger(LogLevel level, char * folder, char * prefix);

	public:
		static Logger &GetInstance();
		static Logger *GetInstancePtr();
		virtual ~Logger();

		inline fstream & stream()
		{
			return logFile;
		}

#define DECLARE_LOG_FUNC(name)		\
		virtual void name(string msg);			\
		virtual void name(const char* fmt, ...);

#define DECLARE_LOG_FUNC_X(name) \
		virtual void name(LogLevel lv, string msg); \
		virtual void name(LogLevel lv, const char* fmt, ...);	\
		virtual void name(const char* file, int line, LogLevel lv, string msg);		\
		virtual void name(const char* file, int line, LogLevel lv, const char* fmt, ...);

		DECLARE_LOG_FUNC(Debug)
		DECLARE_LOG_FUNC(Info)
		DECLARE_LOG_FUNC(Warn)
		DECLARE_LOG_FUNC(Error)
		DECLARE_LOG_FUNC(Fatal)
		DECLARE_LOG_FUNC_X(Log)

#undef DECLARE_LOG_FUNC_X
#undef DECLARE_LOG_FUNC
	};


	class LogMessage
	{
		Logger* logger;
		static Mutex mutex;

	public:
		LogMessage(const char* file, int line, LogLevel lv);
		LogMessage(LogLevel lv);
		virtual ~LogMessage();

		ostream& stream()
		{
			return logger->stream();
		}
	};

	void InitLogging(const char* filename, LogLevel minlevel, const char* destFolder);


#define LOG(level)			LogMessage(__FILE__, __LINE__, level).stream()
#define LOGFILE(level)	LogMessage(level).stream()

#define LOG_IF(severity, condition)		!(condition) ? (void) 0 : LOG(severity)
#define LOG_ASSERT(condition)			LOG_IF(FATAL, !(condition)) << "Assert failed: " #condition
#define CHECK(condition)						LOG_IF(FATAL, !(condition)) << "Check failed: " #condition " "

}

#endif  /*_LOG_FILE_WORK_H*/
