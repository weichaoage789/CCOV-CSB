/**
  ******************************************************************************
  * @copyright	:
  * @file		        : RsovFileCtrl.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef DIERECTORY_MANAGEMENT_H
#define DIERECTORY_MANAGEMENT_H

#include <mutex>
#include "Utilities/TimerCtrl.h"


typedef struct
{
	unsigned short year;
	unsigned short month;
	unsigned short day;
	unsigned short hour;
	unsigned short min;
	unsigned short sec;
}FNAME_INFO;


class CRsovFileCtrl
{
public:
	CRsovFileCtrl();
	~CRsovFileCtrl();

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  rectify and update the system time.
	  * @param	 :  [in] unsigned long newtime, input new time to be rectified.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void RectifyTime(unsigned long newtime);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  record data to local files.
	  * @param	 :  [in] const char *data, source data tobe recorded.
	  * @param	 :  [in] const int len, source data length.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool DataRecord(const char *data, const int len);

protected:
	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  set timer.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void SetTimer();

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  set timer loop type according to the remain minutes of day.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void LoopDetail(bool resetflag = false);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  file task handler.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void FileHandler();

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  file rename and move after rectify the system time.
	  * @param	 :  [in] long long differ, rectify system time difference.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void FileRename(long long differ);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  separate the file address into root directory, ip address, year, month, day, hour, minute, second and other information.
	  * @param	 :  [in] string fname, input file name.
	  * @param	 :  [out] FNAME_INFO &info, output the info.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void FNameSeperate(string fname, FNAME_INFO &info);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  judge whether it is half an hour to reach the new day.
	  * @param	 :  [out] long &remain, output the remaining seconds of the day.
	  * @return	 :  [type] bool, less than 30 minutes -- true; over than 30 minutes -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool ComingDayJudge(long &remain);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  check whether the remaining free space of the disk meets the specified requirements, and delete the earliest created folder until the requirements are met.
	  * @param	 :  [in] long long differ, rectify system time difference.
	  * @return	 :  [type] int, satisfy the requirements -- 1, delete files failed -- 0
	  *-----------------------------------------------------------------------------
	  */
	int DiskSpaceManagment();

private:
	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  timer task function.
	  * @param	 :  [in] void *arg, input param.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	static void OnTimer(void *arg);

private:
	/** @rectify tag */
	bool _RectifyTag;

	/** @file rename tag */
	bool _RenameTag;
	
	/** @rectify time difference */
	long _DifferTime;

	/** @remain time of a day */
	long _RemainTime;

	/** @folder base path */
	std::string _BasePath;

	/** @current folder path */
	std::string _FolderPath;

	/** @current file path */
	std::string _FilePath;

	/** @file list operate mutex */
	std::mutex _FListLock;

	/** @list of files created this time */
	std::vector<std::string> _FList;

	/** @record file descriptor */
	FILE *_FpRecord;
};


#endif  /*DIERECTORY_MANAGEMENT_H*/
