#include <sys/ioctl.h> 
#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <arpa/inet.h>

#include <net/if.h>  
#include <net/route.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ether.h> 

#include <string.h>
#include <thread>


#include "NetWork.h"


namespace NetWork
{
	INetWork::INetWork()
	{
		_Socket = -1;
	}

	INetWork::~INetWork()
	{
		close(_Socket);
	}

	bool INetWork::socketinit(bool udpflag /* = false */)
	{
		if (true == udpflag)
		{
			_Socket = socket(AF_INET, SOCK_DGRAM, 0);
		}
		else
		{
			_Socket = socket(AF_INET, SOCK_STREAM, 0);
		}
		return (-1 != _Socket) ? true : false;
	}

	bool INetWork::socketbind(const char *ip, const unsigned short port, unsigned long mode)
	{
		/** @将IP转换成网络字节序 */
		unsigned long tmp;
		inet_pton(AF_INET, ip, &tmp);
		return socketbind(tmp, port, mode);
	}

	bool INetWork::socketbind(const unsigned long ip, const unsigned short port, unsigned long mode)
	{
		sockaddr_in addr_local;
		memset(&addr_local, 0, sizeof(addr_local));
		addr_local.sin_family = AF_INET;
		addr_local.sin_addr.s_addr = ip;
		addr_local.sin_port = htons(port);

		/** @设置端口复用 */
		int opt = 1;
		setsockopt(_Socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));

