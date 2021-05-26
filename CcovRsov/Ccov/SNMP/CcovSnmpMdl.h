/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  CcovSnmpMdl.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _CCOV_SNMP_MDL_H
#define _CCOV_SNMP_MDL_H

#include "CcovSnmpPack.h"
#include "CcovSnmpUnpack.h"


/*
*输入参数：    const unsigned char reqType,请求的类型,支持Get、GetNext请求;
*              enum bandNumber bandVarNum,绑定变量个数;
*              const REQ_ARGV_STRU *preqArgvStru,请求组帧输入参数结构体;
*输出参数：    unsigned char *pReqCode, 整个请求帧编码后的字符串;
*返回值：      整个请求帧编码后的总长度;
*函数调用：    ReqIdCode函数;
*              LenCode函数;
*              OidCode函数;
*关键量:       BANDNUM:3,参考CcovSnmpMdl.h文件,如修改它的宏定义值,请对应修改和BANDNUM
*              相关的变量的初始化;
*使用条件:     1个请求报文最多支持4个OID,可以选择1个、或2个、或3个OID、或4个OID;
**/

unsigned short RequestFlameCode(unsigned char *pReqCode, const unsigned char reqType, enum bandNumber bandVarNum, const REQ_ARGV_STRU *preqArgvStru);

/*
*输入参数：    const unsigned char *pRespStr,待解码的响应信息帧;
*              const unsigned short recvLen,响应信息帧占的字节数;
*输出参数：    RESPONSE_INFO_NEWSTRU *pRespInfoStru, 存放响应信息的结构体;
*返回值：      返回已解码的报文的长度;
*函数调用：    LenDecode     函数;
*              UintTypeDecode函数;
*              SeqTagDecode  函数;
*              OidDecode     函数;
*              IntTypeDecode 函数;
*              IpInfoDecode  函数;
*
*关键量:       BANDNUM:3,参考CcovSnmpMdl.h文件,如修改它的宏定义值,请对应修改和BANDNUM
*              相关的变量的初始化;
*使用条件:     与请求编码函数相对应;
**/
unsigned short ResponseDecode(RESPONSE_INFO_NEWSTRU *pRespInfoStru, const unsigned char *pRespStr, const unsigned short recvLen);


#endif  /*_CCOV_SNMP_MDL_H*/