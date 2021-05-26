#include <dirent.h>
#include <string.h>
#include <fstream>
#include <sys/statfs.h>
#include <algorithm>
#include <openssl/md5.h>
using namespace std;

#include "FileWork.h"


namespace FileWork
{
	/** @record file mutex */
	std::mutex _FpLock;

	bool IsFolderExist(const char *foldername)
	{
		DIR *dp;
		if (nullptr != foldername && nullptr != (dp = opendir(foldername)))
		{
			closedir(dp);
			return true;
		}
		return false;
	}

	bool CreateFolder(const char *foldername)
	{
		if (nullptr != foldername)
		{
			if (!IsFolderExist(foldername))
			{
				MKDIR(foldername);
			}
			return true;
		}
		return false;
	}

	bool FolderDelete(std::string foldername)
	{
		DIR* dirp = opendir(foldername.c_str());
		if (!dirp) { return false; }

		struct dirent *dir;
		struct stat st;
		while ((dir = readdir(dirp)) != nullptr)
		{
			if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
			{
				continue;
			}
			std::string sub_path = foldername + '/' + dir->d_name;
			if (lstat(sub_path.c_str(), &st) == -1)
			{
				continue;
			}
			if (S_ISDIR(st.st_mode))
			{
				if (!FolderDelete(sub_path))	// it's folder of directory
				{
					closedir(dirp);
					return false;
				}
				rmdir(sub_path.c_str());
			}
			else if (S_ISREG(st.st_mode))
			{
				unlink(sub_path.c_str());	// it's normal file, unlink
			}
			else
			{
				continue;
			}
		}

		closedir(dirp);
		if (rmdir(foldername.c_str()) == -1)		//delete dir itself.
			return false;
		return true;
	}

	bool IsFileExist(const char *filename)
	{
		if (nullptr != filename)
		{
			return 0 == ACCESS(filename, F_OK);
		}
		return false;
	}

	bool CreateFile(FILE **fp, const char *filename)
	{
		if (nullptr != filename)
		{
			_FpLock.lock();
			if (nullptr != *fp)
			{
				fflush(*fp);
				fclose(*fp);
				*fp = nullptr;
			}
			*fp = fopen(filename, "wb");
			_FpLock.unlock();

			if (nullptr != *fp) { return true; }
		}
		return false;
	}

	bool FileDelete(std::string fname)
	{
		if (0 == IsFolderOrFile(fname.c_str()))
		{
			if (unlink(fname.c_str()) != -1)
				return true;
		}
		else if (1 == IsFolderOrFile(fname.c_str()))
		{
			if (FolderDelete(fname))
				return true;
		}

		return false;
	}

	bool SaveToFile(FILE *fp, const char *info, const int len)
	{
		if (nullptr != fp && nullptr != info)
		{
			_FpLock.lock();
			fwrite(info, len, 1, fp);
			fflush(fp);
			_FpLock.unlock();

			return true;
		}
		return false;
	}

	bool FolderSort(const char *basefolder, std::vector<std::string> &flist, bool flag/* = true*/)
	{
		DIR *dir;
		struct dirent *ptr;

		if (nullptr != basefolder && nullptr != (dir = opendir(basefolder)))
		{
			while (nullptr != (ptr = readdir(dir)))
			{
				if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parent dir
					continue;
				else if (ptr->d_type == 8)    ///file
				{
					if (flag)
						continue;
					else
						flist.push_back(ptr->d_name);
				}
				else if (ptr->d_type == 10)    ///link file
					continue;
				else if (ptr->d_type == 4)    ///dir
					flist.push_back(ptr->d_name);
			}

			closedir(dir);
			sort(flist.begin(), flist.end());
			return true;
		}

		return false;
	}

	int IsFolderOrFile(const char *path)
	{
		struct stat s_buf;
		memset(&s_buf, 0, sizeof(s_buf));

		if (0 == stat(path, &s_buf))
		{
			if (S_ISDIR(s_buf.st_mode))
				return 1;
			else if (S_ISREG(s_buf.st_mode))
				return 0;
		}

		return -1;
	}

	bool DoCopy(const std::string &srcpath, const std::string &destfolder)
	{
		/** @determine whether the destination folder path exists */
		if (IsFolderExist(destfolder.c_str()))
		{
			std::string destfilepath;
			std::vector<std::string> flist;
			std::vector<std::pair<std::string, std::string>> copyflist;

			/** @get the file name or folder/directory name to be copied */
			int pos = srcpath.rfind('/');
			destfilepath.assign(srcpath, pos, strlen(srcpath.c_str()) - pos + 1);
			destfilepath = destfolder + destfilepath;

			/** @check whether the source path is a file or a folder */
			if (1 == IsFolderOrFile(srcpath.c_str()))
			{
				/** @create the same folder under dest folder */
				CreateFolder(destfilepath.c_str());

				/** @srcpath is a folder and get all sub-files/folders */
				FolderSort(srcpath.c_str(), flist, false);

				/** @sort the source file to be copy into list */
				for (auto iter = flist.begin(); iter != flist.end(); iter++)
				{
					std::string src = srcpath + std::string("/") + *iter;
					std::string dst = destfilepath + std::string("/") + *iter;

					/** @check whether the src is folder/directory still */
					if (1 == IsFolderOrFile(src.c_str()))
					{
						/** @src is folder, and create the same folder under dst path */
						CreateFolder(dst.c_str());
						/** @process recursively, copy files under src folder to dst folder */
						DoCopy(src, destfilepath);
					}
					else if (0 == IsFolderOrFile(src.c_str()))
					{
						/** @src is file */
						copyflist.push_back(std::pair<std::string, std::string>(src, dst));
					}
				}
			}
			else if (0 == IsFolderOrFile(srcpath.c_str()))
			{
				/** @srcpath is a file */
				copyflist.push_back(std::pair<std::string, std::string>(srcpath, destfilepath));
			}
			else
			{
				/** @srcpath is not a valid path */
				return false;
			}

			/** @copy from source path to dest folder */
			for (auto iter = copyflist.begin(); iter != copyflist.end(); iter++)
			{
				FILE *fp1 = fopen(iter->first.c_str(), "r");
				if (!fp1)
				{
					continue;
				}

				FILE *fp2 = fopen(iter->second.c_str(), "w");
				if (!fp2)
				{
					fclose(fp1);
					continue;
				}

				char buff[1024] = { '\0' };
				int count = 0;
				while ((count = fread(buff, 1, 1024, fp1)) != 0)
				{
					fwrite(buff, 1, count, fp2);
					fflush(fp2);
				}

				fclose(fp1);
				fclose(fp2);
			}

			return true;
		}
		return false;
	}