		return (0 == bind(_Socket, (sockaddr *)&addr_local, sizeof(sockaddr_in))) ? true : false;
	}

	bool INetWork::socketcheck(bool flag, unsigned int delay /* = 0 */)
	{
		int rt = 0;
		timeval blocktime = { 0, 0 };
		blocktime.tv_sec = delay / 1000;
		blocktime.tv_usec = (delay % 1000) * 1000;

		fd_set ready;
		FD_ZERO(&ready);
		FD_SET(_Socket, &ready);

		if (!flag)
		{
			rt = select(_Socket + 1, &ready, nullptr, nullptr, &blocktime);
		}
		else
		{
			rt = select(_Socket + 1, nullptr, &ready, nullptr, &blocktime);
		}

		if (rt > 0)
		{
			if (0 != FD_ISSET(_Socket, &ready))
			{
				return true;
			}
		}
		return false;
	}


	CTCP::CTCP() : _ServerFlag(false), _LintenningFlag(false), Accept_Func(nullptr)
	{
	}

	CTCP::CTCP(Func_Accept_Proc func) : _ServerFlag(true), _LintenningFlag(false), Accept_Func(func)
	{
	}

	CTCP::CTCP(const CTCP &tcp)
	{
		_Socket = tcp._Socket;
		_ServerFlag = tcp._ServerFlag;
		_LintenningFlag = tcp._LintenningFlag;
		Accept_Func = tcp.Accept_Func;
	}

	CTCP::~CTCP()
	{
		_LintenningFlag = false;
	}

	bool CTCP::init(const char *ip, const unsigned short port, unsigned long mode /* = 1 */)
	{
		if (nullptr != ip)
		{
			/** @将IP转换成网络字节序 */
			unsigned long tmp;
			inet_pton(AF_INET, ip, &tmp);
			return init(tmp, port, mode);
		}
		return false;
	}

	bool CTCP::init(const unsigned long ip, const unsigned short port, unsigned long mode /* = 1 */)
	{
		if (socketinit(false))
		{
			if (_ServerFlag)
			{
				if (socketbind(ip, port, mode))
				{
					if (0 != listen(_Socket, SOMAXCONN))
					{
						std::thread sevr_listen(ListenThread, this);
						sevr_listen.detach();
						return true;
					}
				}
			}
			else
			{
				if (tcpconn(ip, port, 0))
				{
					return true;
				}
			}
		}
		return false;
	}

	void CTCP::tcpexit()
	{
		_LintenningFlag = false;
	}

	bool CTCP::tcpconn(const unsigned long ip, const unsigned short port, unsigned int timeout)
	{
		sockaddr_in  addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = ip;
		addr.sin_port = htons(port);
		int erro = EISCONN;

		if (0 == connect(_Socket, (sockaddr *)(&addr), sizeof(sockaddr_in)))
		{
			return true;
		}
		else
		{
			if (EISCONN == erro)
			{
				return true;
			}
			else if ((EWOULDBLOCK == erro) || (EINPROGRESS == erro))
			{
				/** @brief socket 正在连接过程中 */
				if (socketcheck(true, timeout))
				{
					/** @brief socket 可写 */
					int error = 0;
					int error_len = sizeof(int);
					if (getsockopt(_Socket, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&error_len) < 0)
					{
						/** @brief 套接字层次获取socket信息信息失败 */
						return false;
					}
					else
					{
						if (error != 0)
						{
							/** @brief 获取socket信息信息成功, 存在错误信息 */
							return false;
						}
						else
						{
							/** @brief 获取socket信息信息成功 */
							sockaddr peername = { 0 };
							int len = sizeof(peername);
							if (0 != getpeername(_Socket, &peername, (socklen_t *)&len))
							{
								/** @brief 获取与socket相连的端地址失败 */
								return false;
							}
							else
							{
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}

	int CTCP::sendmesg(const char *buf, const int len)
	{
		int bytesend = 0;
		int bytesendtotal = 0;

		if (socketcheck(true))
		{
			while (bytesendtotal < len)
			{
				bytesend = send(_Socket, buf + bytesendtotal, len - bytesendtotal, 0);
				if (bytesend < 0)
				{
					break;
				}
				bytesendtotal += bytesend;
			}
		}
		return bytesendtotal;
	}

	int CTCP::recvmesg(char *buf, int len)
	{
		int recved = 0;

		if ((nullptr != buf) && (0 <= len))
		{
			if (socketcheck(false))
			{
				recved = recv(_Socket, buf, len, 0);
				if (0 > recved)
				{
					struct tcp_info info;
					int len = sizeof(info);
					getsockopt(_Socket, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len);
					if ((info.tcpi_state == TCP_ESTABLISHED))
					{
						recved = 0;
					}
				}
			}
		}
		return recved;
	}

	unsigned int CTCP::ListenThread(void *lp)
	{
		if (nullptr == lp)
		{
			return 1;
		}
		CTCP *pThisObject = (CTCP *)lp;
		pThisObject->_LintenningFlag = true;

		while (pThisObject->_LintenningFlag)
		{
			int accptsocket = accept(pThisObject->_Socket, nullptr, nullptr);
			if (-1 != accptsocket)
			{
				if (nullptr != pThisObject->Accept_Func)
				{
					pThisObject->Accept_Func(accptsocket, pThisObject);
				}
			}
			usleep(10000);
		}

		return 0;
	}


	bool CUDP::init(const char *ip, const unsigned short port, unsigned long mode /* = 1 */)
	{
		if (nullptr != ip)
		{
			/** @将IP转换成网络字节序 */
			unsigned long tmp;
			inet_pton(AF_INET, ip, &tmp);
			return init(tmp, port, mode);
		}
		return false;
	}

	bool CUDP::init(const unsigned long ip, const unsigned short port, unsigned long mode /* = 1 */)
	{
		if (socketinit(true))
		{
			return socketbind(ip, port, mode);
		}
		return false;
	}

	int CUDP::sendmesg(const char *data, const int len)
	{
		unsigned long ip = 0;
		unsigned short port = 0;
		return sendmesg(data, len, ip, port);
	}

	int CUDP::sendmesg(const char *buf, const int len, const char *remote_ip, const unsigned short port)
	{
		if (nullptr != remote_ip)
		{
			/** @将IP转换成网络字节序 */
			unsigned long ip = 0;
			inet_pton(AF_INET, remote_ip, &ip);
			return sendmesg(buf, len, ip, port);
		}
		return 0;
	}

	int CUDP::sendmesg(const char *buf, const int len, const unsigned long remote_ip, const unsigned short port)
	{
		int bytesend = 0;
		int bytesendtotal = 0;

		if (socketcheck(true))
		{
			sockaddr_in addr;
			memset((void *)(&addr), 0, (size_t)sizeof(sockaddr_in));
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = remote_ip;
			addr.sin_port = htons(port);

			while (bytesendtotal < len)
			{
				bytesend = sendto(_Socket, buf + bytesendtotal, len - bytesendtotal, 0, (sockaddr *)(&addr), sizeof(sockaddr_in));
				if (bytesend < 0)
				{
					break;
				}
				bytesendtotal += bytesend;
			}
		}

		return bytesendtotal;
	}

	int CUDP::recvmesg(char *buf, int len)
	{
		unsigned long ip = 0;
		unsigned short port = 0;
		return recvmesg(buf, len, ip, port);
	}

	int CUDP::recvmesg(char *buf, int len, char *remote_ip, unsigned short &remote_port)
	{
		unsigned long ip = 0;
		int recved = recvmesg(buf, len, ip, remote_port);
		inet_ntop(AF_INET, (void *)&ip, remote_ip, 16);
		return recved;
	}

	int CUDP::recvmesg(char *buf, int len, unsigned long &remote_ip, unsigned short &remote_port)
	{
		int recved = 0;

		if ((nullptr != buf) && (0 <= len))
		{
			sockaddr_in addr;
			int fromlen = sizeof(sockaddr_in);
			memset((void*)(&addr), 0, (size_t)sizeof(sockaddr_in));

			recved = recvfrom(_Socket, buf, len, 0, (sockaddr*)&addr, (socklen_t *)&fromlen);
			if (recved > 0)
			{
				remote_ip = addr.sin_addr.s_addr;
				remote_port = ntohs(addr.sin_port);
			}
		}

		return recved;
	}


	bool SetLocalIp(const char *ip, const char *eth)
	{
		if (nullptr != ip)
		{
			int sock_set_ip;
			struct sockaddr_in sin_set_ip;
			struct ifreq ifr_set_ip;
			bzero(&ifr_set_ip, sizeof(ifr_set_ip));
			if (-1 == (sock_set_ip = socket(AF_INET, SOCK_STREAM, 0)))
			{
				return false;
			}

			memset(&sin_set_ip, 0, sizeof(sin_set_ip));
			strncpy(ifr_set_ip.ifr_name, eth, sizeof(ifr_set_ip.ifr_name) - 1);

			sin_set_ip.sin_family = AF_INET;
			sin_set_ip.sin_addr.s_addr = inet_addr(ip);
			memcpy(&ifr_set_ip.ifr_addr, &sin_set_ip, sizeof(sin_set_ip));

			if (ioctl(sock_set_ip, SIOCSIFADDR, &ifr_set_ip) < 0)
			{
				close(sock_set_ip);
				return false;
			}

			ifr_set_ip.ifr_flags |= IFF_UP | IFF_RUNNING;

			if (ioctl(sock_set_ip, SIOCSIFFLAGS, &ifr_set_ip) < 0)
			{
				close(sock_set_ip);
				return false;
			}

			close(sock_set_ip);
			return true;
		}

		return false;
	}

	bool SetLocalNetMask(const char *netmask, const char *eth)
	{
		if (nullptr != netmask)
		{
			int socketfd;

			if (-1 == (socketfd = socket(PF_INET, SOCK_STREAM, 0)))
			{
				return false;
			}

			struct ifreq ifr_mask;
			struct sockaddr_in *sin_net_mask;

			memset(&ifr_mask, 0, sizeof(ifr_mask));
			strncpy(ifr_mask.ifr_name, eth, sizeof(ifr_mask.ifr_name) - 1);
			sin_net_mask = (struct sockaddr_in *)&ifr_mask.ifr_addr;
			sin_net_mask->sin_family = AF_INET;
			inet_pton(AF_INET, netmask, &sin_net_mask->sin_addr);

			if (0 > ioctl(socketfd, SIOCSIFNETMASK, &ifr_mask))
			{
				close(socketfd);
				return false;
			}

			close(socketfd);
			return true;
		}

		return false;
	}

	bool SetLocalGateway(const char *gateway, char *eth)
	{
		if (nullptr != gateway)
		{
			int socketfd, err;

			if (-1 == (socketfd = socket(AF_INET, SOCK_DGRAM, 0)))
			{
				return false;
			}

			struct sockaddr_in ic_gateway;
			ic_gateway.sin_family = AF_INET;
			ic_gateway.sin_addr.s_addr = inet_addr(gateway);
			ic_gateway.sin_port = 0;

			struct rtentry rmten;
			memset(&rmten, 0, sizeof(rmten));

			((struct sockaddr_in*)&rmten.rt_dst)->sin_family = AF_INET;
			((struct sockaddr_in*)&rmten.rt_dst)->sin_addr.s_addr = 0;
			((struct sockaddr_in*)&rmten.rt_dst)->sin_port = 0;

			((struct sockaddr_in*)&rmten.rt_genmask)->sin_family = AF_INET;
			((struct sockaddr_in*)&rmten.rt_genmask)->sin_addr.s_addr = 0;
			((struct sockaddr_in*)&rmten.rt_genmask)->sin_port = 0;

			rmten.rt_dev = eth;
			rmten.rt_flags = RTF_UP | RTF_GATEWAY;
			memcpy((void *)&rmten.rt_gateway, &ic_gateway, sizeof(ic_gateway));

			if ((err = ioctl(socketfd, SIOCADDRT, &rmten)) < 0)
			{
				perror("11");
				close(socketfd);
				return false;
			}

			close(socketfd);
			return true;
		}

		return false;
	}

	bool GetLocalIpMacMaskAddr(unsigned char *ip, unsigned char *macaddr, unsigned char *netmask)
	{
		if (nullptr != ip && nullptr != macaddr && nullptr != netmask)
		{
			struct ifreq struReq;
			memset(&struReq, 0x00, sizeof(struct ifreq));
			strncpy(struReq.ifr_name, "eth0", sizeof(struReq.ifr_name));

			int socketfd;
			if (-1 == (socketfd = socket(PF_INET, SOCK_STREAM, 0)))
			{
				return false;
			}

			if (-1 == ioctl(socketfd, SIOCGIFHWADDR, &struReq))
			{
				return false;
			}
			strcpy((char *)macaddr, ether_ntoa((ether_addr*)struReq.ifr_hwaddr.sa_data));

			if (-1 == ioctl(socketfd, SIOCGIFADDR, &struReq))
			{
				return false;
			}
			strcpy((char *)ip, inet_ntoa(((struct sockaddr_in *)&(struReq.ifr_addr))->sin_addr));

			if (-1 == ioctl(socketfd, SIOCGIFNETMASK, &struReq))
			{
				exit(EXIT_FAILURE);
			}
			strcpy((char *)netmask, inet_ntoa(((struct sockaddr_in *)&(struReq.ifr_netmask))->sin_addr));

			close(socketfd);
			return true;
		}

		return false;
	}
}