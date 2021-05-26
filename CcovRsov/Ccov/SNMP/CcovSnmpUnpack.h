/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  CcovSnmpUnpack.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _CCOV_SNMP_UNPACK_H
#define _CCOV_SNMP_UNPACK_H

#include "CcovSnmpConfig.h"


/*
*输入参数：    const unsigned char *psrcCodeOidStr,输入编码后的OID信息;
*              const unsigned short srcCodeOidStrLen,输入编码后的OID信息的长度;
*输出参数：    unsigned char *poidDecodeStr,解码后的OID信息,信息以‘.’符号间隔表示;
*返回值：      解码后的OID信息总共所占的字节数;
**/
unsigned short OidDecode(unsigned char *pOidDecodeStr, const unsigned char *pSrcCodeOidStr, const unsigned short srcCodeOidStrLen);


/*
*输入参数：    const unsigned int oidFir2NodeSum,OID第一个和第二个节点值的BER和;
*输出参数：    unsigned char *pOidFirNode,OID第一个节点的值的地址;
*              unsigned char *pOidSndNode,OID第二个节点的值的地址;
*返回值：      解码后的OID信息总共所占的字节数;
**/
char OidFir2NodeDecode(unsigned char *pOidFirNode, unsigned char *pOidSndNode, const unsigned int oidFir2NodeSum);

/*
*函数功能:     将数值形式的整型数表示为字符串形式的整型数;
*输入参数：    const unsigned int oidNodeVal,解码后的OID节点整型信息;
*输出参数：    unsigned char *pOidNodeStr,以字符串表示的解码后的OID节点信息;
*返回值：      解码后的OID信息总共所占的字节数;
**/
unsigned char OidNodeDecode(unsigned char *pOidNodeStr, const unsigned int oidNodeVal);

/*
*输入参数：    const unsigned short oidNodeVal,解码后OID整型节点值;
*              const unsigned char dotKey,值为0x00时,在解码后OID字符串添加‘.’;0x01时,不添加'.';
*输出参数：    unsigned char *poidDecodeStr,解码后OID字符串;
*输入输出参数：unsigned int *pDecodeStrNo,解码后OID字符串中的下标,调用本函数后会自动更新;
*返回值：      调用本函数后,OID字符串中的下标更新后的值;
*说明：        调用oidNodeDecode函数;
**/
unsigned short OidNodeValDecode(unsigned char *poidDecodeStr, unsigned int decodeStrNo, const unsigned short oidNodeVal, const unsigned char dotKey);


/*
*函数功能:     将数值形式的整型数表示为字符串形式的整型数;
*输入参数：    const unsigned int uintVal,整型数;
*输出参数：    unsigned char *pUintStr,以字符串表示的整型数;
*返回值：      字符串表示的整型数所占的字节数;
**/
unsigned char Uint32ToUchar(unsigned char *pUintStr, const unsigned int uintVal);


/*
*输入参数：    const unsigned char *lenCodeStr,长度信息使用BER编码后的信息
*输出参数：    unsigned short length,长度值
*返回值：      长度信息使用BER编码后所占字节数
**/
unsigned char LenDecode(unsigned short *pLength, const unsigned char *pLenCodeStr);

/*
*输入参数：    const unsigned char *pIntValStr,整型数BER信息
*              const unsigned char intStrLen,整型数BER信息的字节数
*输出参数：    unsigned int *pInteger,整型数地址
*返回值：      解码成功标志
*              0x00:失败
*              0x01:成功
**/
char IntTypeDecode(int *pInteger, const unsigned char *pIntValStr, const unsigned char intStrLen);

/*
*输入参数：    const unsigned char *puIntValStr,整型数BER信息
*              const unsigned char uintStrLen,整型数BER信息的字节数
*输出参数：    int *puInteger,整型数地址
*返回值：      解码成功标志
*              0x00:失败
*              0x01:成功
**/
char UintTypeDecode(unsigned int *puInteger, const unsigned char *puIntValStr, const unsigned char uintStrLen);

/*
*输入参数：    const unsigned short strNo,响应报文字符串的下标;
*              const unsigned char *pResponsStr,响应报文字符串的首地址;
*输出参数：    SEQ_STRU *pSeqStru,解码后的序列类型和长度信息;
*              unsigned short *pErrPosition,解析响应报文时,出现解析错误的位置;
*
*返回值：      调用本函数后,响应报文字符串的下标更新后的值;
*说明：        调用lenDecode函数;
**/
unsigned short SeqTagDecode(SEQ_STRU *pSeqStru, unsigned short *pErrPosition, const unsigned short strNo, const unsigned char *pResponsStr);

/*
*输入参数：    const unsigned char *pIpCodeStr,BER编码后的IP信息;
*              const unsigned char ipCodeLen,BER编码后的IP信息占的字节数;
*输出参数：    unsigned char *pIpInfoStr,解码后的IP信息;
*返回值：      解码后ip信息占的字节数;
*函数调用：    oidNodeDecode函数
**/
unsigned char IpInfoDecode(unsigned char *pIpInfoStr, const unsigned char *pIpCodeStr, const unsigned char ipCodeLen);


#endif  /*_CCOV_SNMP_UNPACK_H*/
