/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  CcovSnmpConfig.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _CCOV_SNMP_CONFIG_H
#define _CCOV_SNMP_CONFIG_H


/*�����󶨸���*/
#define BANDNUM         4

/* �����ĸ����� */
#define VARINUM         30

#ifndef RAND_MAX
#define RAND_MAX   (unsigned short)0x7fff   /*0x7fff <-> 32767*/
#endif


#define REQID_MAX  (UINT64)0xFFFFFFFF

#define MANA_RCV_PORT              161      /*�����������ն˿�*/

#define AGNT_RCV_PORT              161      /*�������Ӧ���ݽ��ն˿�*/


/*SNMPЭ������������,��tag,�궨����������ĸΪ��д'T'*/

/*ͨ�����͵�Tag�ֽڽṹ*/
#define TINTEGER                   0x02
#define TCOTET_STRING              0x04
#define TNULL                      0x05
#define TOBJECT_IDENTIFIER         0x06
#define TSEQUENCE                  0x30
#define TSEQUENCE_OF               0x30

/*SNMPӦ�ó������͵�Tag�ֽڽṹ*/
#define TIpAddress                 0x40
#define TCounter                   0x41
#define TGauge                     0x42
#define TTime_Ticks                0x43
#define TOpaque                    0x44

/*�������й����Tag�ֽڽṹ*/
#define TGetRequest                0xa0
#define TGetNextRequest            0xa1
#define TGetResponse               0xa2
#define TSetRequest                0xa3
#define TTrap                      0xa4

/*�����󶨵ĸ���,ʹ��ö������*/
enum bandNumber { num_Zero = 0, num_One = 1, num_Two = 2, num_Three = 3, num_Four = 4, num_Five = 5 };


typedef struct
{
	int intVal;                     /*�����Ӧ���Ľ����ı�����������Ϣ*/
	unsigned char oidNo;                      /*OID����ı��*/
}INT_STRU;

typedef struct
{
	unsigned int uintVal;                   /*�����Ӧ���Ľ����ı������޷���������Ϣ*/
	unsigned char oidNo;                      /*OID����ı��*/
}UINT_STRU;

typedef struct
{
	unsigned char str[20];                    /*�����Ӧ���Ľ����ı������ַ�����Ϣ*/
	unsigned char strLen;                     /*�����Ӧ���Ľ����ı������ַ�����Ϣʵ��ռ�ֽ���*/
	unsigned char oidNo;                      /*OID����ı��*/
}STR_STRU;

typedef struct
{
	unsigned char version;                     /*0 -snmp��v1,1 - snmp��v2c,2 - snmp��v3*/
	unsigned char communityStr[12];            /*community�ֶ�*/
	unsigned char communityLen;                /*community�ĳ���*/

	unsigned char cmdType;                      /*SNMP��������*/
	unsigned short reqId;                      /*�����ID��*/
	unsigned char errStatus;                   /*����״̬��Ϣ*/
	unsigned char errIndex;                    /*����������,ָ�����������OID���*/

	unsigned char oidNum;                      /*��Ӧ�����а���OID�ĸ���*/
	unsigned char oidStr[BANDNUM][150];        /*�����ĳ�ǿ������ȡ�IP��OID,oidStr[i]:��ʾ��i��OID*/
	unsigned char oidStrLen[BANDNUM];          /*�����ĳ�ǿ������ȡ�IP�ĳ���*/

	INT_STRU intTypeStru[BANDNUM];     /*����ȡ�ĳ�ǿ��Ϣ,�����Ǹ�����,�糡ǿ-60*/
	unsigned char intStruSize;                 /*��¼һ֡��Ӧ�����г�ǿ��Ϣ�ĸ���,�����Ӧ������
									   *��2����ǿ��Ϣ,��ֵΪ2*/

	UINT_STRU uintTypeStru[BANDNUM];   /*����ȡ���������Ϣ,Ϊ������*/
	unsigned char uintStruSize;                /*��¼һ֡��Ӧ�������������Ϣ�ĸ���,�����Ӧ������
									   *��2���������Ϣ,��ֵΪ2*/

	STR_STRU strTypeStru[BANDNUM];     /*IP�ֶ���Ϣ,��'.'��ʽ��ʾIP,��30.13.300.100*/
	unsigned char strStruSize;                 /*��¼һ֡��Ӧ������IP��Ϣ�ĸ���,�����Ӧ������
									   *��2��IP��Ϣ,��ֵΪ2*/

	unsigned short flameSize;                  /*֡�ĳ���*/
	unsigned short respSize;                   /*��ӦPDU�ĳ���*/
	unsigned short VarBindSize;                /*variable-bindings�ĳ���*/
	unsigned short BindCompSize[BANDNUM];      /*variable-bindings������ĳ���,BindCompSize[i]��ʾ��i������ĳ���*/

	unsigned short decodeErrPos;               /*������Ӧ����ʱ,���ֽ��������λ��*/
	unsigned char errVariType;                 /*��¼δ֪,or����ı�����������*/

}RESPONSE_INFO_NEWSTRU;                /*��Ž�����Ӧ���ĺ���Ϣ�Ľṹ��*/


typedef struct
{
	unsigned char version;            /*0 -snmp��v1,1 - snmp��v2c,2 - snmp��v3*/
	unsigned char communityStr[25];   /*default:public*/
	unsigned char *poid[BANDNUM];     /*�����OID��Ϣ*/
	unsigned char nodeInfoTag;        /*Ԥ��,default:������nullptr,����ֵ0x05*/
	unsigned char nodeInfoLen;        /*Ԥ��,default:0x00*/
}REQ_ARGV_STRU;               /*������֡��������ṹ��*/

