/************************************************************************
* 文件名	：  vSfpInnetDataVar.h
* 版权说明	：   
* 版本号	：  v1.0  
* 创建时间	：  2009.11.12
* 作者		：	刘宏杰、李雁
* 功能描述	：  VOBC内网各子系统间通信使用SFP协议时的数据库文件
* 使用注意	：  本文件属于VOBC公共文件,暂由刘宏杰维护(感谢李雁帮忙提供初稿)
				该头文件供集成人员调用
* 修改记录	：	无
************************************************************************/
#ifndef _V_SFP_INNET_DATA_VAR_H_
#define _V_SFP_INNET_DATA_VAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "CommonTypes.h"

/*ATP、ATO、MMI间通信时,SFP协议使用的数据库数组:
*暂定两端ATP分别当作ZC1和ZC2,ZC1为发起方
*ATO为CI3,MMI为CI4,CCOV为CI5(CCOV仅为预留,暂无通信的需求).ZC与CI通信时ZC为发起方
*总结各子系统ID为:两端ATP分别为0x5501,0x5502;ATO为0x7103;MMI为0x7104;CCOV为0x7105*/
extern const UINT8 vSfpInnetDataBuf[];

#ifdef __cplusplus
}
#endif

#else

#endif

