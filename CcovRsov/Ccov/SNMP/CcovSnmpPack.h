/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  CcovSnmpPack.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _CCOV_SNMP_PACK_H
#define _CCOV_SNMP_PACK_H

#include "AppDefine.h"

/*
*输入参数：    const unsigned short length,编码信息占的字节数;
*输出参数：    unsigned char *pLenCodeStr,长度信息使用BER编码后的信息;
*返回值：      长度信息使用BER编码后所占字节数;
**/
unsigned char LenCode(unsigned char *pLenCodeStr, const unsigned short length);

/*
*输入参数：    const unsigned char *pSrcOidStr
*输出参数：    unsigned char *pOidCodeStr,包含OID的编码信息和OID编码后信息的长度使用BER编码的信息
*返回值：      OID编码信息及信息的长度使用BER编码后总共所占的字节数;
**/
unsigned short OidCode(unsigned char *pOidCodeStr, const unsigned char *pSrcOidStr);

/*
*输入参数：    const unsigned int oidNum
*输出参数：    unsigned char *pOidNumCodeStr,OID辅助节点值使用BER编码后的的字符串
*返回值：      OID使用BER编码后所占的字节数;
**/
unsigned char OidUint2Uchar(unsigned char *pOidNumCodeStr, const unsigned int oidNum);

/*
*输入参数：    const unsigned int oidFirNode
*              const unsigned int oidSndNode
*输出参数：    unsigned char *pOidNodesSumStr,OID前两个辅助节点整数值之和,使用BER编码后的的字符串
*返回值：      OID使用BER编码后所占的字节数;
*说明：        调用了oidUint2Uchar进行编码;
*              此函数暂时未被使用;
*
**/
unsigned char OidFir2NodesCode(unsigned char *pOidNodesSumStr, const unsigned int oidFirNode, const unsigned int oidSndNode);

/*
*功能描述：    将字符串形式OID节点值转化为整型OID节点值
*输入参数：    const unsigned char *pOidNumStr,
*输出参数：    unsigned int *pOidNum,OID辅助节点值
*返回值：      0：转化失败； 1：转存成功;
**/
int OidStr2Uint(unsigned int *pOidNum, const unsigned char *pOidNumStr);

/*
*功能描述： 将2个字节长的整型变为字节表示  BIG endian
*输入参数： const unsigned short Input,整型数;
*输出参数： unsigned char *pOutput,字节表示的整型数;
*返回值：   输出字符串有效字节数
*/
unsigned char Uint16ToChar(unsigned char *pOutput, const unsigned short Input);

/*
*功能描述： 将整型数变为字节表示  BIG endian
正数：首字节为00,负数:保存符号位外的数值部分,忽略符号;
*输入参数： const unsigned int reqId,请求ID整型值;
*输出参数： unsigned char * pReqIdStr,请求ID编码后的字符串;
*限制条件： 1、Request ID 范围：[0,RAND_MAX]
*           2、由于输出的字符串的首字节可以为'\0',在执行字符串复制操作时,得使用memcpy,
*           不能使用strcpy;
*返回值：   请求ID编码后的长度
*
*/
unsigned char ReqIdCode(unsigned char * pReqIdStr, const unsigned int reqId);

/*
*功能描述： 将整型数变为字节表示  BIG endian
正数：首字节为00,负数:保存符号位外的数值部分,忽略符号;
*输入参数： const UINT64 reqId,请求ID整型值;
*输出参数： unsigned char * pReqIdStr,请求ID编码后的字符串;
*限制条件： 1、Request ID 范围：[0,REQID_MAX],增强处理范围为0xffffffff;
*           2、由于输出的字符串的首字节可以为'\0',在执行字符串复制操作时,得使用memcpy,
*           不能使用strcpy;
*返回值：   请求ID编码后的长度
*
*/
unsigned char u64ReqIdCode(unsigned char * pReqIdStr, const UINT64 reqId);


#endif  /*_CCOV_SNMP_PACK_H*/