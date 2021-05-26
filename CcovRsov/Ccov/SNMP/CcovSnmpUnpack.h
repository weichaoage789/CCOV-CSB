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
*���������    const unsigned char *psrcCodeOidStr,���������OID��Ϣ;
*              const unsigned short srcCodeOidStrLen,���������OID��Ϣ�ĳ���;
*���������    unsigned char *poidDecodeStr,������OID��Ϣ,��Ϣ�ԡ�.�����ż����ʾ;
*����ֵ��      ������OID��Ϣ�ܹ���ռ���ֽ���;
**/
unsigned short OidDecode(unsigned char *pOidDecodeStr, const unsigned char *pSrcCodeOidStr, const unsigned short srcCodeOidStrLen);


/*
*���������    const unsigned int oidFir2NodeSum,OID��һ���͵ڶ����ڵ�ֵ��BER��;
*���������    unsigned char *pOidFirNode,OID��һ���ڵ��ֵ�ĵ�ַ;
*              unsigned char *pOidSndNode,OID�ڶ����ڵ��ֵ�ĵ�ַ;
*����ֵ��      ������OID��Ϣ�ܹ���ռ���ֽ���;
**/
char OidFir2NodeDecode(unsigned char *pOidFirNode, unsigned char *pOidSndNode, const unsigned int oidFir2NodeSum);

/*
*��������:     ����ֵ��ʽ����������ʾΪ�ַ�����ʽ��������;
*���������    const unsigned int oidNodeVal,������OID�ڵ�������Ϣ;
*���������    unsigned char *pOidNodeStr,���ַ�����ʾ�Ľ�����OID�ڵ���Ϣ;
*����ֵ��      ������OID��Ϣ�ܹ���ռ���ֽ���;
**/
unsigned char OidNodeDecode(unsigned char *pOidNodeStr, const unsigned int oidNodeVal);

/*
*���������    const unsigned short oidNodeVal,�����OID���ͽڵ�ֵ;
*              const unsigned char dotKey,ֵΪ0x00ʱ,�ڽ����OID�ַ�����ӡ�.��;0x01ʱ,�����'.';
*���������    unsigned char *poidDecodeStr,�����OID�ַ���;
*�������������unsigned int *pDecodeStrNo,�����OID�ַ����е��±�,���ñ���������Զ�����;
*����ֵ��      ���ñ�������,OID�ַ����е��±���º��ֵ;
*˵����        ����oidNodeDecode����;
**/
unsigned short OidNodeValDecode(unsigned char *poidDecodeStr, unsigned int decodeStrNo, const unsigned short oidNodeVal, const unsigned char dotKey);


/*
*��������:     ����ֵ��ʽ����������ʾΪ�ַ�����ʽ��������;
*���������    const unsigned int uintVal,������;
*���������    unsigned char *pUintStr,���ַ�����ʾ��������;
*����ֵ��      �ַ�����ʾ����������ռ���ֽ���;
**/
unsigned char Uint32ToUchar(unsigned char *pUintStr, const unsigned int uintVal);


/*
*���������    const unsigned char *lenCodeStr,������Ϣʹ��BER��������Ϣ
*���������    unsigned short length,����ֵ
*����ֵ��      ������Ϣʹ��BER�������ռ�ֽ���
**/
unsigned char LenDecode(unsigned short *pLength, const unsigned char *pLenCodeStr);

/*
*���������    const unsigned char *pIntValStr,������BER��Ϣ
*              const unsigned char intStrLen,������BER��Ϣ���ֽ���
*���������    unsigned int *pInteger,��������ַ
*����ֵ��      ����ɹ���־
*              0x00:ʧ��
*              0x01:�ɹ�
**/
char IntTypeDecode(int *pInteger, const unsigned char *pIntValStr, const unsigned char intStrLen);

/*
*���������    const unsigned char *puIntValStr,������BER��Ϣ
*              const unsigned char uintStrLen,������BER��Ϣ���ֽ���
*���������    int *puInteger,��������ַ
*����ֵ��      ����ɹ���־
*              0x00:ʧ��
*              0x01:�ɹ�
**/
char UintTypeDecode(unsigned int *puInteger, const unsigned char *puIntValStr, const unsigned char uintStrLen);

/*
*���������    const unsigned short strNo,��Ӧ�����ַ������±�;
*              const unsigned char *pResponsStr,��Ӧ�����ַ������׵�ַ;
*���������    SEQ_STRU *pSeqStru,�������������ͺͳ�����Ϣ;
*              unsigned short *pErrPosition,������Ӧ����ʱ,���ֽ��������λ��;
*
*����ֵ��      ���ñ�������,��Ӧ�����ַ������±���º��ֵ;
*˵����        ����lenDecode����;
**/
unsigned short SeqTagDecode(SEQ_STRU *pSeqStru, unsigned short *pErrPosition, const unsigned short strNo, const unsigned char *pResponsStr);

/*
*���������    const unsigned char *pIpCodeStr,BER������IP��Ϣ;
*              const unsigned char ipCodeLen,BER������IP��Ϣռ���ֽ���;
*���������    unsigned char *pIpInfoStr,������IP��Ϣ;
*����ֵ��      �����ip��Ϣռ���ֽ���;
*�������ã�    oidNodeDecode����
**/
unsigned char IpInfoDecode(unsigned char *pIpInfoStr, const unsigned char *pIpCodeStr, const unsigned char ipCodeLen);


#endif  /*_CCOV_SNMP_UNPACK_H*/
