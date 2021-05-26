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


#define DISK_BASE_PATH			"/run/media/sda1"					/** < 记录系统根目录 */

#define MIN_FOLDER_NUM			0						/** < 系统最少允许记录的文件夹数目 */
#define MAX_FOLDER_NUM         8						/** < 系统最多允许记录的文件夹数目 */
#define DISK_SPACE_FULLTAG      1000				/** < 剩余磁盘空间大小规定为1G */

#define SECONDS_OF_DAY				86400				/** < 一天的秒数 */
#define HALF_HOUR						1800				/** < 定时创建记录文件 */

#define USUAL_HEAD_LEN   			10			/** < 时间,红蓝网信息,冗余与正常标记,长度；如16.17.00红网,则为16.17.00A0 */
#define REDUND_DATA_LEN  		5				/** < 冗余帧记录的长度,长度(2)+类型(1)+序列号(4) */
#define TYPE_SN_NUM					70			/** < 类型的最大数目 */

const char NET_RED[2] = { 'A', '\0' };					/** < 红网标识 */
const char NET_BLUE[2] = { 'B', '\0' };				/** < 蓝网标识*/


#endif  /*_RSOV_DEFINE_H*/