	bool DelFoldersOverMax(const char *foldername, const unsigned int maxnum)
	{
		/** @get the sorted names of all subdirectories of the specified directory*/
		std::vector<std::string> tmp;
		FolderSort(foldername, tmp);

		/**@determine whether the number of subdirectory exceeds the specified maximum number */
		if (tmp.size() > maxnum)
		{
			for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end() - maxnum; )
			{
				std::string filename = foldername + std::string("/") + *it;
				if (!FolderDelete(filename))
				{
					return false;
				}
				it = tmp.erase(it);
			}
		}

		return true;
	}

	bool DeleteEarliestFolder(const char *foldername)
	{
		if (nullptr != foldername)
		{
			std::vector<std::string> tmp;
			if (FolderSort(foldername, tmp))
			{
				if (tmp.size() > 0)
				{
					std::vector<std::string>::iterator it = tmp.begin();
					std::string filename = foldername + std::string("/") + *it;
					return FolderDelete(filename);
				}
			}
		}
		return false;
	}

	unsigned int GetDiskSpaceValue(const char *diskname)
	{
		if (nullptr != diskname)
		{
			struct statfs diskInfo;
			statfs(diskname, &diskInfo);

			unsigned long long totalBlocks = diskInfo.f_bsize;
			unsigned long long totalSize = totalBlocks * diskInfo.f_blocks;
			unsigned long long freeDisk = diskInfo.f_bfree * totalBlocks;

			return static_cast<unsigned int>(freeDisk >> 20);
		}
		return 0;
	}

	bool IsDiskSpaceIsFull(const char *diskname, const unsigned int minvalue)
	{
		return GetDiskSpaceValue(diskname) < minvalue;
	}

	unsigned int GetFolderSize(const char *foldername)
	{
		DIR *dp;
		struct dirent *entry;
		struct stat s_buf;
		unsigned long long totalsize = 0;

		if (nullptr != (dp = opendir(foldername)))
		{
			/** @add the size of its own directory first */
			lstat(foldername, &s_buf);
			totalsize += s_buf.st_size;

			while (nullptr != (entry = readdir(dp)))
			{
				char subdir[256] = { 0 };
				sprintf(subdir, "%s%s", foldername, entry->d_name);
				lstat(subdir, &s_buf);

				if (S_ISDIR(s_buf.st_mode))
				{
					if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
						continue;

					long long subdirsize = GetFolderSize(subdir);
					totalsize += subdirsize;
				}
				else
					totalsize += s_buf.st_size;
			}

			closedir(dp);
		}

		return static_cast<unsigned int>(totalsize >> 20);
	}

	unsigned int GetFileSize(const char *filename)
	{
		struct stat s_buf;
		memset(&s_buf, 0, sizeof(s_buf));

		if (0 == stat(filename, &s_buf))
			return static_cast<unsigned int>(s_buf.st_size >> 20);
		return 0;
	}

	bool get_file_md5(const char *filename, std::string &md5_value)
	{
		md5_value.clear();

		std::ifstream file(filename, std::ifstream::binary);
		if (!file)	return false;

		MD5_CTX md5Context;
		MD5_Init(&md5Context);

		char buf[1024 * 16];
		while (file.good()) {
			file.read(buf, sizeof(buf));
			MD5_Update(&md5Context, buf, file.gcount());
		}

		unsigned char result[MD5_DIGEST_LENGTH];
		MD5_Final(result, &md5Context);

		char hex[35];
		memset(hex, 0, sizeof(hex));
		for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
		{
			sprintf(hex + i * 2, "%02x", result[i]);
		}
		hex[32] = '\0';
		md5_value = string(hex);

		return true;
	}


	std::string GetLocalDate(void)
	{
		time_t t = time(0);
		tm *ld = localtime(&t);
		char tmp[64] = { 0 };
		strftime(tmp, sizeof(tmp), "%Y-%m-%d", ld);
		return std::string(tmp);
	}

	std::string GetLocalTime(void)
	{
		time_t t = time(0);
		tm *ld = localtime(&t);
		char tmp[64] = { 0 };
		strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", ld);
		return std::string(tmp);
	}

	void GetSysDateAndTime(char *sysdate, char *systime)
	{
		time_t t = time(0);
		tm *ld = localtime(&t);
		char tmp[64] = { 0 };

		if (nullptr != sysdate)
		{
			memset(tmp, 0, sizeof(tmp));
			strftime(tmp, sizeof(tmp), "%Y.%m.%d", ld);
			strcpy(sysdate, tmp);
		}

		if (nullptr != systime)
		{
			memset(tmp, 0, sizeof(tmp));
			strftime(tmp, sizeof(tmp), "%H.%M.%S", ld);
			strcpy(systime, tmp);
		}
	}
}