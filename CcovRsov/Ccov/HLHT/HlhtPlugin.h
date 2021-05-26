/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  HlhtPlugin.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _CCOV_HLHT_H
#define _CCOV_HLHT_H

#include "HlhtConfig.h"


#define FS_STORAGE_DIRECTORY    (char *)"/Fsdir"     /*配置文件路径*/

/*数据请求帧回复类型*/
#define DATAVER_REPLY_TYPE				0x01			/*数据版本号请求帧回复类型*/
#define DATACONTENT_REPLY_TYPE 			0x02			/*数据内容下载请求帧回复类型*/
#define AllDATAVER_REPLY_TYPE			0x03			/*全部数据版本号请求帧回复类型*/

/*数据请求帧类型*/
#define DATAVER_ASK_TYPE				0x01			/*数据版本号请求帧类型*/
#define DATACONTENT_ASK_TYPE 			0x02			/*数据内容下载请求帧类型*/
#define AllDATAVER_ASK_TYPE				0x03			/*全部数据版本号请求*帧类型*/
#define CONFIRM_TYPE					0x04			/*确认回复帧类型*/
#define DATAVERANDMD5_ASK_TYPE			0x05		/*数据版本号和MD5码请求帧类型*/
#define NDSU2ATP_DATAVERANDMD5_REPLY_TYPE 0x06		/*NDSU给ATP数据版本号和MD5回复帧类型*/
#define DATAVERANDMD5_REPLY_TYPE		0x07		/*数据版本号和MD5回复帧类型*/

#define DOWNLOAD_FROM_VDSU_LENGTH 		1024

/*数据准备完成情况*/
#define DATA_DOWNLOAD_SUCESS			0x01			/*数据下载成功*/
#define DATA_DOWNLOADING				0x02			/*数据下载中*/
#define DATA_DOWNLOAD_FAULT				0x03			/*数据下载失败*/
/*数据传输数状态*/
#define DATA_TRANSLATE_SUCESS			0x00			/*单包或整包数据下载成功*/
#define DATA_TRANSLATEING				0x01			/*整包数据下载成功*/
#define DATA_TRANSLATE_FALUT			0x02			/*单包或整包数据下载失败*/

/*子系统数据请求阶段*/
#define DATA_VERSION_ASK   				0x01			/*数据版本号请求*/
#define DATA_CONTENT_ASK			    0x02			/*数据内容下载请求*/
#define DATA_CONTENT_CONFIRM			0x03			/*数据下载过程中*/


/*子子系统数据更新情况*/
#define UPDATA_SUCCED					0x00			/*数据更新成功*/
#define SINGAL_FRAM_CHECK_FAULT			0x01			/*单帧校验失败*/


/*子子系统数据类型*/
#define ATP_SUBSYSTEM_TYPE				0x0001	 /*请求系统为ATP子子系统类型*/
#define ATO_SUBSYSTEM_TYPE				0x0002	 /*请求系统为ATO子子系统类型*/
#define CCOV_SUBSYSTEM_TYPE				0x0003   /*请求系统为CCOV子子系统类型*/



class CHLHTCtrl
{
private:
	CHLHTCtrl();

public:
	~CHLHTCtrl() {}

	static CHLHTCtrl &Getinstance()
	{
		static CHLHTCtrl instance;
		return instance;
	}

public:
	void SubsysDataHandle(unsigned char *data, unsigned char systype);

private:
	CCOV_FileInfo _Atp1FInfo;
	CCOV_FileInfo _Atp2FInfo;
	CCOV_FileInfo _Atp3FInfo;
	CCOV_FileInfo _Ato1FInfo;
	CCOV_FileInfo _Ato2FInfo;

	/* ATP数据下载成功标志。0x01：下载成功；0x02：下载中；0x03：下载失败 */
	unsigned char _AtpDLoadState;

	/* ATO数据下载成功标志。0x01：下载成功；0x02：下载中；0x03：下载失败 */
	unsigned char _AtoDLoadState;
};


#endif  /*_CCOV_HLHT_H*/