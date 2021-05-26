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
*���������    const unsigned short length,������Ϣռ���ֽ���;
*���������    unsigned char *pLenCodeStr,������Ϣʹ��BER��������Ϣ;
*����ֵ��      ������Ϣʹ��BER�������ռ�ֽ���;
**/
unsigned char LenCode(unsigned char *pLenCodeStr, const unsigned short length);

/*
*���������    const unsigned char *pSrcOidStr
*���������    unsigned char *pOidCodeStr,����OID�ı�����Ϣ��OID�������Ϣ�ĳ���ʹ��BER�������Ϣ
*����ֵ��      OID������Ϣ����Ϣ�ĳ���ʹ��BER������ܹ���ռ���ֽ���;
**/
unsigned short OidCode(unsigned char *pOidCodeStr, const unsigned char *pSrcOidStr);

/*
*���������    const unsigned int oidNum
*���������    unsigned char *pOidNumCodeStr,OID�����ڵ�ֵʹ��BER�����ĵ��ַ���
*����ֵ��      OIDʹ��BER�������ռ���ֽ���;
**/
unsigned char OidUint2Uchar(unsigned char *pOidNumCodeStr, const unsigned int oidNum);

/*
*���������    const unsigned int oidFirNode
*              const unsigned int oidSndNode
*���������    unsigned char *pOidNodesSumStr,OIDǰ���������ڵ�����ֵ֮��,ʹ��BER�����ĵ��ַ���
*����ֵ��      OIDʹ��BER�������ռ���ֽ���;
*˵����        ������oidUint2Uchar���б���;
*              �˺�����ʱδ��ʹ��;
*
**/
unsigned char OidFir2NodesCode(unsigned char *pOidNodesSumStr, const unsigned int oidFirNode, const unsigned int oidSndNode);

/*
*����������    ���ַ�����ʽOID�ڵ�ֵת��Ϊ����OID�ڵ�ֵ
*���������    const unsigned char *pOidNumStr,
*���������    unsigned int *pOidNum,OID�����ڵ�ֵ
*����ֵ��      0��ת��ʧ�ܣ� 1��ת��ɹ�;
**/
int OidStr2Uint(unsigned int *pOidNum, const unsigned char *pOidNumStr);

/*
*���������� ��2���ֽڳ������ͱ�Ϊ�ֽڱ�ʾ  BIG endian
*��������� const unsigned short Input,������;
*��������� unsigned char *pOutput,�ֽڱ�ʾ��������;
*����ֵ��   ����ַ�����Ч�ֽ���
*/
unsigned char Uint16ToChar(unsigned char *pOutput, const unsigned short Input);

/*
*���������� ����������Ϊ�ֽڱ�ʾ  BIG endian
���������ֽ�Ϊ00,����:�������λ�����ֵ����,���Է���;
*��������� const unsigned int reqId,����ID����ֵ;
*��������� unsigned char * pReqIdStr,����ID�������ַ���;
*���������� 1��Request ID ��Χ��[0,RAND_MAX]
*           2������������ַ��������ֽڿ���Ϊ'\0',��ִ���ַ������Ʋ���ʱ,��ʹ��memcpy,
*           ����ʹ��strcpy;
*����ֵ��   ����ID�����ĳ���
*
*/
unsigned char ReqIdCode(unsigned char * pReqIdStr, const unsigned int reqId);

/*
*���������� ����������Ϊ�ֽڱ�ʾ  BIG endian
���������ֽ�Ϊ00,����:�������λ�����ֵ����,���Է���;
*��������� const UINT64 reqId,����ID����ֵ;
*��������� unsigned char * pReqIdStr,����ID�������ַ���;
*���������� 1��Request ID ��Χ��[0,REQID_MAX],��ǿ����ΧΪ0xffffffff;
*           2������������ַ��������ֽڿ���Ϊ'\0',��ִ���ַ������Ʋ���ʱ,��ʹ��memcpy,
*           ����ʹ��strcpy;
*����ֵ��   ����ID�����ĳ���
*
*/
unsigned char u64ReqIdCode(unsigned char * pReqIdStr, const UINT64 reqId);


#endif  /*_CCOV_SNMP_PACK_H*/