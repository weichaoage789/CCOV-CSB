/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  CcovDefine.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _CCOV_DEFINE_H
#define _CCOV_DEFINE_H

#define IP_VERSION_ID      0x40
#define CBTC_TRUE				0x55
#define CBTC_FALSE			0xAA

#define RSSP1_PROTOCOL   			1			/*RSSP-1协议*/
#define RSSP2_PROTOCOL				2			/*RSSP-2协议*/

/*通信端口定义,发送和接收的双方端口相同*/
#define PORT_123				123				/** < NTP通信端口 */
#define PORT_50001			50001			/** < MSS通信端口 */
#define PORT_50002			50002			/** < ZC\ATS\CI通信端口 */
#define PORT_50003			50003			/** < AOM通信端口 */

#define PORT_WGB_RX		5801			/** <WGB发送端口*/
#define PORT_WGB_TX		161				/** <WGB接收端口*/

/*ARCNET接收数据后,根据下面的类型做不同的处理(这些类型间的数据格式不一致)*/
#define SFP_TYPE				0
#define RP_TYPE				1
#define RSR_TYPE			2
#define NTP_TYPE			3
#define NMS_TYPE			4
#define RSSP_TYPE			5

/*定义不同设备类型*/
#define DEV_ZC 			0x1E
#define DEV_CI 			0x3C
#define DEV_ATS 			0x03
#define DEV_AOM 		0x15
#define DEV_PSC 			0x61

#define SERIES_NUM_MAX  255   /*源系和目标系最大个数*/
#define ONE_SERIES_LEN      12    /*每一系IP和端口所占字节数*/

#define MAX_IP_INFO_NUM (8)
#define SFP_FRA_LENGTH_VAR_SIZE (2)
#define SFP_FRA_PROTOCOL_VAR_SIZE (1)
#define SFP_FRA_DEV_TYPE_VAR_SIZE (1)
#define SFP_FRA_DEV_ID_VAR_SIZE (1)
#define SFP_FRA_RESERVE_VAR_SIZE (2)


#endif  /*_CCOV_DEFINE_H*/