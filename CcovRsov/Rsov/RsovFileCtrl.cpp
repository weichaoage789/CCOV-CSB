#include "AppDefine.h"
#include "AppConfig.h"

#include "Utilities/FileWork/ZipWork.h"
#include "Utilities/FileWork/FileWork.h"
#include "Utilities/FileWork/LogFileWork.h"
using namespace FileWork;

#include "RsovDefine.h"
#include "RsovFileCtrl.h"


CRsovFileCtrl::CRsovFileCtrl() 
	: _DifferTime(0)
	, _RemainTime(0)
	, _RectifyTag(false)
	, _RenameTag(false)
	, _FpRecord(nullptr)
{
	/** @set base folder path */
	_BasePath = DISK_BASE_PATH + std::string("/") + CAppCfg::GetInstance()._LocalIp;
	CreateFolder(_BasePath.c_str());

	/** @disk space management */
	DiskSpaceManagment();
	DelFoldersOverMax(_BasePath.c_str(), MAX_FOLDER_NUM);

	/** @create record file and add timer */
	FileHandler();
	LoopDetail();
}

CRsovFileCtrl::~CRsovFileCtrl()
{
	if (nullptr != _FpRecord)
	{
		fclose(_FpRecord);
		_FpRecord = nullptr;
	}
}

void CRsovFileCtrl::LoopDetail(bool resetflag/* = false*/)
{
	if (!ComingDayJudge(_RemainTime))
	{
		/** @remain time is bigger than 30 minutes */
		if (resetflag)
			CMyTimer::GetInstance().ResetTimer(HALF_HOUR);
		else
			CMyTimer::GetInstance().AddTimer(HALF_HOUR, OnTimer, this);
	}
	else
	{
		/** @remain time is less than 30 minutes */
		if (resetflag)
			CMyTimer::GetInstance().ResetTimer(_RemainTime);
		else
			CMyTimer::GetInstance().AddTimer(_RemainTime, OnTimer, this);
	}
}

void CRsovFileCtrl::SetTimer()
{
	/** @update recording file */
	FileHandler();

	if (_RenameTag)
	{
		/** @needs to rename files and reset timer after time rectified */
		FileRename(_DifferTime);
	}

	/** @reset timer */
	LoopDetail(true);
}

void CRsovFileCtrl::OnTimer(void *arg)
{
	if (!arg)	return;
	CRsovFileCtrl *pThisObject = (CRsovFileCtrl *)arg;
	pThisObject->SetTimer();
}

void CRsovFileCtrl::FileHandler()
{
	/** @save the filepath last created for zip */
	string srcfile = _FilePath;

	char datebuf[16] = { 0 };
	char timebuf[16] = { 0 };
	GetSysDateAndTime(datebuf, timebuf);

	_FolderPath = _BasePath + "/" + datebuf;
	CreateFolder(_FolderPath.c_str());

	_FilePath = _FolderPath + "/" + timebuf;
	if (!_RectifyTag)
	{
		_FilePath += "-";
	}
	_FilePath += ".txt";
	
	/** @create new file for record */
	if (CreateFile(&_FpRecord, _FilePath.c_str()))
	{
		/** @zip the file except the first time */
		if (IsFileExist(srcfile.c_str()))
		{
			LOG(INFO) << "create file name = " << _FilePath.c_str();

			/** @zip the last file if new file created successfully */
			std::string dstfile = srcfile + std::string(".gz");
			LOG(INFO) << "filepath last created zip to : " << dstfile.c_str();

			if (Z_OK == compress_file_to_gzip(srcfile.c_str(), dstfile.c_str()))
			{
				/** @record the zipped file for rename after rectify time*/
				_FListLock.lock();
				_FList.push_back(dstfile);
				_FListLock.unlock();
				LOG(INFO) << "compress " << srcfile.c_str() << " to " << dstfile.c_str();
			}
			else
				LOG(INFO) << "compress " << srcfile.c_str() << " to " << dstfile.c_str() << " failed";
		}
	}
	else
		LOG(ERROR) << "create file " << _FilePath.c_str() << " failed";
}