typedef struct
{
	unsigned char verInfoType;                  /*�汾��Ϣ����������*/
	unsigned char verInfolen;                   /*�汾��Ϣ����*/
	unsigned char verInfo;                      /*�汾��Ϣ:0 -snmp��v1,1 - snmp��v2c,2 - snmp��v3*/
	unsigned char verInfoSize;                  /*�����汾��Ϣ������tlv��ռ���ֽ���*/
}VER_INFO_STRU;                         /*�汾��Ϣ�ṹ��*/

typedef struct
{
	unsigned char commInfoType;                 /*Community��Ϣ����������*/
	unsigned char commInfoLenStr[3];            /*Community��Ϣ��������Ϣ�ĳ��Ⱦ�BER��������Ϣ*/
	unsigned char commLenCodeSize;              /*Community��Ϣ��������Ϣ�ĳ��Ⱦ�BER����ռ���ֽ���*/
	unsigned char commInfoStr[20];              /*Community��Ϣ��������Ϣ*/
	unsigned char commLen;                      /*Communityԭʼ��Ϣ�ĳ���*/
	unsigned char commInfoSize;                 /*Community��Ϣ������tlv�ĳ���*/
}COMM_INFO_STRU;                        /*Community��Ϣ�ṹ��*/

typedef struct
{
	unsigned char oidInfoType;                  /*OID��Ϣ����������*/

										/*����ʱ,���OID������Ϣ��OID�������Ϣ�ĳ���ʹ��BER�������Ϣ;
										**����ʱ,��Ž�����OID��Ϣ,OID�Ե㡮.���ַ�����ʾ��
										**/
	unsigned char oidInfoStr[256];
	unsigned short oidInfolen;                  /*OID��Ϣ����*/
	unsigned char  oidInfolenSize;              /*OID��Ϣ����BER�����ռ���ֽ���*/
	unsigned short oidInfoSize;                 /*����OID��Ϣ������tlv��ռ���ֽ���*/
}OID_INFO_STRU;                         /*OID��Ϣ�ṹ��*/

typedef struct
{
	unsigned char  reqIdInfoType;              /*����ID��Ϣ����������*/
	unsigned char  reqIdLenStr[2];             /*����ID��ϢBER������Ϣ���ȵ�BER����*/
	unsigned char  reqIdLenCodeSize;           /*����ID��ϢBER������Ϣ���ȵ�BER������Ϣռ���ֽ���*/
	unsigned char  reqIdInfoStr[5];            /*����ID��ϢBER������Ϣ*/
	unsigned char  reqIdInfoLen;               /*����ID��ϢBER������Ϣ����*/
	unsigned char  InfoSize;                   /*����Request ID��Ϣ������tlv��ռ���ֽ���*/
}REQID_INFO_STRU;                      /*����ID��Ϣ�ṹ��*/

typedef struct
{
	unsigned char seqType;                     /*�ṹ���͵�tag*/
	unsigned short seqLen;                     /*�ṹ���͵ĳ���*/
	unsigned char seqLenCodeSize;              /*�ṹ���͵ĳ���BER�������ֽ���*/
	unsigned char seqSize;                     /*�ṹ�����ܹ�ռ�õ��ֽ���*/

}SEQ_STRU;


typedef struct
{
	unsigned char version;                     /*0 -snmp��v1,1 - snmp��v2c,2 - snmp��v3*/
	unsigned char communityStr[12];            /*community�ֶ�*/
	unsigned char communityLen;                /*community�ĳ���*/

	unsigned char cmdType;                      /*SNMP��������*/
	unsigned short reqId;                      /*�����ID��*/
	unsigned char errStatus;                   /*����״̬��Ϣ*/
	unsigned char errIndex;                    /*����������,ָ�����������OID���*/

	unsigned char firOidStr[150];	           /*�����ǿ��OID*/
	unsigned char firOidStrLen;                /*�����ǿ��OID�ĳ���*/
	int variSTR;                     /*����ȡ�ĳ�ǿ��Ϣ,�����Ǹ�����,�糡ǿ-60*/

	unsigned char sndOidStr[150];	           /*���������ȵ�OID*/
	unsigned char sndOidStrLen;                /*���������ȵ�OID�ĳ���*/
	unsigned int variSNR;                    /*����ȡ���������Ϣ,Ϊ������*/

	unsigned char trdOidStr[150];	           /*�����IP��OID*/
	unsigned char trdOidStrLen;                /*�����IP��OID�ĳ���*/
	unsigned char ipAddrStr[20];               /*IP�ֶ���Ϣ,��'.'��ʽ��ʾIP,��30.13.300.100*/
	unsigned char ipAddrStrLen;                /*IP�ֶ���Ϣ�ĳ���*/

	unsigned short flameSize;                   /*֡�ĳ���*/
	unsigned short respSize;                    /*��ӦPDU�ĳ���*/
	unsigned short VarBindSize;                 /*variable-bindings�ĳ���*/
	unsigned short BindComp1Size;               /*variable-bindings�ĵ�1������ĳ���*/
	unsigned short BindComp2Size;               /*variable-bindings�ĵ�2������ĳ���*/
	unsigned short BindComp3Size;               /*variable-bindings�ĵ�3������ĳ���*/

	unsigned short decodeErrPos;               /*������Ӧ����ʱ,���ֽ��������λ��*/

}RESPONSE_INFO_STRU;                   /*��Ž�����Ӧ���ĺ���Ϣ�Ľṹ��*/



#endif  /*_CCOV_SNMP_CONFIG_H*/