/**
  ******************************************************************************
  * @copyright	:
  * @file		        : AppDefine.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _APPLICATION_DEFINE_H
#define _APPLICATION_DEFINE_H

#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h> 
#include <unistd.h> 

#include <sys/syscall.h>
#include <sys/time.h>

#include <string>
#include <iostream>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>
#include <algorithm>
using namespace std;


typedef unsigned long long		UINT64;
typedef long long							INT64;


#define VOBC_DEBUG		1


#define APP_CONFIG_FILE					(char *)"/home/root/projects/CCOV.FS"
#define APP_CONFIG_PROFILE			(const char *)"/home/root/projects/CCOV.INI"  
#define APP_INTERNET						(const char *)"Internet"						/** < �����ļ������������������� */
#define INTERNET_LOCALIP				(const char *)"LocalIp"						/** < �����ļ�KEY������IP */
#define INTERNET_NETMASK			(const char *)"Netmask"						/** < �����ļ�KEY���������� */
#define INTERNET_GATEWAY			(const char *)"Gateway"						/** < �����ļ�KEY������ */
#define APP_WGB								(const char *)"WGB"							/** < �����ļ�������WGB */
#define WGB_REQMODE					(const char *)"REQ_CODE"					/** < �����ļ�KEY������ʽ */     
#define WGB_OID1								(const char *)"OID1"							/** < �����ļ�KEY��OID1 */
#define WGB_OID2								(const char *)"OID2"							/** < �����ļ�KEY��OID2 */
#define APP_INFO								(const char *)"INFO"							/** < �����ļ�������APP�������� */
#define APP_TYPE								(const char *)"AppType"						/** < �����ļ�KEY��APP����ֵ����0��CCOV��0��RSOV */
#define PRODUCT_TYPE						(const char *)"ProductType"				/** < �����ļ�KEY����Ʒ����ֵ��300��300��Ʒ��500��500��Ʒ��510��510��Ʒ */
#define FRAME_DATA_LEN     			(const char *)"DataLen"						/** < �����ļ�KEY����¼����ÿ֡������Сֵ */
#define PROTOCOL_TYPE					(const char *)"ProtocolType"  			/** < �����ļ�KEY��Э���������ã�1��RSSP-1Э�飬2��RSSP-2Э�� */

#define NET_TYPE_RED		1						/** < CCOV���ں��� */
#define NET_TYPE_BLUE	2						/** < CCOV�������� */
#define DEVICE_CCOV			0x14				/** < ͨ���豸���� */

#define Ccov_IP			(char *)"2.72.1.11"			/** < Ccovͨ��IP */
#define Rsov_IP			(char *)"2.72.1.10"			/** < Ccovͨ��IP */
#define Net_Mask		(char *)"255.252.0.0"			/** < �������� */
#define Gateway			(char *)"2.72.254.254"			/** < ���� */


#endif /*_APPLICATION_DEFINE_H*/