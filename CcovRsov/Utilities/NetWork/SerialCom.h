/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  SerialCom.h
  * @brief	        :  Serial communication class
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _SERIAL_COMMUNICATION_H
#define _SERIAL_COMMUNICATION_H


namespace NetWork
{
	class CCOMCtrl
	{
	public:
		CCOMCtrl() {}
		virtual ~CCOMCtrl() {  Close();  }

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  Open.
		  * @param	 :  [in] int id, serial device number.
		  * @return	 :  [type] int, fail -- < 0; success -- >= 0.
		  *-----------------------------------------------------------------------------
		  */
		int Open(int id);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  Close.
		  * @return	 :  [type] 
		  *-----------------------------------------------------------------------------
		  */
		void Close();

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  Set serial-comm device baudrate, data bits, parity and stop bits.
		  * @param	 :  [in] int baudrate, baudrate value
		  * @param	 :  [in] int databits, 7 or 8.
		  * @param	 :  [in] char parity, 'N', 'E', 'O'.
		  * @param	 :  [in] int stopbits, 1 or 2.
		  * @return	 :  [type] int, fail -- -1; success -- 0.
		  *-----------------------------------------------------------------------------
		  */
		int SetOptions(int baudrate, int databits, char parity, int stopbits);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  Serial device send data function.
		  * @param	 :  [in] char *data, data to be send.
		  * @param	 :  [in] int len, data length.
		  * @return	 :  [type] int, sent data length.
		  *-----------------------------------------------------------------------------
		  */
		int Send(char *data, int len);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  Serial device receive data function.
		  * @param	 :  [out] char *buf, data receive buffer.
		  * @param	 :  [out] int len, receive buffer length.
		  * @return	 :  [type] int, data received length.
		  *-----------------------------------------------------------------------------
		  */
		int Recv(char *buf, int len);

	private:
		/** @serial device number */
		int _Fd;

		/** @serial device name */
		char _Name[16];
	};
}


#endif  /*_SERIAL_COMMUNICATION_H*/