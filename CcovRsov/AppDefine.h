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
#define APP_INTERNET						(const char *)"Internet"						/** < 配置文件段名：网络配置数据 */
#define INTERNET_LOCALIP				(const char *)"LocalIp"						/** < 配置文件KEY：本地IP */
#define INTERNET_NETMASK			(const char *)"Netmask"						/** < 配置文件KEY：子网掩码 */
#define INTERNET_GATEWAY			(const char *)"Gateway"						/** < 配置文件KEY：网关 */
#define APP_WGB								(const char *)"WGB"							/** < 配置文件段名：WGB */
#define WGB_REQMODE					(const char *)"REQ_CODE"					/** < 配置文件KEY：请求方式 */     
#define WGB_OID1								(const char *)"OID1"							/** < 配置文件KEY：OID1 */
#define WGB_OID2								(const char *)"OID2"							/** < 配置文件KEY：OID2 */
#define APP_INFO								(const char *)"INFO"							/** < 配置文件段名：APP类型配置 */
#define APP_TYPE								(const char *)"AppType"						/** < 配置文件KEY：APP类型值，非0：CCOV；0：RSOV */
#define PRODUCT_TYPE						(const char *)"ProductType"				/** < 配置文件KEY：产品类型值，300：300产品；500：500产品；510：510产品 */
#define FRAME_DATA_LEN     			(const char *)"DataLen"						/** < 配置文件KEY：记录数据每帧长度最小值 */
#define PROTOCOL_TYPE					(const char *)"ProtocolType"  			/** < 配置文件KEY：协议类型配置，1：RSSP-1协议，2：RSSP-2协议 */

#define NET_TYPE_RED		1						/** < CCOV属于红网 */
#define NET_TYPE_BLUE	2						/** < CCOV属于蓝网 */
#define DEVICE_CCOV			0x14				/** < 通控设备类型 */

#define Ccov_IP			(char *)"2.72.1.11"			/** < Ccov通信IP */
#define Rsov_IP			(char *)"2.72.1.10"			/** < Ccov通信IP */
#define Net_Mask		(char *)"255.252.0.0"			/** < 子网掩码 */
#define Gateway			(char *)"2.72.254.254"			/** < 网关 */


#endif /*_APPLICATION_DEFINE_H*/