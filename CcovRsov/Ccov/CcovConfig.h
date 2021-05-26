/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  CcovConfig.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _CCOV_CONFIG_H
#define _CCOV_CONFIG_H


/* ATP发给CCOV的消息 */
typedef struct
{
	unsigned char CurLineNum;					/* 当前线路编号 */
	unsigned char NextLineNum;					/* 下一线路编号 */
	unsigned char NextLineVer[4];				/* 下一线路数据版本号 */
	unsigned char NextLineMd5[16];			/* 下一线路数据MD5码 */
	unsigned char VobcProperties; 				/* 加入VOBC端属性，55:控制端 AA:等待端 */
	unsigned char arc_id; 								/* 控制端使用的ArcnetID */
	unsigned short LocalDevName;				/* 本地设备名称 */
	unsigned short TcpCtrlMsgLen;				/* TCP控制报文长度 */
	unsigned char TcpCtrlMsg[200];			/* TCP控制报文 */
	unsigned char IsZeroSpeed;					/* 是否零速，0xaa表示不零速，0x55表示零速 */
	unsigned char IsLocation;						/* 是否有位置，0xaa表示无位置，0x55表示有位置 */
	unsigned char PreviewResults;				/* 预检结果，0xaa表示预检失败，0x55表示预检成功 */
}ATP_TO_CCOV_STRU;


/* CCOV发给ATP的消息 */
typedef struct
{
	unsigned char CurlineNum;
	unsigned char NextLienNum;
	unsigned char NextLineVer[4];
	unsigned char IsSwitchAvailable;
	unsigned char MMI_Show_Restart;		/*通知MMI重启:  0xaa-表示不重启;  0x55-表示重启*/
	unsigned char UpdateRecv;					/*更新请求:  0xaa-表示不更新; 0x55-表示更新*/
	unsigned char StateRecv;						/*状态请求:  0xaa-表示未请求; 0x55-表示请求*/
}CCOV_TO_ATP_STRU;


/*数据下载加载以及切换控制结构体*/
typedef struct
{
	unsigned char NdsuAskCtrl;				/*0x55开始请求校验信息,0XAA已经获取到校验信息,0xff不允许求情*/
	unsigned char VdsuDownCtrl;			/*0x55允许下载,0xaa已经下载完成;0xff不允许下载*/
	unsigned char DataSwitchCtrl;			/*0x55切换成功,0xaa切换失败,0xff默认值*/
	unsigned char DataLoadCtrl;				/*0x55允许加载,0XAA加载完成,0xff不允许加载*/
	unsigned char LoadSta;						/*0x00加载失败;0x01加载成功;0x02加载中;0xff未知状态*/
}HEATE_LOAD_CTRL_STRUCT;


#endif  /*_CCOV_CONFIG_H*/