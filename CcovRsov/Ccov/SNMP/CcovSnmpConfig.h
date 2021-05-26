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


/*变量绑定个数*/
#define BANDNUM         4

/* 变量的个数据 */
#define VARINUM         30

#ifndef RAND_MAX
#define RAND_MAX   (unsigned short)0x7fff   /*0x7fff <-> 32767*/
#endif


#define REQID_MAX  (UINT64)0xFFFFFFFF

#define MANA_RCV_PORT              161      /*管理端请求接收端口*/

#define AGNT_RCV_PORT              161      /*代理端响应数据接收端口*/


/*SNMP协议中数据类型,即tag,宏定义类型首字母为大写'T'*/

/*通用类型的Tag字节结构*/
#define TINTEGER                   0x02
#define TCOTET_STRING              0x04
#define TNULL                      0x05
#define TOBJECT_IDENTIFIER         0x06
#define TSEQUENCE                  0x30
#define TSEQUENCE_OF               0x30

/*SNMP应用程序类型的Tag字节结构*/
#define TIpAddress                 0x40
#define TCounter                   0x41
#define TGauge                     0x42
#define TTime_Ticks                0x43
#define TOpaque                    0x44

/*上下文有关类的Tag字节结构*/
#define TGetRequest                0xa0
#define TGetNextRequest            0xa1
#define TGetResponse               0xa2
#define TSetRequest                0xa3
#define TTrap                      0xa4

/*变量绑定的个数,使用枚举类型*/
enum bandNumber { num_Zero = 0, num_One = 1, num_Two = 2, num_Three = 3, num_Four = 4, num_Five = 5 };


typedef struct
{
	int intVal;                     /*存放响应报文解码后的变量的整型信息*/
	unsigned char oidNo;                      /*OID数组的标号*/
}INT_STRU;

typedef struct
{
	unsigned int uintVal;                   /*存放响应报文解码后的变量的无符号整型信息*/
	unsigned char oidNo;                      /*OID数组的标号*/
}UINT_STRU;

typedef struct
{
	unsigned char str[20];                    /*存放响应报文解码后的变量的字符串信息*/
	unsigned char strLen;                     /*存放响应报文解码后的变量的字符串信息实际占字节数*/
	unsigned char oidNo;                      /*OID数组的标号*/
}STR_STRU;

typedef struct
{
	unsigned char version;                     /*0 -snmp的v1,1 - snmp的v2c,2 - snmp的v3*/
	unsigned char communityStr[12];            /*community字段*/
	unsigned char communityLen;                /*community的长度*/

	unsigned char cmdType;                      /*SNMP报文类型*/
	unsigned short reqId;                      /*请求的ID号*/
	unsigned char errStatus;                   /*错误状态信息*/
	unsigned char errIndex;                    /*错误索引号,指明发生错误的OID编号*/

	unsigned char oidNum;                      /*响应报文中包含OID的个数*/
	unsigned char oidStr[BANDNUM][150];        /*解码后的场强、信噪比、IP的OID,oidStr[i]:表示第i个OID*/
	unsigned char oidStrLen[BANDNUM];          /*解码后的场强、信噪比、IP的长度*/

	INT_STRU intTypeStru[BANDNUM];     /*待获取的场强信息,可以是负整数,如场强-60*/
	unsigned char intStruSize;                 /*记录一帧响应报文中场强信息的个数,如果响应报文中
									   *有2个场强信息,则值为2*/

	UINT_STRU uintTypeStru[BANDNUM];   /*待获取的信噪比信息,为正整数*/
	unsigned char uintStruSize;                /*记录一帧响应报文中信噪比信息的个数,如果响应报文中
									   *有2个信噪比信息,则值为2*/

	STR_STRU strTypeStru[BANDNUM];     /*IP字段信息,以'.'形式显示IP,如30.13.300.100*/
	unsigned char strStruSize;                 /*记录一帧响应报文中IP信息的个数,如果响应报文中
									   *有2个IP信息,则值为2*/

	unsigned short flameSize;                  /*帧的长度*/
	unsigned short respSize;                   /*响应PDU的长度*/
	unsigned short VarBindSize;                /*variable-bindings的长度*/
	unsigned short BindCompSize[BANDNUM];      /*variable-bindings的组件的长度,BindCompSize[i]表示第i个组件的长度*/

	unsigned short decodeErrPos;               /*解析响应报文时,出现解析错误的位置*/
	unsigned char errVariType;                 /*记录未知,or错误的变量数据类型*/

}RESPONSE_INFO_NEWSTRU;                /*存放解析响应报文后信息的结构体*/


typedef struct
{
	unsigned char version;            /*0 -snmp的v1,1 - snmp的v2c,2 - snmp的v3*/
	unsigned char communityStr[25];   /*default:public*/
	unsigned char *poid[BANDNUM];     /*请求的OID信息*/
	unsigned char nodeInfoTag;        /*预留,default:空类型nullptr,编码值0x05*/
	unsigned char nodeInfoLen;        /*预留,default:0x00*/
}REQ_ARGV_STRU;               /*请求组帧输入参数结构体*/

