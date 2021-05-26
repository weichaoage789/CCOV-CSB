/**
  ******************************************************************************
  * @copyright	:  
  * @file		        :  main.cpp
  * @brief	        :  Main Entrance
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#include "AppConfig.h"
#include "Rsov/RsovCtrl.h"
#include "Ccov/CcovCtrl.h"


int main(int argc, char *argv[])
{
	/** @start RSOV or CCOV according to the config type*/
	if (0 == CAppCfg::GetInstance().GetAppType())
		CRsovCtrl::GetInstance().Start();
	else
		CCcovCtrl::GetInstance().Start();

	while (1)	{	sleep(10);	}
    return 0;
}