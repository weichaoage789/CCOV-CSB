/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  NetSocket.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _NETWORK_H
#define _NETWORK_H


namespace NetWork
{
#ifndef ADDR_ANY
#define ADDR_ANY 0UL
#endif

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  set local ip address.
	  * @param	 :  [in] const char *ip, source ip value.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool SetLocalIp(const char *ip, const char *eth);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  set local netmask.
	  * @param	 :  [in] const char *netmask, source netmask value.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool SetLocalNetMask(const char *netmask, const char *eth);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  set local gateway.
	  * @param	 :  [in] const char *gateway, source gateway value.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool SetLocalGateway(const char *gateway, char *eth);

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get local ip address, netmask, mac address.
	  * @param	 :  [out] unsigned char *ip, local ip address value got.
	  * @param	 :  [out] unsigned char *macaddr, local mac address value got.
	  * @param	 :  [out] unsigned char *netmask, local netmask address value got.
	  * @return	 :  [type] bool, success -- true; fail -- false.
	  *-----------------------------------------------------------------------------
	  */
	bool GetLocalIpMacMaskAddr(unsigned char *ip, unsigned char *macaddr, unsigned char *netmask);

	/**
	*-----------------------------------------------------------------------------
	* @brief		 :  socket accepted pointer function.
	* @param	 :  [in] int &socketfd, SOCKET instance.
	* @param	 :  [in] void *arg, input params.
	* @return	 :  [type]
	*-----------------------------------------------------------------------------
	*/
	typedef void(*Func_Accept_Proc)(int &socketfd, void *arg);


	class INetWork
	{
	public:
		INetWork();
		virtual ~INetWork();

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  initial function.
		  * @param	 :  [in] const char *ip, local ip address.
		  * @param	 :  [in] const unsigned short port, local port number.
		  * @param	 :  [in] unsigned long mode, 0 -- blocking mode, not 0 -- non-blocking mode.
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
		virtual bool init(const char *ip = "0.0.0.0", const unsigned short port = 8080, unsigned long mode = 1) = 0;

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  initialize function.
		  * @param	 :  [in] const unsigned long ip, local ip address.
		  * @param	 :  [in] const unsigned short port, local port number.
		  * @param	 :  [in] unsigned long mode, 0 -- blocking mode, not 0 -- non-blocking mode.
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
		virtual bool init(const unsigned long ip = ADDR_ANY, const unsigned short port = 8080, unsigned long mode = 1) = 0;

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  socket send data function.
		  * @param	 :  [in] const char *data, data to be send.
		  * @param	 :  [in] const int len, length of data to be send.
		  * @return	 :  [type] int, data sent length.
		  *-----------------------------------------------------------------------------
		  */
		virtual int sendmesg(const char *data, const int len) = 0;

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  socket receive data function.
		  * @param	 :  [in] char *buf, receive data buffer.
		  * @param	 :  [in] int len, receive data buffer length.
		  * @return	 :  [type] int, data received length.
		  *-----------------------------------------------------------------------------
		  */
		virtual int recvmesg(char *buf, int len) = 0;

	protected:
		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  net socket initial.
		  * @param	 :  [in] bool udpflag = false, protocol flag (true -- UDP, false -- TCP).
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
		bool socketinit(bool udpflag = false);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  net socket bind ip, port.
		  * @param	 :  [in] const char *ip, ip address.
		  * @param	 :  [in] const unsigned short port, port number.
		  * @param	 :  [in] unsigned long mode, 0 -- blocking mode, not 0 -- non-blocking mode.
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
		bool socketbind(const char *ip, const unsigned short port, unsigned long mode);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  net socket bind ip, port.
		  * @param	 :  [in] const unsigned long ip, ip address number.
		  * @param	 :  [in] const unsigned short port, port number.
		  * @param	 :  [in] unsigned long mode, 0 -- blocking mode, not 0 -- non-blocking mode.
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
		bool socketbind(const unsigned long ip, const unsigned short port, unsigned long mode);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  Check whether SOCKET is writable or readable.
		  * @param	 :  [in] bool flag, Check whether it is writable -- true, Check whether it is readable -- false.
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
		bool socketcheck(bool flag, unsigned int delay = 0);

	protected:
		/** @network socket */
		int _Socket;
	};


	class CTCP : public INetWork
	{
	public:
		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  TCP CLIENT construction.
		  *-----------------------------------------------------------------------------
		  */
		CTCP();

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  TCP SERVER construction.
		  * @param	 :  [in] Func_Accept_Proc func func, socket accepted pointer function instance.
		  *-----------------------------------------------------------------------------
		  */
		CTCP(Func_Accept_Proc func);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  TCP CLIENT copy construction.
		  *-----------------------------------------------------------------------------
		  */
		CTCP(const CTCP &);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  delete TCP CLIENT assignment construction.
		  *-----------------------------------------------------------------------------
		  */
		CTCP & operator = (const CTCP &) = delete;

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  TCP  deconstruction.
		  *-----------------------------------------------------------------------------
		  */
		virtual ~CTCP();

