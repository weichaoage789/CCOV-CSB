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
*���������    const unsigned char reqType,���������,֧��Get��GetNext����;
*              enum bandNumber bandVarNum,�󶨱�������;
*              const REQ_ARGV_STRU *preqArgvStru,������֡��������ṹ��;
*���������    unsigned char *pReqCode, ��������֡�������ַ���;
*����ֵ��      ��������֡�������ܳ���;
*�������ã�    ReqIdCode����;
*              LenCode����;
*              OidCode����;
*�ؼ���:       BANDNUM:3,�ο�CcovSnmpMdl.h�ļ�,���޸����ĺ궨��ֵ,���Ӧ�޸ĺ�BANDNUM
*              ��صı����ĳ�ʼ��;
*ʹ������:     1�����������֧��4��OID,����ѡ��1������2������3��OID����4��OID;
**/

unsigned short RequestFlameCode(unsigned char *pReqCode, const unsigned char reqType, enum bandNumber bandVarNum, const REQ_ARGV_STRU *preqArgvStru);

/*
*���������    const unsigned char *pRespStr,���������Ӧ��Ϣ֡;
*              const unsigned short recvLen,��Ӧ��Ϣ֡ռ���ֽ���;
*���������    RESPONSE_INFO_NEWSTRU *pRespInfoStru, �����Ӧ��Ϣ�Ľṹ��;
*����ֵ��      �����ѽ���ı��ĵĳ���;
*�������ã�    LenDecode     ����;
*              UintTypeDecode����;
*              SeqTagDecode  ����;
*              OidDecode     ����;
*              IntTypeDecode ����;
*              IpInfoDecode  ����;
*
*�ؼ���:       BANDNUM:3,�ο�CcovSnmpMdl.h�ļ�,���޸����ĺ궨��ֵ,���Ӧ�޸ĺ�BANDNUM
*              ��صı����ĳ�ʼ��;
*ʹ������:     ��������뺯�����Ӧ;
**/
unsigned short ResponseDecode(RESPONSE_INFO_NEWSTRU *pRespInfoStru, const unsigned char *pRespStr, const unsigned short recvLen);


#endif  /*_CCOV_SNMP_MDL_H*/