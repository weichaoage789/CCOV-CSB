#include "AppDefine.h"
#include "AppConfig.h"

#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/mount.h>

#include "Utilities/FileWork/FileWork.h"
#include "Utilities/FileWork/IniFileWork.h"
#include "Utilities/FileWork/LogFileWork.h"
using namespace FileWork;

#include "USBCtrl.h"


CUSBCtrl::CUSBCtrl() 
	: _Hotplug(0)
	, _RunningFlag(false)
	, _UsbStatus(USB_STATUS::STATE_IDLE)
	,_CopyStatus(COPY_STATUS::STATE_NORMAL)
{
	memset(&_TransCfg, 0, sizeof(USB_TRANSCFG));
	_RunningFlag = init_hotplug_sock();
}

bool CUSBCtrl::get_usbtrans_cfg()
{
	if (IsFileExist(USB_CONFIG_PROFILE))
	{
		_TransCfg.transmode = ReadIntValue(USB_TRANS, TRANS_MODE, USB_CONFIG_PROFILE);

		if (1 == _TransCfg.transmode)
		{
			_TransCfg.transnum = ReadIntValue(USB_TRANS, TRANS_NUM, USB_CONFIG_PROFILE);
		}
		else if (3 == _TransCfg.transmode)
		{
			ReadStringValue(USB_TRANS, TRANS_DATE, _TransCfg.transdate, USB_CONFIG_PROFILE);
			_TransCfg.starttime = ReadIntValue(USB_TRANS, START_TIME, USB_CONFIG_PROFILE);
			_TransCfg.endtime = ReadIntValue(USB_TRANS, END_TIME, USB_CONFIG_PROFILE);
		}

		return true;
	}
	return false;
}