void CRsovFileCtrl::FileRename(long long differ)
{
	char FilePath[NAME_MAX];
	time_t t_current, t_rectify;
	struct tm tm_current, *tm_rectify;

	/** file rename after rectify time */
	_FListLock.lock();
	for (auto iter = _FList.begin(); iter != _FList.end(); iter++)
	{
		FNAME_INFO fname;
		FNameSeperate(*iter, fname);

		tm_current.tm_year = fname.year - 1900;
		tm_current.tm_mon = fname.month - 1;
		tm_current.tm_mday = fname.day;
		tm_current.tm_hour = fname.hour;
		tm_current.tm_min = fname.min;
		tm_current.tm_sec = fname.sec;

		/** @update rectify time*/
		t_current = mktime(&tm_current);
		t_rectify = t_current + differ - 1;
		tm_rectify = localtime(&t_rectify);

		/** @not the same day, create new folder */
		if ((tm_rectify->tm_year != tm_current.tm_year)
			|| (tm_rectify->tm_mon != tm_current.tm_mon)
			|| (tm_rectify->tm_mday != tm_current.tm_mday))
		{
			memset(FilePath, 0, sizeof(FilePath));
			sprintf(FilePath, "%s/%04d.%02d.%02d", _BasePath.c_str(), tm_rectify->tm_year + 1900, tm_rectify->tm_mon + 1, tm_rectify->tm_mday);
			CreateFolder(FilePath);
		}

		memset(FilePath, 0, sizeof(FilePath));
		sprintf(FilePath, "%s/%04d.%02d.%02d/%02d.%02d.%02d.txt.gz", _BasePath.c_str()
			, tm_rectify->tm_year + 1900, tm_rectify->tm_mon + 1, tm_rectify->tm_mday, tm_rectify->tm_hour, tm_rectify->tm_min, tm_rectify->tm_sec);

		/** @file rename */
		if (0 == rename((*iter).c_str(), FilePath))
		{
			LOG(INFO) << "File rename success. Differ time = " << _DifferTime << ". rename file " << (*iter).c_str() << " to " << FilePath;
			*iter = FilePath;
			_RenameTag = false;
		}
		else
			LOG(ERROR) << "rename file " << (*iter).c_str() << " to " << FilePath << "failed. Differ time = " << _DifferTime;
	}
	_FListLock.unlock();

	return;
}

void CRsovFileCtrl::FNameSeperate(string fname, FNAME_INFO &info)
{
	fname = fname.erase(0, _BasePath.size() + 1);

	string tmp;
	tmp.assign(fname, 0, 4);
	info.year = atoi(tmp.c_str());
	tmp.assign(fname, 5, 2);
	info.month = atoi(tmp.c_str());
	tmp.assign(fname, 8, 2);
	info.day = atoi(tmp.c_str());
	tmp.assign(fname, 11, 2);
	info.hour = atoi(tmp.c_str());
	tmp.assign(fname, 14, 2);
	info.min = atoi(tmp.c_str());
	tmp.assign(fname, 17, 2);
	info.sec = atoi(tmp.c_str());
}

void CRsovFileCtrl::RectifyTime(unsigned long newtime)
{
	/** @Calculate the time difference between the current time and the corrected time */
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	long differtime = long(newtime - tv.tv_sec);

	/** @update system time and rename created files */
	if (!_RectifyTag)
	{
		tv.tv_sec = newtime;
		if (0 == settimeofday(&tv, &tz))
		{
			/** @update the rectify flag and save the differ time */
			_RectifyTag = true;
			_RenameTag = true;
			_DifferTime = differtime;

			/** @reset timer after rectify time */
			SetTimer();
			LOG(INFO) << "Rectify time, newtime = " << newtime << ", Differ = " << _DifferTime;
		}
		else
			LOG(INFO) << "Rectify time fail, newtime = " << newtime << ", Differ = " << _DifferTime;
	}
}

bool CRsovFileCtrl::ComingDayJudge(long &remain)
{
	time_t now;
	time(&now);
	remain = SECONDS_OF_DAY - now % SECONDS_OF_DAY;
	return (remain < HALF_HOUR) ? true : false;
}

int CRsovFileCtrl::DiskSpaceManagment()
{
	/** @check whether the disk space if full */
	if (IsDiskSpaceIsFull(DISK_BASE_PATH, DISK_SPACE_FULLTAG))
	{
		/** @full, delete the earliest created folder of the disk */
		if (DeleteEarliestFolder(_BasePath.c_str()))
		{
			/** @go on checking recursive */
			DiskSpaceManagment();
		}
		return 0;
	}
	return 1;
}

bool CRsovFileCtrl::DataRecord(const char *data, const int len)
{
	if (nullptr != data)
	{
		return SaveToFile(_FpRecord, data, len);
	}
	return false;
}