/**
  ******************************************************************************
  * @copyright	:
  * @file		        : RsovConfig.h
  * @brief	        :  
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _RSOV_CONFIG_H
#define _RSOV_CONFIG_H


/** @类型和序列号结构体 */
typedef struct
{
	unsigned char Type;		/** < 数据类型 */
	unsigned int 	Sn;			/** < 序列号 */
}TYPE_SN_CFG;


#endif  /*_RSOV_CONFIG_H*/