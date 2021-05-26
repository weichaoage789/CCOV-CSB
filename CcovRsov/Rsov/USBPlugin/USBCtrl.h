/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  USBCtrl.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _USB_CONTROL_H
#define _USB_CONTROL_H

#define USB_CONFIG_PROFILE			(const char *)"/mnt/USBTRANS.INI"				/** < �����ļ�·�� */
#define USB_TRANS							(const char *)"UsbTrans"									/** < �����ļ����� */
#define TRANS_MODE						(const char *)"TransMode"								/** < ת��ģʽ mode=1,2,3 */
#define TRANS_NUM  						(const char *)"TransNum"									/** < ת������ */
#define TRANS_DATE   						(const char *)"TransDate"									/** < ת��ĳ�� */
#define START_TIME    						(const char *)"StartTime"									/** < ��ʼʱ�� */
#define END_TIME  						        (const char *)"EndTime"									    /** < ����ʱ�� */

#define USBTRANS_DATA_SRC			(const char *)"/run/media/sda1"
#define USBTRANS_DATA_DEST		(const char *)"/mnt"
#define USBTRANS_MODE_FAST					1
#define USBTRANS_MODE_SUPER				2
#define USBTRANS_MODE_SPECIFY				3
#define USBTRANS_DAY_MINNUM				1
#define USBTRANS_DAY_MAXNUM				7


enum USB_STATUS
{
	STATE_IDLE,
	STATE_ADD,
	STATE_REMOVE
};

enum COPY_STATUS
{
	STATE_NORMAL,
	STATE_SUCCESS,
	STATE_FAIL
};

typedef struct
{
	int transmode;						/** < ת��ģʽ��1-����ģʽ��2-����ģʽ��3-ָ��ģʽ */
	int transnum;						/** < ת������<����ģʽ> ȡֵ��Χ[1,7] */
	int starttime;							/** < ��ʼʱ��<ָ��ģʽ> ȡֵ��Χ[0,24]*/
	int endtime;							/** < ����ʱ��<ָ��ģʽ> ȡֵ��Χ[0,24] */
	char transdate[16];				/** < ת������<ָ��ģʽ> ��ʽ:2013.07.02 */
} USB_TRANSCFG;


class CUSBCtrl
{
public:
	CUSBCtrl();
	CUSBCtrl(const CUSBCtrl &) = delete;
	CUSBCtrl & operator = (const CUSBCtrl &) = delete;

public:
	static CUSBCtrl &Getinstance()
	{
		static CUSBCtrl instance;
		return instance;
	}

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  start.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void Start();

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  stop.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	void Stop();

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get USB status.
	  * @return	 :  [type] USB_STATUS, STATE_ADD -- USB added; STATE_REMOVE -- USB removed.
	  *-----------------------------------------------------------------------------
	  */
	USB_STATUS GetUSBStatus()
	{
		return _UsbStatus;
	}

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get copy status.
	  * @return	 :  [type] USB_STATUS, STATE_SUCCESS -- copy success; STATE_FAIL -- copy failed.
	  *-----------------------------------------------------------------------------
	  */
	COPY_STATUS GetCOPYStatus()
	{
		return _CopyStatus;
	}

protected:
	/**
	 *-----------------------------------------------------------------------------
	  * @brief		 :  get USB transmit config from INI file.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	 */
	bool get_usbtrans_cfg();

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  register hot-plugin events into the Kernel.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool init_hotplug_sock();

	/**
	 *-----------------------------------------------------------------------------
	 * @brief		 :  fast mode.
	 * @param		 :  [in] const char *filepath, file path.
	 * @return	     :  [type] bool, success -- true; fail -- false.
	 *-----------------------------------------------------------------------------
	 */
	bool copy_fast(const char *filepath);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  super mode.
	  * @param	 :  [in] const char *filepath, file path.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool copy_super(const char *filepath);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  specify mode.
	  * @param	 :  [in] const char *filepath, file path.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool copy_specify(const char *filepath);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  copy to UDisk.
	  * @param	 :  [in] const char *filepath, file path.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool copy_to_udisk(const char *filepath);

	bool my_mount(const char *info);

private:
	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  USB listening thread function.
	  * @param	 :  [in] void *arg, pointer of parameters.
	  * @return	 :  [type]
	  *-----------------------------------------------------------------------------
	  */
	static void Thread_UsbListener(void *arg);

private:
	/** @network operator */
	int _Hotplug;

	/** @running flag */
	bool _RunningFlag;

	/** @USB status  */
	USB_STATUS _UsbStatus;

	/** @COPY status  */
	COPY_STATUS _CopyStatus;

	/** @USB transmit config */
	USB_TRANSCFG _TransCfg;
};


#endif  /*_USB_CONTROL_H*/