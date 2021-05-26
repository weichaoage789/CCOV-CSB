#include <string.h>

#include "FileWork.h"
#include "LogFileWork.h"


namespace FileWork
{
	Mutex LogMessage::mutex;

	const int BUFFER_SIZE = 8196;
	static char _gBuffer[BUFFER_SIZE];
	static char _appName[_MAXPATH_];
	static char _appFolder[_MAXPATH_];
	static char _destFolder[_MAXPATH_];
	static char _destPrefix[_MAXPATH_];
	static char _defaltFolder[] = "/var/tmp/";
	static char _levelInfos[][16] = { "Debug","Info","Warn","Error","Fatal" };

	static LogLevel _destLevel;


	static void InitPaths(const char* filename, const char* destFolder)
	{
		memset(_appName, 0, _MAXPATH_);
		memset(_appFolder, 0, _MAXPATH_);
		memset(_destFolder, 0, _MAXPATH_);
		memset(_destPrefix, 0, _MAXPATH_);

		strcpy(_appName, filename);
		int len = strlen(filename), lend;
		int pos = len - 1, posd, start;
		while (pos > 0 && filename[pos] != _SPLITPATH_) pos--;
		strncpy(_appFolder, filename, pos + 1);
		lend = strlen(destFolder);
		posd = lend - 1;
		if (destFolder[lend - 1] != _SPLITPATH_)
		{
			//has prefix
			while (posd > 0 && destFolder[posd] != _SPLITPATH_)
				posd--;
		}
		if (destFolder[0] == '.' && destFolder[1] == _SPLITPATH_) {
			strncpy(_destFolder, filename, pos + 1);
			start = 2;
		}
		else {
			pos = 8;
			strcpy(_destFolder, _defaltFolder);
			if (destFolder[0] != _SPLITPATH_) {
				start = 0;
			}
			else {
				start = 1;
			}
		}
		strncpy(_destFolder + pos + 1, destFolder + start, posd - start + 1);
		strncpy(_destPrefix, filename, pos + 1);
		strncpy(_destPrefix + pos + 1, destFolder + start, lend - start);
	}


	void InitLogging(const char* filename, LogLevel minlevel, const char* destFolder)
	{
		InitPaths(filename, destFolder);
		_destLevel = minlevel;
	}

	Logger::Logger(LogLevel level, char * folder, char * prefix) : level(level)
	{
		std::string path;
		path.append(prefix);
		path.append(GetLocalDate());
		path.append(".log");
		logPrefix.append(prefix);
		logPath = path;
		logFile.open(path.c_str(), ios::app | ios::out);
		logFile << "Log file created at:" << GetLocalTime() << endl;
	}

	Logger::~Logger()
	{
		logFile.close();
	}

#define IMPLEMENT_LOG_FUNC1(cname,fname,lv) \
void cname::fname(string msg) {\
    if(level <= lv){\
        WriterMutexLock lock(&mutex);\
        logFile<<"["<<GetLocalTime().c_str()<<"][" #lv "]"<<msg.c_str()<<endl;\
        logFile.flush();\
    }\
}

#define PRINT_ARGS_TO_BUFFER(fmt,buf) \
    {\
        memset(buf,0,sizeof(buf));\
        va_list argp;\
        va_start(argp,fmt);\
        vsprintf(buf,fmt,argp);\
        va_end(argp);\
    }

#define IMPLEMENT_LOG_FUNC2(cname,fname,lv) \
void cname::fname(const char* format,...) {\
    if(level <= lv){\
        WriterMutexLock lock(&mutex);\
        PRINT_ARGS_TO_BUFFER(format,_gBuffer)\
        logFile<<"["<<GetLocalTime().c_str()<<"][" #lv "]"<<_gBuffer<<endl;\
        logFile.flush();\
    }\
}


#define IMPLEMENT_LOG_FUNC(cname,fname,lv) \
	IMPLEMENT_LOG_FUNC1(cname,fname,lv)\
	IMPLEMENT_LOG_FUNC2(cname,fname,lv)

	IMPLEMENT_LOG_FUNC(Logger, Debug, DEBUG)
	IMPLEMENT_LOG_FUNC(Logger, Info, INFO)
	IMPLEMENT_LOG_FUNC(Logger, Warn, WARN)
	IMPLEMENT_LOG_FUNC(Logger, Error, ERROR)
	IMPLEMENT_LOG_FUNC(Logger, Fatal, FATAL)

	Logger &Logger::GetInstance()
	{
		static Logger _logger(_destLevel, _destFolder, _destPrefix);
		return _logger;
	}

	Logger* Logger::GetInstancePtr()
	{
		return &GetInstance();
	}

	void Logger::Log(LogLevel lv, string msg)
	{
		if (level <= lv)
		{
			WriterMutexLock lock(&mutex);
			logFile << "[" << GetLocalTime().c_str() << "][" << _levelInfos[lv + 1] << "]" << msg.c_str() << endl;
			logFile.flush();
		}
	}

	void Logger::Log(LogLevel lv, const char* format, ...)
	{
		if (level <= lv)
		{
			WriterMutexLock lock(&mutex);
			PRINT_ARGS_TO_BUFFER(format, _gBuffer)
				logFile << "[" << GetLocalTime().c_str() << "][" << _levelInfos[lv + 1] << "]" << _gBuffer << endl;
			logFile.flush();
		}
	}

	void Logger::Log(const char* file, int line, LogLevel lv, string msg)
	{
		if (level <= lv)
		{
			WriterMutexLock lock(&mutex);
			logFile << "[" << GetLocalTime().c_str() << "][" << _levelInfos[lv + 1] << "][" << file << "][" << line << "]" << msg.c_str();
			logFile.flush();
		}
	}

	void Logger::Log(const char* file, int line, LogLevel lv, const char* format, ...)
	{
		if (level <= lv)
		{
			WriterMutexLock lock(&mutex);
			PRINT_ARGS_TO_BUFFER(format, _gBuffer)
				logFile << "[" << GetLocalTime().c_str() << "][" << _levelInfos[lv + 1] << "][" << file << "][" << line << "]" << _gBuffer;
			logFile.flush();
		}
	}

	LogMessage::LogMessage(const char* file, int line, LogLevel lv)
	{
		logger = Logger::GetInstancePtr();
		mutex.Lock();
		logger->Log(file, line, lv, "");
	}

	LogMessage::LogMessage(LogLevel lv)
	{
		logger = Logger::GetInstancePtr();
		mutex.Lock();
		logger->Log(lv, "");
	}

	LogMessage::~LogMessage()
	{
		logger->stream() << endl;
		logger->stream().flush();
		mutex.Unlock();
	}
}