typedef struct
{
	unsigned char verInfoType;                  /*版本信息的数据类型*/
	unsigned char verInfolen;                   /*版本信息长度*/
	unsigned char verInfo;                      /*版本信息:0 -snmp的v1,1 - snmp的v2c,2 - snmp的v3*/
	unsigned char verInfoSize;                  /*整个版本信息编码后的tlv的占的字节数*/
}VER_INFO_STRU;                         /*版本信息结构体*/

typedef struct
{
	unsigned char commInfoType;                 /*Community信息的数据类型*/
	unsigned char commInfoLenStr[3];            /*Community信息编码后的信息的长度经BER编码后的信息*/
	unsigned char commLenCodeSize;              /*Community信息编码后的信息的长度经BER编码占的字节数*/
	unsigned char commInfoStr[20];              /*Community信息编码后的信息*/
	unsigned char commLen;                      /*Community原始信息的长度*/
	unsigned char commInfoSize;                 /*Community信息编码后的tlv的长度*/
}COMM_INFO_STRU;                        /*Community信息结构体*/

typedef struct
{
	unsigned char oidInfoType;                  /*OID信息的数据类型*/

										/*编码时,存放OID编码信息和OID编码后信息的长度使用BER编码的信息;
										**解码时,存放解码后的OID信息,OID以点‘.’字符串表示。
										**/
	unsigned char oidInfoStr[256];
	unsigned short oidInfolen;                  /*OID信息长度*/
	unsigned char  oidInfolenSize;              /*OID信息长度BER编码后占的字节数*/
	unsigned short oidInfoSize;                 /*整个OID信息编码后的tlv的占的字节数*/
}OID_INFO_STRU;                         /*OID信息结构体*/

typedef struct
{
	unsigned char  reqIdInfoType;              /*请求ID信息的数据类型*/
	unsigned char  reqIdLenStr[2];             /*请求ID信息BER编码信息长度的BER编码*/
	unsigned char  reqIdLenCodeSize;           /*请求ID信息BER编码信息长度的BER编码信息占的字节数*/
	unsigned char  reqIdInfoStr[5];            /*请求ID信息BER编码信息*/
	unsigned char  reqIdInfoLen;               /*请求ID信息BER编码信息长度*/
	unsigned char  InfoSize;                   /*整个Request ID信息编码后的tlv的占的字节数*/
}REQID_INFO_STRU;                      /*请求ID信息结构体*/

typedef struct
{
	unsigned char seqType;                     /*结构类型的tag*/
	unsigned short seqLen;                     /*结构类型的长度*/
	unsigned char seqLenCodeSize;              /*结构类型的长度BER编码后的字节数*/
	unsigned char seqSize;                     /*结构类型总共占用的字节数*/

}SEQ_STRU;


typedef struct
{
	unsigned char version;                     /*0 -snmp的v1,1 - snmp的v2c,2 - snmp的v3*/
	unsigned char communityStr[12];            /*community字段*/
	unsigned char communityLen;                /*community的长度*/

	unsigned char cmdType;                      /*SNMP报文类型*/
	unsigned short reqId;                      /*请求的ID号*/
	unsigned char errStatus;                   /*错误状态信息*/
	unsigned char errIndex;                    /*错误索引号,指明发生错误的OID编号*/

	unsigned char firOidStr[150];	           /*解码后场强的OID*/
	unsigned char firOidStrLen;                /*解码后场强的OID的长度*/
	int variSTR;                     /*待获取的场强信息,可以是负整数,如场强-60*/

	unsigned char sndOidStr[150];	           /*解码后信噪比的OID*/
	unsigned char sndOidStrLen;                /*解码后信噪比的OID的长度*/
	unsigned int variSNR;                    /*待获取的信噪比信息,为正整数*/

	unsigned char trdOidStr[150];	           /*解码后IP的OID*/
	unsigned char trdOidStrLen;                /*解码后IP的OID的长度*/
	unsigned char ipAddrStr[20];               /*IP字段信息,以'.'形式显示IP,如30.13.300.100*/
	unsigned char ipAddrStrLen;                /*IP字段信息的长度*/

	unsigned short flameSize;                   /*帧的长度*/
	unsigned short respSize;                    /*响应PDU的长度*/
	unsigned short VarBindSize;                 /*variable-bindings的长度*/
	unsigned short BindComp1Size;               /*variable-bindings的第1个组件的长度*/
	unsigned short BindComp2Size;               /*variable-bindings的第2个组件的长度*/
	unsigned short BindComp3Size;               /*variable-bindings的第3个组件的长度*/

	unsigned short decodeErrPos;               /*解析响应报文时,出现解析错误的位置*/

}RESPONSE_INFO_STRU;                   /*存放解析响应报文后信息的结构体*/



#endif  /*_CCOV_SNMP_CONFIG_H*/