	public:
		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  TCP initial function.
		  * @param	 :  [in] const char *ip, local ip address.
		  * @param	 :  [in] const unsigned short port, local port number.
		  * @param	 :  [in] unsigned long mode, 0 -- blocking mode, not 0 -- non-blocking mode.
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
		virtual bool init(const char *ip = "0.0.0.0", const unsigned short port = 8080, unsigned long mode = 1);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  TCP initial function.
		  * @param	 :  [in] const unsigned long ip, local ip address.
		  * @param	 :  [in] const unsigned short port, local port number.
		  * @param	 :  [in] unsigned long mode, 0 -- blocking mode, not 0 -- non-blocking mode.
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
		virtual bool init(const unsigned long ip = ADDR_ANY, const unsigned short port = 8080, unsigned long mode = 1);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  TCP server exit function.
		  * @return	 :  [type]
		  *-----------------------------------------------------------------------------
		  */
		void tcpexit();

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  TCP send data function.
		  * @param	 :  [in] const char *data, data to be send.
		  * @param	 :  [in] const int len, length of data to be send.
		  * @return	 :  [type] int, data sent length.
		  *-----------------------------------------------------------------------------
		  */
		virtual int sendmesg(const char *data, const int len);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  TCP receive data function.
		  * @param	 :  [in] char *buf, receive data buffer.
		  * @param	 :  [in] int len, receive data buffer length.
		  * @return	 :  [type] int, data received length.
		  *-----------------------------------------------------------------------------
		  */
		virtual int recvmesg(char *buf, int len);

	protected:
		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  TCP connect function.
		  * @param	 :  [in] const unsigned long ip, remote host ip address.
		  * @param	 :  [in] const unsigned short port, remote host port number.
		   * @param	 :  [in] unsigned int timeout, overtime/timeout time.
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
		bool tcpconn(const unsigned long ip, const unsigned short port, unsigned int timeout);

	private:
		/**  @server listening thread */
		static unsigned int ListenThread(void *lp);

		/**  @server listening thread function pointer */
		Func_Accept_Proc Accept_Func;

	protected:
		/**  @server flag */
		bool _ServerFlag;

		/**  @server listening thread running flag */
		bool _LintenningFlag;
	};


	class CUDP : public INetWork
	{
	public:
		CUDP() {}
		virtual ~CUDP() {}

	public:
		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  UDP initial function.
		  * @param	 :  [in] const char *ip, local ip address.
		  * @param	 :  [in] const unsigned short port, local port number.
		  * @param	 :  [in] unsigned long mode, 0 -- blocking mode, not 0 -- non-blocking mode.
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
		virtual bool init(const char *ip = "0.0.0.0", const unsigned short port = 8080, unsigned long mode = 1);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  UDP initial function.
		  * @param	 :  [in] const unsigned long ip, local ip address.
		  * @param	 :  [in] const unsigned short port, local port number.
		  * @param	 :  [in] unsigned long mode, 0 -- blocking mode, not 0 -- non-blocking mode.
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
		virtual bool init(const unsigned long ip = ADDR_ANY, const unsigned short port = 8080, unsigned long mode = 1);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  UDP send data function.
		  * @param	 :  [in] const char *data, data to be send.
		  * @param	 :  [in] const int len, length of data to be send.
		  * @return	 :  [type] int, data sent length
		  *-----------------------------------------------------------------------------
		  */
		virtual int sendmesg(const char *data, const int len);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  UDP send data function.
		  * @param	 :  [in] const char *data, data to be send.
		  * @param	 :  [in] const int len, length of data to be send.
		  * @param	 :  [in] char *remote_ip, remote ip address.
		  * @param	 :  [in] unsigned short &remote_port, remote port number.
		  * @return	 :  [type] int, data sent length.
		  *-----------------------------------------------------------------------------
		  */
		virtual int sendmesg(const char *buf, const int len, const char *remote_ip, const unsigned short remote_port);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  UDP send data function.
		  * @param	 :  [in] const char *data, data to be send.
		  * @param	 :  [in] const int len, length of data to be send.
		  * @param	 :  [in] unsigned long &remote_ip, remote ip address.
		  * @param	 :  [in] unsigned short &remote_port, remote port number.
		  * @return	 :  [type] int, data sent length.
		  *-----------------------------------------------------------------------------
		  */
		virtual int sendmesg(const char *buf, const int len, const unsigned long remote_ip, const unsigned short remote_port);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  UDP receive data function.
		  * @param	 :  [in] char *buf, receive data buffer.
		  * @param	 :  [in] int len, receive data buffer length.
		  * @return	 :  [type] int, data received length.
		  *-----------------------------------------------------------------------------
		  */
		virtual int recvmesg(char *buf, int len);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  UDP receive data function.
		  * @param	 :  [in] char *buf, receive data buffer.
		  * @param	 :  [in] int len, receive data buffer length.
		  * @param	 :  [in] char *remote_ip, remote ip address.
		  * @param	 :  [in] unsigned short &remote_port, remote port number.
		  * @return	 :  [type] int, data received length.
		  *-----------------------------------------------------------------------------
		  */
		virtual int recvmesg(char *buf, int len, char *remote_ip, unsigned short &remote_port);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  UDP receive data function.
		  * @param	 :  [in] char *buf, receive data buffer.
		  * @param	 :  [in] int len, receive data buffer length.
		  * @param	 :  [in] unsigned long &remote_ip, remote ip address.
		  * @param	 :  [in] unsigned short &remote_port, remote port number.
		  * @return	 :  [type] int, data received length.
		  *-----------------------------------------------------------------------------
		  */
		virtual int recvmesg(char *buf, int len, unsigned long &remote_ip, unsigned short &remote_port);
	};
}


#endif  /*_NETWORK_H*/