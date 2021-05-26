/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  HlhtConfig.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _HLHT_CONFIG_H
#define _HLHT_CONFIG_H


/*文件信息*/
typedef struct SingFileInfo
{
	char	 file_path[30];		    								/* 声明文件路径 */
	unsigned char File_LineNum[2];					/* 文件所属线路编号 */
	unsigned char File_SubSystem[2];	    		/* 子系统线路数据类型 (0x0001 指 ATP,0x0002 指 ATO) */
	unsigned char File_DataVer[4];						/* 16字节的数据版本号 （从V-DSU收到的最新的数据版本号）（版本号请求的时候不需要线路数据版本号） */
	unsigned char File_SuccessFlag;					/* 文件传输状态标志 0x00:就绪(上一个文件(或版本号)传输完成) 0x01:更新中(正在传输当前文件(或版本号)) */
}SingleFileInfo;

/* 文件属性结构体 */
typedef struct
{
	unsigned int DataLength;						/* 整帧报文的长度信息*/
	unsigned short SubSystemType;						/*子系统类型信息*/
	unsigned char File_TrainGroup[2];				/* 文件所属列车车组号 */
	unsigned char File_IsApply;						/* 是否请求文件下载 请求帧类型 0x01:数据版本号请求帧 0x02:数据内容请求帧 0x03:全部数据版本号请求帧 */
	unsigned short File_MaxPackLength;				/* 单包传送大小(已字节为单位) */
	unsigned char File_SuccessFlag;					/* 文件传输状态标志 0x01:就绪(上一个文件(或版本号)传输完成) 0x02:更新中(正在传输当前文件(或版本号)) 0x03:下载失败*/
	unsigned char File_FileNum;						/* 需要传送文件个数 */
	unsigned int File_Size;							/* 文件大小(已字节为单位) */
	unsigned short File_ReadTimes;					/* 文件需要读取次数 */
	unsigned short File_ReadCurTimes;					/* 文件已读取次数从0开始计数 */
	unsigned int		 File_FramTotalNum;					/* 文件传输总帧数 */
	unsigned int		 File_FramNum;						/* V-DSU传输文件帧序号 */
	unsigned int		 File_FileContentNum;				/* 传输文件内容字节数 */
	char		 File_FileContent[10240]; 			/* 传输文件内容 */
	SingleFileInfo	Single_FileInfo[50]; 	/*单帧文件信息结构体*/
	unsigned char		 File_Md5Code[16];					/* 文件生成的MD5码 */
	unsigned int 		 File_CrcCode;
	unsigned int       CalcrcCode;						/*本地计算的CRC*/
	unsigned char        File_LoadDataAre;					/* 	是否允许VDSU向NDSU进行数据下载：	0x55:允许（库线、试车线、跨线重叠区域）	0xaa:不允许 */
	unsigned char        Vdsu_status;						/*	VDSU自身的的状态以及全部数据下载状态：
														Bit0:为1表示A系工作正常；0表示工作故障；
														Bit1：为1表示B系工作正常；0表示工作故障；
														Bit2：为1表示A系全部数据更新成功；为0表示未成功；
														Bit3：为1表示B系全部数据更新成功；为0表示未成功；
														0：默认值*/

														/*车载子系统回复的信息*/
	unsigned int      FramNum;  							/*传输的帧序号*/
	unsigned char       singalPakageFlag;
	unsigned char       wholePackageFlag;
}CCOV_FileInfo;


typedef struct
{
	unsigned int AtpCrcCode;
	unsigned char AtpIsFlag;
	unsigned int AtoCrcCode;
	unsigned char AtoIsFlag;
}SUBSYS_CRC;

typedef struct
{
	unsigned char LineNum;								/*收到ATS发来的下一条运行的线路编号*/
	unsigned char NdsuToCcovFsVer[4];					/*NDSU发送给CCOV的需要更新的数据版本号*/
	unsigned char NdsuToCcovFsMd5[16];					/*NDSU发送给CCOV的需要更新的数据Md5码*/

	unsigned char CcovFsSta;							/*CCOV根据线路号查找本地数据结果
													0x01:数据存在;0x02数据不存在;默认值0xff*/

	unsigned char HeatStartCmd;							/*0x55:开始进行热加载;0xff:默认值*/
	unsigned char CcovLoadSta;							/*0x01:热加载成功;0X02:热加载中;0x03:热加载失败;0xff默认值*/
	unsigned char SwitchStartCmd;						/*数据开始切换命令:0x55开始切换;0xaa不切换;0xff默认值*/

	unsigned char CcovSwitchSta;						/*CCOV数据切换状态:0x55切换成功;0xaa切换失败;0xff默认值*/
}SWITCH_STRUCT;

/*1.子子系统向CCOV请求帧结构体
  2.CCOV向VDSU请求帧结构体*/
typedef struct
{
	unsigned int FramSn;											/*帧序号*/
	unsigned char singalPakageFlag;									/*单包更新成功标志:0x00:更新成功0x01:更新超时0x02:数据校验失败0xff:无效值*/
	unsigned char wholePackageFlag;									/*整包数据回复确认,0x00:接收成功;0x01:接收中;0x02:接收失败;0xff:无效*/
	unsigned int FileSize;										/*接收到的文件字节数*/
}ASK_STRUCT;

/*1.CCOV向子子系统回复数据版本号和内容请求结构体
* 2.VDSU向CCOV回复的数据版本号和内容请求结构体*/
typedef struct
{
	unsigned int transFramLength;									/*传输帧长度,不含长度自身*/
	unsigned short trainGroup;										/*列车车组号*/
	unsigned char dataFramType;										/*数据帧类型,0x01数据版本号请求帧;0x02数据内容请求帧*/
	unsigned char dataVerState;										/*数据版本号就绪状态0x01:就绪;0x02:获取中;0x03:获取失败*/
	unsigned short lineNum;											/*回复请求的数据线路编号*/
	unsigned short subSystemType;									/*请求数据的子子系统类型,0x0001:ATP数据类型;0x0002:ATO数据类型;其余预留*/
	unsigned int framTotalNum;									/*文件传输总帧数*/
	unsigned int FramNum;											/*文件传输帧序号*/
	unsigned int fileContentLength;								/*该帧文件内容的长度*/
	unsigned char dataContentBuf[1024];	/*存放接收到的数据内容临时数组*/
	unsigned char Md5CheckCode[16]; 							/*MD5文件校验码*/
	unsigned int crcCode;											/*CRC校验码*/
}REPLY_STRUCT;

#endif  /*_HLHT_CONFIG_H*/