bool CUSBCtrl::init_hotplug_sock()
{
	int on = 1;
	const int buffersize = 2048;
	
	struct sockaddr_nl snl;
	bzero(&snl, sizeof(struct sockaddr_nl));
	snl.nl_family = AF_NETLINK;
	snl.nl_pid = getpid();
	snl.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR;

	if (-1 == (_Hotplug = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT)))
	{
		LOG(ERROR) << "USB module init_hotplug_sock() socket error ";
		return false;
	}
	
	if (0 > setsockopt(_Hotplug, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize)))
	{
		LOG(ERROR) << "USB module init_hotplug_sock() setsockopt error ";
		return false;
	}

	if (0 > setsockopt(_Hotplug, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
	{
		LOG(ERROR) << "USB module init_hotplug_sock() setsockopt error ";
		return false;
	}

	if (0 > bind(_Hotplug, (struct sockaddr *) &snl, sizeof(struct sockaddr_nl)))
	{
		LOG(ERROR) << "USB module init_hotplug_sock() bind error ";
		close(_Hotplug);
		return false;
	}

	LOG(INFO) << "USB detecting initialization successfully";
	return true;
}

void CUSBCtrl::Start()
{
	std::thread t_usb(Thread_UsbListener, this);
	t_usb.detach();
}

void CUSBCtrl::Stop()
{
	_RunningFlag = false;
}

bool CUSBCtrl::copy_super(const char *filepath)
{
	if (GetDiskSpaceValue(USBTRANS_DATA_DEST) > GetFolderSize(filepath))
	{
		if (DoCopy(filepath, USBTRANS_DATA_DEST))
		{
			LOG(INFO) << "super mode copy " << filepath << " to USB success";
			return true;
		}
		else
			LOG(ERROR) << "super mode copy " << filepath << " to USB failed";
	}
	else
		LOG(ERROR) << "super mode copy " << filepath << " to USB failed, USB space value is not enough";

	return false;
}

bool CUSBCtrl::copy_fast(const char *filepath)
{
	std::vector<std::string> flist;
	if (FolderSort(filepath, flist))
	{
		/** @the specified number is bigger than the number of folders in the filepath */
		if (_TransCfg.transnum >= flist.size())
			return copy_super(filepath);
		else
		{
			std::string dstFolder = USBTRANS_DATA_DEST + std::string("/") + std::string(CAppCfg::GetInstance()._LocalIp);
			CreateFolder(dstFolder.c_str());

			for (int i = flist.size() - _TransCfg.transnum; i < flist.size(); i++)
			{
				std::string tmp = filepath + std::string("/") + flist.at(i);

				if (GetDiskSpaceValue(USBTRANS_DATA_DEST) > GetFolderSize(tmp.c_str()))
				{
					if (DoCopy(tmp, dstFolder))
						LOG(INFO) << "fast mode copy " << tmp.c_str() << " to USB success";
					else
						LOG(ERROR) << "fast mode copy " << tmp.c_str() << " to USB failed";
				}
				else
					LOG(ERROR) << "fast mode copy " << tmp.c_str() << " to USB failed, USB space value is not enough";
			}
			return true;
		}
	}
	else
		LOG(ERROR) << "fast mode copy to USB failed, FolderSort error";

	return false;
}

bool CUSBCtrl::copy_specify(const char *filepath)
{
	std::vector<std::string> flist;
	if (FolderSort(filepath, flist))
	{
		std::string dstFolder = USBTRANS_DATA_DEST + std::string("/") + std::string(CAppCfg::GetInstance()._LocalIp);
		CreateFolder(dstFolder.c_str());

		for (auto iter = flist.begin(); iter != flist.end(); iter++)
		{
			if (0 == iter->compare(_TransCfg.transdate))
			{
				std::string srcfolder = filepath + std::string("/") + *iter;
				std::string datefolder = dstFolder + std::string("/") + *iter;
				CreateFolder(datefolder.c_str());

				std::vector<std::string> fflist;
				if (FolderSort(srcfolder.c_str(), fflist, false))
				{
					for (auto it = fflist.begin(); it != fflist.end(); it++)
					{
						std::string tmp;
						tmp.assign(*it, 0, 2);
						int t_hour = atoi(tmp.c_str());

						if ((_TransCfg.starttime <= t_hour) && (t_hour <= _TransCfg.endtime))
						{
							std::string datefile = srcfolder + std::string("/") + *it;
							
							if (GetDiskSpaceValue(USBTRANS_DATA_DEST) > GetFileSize(datefile.c_str()))
							{
								if (DoCopy(datefile, datefolder))
									LOG(INFO) << "specified mode copy " << datefile.c_str() << " to USB success";
								else
									LOG(ERROR) << "specified mode copy " << datefile.c_str() << " to USB failed";
							}
							else
								LOG(ERROR) << "specified mode copy " << datefile.c_str() << " to USB failed, USB space value is not enough";
						}
					}
				}

				return true;
			}
		}
	}
	else
		LOG(ERROR) << "specified mode copy to USB failed, FolderSort error";
	return false;
}

bool CUSBCtrl::copy_to_udisk(const char *filepath)
{
	if (nullptr != filepath && IsFolderExist(filepath))
	{
		switch (_TransCfg.transmode)
		{
		case USBTRANS_MODE_FAST:
			return copy_fast(filepath);

		case USBTRANS_MODE_SUPER:
			return copy_super(filepath);

		case USBTRANS_MODE_SPECIFY:
			return copy_specify(filepath);
			
		default:
			break;
		}
	}

	return false;
}

void CUSBCtrl::Thread_UsbListener(void *arg)
{
	if (nullptr == arg)
	{
		return;
	}
	CUSBCtrl *usbctrl = (CUSBCtrl *)arg;

	char buf[256] = { 0 };
	LOG(INFO) << "USB Listening thread start, thread id = " << syscall(SYS_gettid);

	while (usbctrl->_RunningFlag)
	{
		memset(buf, 0, sizeof(buf));
		if (0 < recv(usbctrl->_Hotplug, &buf, sizeof(buf), 0))
		{
			/** @USB detecting */
			std::string tmp = buf;
			int npos = tmp.rfind("/sd");
			if(npos == -1) { continue; }

			tmp.assign(tmp, npos, strlen(tmp.c_str()) - npos);
			if (tmp.length() < 5)	{	continue;	}

			if (0 == memcmp(buf, "add@", 4))
			{
				if (0 == memcmp(tmp.c_str(), "/sd", 3))
				{
					LOG(INFO) << "add U Disk : " << tmp.c_str();
					usbctrl->_UsbStatus = USB_STATUS::STATE_ADD;

					char cmd[128] = { 0 };
					sprintf(cmd, "/dev%s", tmp.c_str());
					
					if (!usbctrl->my_mount(cmd))
					{
						usbctrl->_CopyStatus = COPY_STATUS::STATE_FAIL;
						continue;
					}
					else
					{
						/** @mount successful */
						if (usbctrl->get_usbtrans_cfg())
						{
							memset(cmd, 0, sizeof(cmd));
							sprintf(cmd, "%s/%s", USBTRANS_DATA_SRC, CAppCfg::GetInstance()._LocalIp);

							if (usbctrl->copy_to_udisk(cmd))
							{
								LOG(INFO) << "file copy success.";
								usbctrl->_CopyStatus = COPY_STATUS::STATE_SUCCESS;
							}
							else
							{
								LOG(ERROR) << "file copy fail.";
								usbctrl->_CopyStatus = COPY_STATUS::STATE_FAIL;
							}
						}
						else
						{
							LOG(ERROR) << "USB_CONFIG_PROFILE doesn't exist";
							usbctrl->_CopyStatus = COPY_STATUS::STATE_FAIL;
						}

						/** @umount */
						memset(cmd, 0, sizeof(cmd));
						sprintf(cmd, "%s", "/mnt");
						umount(cmd);
					}
				}
			}
			else if (0 == memcmp(buf, "remove@", 7))
			{
				if (0 == memcmp(tmp.c_str(), "/sd", 3))
				{
					LOG(INFO) << "remove U Disk : " << tmp.c_str();
					usbctrl->_UsbStatus = USB_STATUS::STATE_REMOVE;
					usbctrl->_CopyStatus = COPY_STATUS::STATE_NORMAL;
				}
			}
		}

		usleep(10000);
	}

	LOG(INFO) << "USB Listening thread stop";
	return;
}

bool CUSBCtrl::my_mount(const char *info)
{
	if (-1 == mount(info, "/mnt", "vfat", MS_NOATIME, NULL))
	{
		if (errno == EBUSY) {
			LOG(ERROR) << "USB mount point busy";
			return false;
		}
		else {
			LOG(ERROR) << "USB mount error : " << strerror(errno);
			return false;
		}
	}
	else 
		LOG(INFO) << "USB mount successful";
	return true;
}

