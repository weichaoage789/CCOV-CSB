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

#define RSSP1_PROTOCOL   			1			/*RSSP-1Э��*/
#define RSSP2_PROTOCOL				2			/*RSSP-2Э��*/

/*ͨ�Ŷ˿ڶ���,���ͺͽ��յ�˫���˿���ͬ*/
#define PORT_123				123				/** < NTPͨ�Ŷ˿� */
#define PORT_50001			50001			/** < MSSͨ�Ŷ˿� */
#define PORT_50002			50002			/** < ZC\ATS\CIͨ�Ŷ˿� */
#define PORT_50003			50003			/** < AOMͨ�Ŷ˿� */

#define PORT_WGB_RX		5801			/** <WGB���Ͷ˿�*/
#define PORT_WGB_TX		161				/** <WGB���ն˿�*/

/*ARCNET�������ݺ�,�����������������ͬ�Ĵ���(��Щ���ͼ�����ݸ�ʽ��һ��)*/
#define SFP_TYPE				0
#define RP_TYPE				1
#define RSR_TYPE			2
#define NTP_TYPE			3
#define NMS_TYPE			4
#define RSSP_TYPE			5

/*���岻ͬ�豸����*/
#define DEV_ZC 			0x1E
#define DEV_CI 			0x3C
#define DEV_ATS 			0x03
#define DEV_AOM 		0x15
#define DEV_PSC 			0x61

#define SERIES_NUM_MAX  255   /*Դϵ��Ŀ��ϵ������*/
#define ONE_SERIES_LEN      12    /*ÿһϵIP�Ͷ˿���ռ�ֽ���*/

#define MAX_IP_INFO_NUM (8)
#define SFP_FRA_LENGTH_VAR_SIZE (2)
#define SFP_FRA_PROTOCOL_VAR_SIZE (1)
#define SFP_FRA_DEV_TYPE_VAR_SIZE (1)
#define SFP_FRA_DEV_ID_VAR_SIZE (1)
#define SFP_FRA_RESERVE_VAR_SIZE (2)


#endif  /*_CCOV_DEFINE_H*/