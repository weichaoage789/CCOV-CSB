/**
  ******************************************************************************
  * @copyright	:
  * @file		        : RsovDefine.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef  _RSOV_DEFINE_H
#define  _RSOV_DEFINE_H


#define DISK_BASE_PATH			"/run/media/sda1"					/** < ��¼ϵͳ��Ŀ¼ */

#define MIN_FOLDER_NUM			0						/** < ϵͳ���������¼���ļ�����Ŀ */
#define MAX_FOLDER_NUM         8						/** < ϵͳ��������¼���ļ�����Ŀ */
#define DISK_SPACE_FULLTAG      1000				/** < ʣ����̿ռ��С�涨Ϊ1G */

#define SECONDS_OF_DAY				86400				/** < һ������� */
#define HALF_HOUR						1800				/** < ��ʱ������¼�ļ� */

#define USUAL_HEAD_LEN   			10			/** < ʱ��,��������Ϣ,�������������,���ȣ���16.17.00����,��Ϊ16.17.00A0 */
#define REDUND_DATA_LEN  		5				/** < ����֡��¼�ĳ���,����(2)+����(1)+���к�(4) */
#define TYPE_SN_NUM					70			/** < ���͵������Ŀ */

const char NET_RED[2] = { 'A', '\0' };					/** < ������ʶ */
const char NET_BLUE[2] = { 'B', '\0' };				/** < ������ʶ*/


#endif  /*_RSOV_DEFINE_H*/