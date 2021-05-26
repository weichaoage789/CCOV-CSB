/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  Ping.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _PING_H
#define _PING_H

#include "AppDefine.h"

#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <netdb.h>
#include <setjmp.h>


#define PACKET_SIZE     4096
#define MAX_WAIT_TIME   5
#define MAX_NO_PACKETS  4


namespace NetWork
{
	struct IcmpEchoReply
	{
		int icmpSeq;
		int icmpLen;
		int ipTtl;
		double rtt;
		std::string fromAddr;
		bool isReply;
	};

	struct PingResult
	{
		int dataLen;
		int nsend;
		int nreceived;
		std::string ip;
		std::string error;
		std::vector<IcmpEchoReply> icmpEchoReplys;
	};


	class Ping
	{
	public:
		Ping();

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  ip ping function.
		  * @param	 :  [in] std::string host, host ip address.
		  * @param	 :  [out] PingResult &pingResult, ping result.
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
		bool ping(std::string host, PingResult &pingResult);

		/**
		  *-----------------------------------------------------------------------------
		  * @brief		 :  ip ping function.
		  * @param	 :  [in] std::string host, host ip address.
		  * @param	 :  [in] int count, ping counts.
		  * @param	 :  [out] PingResult &pingResult, ping result.
		  * @return	 :  [type] bool, success -- true; fail -- false.
		  *-----------------------------------------------------------------------------
		  */
		bool ping(std::string host, int count, PingResult &pingResult);

	private:
		/* 校验和算法 */
		unsigned short getChksum(unsigned short *addr, int len);

		/* 设置ICMP报头 */
		int packIcmp(int pack_no, struct icmp* icmp);

		/* 剥去ICMP报头 */
		bool unpackIcmp(char *buf, int len, struct IcmpEchoReply *icmpEchoReply);

		/* 两个timeval结构相减 */
		struct timeval tvSub(struct timeval timeval1, struct timeval timval2);

		/* 获取地址 */
		bool getsockaddr(const char * hostOrIp, sockaddr_in* sockaddr);

		/* 发送ICMP报文 */
		bool sendPacket();

		/* 接收所有ICMP报文 */
		bool recvPacket(PingResult &pingResult);

	private:
		char _SendBuf[PACKET_SIZE];
		char _RecvBuf[PACKET_SIZE];

		int _SocketFd;
		int _DataLen;

		int _SendCnt;
		int _RecvCnt;
		int _MaxCnt;
		int _ICMP_SEQ;

		pid_t _Pid;

		struct sockaddr_in _DestAddr;
		struct sockaddr_in _HostAddr;
	};
}


#endif  /*_PING_H*/
