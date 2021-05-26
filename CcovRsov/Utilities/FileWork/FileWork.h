/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  FileWork.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _FILE_WORK_H
#define _FILE_WORK_H

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

#include <vector>
#include <mutex>
#include <string>


namespace FileWork
{
#define GB(x) (((x).HighPart << 2) + ((float)(x).LowPart) / 1024.0 / 1024.0 / 1024.0)
#define MB(x) (((x).HighPart << 2) + ((float)(x).LowPart) / 1024.0 / 1024.0)

#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#else
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
#endif

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  check if the folder exists.
	  * @param	 :  [in] const char *foldername, input folder name/path.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool IsFolderExist(const char *foldername);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  create folder.
	  * @param	 :  [in] const char *foldername, input folder name/path.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool CreateFolder(const char *foldername);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  delete folder.
	  * @param	 :  [in] string foldername, input folder name/path.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool FolderDelete(std::string foldername);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  check if the file exists.
	  * @param	 :  [in] const char *filename, input file name/path.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool IsFileExist(const char *filename);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  create file.
	  * @param	 :  [in] FILE **fp, input file operator.
	  * @param	 :  [in] const char *filename, input file name/path.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool CreateFile(FILE **fp, const char *filename);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  delete file.
	  * @param	 :  [in] string fname, input file name/path.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool FileDelete(std::string fname);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  save data to file.
	  * @param	 :  [in] FILE *fp, input file operator.
	  * @param	 :  [in] const char *info, input data info.
	  * @param	 :  [in] const int len, input data info length.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool SaveToFile(FILE *fp, const char *info, const int len);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  sort all subfile names in the specified directory folder.
	  * @param	 :  [in] const char *basefolder, input disk name/path.
	  * @param	 :  [in & out] vector<string> &flist, input & output the sort results.
	  * @param	 :  [bool] flag, folder only -- true; folder and file -- false.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool  FolderSort(const char *basefolder, std::vector<std::string> &flist, bool flag = true);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  judge whether a path is a folder or a file.
	  * @param	 :  [in] const char *path, input path.
	  * @return	 :  [type] int, -1 : path no exists; 0 : file; 1 : folder/directory.
	  *-----------------------------------------------------------------------------
	  */
	int IsFolderOrFile(const char *path);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  file or folder copy.
	  * @param	 :  [in] const char *srcpath, source file/folder path.
	  * @param	 :  [in] const char *destpath, dest folder path.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool DoCopy(const std::string &srcpath, const std::string &destfolder);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  delete folder over target number.
	  * @param	 :  [in] const char *foldername, input folder name/path.
	  * @param	 :  [in] const unsigned int maxnum, input folder target number.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool DelFoldersOverMax(const char *foldername, const unsigned int maxnum);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  delete the earliest created folder and its subfiles under the specified path.
	  * @param	 :  [in] const char *foldername, input folder name/path.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool DeleteEarliestFolder(const char *foldername);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get the size of the remaining space on the specified disk. Unit: MB.
	  * @param	 :  [in] const char *diskname, input disk name/path.
	  * @return	 :  [type] unsigned int, the amount of remaining free space.
	  *-----------------------------------------------------------------------------
	  */
	unsigned int GetDiskSpaceValue(const char *diskname);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  check whether the remaining free space of the disk is less than the specified value.
	  * @param	 :  [in] const char *diskname, input disk name/path.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool IsDiskSpaceIsFull(const char *diskname, const unsigned int minvalue);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get the specified folder size.
	  * @param	 :  [in] const char *foldername, input folder name/path.
	  * @return	 :  [type] unsigned int, size of folder.
	  *-----------------------------------------------------------------------------
	  */
	unsigned int GetFolderSize(const char *foldername);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get the specified file size.
	  * @param	 :  [in] const char *filename, input file name/path.
	  * @return	 :  [type] unsigned int, size of file.
	  *-----------------------------------------------------------------------------
	  */
	unsigned int GetFileSize(const char *filename);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get file md5.
	  * @param	 :  [in] const char *filename, input file name/path.
	  * @param	 :  [in] std::string &md5, input file name/path.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool get_file_md5(const char *filename, std::string &md5);


	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get local date.
	  * @return	 :  [type] std::string, local date, format "%Y-%m-%d"
	  *-----------------------------------------------------------------------------
	  */
	std::string GetLocalDate();

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get local time.
	  * @return	 :  [type] std::string, local time, format "%Y-%m-%d %H:%M:%S"
	  *-----------------------------------------------------------------------------
	  */
	std::string GetLocalTime();

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get system current date and time.
	  * @param	 :  [out] char *sysdate, system date, format "2013.06.27" .
	  * @param	 :  [out] char *systime, system time, format "19.28.07" .
	  * @return	 :  [type] void.
	  *-----------------------------------------------------------------------------
	  */
	void GetSysDateAndTime(char *sysdate, char *systime);
}


#endif  /*_FILE_WORK_H*/