/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  LedCtrl.h
  * @brief	        :  LED control
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _LED_CTRL_H
#define _LED_CTRL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#define SYSFS_GPIO_EXPORT	"/sys/class/gpio/export"
#define SYSFS_GPIO_RST_DIR	"/sys/class/gpio/gpio70/direction"

#define LED1_DIR "/sys/class/gpio/gpio99/direction"
#define LED1_VAL "/sys/class/gpio/gpio99/value"

#define LED2_DIR "/sys/class/gpio/gpio100/direction"
#define LED2_VAL "/sys/class/gpio/gpio100/value"

#define LED3_DIR "/sys/class/gpio/gpio101/direction"
#define LED3_VAL "/sys/class/gpio/gpio101/value"

#define OE_DIR "/sys/class/gpio/gpio115/direction"
#define OE_VAL "/sys/class/gpio/gpio115/value"

#define LED4_DIR "/sys/class/gpio/gpio116/direction"
#define LED4_VAL "/sys/class/gpio/gpio116/value"


class CLEDCtrl
{
public:
	~CLEDCtrl() {}

	static CLEDCtrl &GetInstance()
	{
		static CLEDCtrl instance;
		return instance;
	}

	int LED_A2(int val);
	int LED_A3(int val);
	int LED_B2(int val);
	int LED_B3(int val);

protected:
	int LED_Init();

private:
	CLEDCtrl();
	CLEDCtrl(const CLEDCtrl &) = delete;
	CLEDCtrl & operator = (const CLEDCtrl &) = delete;
};


#endif  /*_LED_CTRL_H*/


